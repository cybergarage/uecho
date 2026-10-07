#!/usr/bin/env python3
##################################################################
#
# uEcho for ObjC
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################
"""Generates CGEcho.xcodeproj.

The framework compiles the uEcho C sources directly, so a single target
builds for iOS, the iOS Simulator and macOS. Re-run this script after
adding or removing source files instead of editing project.pbxproj by hand:

    python3 wrapper/objc/xcode/generate_xcodeproj.py

The CGEchoSmoke scheme runs the network smoke test. Pass
--smoke-address IP to target a node on another host; the generated scheme
then sets CGECHO_SMOKE_ADDRESS (keep such a scheme out of commits).
"""

import argparse
import hashlib
import os
import sys
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", "..", ".."))
OBJC_DIR = os.path.normpath(os.path.join(HERE, "..", "uEcho"))
TESTS_DIR = os.path.join(HERE, "Tests")
SRC_DIR = os.path.join(ROOT, "src")
PROJECT = os.path.join(HERE, "CGEcho.xcodeproj")

IOS_TARGET = "15.4"
MACOS_TARGET = "12.0"

LIGHT_FILES = ["main.c", "lighting_dev.c", "lighting_dev.h"]
FW_C_FLAGS = "COMPILER_FLAGS = \"-Wno-strict-prototypes -Wno-shorten-64-to-32\"; "
LIGHT_C_FLAGS = "COMPILER_FLAGS = \"-Wno-strict-prototypes -Wno-shorten-64-to-32 -Wno-format\"; "

BUILD_PHASE = "{isa = %s; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }"
NATIVE_TARGET = (
    "{isa = PBXNativeTarget; buildConfigurationList = %s; buildPhases = (%s); buildRules = (); "
    "dependencies = (%s); name = %s; productName = %s; productReference = %s; productType = \"%s\"; }"
)
BUILDABLE_REF = (
    '<BuildableReference BuildableIdentifier = "primary" BlueprintIdentifier = "%s" BuildableName = "%s" '
    'BlueprintName = "%s" ReferencedContainer = "container:CGEcho.xcodeproj"></BuildableReference>'
)

FILE_TYPES = {
    ".h": "sourcecode.c.h",
    ".m": "sourcecode.c.objc",
    ".c": "sourcecode.c.c",
}

objects = {}


def uid(*parts):
    """Deterministic 24-hex-digit object ID; the hash is only a naming scheme, not a security control."""
    return hashlib.sha256("/".join(parts).encode()).hexdigest()[:24].upper()


def q(s):
    if s and all(c.isalnum() or c in "._/$" for c in s):
        return s
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'


def files(directory, exts, recursive=False):
    root = Path(directory)
    candidates = root.rglob("*") if recursive else root.iterdir()
    return sorted(str(p.relative_to(root)) for p in candidates if p.is_file() and p.suffix in exts)


def add(oid, body):
    objects[oid] = body


def file_ref(group, rel):
    oid = uid("ref", group, rel)
    ext = os.path.splitext(rel)[1]
    add(oid, "{isa = PBXFileReference; lastKnownFileType = %s; path = %s; sourceTree = \"<group>\"; }" % (FILE_TYPES[ext], q(rel)))
    return oid


def product_ref(name, file_type):
    oid = uid("product", name)
    add(oid, "{isa = PBXFileReference; explicitFileType = %s; includeInIndex = 0; path = %s; sourceTree = BUILT_PRODUCTS_DIR; }" % (q(file_type), name))
    return oid


def group(name, path, children):
    oid = uid("group", name)
    path_part = ("path = %s; " % q(path)) if path else ""
    add(oid, "{isa = PBXGroup; children = (%s); %sname = %s; sourceTree = \"<group>\"; }" % (", ".join(children), path_part, q(name)))
    return oid


def build_file(target, ref, settings=None):
    oid = uid("build", target, ref)
    extra = (" settings = {%s};" % settings) if settings else ""
    add(oid, "{isa = PBXBuildFile; fileRef = %s;%s }" % (ref, extra))
    return oid


