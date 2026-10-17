/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "uEcho for C", "index.html", [
    [ "Overview", "index.html", [
      [ "What is uEcho?", "index.html#autotoc_md74", null ]
    ] ],
    [ "Inside of uEcho Controller", "md_doc_controller_inside.html", [
      [ "Node Profile Object", "md_doc_controller_inside.html#autotoc_md1", null ],
      [ "Controller Message Listeners", "md_doc_controller_inside.html#autotoc_md2", null ],
      [ "References", "md_doc_controller_inside.html#autotoc_md3", null ]
    ] ],
    [ "Overview of uEcho Controller", "md_doc_controller_overview.html", [
      [ "Creating Controller", "md_doc_controller_overview.html#autotoc_md5", [
        [ "1. Starting Controller", "md_doc_controller_overview.html#autotoc_md6", null ],
        [ "2. Searching Nodes", "md_doc_controller_overview.html#autotoc_md7", null ],
        [ "3. Getting Nodes and Objects", "md_doc_controller_overview.html#autotoc_md8", null ],
        [ "4. Creating Request Message", "md_doc_controller_overview.html#autotoc_md9", null ],
        [ "5. Sending Messages", "md_doc_controller_overview.html#autotoc_md10", null ]
      ] ],
      [ "Next Steps", "md_doc_controller_overview.html#autotoc_md11", null ]
    ] ],
    [ "Inside of uEcho Device", "md_doc_device_inside.html", [
      [ "Node Profile Object", "md_doc_device_inside.html#autotoc_md13", null ],
      [ "Device Object Super Class", "md_doc_device_inside.html#autotoc_md14", null ],
      [ "Device Message Handler and Listener", "md_doc_device_inside.html#autotoc_md15", [
        [ "Property Message Handler", "md_doc_device_inside.html#autotoc_md16", null ],
        [ "Node Message Listener", "md_doc_device_inside.html#autotoc_md17", null ],
        [ "Object Message Listener", "md_doc_device_inside.html#autotoc_md18", null ]
      ] ],
      [ "Supported Basic Sequences", "md_doc_device_inside.html#autotoc_md19", [
        [ "4.2.1 Basic Sequences for Service Content", "md_doc_device_inside.html#autotoc_md20", null ],
        [ "4.2.2 Basic Sequences for Object Control in General", "md_doc_device_inside.html#autotoc_md21", null ]
      ] ],
      [ "References", "md_doc_device_inside.html#autotoc_md22", null ]
    ] ],
    [ "Overview of uEcho Device", "md_doc_device_overview.html", [
      [ "Making Devices", "md_doc_device_overview.html#autotoc_md24", null ],
      [ "Creating Devices", "md_doc_device_overview.html#autotoc_md25", [
        [ "1. Creating Node", "md_doc_device_overview.html#autotoc_md26", null ],
        [ "2. Creating Device Object", "md_doc_device_overview.html#autotoc_md27", null ],
        [ "3. Handling Request Messages", "md_doc_device_overview.html#autotoc_md28", null ],
        [ "4. Starting Node", "md_doc_device_overview.html#autotoc_md29", null ]
      ] ],
      [ "Next Steps", "md_doc_device_overview.html#autotoc_md30", null ],
      [ "References", "md_doc_device_overview.html#autotoc_md31", null ]
    ] ],
    [ "Scalar EDT codec", "md_doc_edt_codec.html", [
      [ "C usage", "md_doc_edt_codec.html#autotoc_md33", null ],
      [ "HomeKit / Objective-C migration", "md_doc_edt_codec.html#autotoc_md34", null ],
      [ "Supported constraints and explicit limits", "md_doc_edt_codec.html#autotoc_md35", null ]
    ] ],
    [ "ESP32 (ESP-IDF)", "md_doc_espidf.html", [
      [ "Requirements", "md_doc_espidf.html#autotoc_md37", null ],
      [ "Using uecho in your project", "md_doc_espidf.html#autotoc_md38", null ],
      [ "Required configuration", "md_doc_espidf.html#autotoc_md39", [
        [ "uEcho options (<tt>idf.py menuconfig</tt> → <em>Component config</em> → <em>uEcho</em>)", "md_doc_espidf.html#autotoc_md40", null ]
      ] ],
      [ "Starting a node", "md_doc_espidf.html#autotoc_md41", null ],
      [ "Writing a device", "md_doc_espidf.html#autotoc_md42", null ],
      [ "Example: uecholight", "md_doc_espidf.html#autotoc_md43", [
        [ "Configure", "md_doc_espidf.html#autotoc_md44", null ],
        [ "Build, flash and monitor", "md_doc_espidf.html#autotoc_md45", null ],
        [ "Verify from a host", "md_doc_espidf.html#autotoc_md46", null ]
      ] ],
      [ "Standard object database", "md_doc_espidf.html#autotoc_md47", null ],
      [ "Memory usage", "md_doc_espidf.html#autotoc_md48", null ],
      [ "Platform differences", "md_doc_espidf.html#autotoc_md49", null ],
      [ "Troubleshooting", "md_doc_espidf.html#autotoc_md50", null ]
    ] ],
    [ "Examples for ECHONET Lite Controller", "md_doc_examples.html", [
      [ "uechosearch", "md_doc_examples.html#autotoc_md52", null ],
      [ "uechopost", "md_doc_examples.html#autotoc_md53", null ],
      [ "uechodump", "md_doc_examples.html#autotoc_md54", null ],
      [ "Examples for ECHONET Lite Devices", "md_doc_examples.html#autotoc_md55", [
        [ "uecholight", "md_doc_examples.html#autotoc_md56", null ],
        [ "uecholight for ESP32", "md_doc_examples.html#autotoc_md57", null ]
      ] ],
      [ "References", "md_doc_examples.html#autotoc_md58", null ]
    ] ],
    [ "Objective-C and Swift (CGEcho.framework)", "md_doc_objc.html", [
      [ "Requirements", "md_doc_objc.html#autotoc_md60", null ],
      [ "Project layout", "md_doc_objc.html#autotoc_md61", null ],
      [ "Building and testing", "md_doc_objc.html#autotoc_md62", [
        [ "Network smoke test", "md_doc_objc.html#autotoc_md63", null ]
      ] ],
      [ "Usage", "md_doc_objc.html#autotoc_md64", [
        [ "Swift", "md_doc_objc.html#autotoc_md65", null ],
        [ "Objective-C", "md_doc_objc.html#autotoc_md66", null ],
        [ "Notifications", "md_doc_objc.html#autotoc_md67", null ]
      ] ],
      [ "Behavior", "md_doc_objc.html#autotoc_md68", null ],
      [ "iOS apps", "md_doc_objc.html#autotoc_md69", null ],
      [ "SwiftUI sample", "md_doc_objc.html#autotoc_md70", null ],
      [ "Standard metadata (additive API)", "md_doc_objc.html#autotoc_md71", [
        [ "Optional value schemas", "md_doc_objc.html#autotoc_md72", null ]
      ] ]
    ] ],
    [ "Building and Installation", "md_doc_setup.html", [
      [ "Homebrew (macOS, Linux)", "md_doc_setup.html#autotoc_md76", null ],
      [ "Installing from Source", "md_doc_setup.html#autotoc_md77", [
        [ "Standard object database", "md_doc_setup.html#autotoc_md78", null ]
      ] ],
      [ "ESP32 (ESP-IDF)", "md_doc_setup.html#autotoc_md79", null ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"__class_8h.html",
"controller_8c.html#a8b6a65066125cbda0fecc46464e8e540",
"include_2uecho_2message_8h.html#aa57c6180c98ea36577fa6ef009928e5a",
"mcast__server_8c.html#a16fa55f00a4e8441b93e10b18a9f8201",
"node_8c.html#aafa52cd10fd3abb8ca15aedc2d7acf6a",
"objects__mra_8c.html#a09b15f6e52a090bd9b23998c06fd5ae1",
"objects__mra_8c.html#a517f79ad9e2302267b0f43073517828b",
"objects__mra_8c.html#a95bf02cd9900b33b3fae6ab55545a6c7",
"objects__mra_8c.html#ae01837b8a19c85e58406b2c751967bed",
"server_8c.html#aa091f6469b3834990e5e96df3913bf76",
"strings_8c.html#a45eb5232b0d834debb502bab38290526",
"structuEchoProperty.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';