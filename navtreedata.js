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
      [ "What is uEcho?", "index.html#autotoc_md55", null ]
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
    [ "ESP32 (ESP-IDF)", "md_doc_espidf.html", [
      [ "Requirements", "md_doc_espidf.html#autotoc_md33", null ],
      [ "Using uecho in your project", "md_doc_espidf.html#autotoc_md34", null ],
      [ "Required configuration", "md_doc_espidf.html#autotoc_md35", [
        [ "uEcho options (<tt>idf.py menuconfig</tt> → <em>Component config</em> → <em>uEcho</em>)", "md_doc_espidf.html#autotoc_md36", null ]
      ] ],
      [ "Starting a node", "md_doc_espidf.html#autotoc_md37", null ],
      [ "Writing a device", "md_doc_espidf.html#autotoc_md38", null ],
      [ "Example: uecholight", "md_doc_espidf.html#autotoc_md39", [
        [ "Configure", "md_doc_espidf.html#autotoc_md40", null ],
        [ "Build, flash and monitor", "md_doc_espidf.html#autotoc_md41", null ],
        [ "Verify from a host", "md_doc_espidf.html#autotoc_md42", null ]
      ] ],
      [ "Memory usage", "md_doc_espidf.html#autotoc_md43", null ],
      [ "Platform differences", "md_doc_espidf.html#autotoc_md44", null ],
      [ "Troubleshooting", "md_doc_espidf.html#autotoc_md45", null ]
    ] ],
    [ "Examples for ECHONET Lite Controller", "md_doc_examples.html", [
      [ "uechosearch", "md_doc_examples.html#autotoc_md47", null ],
      [ "uechopost", "md_doc_examples.html#autotoc_md48", null ],
      [ "uechodump", "md_doc_examples.html#autotoc_md49", null ],
      [ "Examples for ECHONET Lite Devices", "md_doc_examples.html#autotoc_md50", [
        [ "uecholight", "md_doc_examples.html#autotoc_md51", null ],
        [ "uecholight for ESP32", "md_doc_examples.html#autotoc_md52", null ]
      ] ],
      [ "References", "md_doc_examples.html#autotoc_md53", null ]
    ] ],
    [ "Building and Installation", "md_doc_setup.html", [
      [ "Homebrew (macOS, Linux)", "md_doc_setup.html#autotoc_md57", null ],
      [ "Installing from Source", "md_doc_setup.html#autotoc_md58", null ],
      [ "ESP32 (ESP-IDF)", "md_doc_setup.html#autotoc_md59", null ]
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
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"__class_8h.html",
"controller_8c.html#ae960567b4dd1fc4a7f0a77081590f298",
"include_2uecho_2message_8h.html#afdf949381c2486b99cb37527744e578e",
"md_doc_controller_inside.html#autotoc_md3",
"node__list_8c.html",
"property_8c.html#a8b24a2689f46b69c4394c02b711a6afa",
"src_2uecho_2frame_2message_8h.html#a21e947dd28770601f5693cc779325d0d",
"structUEchoSocket.html#ac271032d92834f5b1efc9a4ed2197034"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';