def build_phase(isa, target, kind, file_ids):
    oid = uid("phase", target, kind)
    add(oid, BUILD_PHASE % (isa, ", ".join(file_ids)))
    return oid


def config_list(name, debug, release):
    ids = []
    for conf, settings in (("Debug", debug), ("Release", release)):
        cid = uid("config", name, conf)
        body = "".join("\t\t\t\t%s = %s;\n" % (k, v) for k, v in sorted(settings.items()))
        add(cid, "{isa = XCBuildConfiguration; buildSettings = {\n%s\t\t\t}; name = %s; }" % (body, conf))
        ids.append(cid)
    lid = uid("configlist", name)
    add(lid, "{isa = XCConfigurationList; buildConfigurations = (%s); defaultConfigurationIsVisible = 0; defaultConfigurationName = Release; }" % ", ".join(ids))
    return lid


def native_target(name, settings, phases, product, product_type, dependencies=()):
    configs = config_list(name, dict(settings), dict(settings))
    oid = uid("target", name)
    add(oid, NATIVE_TARGET % (configs, ", ".join(phases), ", ".join(dependencies), name, name, product, product_type))
    return oid


class Sources:
    """Source files found on disk and their file references."""

    def __init__(self):
        private_dir = os.path.join(OBJC_DIR, "Private")
        self.public_headers = files(OBJC_DIR, {".h"})
        self.objc_sources = files(OBJC_DIR, {".m"})
        self.private_headers = files(private_dir, {".h"})
        self.private_sources = files(private_dir, {".m"})
        self.c_sources = files(SRC_DIR, {".c"}, recursive=True)
        self.c_headers = files(SRC_DIR, {".h"}, recursive=True)
        self.test_files = files(TESTS_DIR, {".h", ".m"})

        self.pub_refs = {f: file_ref("uEcho", f) for f in self.public_headers + self.objc_sources}
        self.prv_refs = {f: file_ref("Private", f) for f in self.private_headers + self.private_sources}
        self.c_refs = {f: file_ref("libuecho", f) for f in self.c_sources + self.c_headers}
        self.test_refs = {f: file_ref("Tests", f) for f in self.test_files}
        self.light_refs = {f: file_ref("uecholight", f) for f in LIGHT_FILES}


def add_groups(src, products):
    private_group = group("Private", "Private", [src.prv_refs[f] for f in sorted(src.prv_refs)])
    objc_group = group("uEcho", "../uEcho", [src.pub_refs[f] for f in sorted(src.pub_refs)] + [private_group])
    c_group = group("libuecho", "../../../src", [src.c_refs[f] for f in sorted(src.c_refs)])
    tests_group = group("Tests", "Tests", [src.test_refs[f] for f in sorted(src.test_refs)])
    light_group = group("uecholight", "../../../examples/device/uecholight", [src.light_refs[f] for f in LIGHT_FILES])
    products_group = group("Products", None, products)
    main_group = uid("group", "main")
    add(main_group, "{isa = PBXGroup; children = (%s); sourceTree = \"<group>\"; }" % ", ".join([objc_group, c_group, tests_group, light_group, products_group]))
    return main_group, products_group


