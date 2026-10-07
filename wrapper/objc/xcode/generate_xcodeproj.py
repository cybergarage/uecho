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

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", "..", ".."))
OBJC_DIR = os.path.normpath(os.path.join(HERE, "..", "uEcho"))
TESTS_DIR = os.path.join(HERE, "Tests")
SRC_DIR = os.path.join(ROOT, "src")
LIGHT_DIR = os.path.join(ROOT, "examples", "device", "uecholight")
PROJECT = os.path.join(HERE, "CGEcho.xcodeproj")

IOS_TARGET = "15.4"
MACOS_TARGET = "12.0"


def uid(*parts):
    return hashlib.md5("/".join(parts).encode()).hexdigest()[:24].upper()


def q(s):
    if s and all(c.isalnum() or c in "._/$" for c in s):
        return s
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'


def files(directory, exts, recursive=False):
    found = []
    if recursive:
        for base, _, names in os.walk(directory):
            for name in names:
                if os.path.splitext(name)[1] in exts:
                    found.append(os.path.relpath(os.path.join(base, name), directory))
    else:
        for name in os.listdir(directory):
            if os.path.splitext(name)[1] in exts and os.path.isfile(os.path.join(directory, name)):
                found.append(name)
    return sorted(found)


FILE_TYPES = {
    ".h": "sourcecode.c.h",
    ".m": "sourcecode.c.objc",
    ".c": "sourcecode.c.c",
}

objects = {}


def add(oid, body):
    objects[oid] = body


def file_ref(group, rel):
    oid = uid("ref", group, rel)
    ext = os.path.splitext(rel)[1]
    add(oid, "{isa = PBXFileReference; lastKnownFileType = %s; path = %s; sourceTree = \"<group>\"; }" % (FILE_TYPES[ext], q(rel)))
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


