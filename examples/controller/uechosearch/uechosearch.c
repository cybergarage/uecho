/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdio.h>
#include <unistd.h>

#include <uecho/uecho.h>

#define UNKNOWN_STRING "?"
#define SEARCH_WAIT_MTIME 200

void usage()
{
  printf("Usage : uechosearch [options]\n");
  printf(" -v : Enable verbose output\n");
  printf(" -h : Print this message\n");
  printf(" -d : Enable debug output\n");
}

uEchoMessage* create_readpropertymessagebycode(int objCode, byte propCode)
{
  uEchoMessage* msg;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, objCode);
  uecho_message_setproperty(msg, propCode, NULL, 0);
  return msg;
}

uEchoMessage* create_readpropertymessage(uEchoProperty* prop)
{
  return create_readpropertymessagebycode(
      uecho_object_getcode(uecho_property_getparentobject(prop)),
      uecho_property_getcode(prop));
}

uEchoMessage* create_readmanufacturecodemessage()
{
  return create_readpropertymessagebycode(
      0x0EF001,
      0x8A);
}

const char* get_nodemanufacturename(uEchoController* ctrl, uEchoDatabase* db, uEchoNode* node)
{
  uEchoMessage* reqMsg;
  uEchoMessage* resMsg;
  uEchoProperty* resProp;
  uEchoManufacture* manufacture;
  const char* manufactureName;
  int manufactureCode;

  manufactureName = NULL;
  reqMsg = create_readmanufacturecodemessage();
  resMsg = uecho_message_new();
  if (uecho_controller_postmessage(ctrl, node, reqMsg, resMsg) && (uecho_message_getopc(resMsg) == 1)) {
    resProp = uecho_message_getproperty(resMsg, 0);
    if (resProp) {
      manufactureCode = uecho_bytes_toint(uecho_property_getdata(resProp), uecho_property_getdatasize(resProp));
      manufacture = uecho_database_getmanufacture(db, manufactureCode);
      if (manufacture) {
        manufactureName = uecho_manufacture_getname(manufacture);
      }
    }
  }
  uecho_message_delete(reqMsg);
  uecho_message_delete(resMsg);

  return manufactureName;
}

void print_founddevices(uEchoController* ctrl, bool verbose)
{
  uEchoDatabase* db;
  uEchoNode* node;
  uEchoObject* obj;
  uEchoProperty* prop;
  uEchoProperty* resProp;
  uEchoMessage* reqMsg;
  uEchoMessage* resMsg;
  int objNo;
  int propNo;
  int resPropNo;
  int n;
  const char* manufactureName;
  byte* resPropBytes;
  bool hasResProp;

  db = uecho_standard_getdatabase();

  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    if (!verbose) {
      printf("%-15s ", uecho_node_getaddress(node));
      objNo = 0;
      for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
        printf("[%d] %06X ", objNo, uecho_object_getcode(obj));
        objNo++;
      }
      printf("\n");
      continue;
    }

    manufactureName = get_nodemanufacturename(ctrl, db, node);
    printf("%-15s (%s)\n", uecho_node_getaddress(node), (manufactureName ? manufactureName : UNKNOWN_STRING));

    objNo = 0;
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf("[%d] %06X (%s)\n", objNo, uecho_object_getcode(obj), (uecho_object_getname(obj) ? uecho_object_getname(obj) : UNKNOWN_STRING));
      propNo = 0;
      for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        if (uecho_property_isreadrequired(prop)) {
          printf("[%d] [%d] %02X ", objNo, propNo, uecho_property_getcode(prop));
          reqMsg = create_readpropertymessage(prop);
          resMsg = uecho_message_new();
          hasResProp = uecho_controller_postmessage(ctrl, node, reqMsg, resMsg);
          if (hasResProp) {
            for (resPropNo = 0; resPropNo < uecho_message_getopc(resMsg); resPropNo++) {
              resProp = uecho_message_getproperty(resMsg, resPropNo);
              if (!resProp)
                continue;
              resPropBytes = uecho_property_getdata(resProp);
              for (n = 0; n < uecho_property_getdatasize(resProp); n++) {
                printf("%02X", resPropBytes[n]);
              }
              printf(" ");
            }
          }
          printf("(%s)", uecho_property_getname(prop));
          printf("\n");
          uecho_message_delete(reqMsg);
          uecho_message_delete(resMsg);
        }
        propNo++;
      }
      objNo++;
    }

    if (uecho_node_next(node))
      printf("\n");
  }
}

int main(int argc, char* argv[])
{
  bool verboseMode;
  bool debugMode;
  uEchoController* ctrl;
  size_t foundNodeCnt;
  int c;

  // Parse options

  verboseMode = false;
  debugMode = false;

  while ((c = getopt(argc, argv, "vhd")) != -1) {
    switch (c) {
    case 'v': {
      verboseMode = true;
    } break;
    case 'd': {
      debugMode = true;
    } break;
    case 'h': {
      usage();
      return EXIT_SUCCESS;
    }
    default: {
      usage();
      return EXIT_FAILURE;
    }
    }
  }

  argc -= optind;
  argv += optind;

  // Debug mode

  if (debugMode) {
    uecho_log_setlevel(UECHO_LOG_DEBUG);
  }

  // Start controller

  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  uecho_controller_search(ctrl);
  uecho_sleep(SEARCH_WAIT_MTIME);

  foundNodeCnt = uecho_controller_getnodecount(ctrl);
  if (0 < foundNodeCnt) {
    print_founddevices(ctrl, verboseMode);
  }

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