def add_framework_target(src, product):
    headers = [build_file("CGEcho", src.pub_refs[f], "ATTRIBUTES = (Public, ); ") for f in src.public_headers]
    headers += [build_file("CGEcho", src.prv_refs[f]) for f in src.private_headers]
    sources = [build_file("CGEcho", src.pub_refs[f]) for f in src.objc_sources]
    sources += [build_file("CGEcho", src.prv_refs[f]) for f in src.private_sources]
    sources += [build_file("CGEcho", src.c_refs[f], FW_C_FLAGS) for f in src.c_sources]
    phases = [
        build_phase("PBXHeadersBuildPhase", "CGEcho", "headers", headers),
        build_phase("PBXSourcesBuildPhase", "CGEcho", "sources", sources),
        build_phase("PBXFrameworksBuildPhase", "CGEcho", "frameworks", []),
    ]
    settings = {
        "PRODUCT_NAME": "CGEcho",
        "PRODUCT_BUNDLE_IDENTIFIER": "org.cybergarage.CGEcho",
        "DEFINES_MODULE": "YES",
        "GENERATE_INFOPLIST_FILE": "YES",
        "CURRENT_PROJECT_VERSION": "1",
        "MARKETING_VERSION": "1.0",
        "DYLIB_COMPATIBILITY_VERSION": "1",
        "DYLIB_CURRENT_VERSION": "1",
        "DYLIB_INSTALL_NAME_BASE": q("@rpath"),
        "INSTALL_PATH": q("$(LOCAL_LIBRARY_DIR)/Frameworks"),
        "SKIP_INSTALL": "YES",
        "LD_RUNPATH_SEARCH_PATHS": q("$(inherited) @executable_path/Frameworks @loader_path/Frameworks"),
        q("LD_RUNPATH_SEARCH_PATHS[sdk=macosx*]"): q("$(inherited) @executable_path/../Frameworks @loader_path/Frameworks"),
    }
    return native_target("CGEcho", settings, phases, product, "com.apple.product-type.framework")


def add_test_target(src, product, fw_product, fw_target, project_id):
    sources = [build_file("CGEchoTests", src.test_refs[f]) for f in src.test_files if f.endswith(".m")]
    phases = [
        build_phase("PBXSourcesBuildPhase", "CGEchoTests", "sources", sources),
        build_phase("PBXFrameworksBuildPhase", "CGEchoTests", "frameworks", [build_file("CGEchoTests", fw_product)]),
    ]
    proxy = uid("proxy", "CGEcho")
    add(proxy, "{isa = PBXContainerItemProxy; containerPortal = %s; proxyType = 1; remoteGlobalIDString = %s; remoteInfo = CGEcho; }" % (project_id, fw_target))
    dependency = uid("dependency", "CGEcho")
    add(dependency, "{isa = PBXTargetDependency; target = %s; targetProxy = %s; }" % (fw_target, proxy))
    settings = {
        "PRODUCT_NAME": "CGEchoTests",
        "PRODUCT_BUNDLE_IDENTIFIER": "org.cybergarage.CGEchoTests",
        "GENERATE_INFOPLIST_FILE": "YES",
        "HEADER_SEARCH_PATHS": q("$(inherited) $(SRCROOT)/../uEcho/Private"),
        "LD_RUNPATH_SEARCH_PATHS": q("$(inherited) @executable_path/Frameworks @loader_path/Frameworks"),
        q("LD_RUNPATH_SEARCH_PATHS[sdk=macosx*]"): q("$(inherited) @executable_path/../Frameworks @loader_path/../Frameworks"),
    }
    return native_target("CGEchoTests", settings, phases, product, "com.apple.product-type.bundle.unit-test", [dependency])


def add_light_target(src, product):
    """uecholight: a macOS ECHONET Lite lighting node for the network smoke test."""
    sources = [build_file("uecholight", src.light_refs[f], LIGHT_C_FLAGS) for f in LIGHT_FILES if f.endswith(".c")]
    sources += [build_file("uecholight", src.c_refs[f], LIGHT_C_FLAGS) for f in src.c_sources]
    phases = [build_phase("PBXSourcesBuildPhase", "uecholight", "sources", sources)]
    settings = {
        "PRODUCT_NAME": "uecholight",
        "SDKROOT": "macosx",
        "SUPPORTED_PLATFORMS": "macosx",
        "SKIP_INSTALL": "YES",
    }
    return native_target("uecholight", settings, phases, product, "com.apple.product-type.tool")