def main():
    parser = argparse.ArgumentParser(description="Generate CGEcho.xcodeproj")
    parser.add_argument("--smoke-address", help="ECHONET Lite node address used by the CGEchoSmoke scheme")
    args = parser.parse_args()
    public_headers = files(OBJC_DIR, {".h"})
    objc_sources = files(OBJC_DIR, {".m"})
    private_headers = files(os.path.join(OBJC_DIR, "Private"), {".h"})
    private_sources = files(os.path.join(OBJC_DIR, "Private"), {".m"})
    c_sources = files(SRC_DIR, {".c"}, recursive=True)
    c_headers = files(SRC_DIR, {".h"}, recursive=True)
    test_files = files(TESTS_DIR, {".h", ".m"})

    # File references and groups.
    pub_refs = {f: file_ref("uEcho", f) for f in public_headers + objc_sources}
    prv_refs = {f: file_ref("Private", f) for f in private_headers + private_sources}
    c_refs = {f: file_ref("libuecho", f) for f in c_sources + c_headers}
    test_refs = {f: file_ref("Tests", f) for f in test_files}
    light_files = ["main.c", "lighting_dev.c", "lighting_dev.h"]
    light_refs = {f: file_ref("uecholight", f) for f in light_files}

    fw_product = uid("product", "CGEcho.framework")
    add(fw_product, "{isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = CGEcho.framework; sourceTree = BUILT_PRODUCTS_DIR; }")
    test_product = uid("product", "CGEchoTests.xctest")
    light_product = uid("product", "uecholight")
    add(light_product, "{isa = PBXFileReference; explicitFileType = \"compiled.mach-o.executable\"; includeInIndex = 0; path = uecholight; sourceTree = BUILT_PRODUCTS_DIR; }")
    add(test_product, "{isa = PBXFileReference; explicitFileType = wrapper.cfbundle; includeInIndex = 0; path = CGEchoTests.xctest; sourceTree = BUILT_PRODUCTS_DIR; }")

    private_group = group("Private", "Private", [prv_refs[f] for f in sorted(prv_refs)])
    objc_group = group("uEcho", "../uEcho", [pub_refs[f] for f in sorted(pub_refs)] + [private_group])
    c_group = group("libuecho", "../../../src", [c_refs[f] for f in sorted(c_refs)])
    tests_group = group("Tests", "Tests", [test_refs[f] for f in sorted(test_refs)])
    light_group = group("uecholight", "../../../examples/device/uecholight", [light_refs[f] for f in light_files])
    products_group = group("Products", None, [fw_product, test_product, light_product])
    main_group = uid("group", "main")
    add(main_group, "{isa = PBXGroup; children = (%s); sourceTree = \"<group>\"; }" % ", ".join([objc_group, c_group, tests_group, light_group, products_group]))

    # Framework target.
    fw_headers = [build_file("CGEcho", pub_refs[f], "ATTRIBUTES = (Public, ); ") for f in public_headers]
    fw_headers += [build_file("CGEcho", prv_refs[f]) for f in private_headers]
    fw_sources = [build_file("CGEcho", pub_refs[f]) for f in objc_sources]
    fw_sources += [build_file("CGEcho", prv_refs[f]) for f in private_sources]
    fw_sources += [build_file("CGEcho", c_refs[f], "COMPILER_FLAGS = \"-Wno-strict-prototypes -Wno-shorten-64-to-32\"; ") for f in c_sources]

    fw_phase_headers = uid("phase", "CGEcho", "headers")
    add(fw_phase_headers, "{isa = PBXHeadersBuildPhase; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }" % ", ".join(fw_headers))
    fw_phase_sources = uid("phase", "CGEcho", "sources")
    add(fw_phase_sources, "{isa = PBXSourcesBuildPhase; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }" % ", ".join(fw_sources))
    fw_phase_frameworks = uid("phase", "CGEcho", "frameworks")
    add(fw_phase_frameworks, "{isa = PBXFrameworksBuildPhase; buildActionMask = 2147483647; files = (); runOnlyForDeploymentPostprocessing = 0; }")

    fw_common = {
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
    fw_configs = config_list("CGEcho", dict(fw_common), dict(fw_common))
    fw_target = uid("target", "CGEcho")
    add(fw_target, "{isa = PBXNativeTarget; buildConfigurationList = %s; buildPhases = (%s, %s, %s); buildRules = (); dependencies = (); name = CGEcho; productName = CGEcho; productReference = %s; productType = \"com.apple.product-type.framework\"; }" % (fw_configs, fw_phase_headers, fw_phase_sources, fw_phase_frameworks, fw_product))

    # Test target.
    test_sources = [build_file("CGEchoTests", test_refs[f]) for f in test_files if f.endswith(".m")]
    test_phase_sources = uid("phase", "CGEchoTests", "sources")
    add(test_phase_sources, "{isa = PBXSourcesBuildPhase; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }" % ", ".join(test_sources))
    test_link = build_file("CGEchoTests", fw_product)
    test_phase_frameworks = uid("phase", "CGEchoTests", "frameworks")
    add(test_phase_frameworks, "{isa = PBXFrameworksBuildPhase; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }" % test_link)

    proxy = uid("proxy", "CGEcho")
    project_id = uid("project")
    add(proxy, "{isa = PBXContainerItemProxy; containerPortal = %s; proxyType = 1; remoteGlobalIDString = %s; remoteInfo = CGEcho; }" % (project_id, fw_target))
    dependency = uid("dependency", "CGEcho")
    add(dependency, "{isa = PBXTargetDependency; target = %s; targetProxy = %s; }" % (fw_target, proxy))

    test_common = {
        "PRODUCT_NAME": "CGEchoTests",
        "PRODUCT_BUNDLE_IDENTIFIER": "org.cybergarage.CGEchoTests",
        "GENERATE_INFOPLIST_FILE": "YES",
        "HEADER_SEARCH_PATHS": q("$(inherited) $(SRCROOT)/../uEcho/Private"),
        "LD_RUNPATH_SEARCH_PATHS": q("$(inherited) @executable_path/Frameworks @loader_path/Frameworks"),
        q("LD_RUNPATH_SEARCH_PATHS[sdk=macosx*]"): q("$(inherited) @executable_path/../Frameworks @loader_path/../Frameworks"),
    }
    test_configs = config_list("CGEchoTests", dict(test_common), dict(test_common))
    test_target = uid("target", "CGEchoTests")
    add(test_target, "{isa = PBXNativeTarget; buildConfigurationList = %s; buildPhases = (%s, %s); buildRules = (); dependencies = (%s); name = CGEchoTests; productName = CGEchoTests; productReference = %s; productType = \"com.apple.product-type.bundle.unit-test\"; }" % (test_configs, test_phase_sources, test_phase_frameworks, dependency, test_product))

    # uecholight: a macOS ECHONET Lite lighting node for the network smoke test.
    c_flags = "COMPILER_FLAGS = \"-Wno-strict-prototypes -Wno-shorten-64-to-32 -Wno-format\"; "
    light_sources = [build_file("uecholight", light_refs[f], c_flags) for f in light_files if f.endswith(".c")]
    light_sources += [build_file("uecholight", c_refs[f], c_flags) for f in c_sources]
    light_phase_sources = uid("phase", "uecholight", "sources")
    add(light_phase_sources, "{isa = PBXSourcesBuildPhase; buildActionMask = 2147483647; files = (%s); runOnlyForDeploymentPostprocessing = 0; }" % ", ".join(light_sources))
    light_common = {
        "PRODUCT_NAME": "uecholight",
        "SDKROOT": "macosx",
        "SUPPORTED_PLATFORMS": "macosx",
        "SKIP_INSTALL": "YES",
    }
    light_configs = config_list("uecholight", dict(light_common), dict(light_common))
    light_target = uid("target", "uecholight")
    add(light_target, "{isa = PBXNativeTarget; buildConfigurationList = %s; buildPhases = (%s); buildRules = (); dependencies = (); name = uecholight; productName = uecholight; productReference = %s; productType = \"com.apple.product-type.tool\"; }" % (light_configs, light_phase_sources, light_product))

    # Project.
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
    project_configs = config_list("project", debug, release)
    add(project_id, "{isa = PBXProject; attributes = {LastUpgradeCheck = 1600; }; buildConfigurationList = %s; compatibilityVersion = \"Xcode 14.0\"; developmentRegion = en; hasScannedForEncodings = 0; knownRegions = (en, Base, ); mainGroup = %s; productRefGroup = %s; projectDirPath = \"\"; projectRoot = \"\"; targets = (%s, %s, %s); }" % (project_configs, main_group, products_group, fw_target, test_target, light_target))

    os.makedirs(PROJECT, exist_ok=True)
    with open(os.path.join(PROJECT, "project.pbxproj"), "w") as f:
        f.write("// !$*UTF8*$!\n{\n\tarchiveVersion = 1;\n\tclasses = {\n\t};\n\tobjectVersion = 56;\n\tobjects = {\n")
        for oid in sorted(objects):
            f.write("\t\t%s = %s;\n" % (oid, objects[oid]))
        f.write("\t};\n\trootObject = %s;\n}\n" % project_id)

    scheme_dir = os.path.join(PROJECT, "xcshareddata", "xcschemes")
    os.makedirs(scheme_dir, exist_ok=True)
    def ref(target, name, product):
        return '<BuildableReference BuildableIdentifier = "primary" BlueprintIdentifier = "%s" BuildableName = "%s" BlueprintName = "%s" ReferencedContainer = "container:CGEcho.xcodeproj"></BuildableReference>' % (target, product, name)

    def write_scheme(name, build_ref, testable_ref=None, selected_tests=None, test_env=None, launch_ref=None, launch_args=None):
        testables = ""
        if testable_ref:
            selection = ""
            if selected_tests:
                selection = "<SelectedTests>%s</SelectedTests>" % "".join('<Test Identifier = "%s"></Test>' % t for t in selected_tests)
            whitelist = ' useTestSelectionWhitelist = "YES"' if selected_tests else ""
            testables = '<TestableReference skipped = "NO"%s>%s%s</TestableReference>' % (whitelist, testable_ref, selection)
        env = ""
        if test_env:
            env = "<EnvironmentVariables>%s</EnvironmentVariables>" % "".join('<EnvironmentVariable key = "%s" value = "%s" isEnabled = "YES"></EnvironmentVariable>' % kv for kv in test_env.items())
        runnable = ""
        if launch_ref:
            runnable = '<BuildableProductRunnable runnableDebuggingMode = "0">%s</BuildableProductRunnable>' % launch_ref
            if launch_args:
                runnable += "<CommandLineArguments>%s</CommandLineArguments>" % "".join('<CommandLineArgument argument = "%s" isEnabled = "YES"></CommandLineArgument>' % a for a in launch_args)
        with open(os.path.join(scheme_dir, name + ".xcscheme"), "w") as f:
            f.write("""<?xml version="1.0" encoding="UTF-8"?>
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
""" % (build_ref, "NO" if test_env else "YES", testables, env, runnable))

    fw_ref = ref(fw_target, "CGEcho", "CGEcho.framework")
    tests_ref = ref(test_target, "CGEchoTests", "CGEchoTests.xctest")
    light_ref = ref(light_target, "uecholight", "uecholight")
    write_scheme("CGEcho", fw_ref, tests_ref)
    write_scheme("CGEchoSmoke", fw_ref, tests_ref, selected_tests=["CGEchoSmokeTests"], test_env=dict({"CGECHO_SMOKE": "1"}, **({"CGECHO_SMOKE_ADDRESS": args.smoke_address} if args.smoke_address else {})))
    write_scheme("uecholight", light_ref, launch_ref=light_ref, launch_args=["-v"])

    print("Generated %s (%d C sources, %d ObjC sources, %d test files)" % (os.path.relpath(PROJECT, ROOT), len(c_sources), len(objc_sources) + len(private_sources), len(test_files)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