def project_settings():
    base = {
        "ALWAYS_SEARCH_USER_PATHS": "NO",
        "CLANG_ENABLE_MODULES": "YES",
        "CLANG_ENABLE_OBJC_ARC": "YES",
        "CLANG_ENABLE_OBJC_WEAK": "YES",
        "CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING": "YES",
        "CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF": "YES",
        "CLANG_WARN_DOCUMENTATION_COMMENTS": "YES",
        "CODE_SIGN_IDENTITY": q("-"),
        "CODE_SIGN_STYLE": "Automatic",
        "GCC_C_LANGUAGE_STANDARD": "gnu11",
        "GCC_WARN_UNUSED_VARIABLE": "YES",
        "HEADER_SEARCH_PATHS": q("$(SRCROOT)/../../../include $(SRCROOT)/../../../src"),
        "IPHONEOS_DEPLOYMENT_TARGET": IOS_TARGET,
        "MACOSX_DEPLOYMENT_TARGET": MACOS_TARGET,
        "SDKROOT": "auto",
        "SUPPORTED_PLATFORMS": q("iphoneos iphonesimulator macosx"),
        "SUPPORTS_MACCATALYST": "NO",
        "TARGETED_DEVICE_FAMILY": q("1,2"),
    }
    debug = dict(base, **{
        "DEBUG_INFORMATION_FORMAT": "dwarf",
        "ENABLE_TESTABILITY": "YES",
        "GCC_OPTIMIZATION_LEVEL": "0",
        "GCC_PREPROCESSOR_DEFINITIONS": q("DEBUG=1 $(inherited)"),
        "ONLY_ACTIVE_ARCH": "YES",
    })
    release = dict(base, **{
        "DEBUG_INFORMATION_FORMAT": q("dwarf-with-dsym"),
        "ENABLE_NS_ASSERTIONS": "NO",
    })
    return debug, release


def write_pbxproj(project_id):
    os.makedirs(PROJECT, exist_ok=True)
    with open(os.path.join(PROJECT, "project.pbxproj"), "w") as f:
        f.write("// !$*UTF8*$!\n{\n\tarchiveVersion = 1;\n\tclasses = {\n\t};\n\tobjectVersion = 56;\n\tobjects = {\n")
        for oid in sorted(objects):
            f.write("\t\t%s = %s;\n" % (oid, objects[oid]))
        f.write("\t};\n\trootObject = %s;\n}\n" % project_id)


SCHEME = """<?xml version="1.0" encoding="UTF-8"?>
<Scheme LastUpgradeVersion = "1600" version = "1.7">
   <BuildAction parallelizeBuildables = "YES" buildImplicitDependencies = "YES">
      <BuildActionEntries>
         <BuildActionEntry buildForTesting = "YES" buildForRunning = "YES" buildForProfiling = "YES" buildForArchiving = "YES" buildForAnalyzing = "YES">
            %s
         </BuildActionEntry>
      </BuildActionEntries>
   </BuildAction>
   <TestAction buildConfiguration = "Debug" selectedDebuggerIdentifier = "Xcode.DebuggerFoundation.Debugger.LLDB" selectedLauncherIdentifier = "Xcode.DebuggerFoundation.Launcher.LLDB" shouldUseLaunchSchemeArgsEnv = "%s">
      <Testables>%s</Testables>
      %s
   </TestAction>
   <LaunchAction buildConfiguration = "Debug" selectedDebuggerIdentifier = "Xcode.DebuggerFoundation.Debugger.LLDB" selectedLauncherIdentifier = "Xcode.DebuggerFoundation.Launcher.LLDB" launchStyle = "0" useCustomWorkingDirectory = "NO" ignoresPersistentStateOnLaunch = "NO" debugDocumentVersioning = "YES" debugServiceExtension = "internal" allowLocationSimulation = "YES">
      %s
   </LaunchAction>
   <ArchiveAction buildConfiguration = "Release" revealArchiveInOrganizer = "YES">
   </ArchiveAction>
</Scheme>
"""


def buildable_ref(target, name, product):
    return BUILDABLE_REF % (target, product, name)


def testables_xml(testable_ref, selected_tests):
    if not testable_ref:
        return ""
    if not selected_tests:
        return '<TestableReference skipped = "NO">%s</TestableReference>' % testable_ref
    selection = "".join('<Test Identifier = "%s"></Test>' % t for t in selected_tests)
    return '<TestableReference skipped = "NO" useTestSelectionWhitelist = "YES">%s<SelectedTests>%s</SelectedTests></TestableReference>' % (testable_ref, selection)


def environment_xml(env):
    if not env:
        return ""
    return "<EnvironmentVariables>%s</EnvironmentVariables>" % "".join(
        '<EnvironmentVariable key = "%s" value = "%s" isEnabled = "YES"></EnvironmentVariable>' % kv for kv in env.items())


def runnable_xml(launch_ref, launch_args):
    if not launch_ref:
        return ""
    args = "".join('<CommandLineArgument argument = "%s" isEnabled = "YES"></CommandLineArgument>' % a for a in launch_args or [])
    runnable = '<BuildableProductRunnable runnableDebuggingMode = "0">%s</BuildableProductRunnable>' % launch_ref
    return runnable + ("<CommandLineArguments>%s</CommandLineArguments>" % args if args else "")


def write_scheme(name, build_ref, testable_ref=None, selected_tests=None, test_env=None, launch_ref=None, launch_args=None):
    scheme_dir = os.path.join(PROJECT, "xcshareddata", "xcschemes")
    os.makedirs(scheme_dir, exist_ok=True)
    body = SCHEME % (
        build_ref,
        "NO" if test_env else "YES",
        testables_xml(testable_ref, selected_tests),
        environment_xml(test_env),
        runnable_xml(launch_ref, launch_args),
    )
    with open(os.path.join(scheme_dir, name + ".xcscheme"), "w") as f:
        f.write(body)


def main():
    parser = argparse.ArgumentParser(description="Generate CGEcho.xcodeproj")
    parser.add_argument("--smoke-address", help="ECHONET Lite node address used by the CGEchoSmoke scheme")
    args = parser.parse_args()

    src = Sources()
    fw_product = product_ref("CGEcho.framework", "wrapper.framework")
    test_product = product_ref("CGEchoTests.xctest", "wrapper.cfbundle")
    light_product = product_ref("uecholight", "compiled.mach-o.executable")
    main_group, products_group = add_groups(src, [fw_product, test_product, light_product])

    project_id = uid("project")
    fw_target = add_framework_target(src, fw_product)
    test_target = add_test_target(src, test_product, fw_product, fw_target, project_id)
    light_target = add_light_target(src, light_product)

    debug, release = project_settings()
    project_configs = config_list("project", debug, release)
    add(project_id, "{isa = PBXProject; attributes = {LastUpgradeCheck = 1600; }; buildConfigurationList = %s; compatibilityVersion = \"Xcode 14.0\"; developmentRegion = en; hasScannedForEncodings = 0; knownRegions = (en, Base, ); mainGroup = %s; productRefGroup = %s; projectDirPath = \"\"; projectRoot = \"\"; targets = (%s, %s, %s); }" % (project_configs, main_group, products_group, fw_target, test_target, light_target))
    write_pbxproj(project_id)

    fw_ref = buildable_ref(fw_target, "CGEcho", "CGEcho.framework")
    tests_ref = buildable_ref(test_target, "CGEchoTests", "CGEchoTests.xctest")
    light_ref = buildable_ref(light_target, "uecholight", "uecholight")
    smoke_env = {"CGECHO_SMOKE": "1"}
    if args.smoke_address:
        smoke_env["CGECHO_SMOKE_ADDRESS"] = args.smoke_address
    write_scheme("CGEcho", fw_ref, tests_ref)
    write_scheme("CGEchoSmoke", fw_ref, tests_ref, selected_tests=["CGEchoSmokeTests"], test_env=smoke_env)
    write_scheme("uecholight", light_ref, launch_ref=light_ref, launch_args=["-v"])

    print("Generated %s (%d C sources, %d ObjC sources, %d test files)" % (
        os.path.relpath(PROJECT, ROOT), len(src.c_sources), len(src.objc_sources) + len(src.private_sources), len(src.test_files)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
