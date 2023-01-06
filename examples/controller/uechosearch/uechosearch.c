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

uEchoMessage* create_readpropertymessagebycode(int obj_code, byte prop_code)
{
  uEchoMessage* msg;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, obj_code);
  uecho_message_setproperty(msg, prop_code, NULL, 0);
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
  uEchoMessage* req_msg;
  uEchoMessage* res_msg;
  uEchoProperty* res_prop;
  uEchoManufacture* manufacture;
  const char* manufacture_name;
  int manufacture_code;

  manufacture_name = NULL;
  req_msg = create_readmanufacturecodemessage();
  res_msg = uecho_message_new();
  if (uecho_controller_postmessage(ctrl, node, req_msg, res_msg) && (uecho_message_getopc(res_msg) == 1)) {
    res_prop = uecho_message_getproperty(res_msg, 0);
    if (res_prop) {
      manufacture_code = uecho_bytes_toint(uecho_property_getdata(res_prop), uecho_property_getdatasize(res_prop));
      manufacture = uecho_database_getmanufacture(db, manufacture_code);
      if (manufacture) {
        manufacture_name = uecho_manufacture_getname(manufacture);
      }
    }
  }
  uecho_message_delete(req_msg);
  uecho_message_delete(res_msg);

  return manufacture_name;
}

void print_founddevices(uEchoController* ctrl, bool verbose)
{
  uEchoDatabase* db;
  uEchoNode* node;
  uEchoObject* obj;
  uEchoProperty* prop;
  uEchoProperty* res_prop;
  uEchoMessage* req_msg;
  uEchoMessage* res_msg;
  int obj_no;
  int prop_no;
  int res_prop_no;
  int n;
  const char* manufacture_name;
  byte* res_prop_bytes;
  bool has_res_prop;

  db = uecho_standard_getdatabase();

  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    if (!verbose) {
      printf("%-15s ", uecho_node_getaddress(node));
      obj_no = 0;
      for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
        printf("[%d] %06X ", obj_no, uecho_object_getcode(obj));
        obj_no++;
      }
      printf("\n");
      continue;
    }

    manufacture_name = get_nodemanufacturename(ctrl, db, node);
    printf("%-15s (%s)\n", uecho_node_getaddress(node), (manufacture_name ? manufacture_name : UNKNOWN_STRING));

    obj_no = 0;
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf("[%d] %06X (%s)\n", obj_no, uecho_object_getcode(obj), (uecho_object_getname(obj) ? uecho_object_getname(obj) : UNKNOWN_STRING));
      prop_no = 0;
      for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        if (uecho_property_isreadrequired(prop)) {
          printf("[%d] [%d] %02X ", obj_no, prop_no, uecho_property_getcode(prop));
          req_msg = create_readpropertymessage(prop);
          res_msg = uecho_message_new();
          has_res_prop = uecho_controller_postmessage(ctrl, node, req_msg, res_msg);
          if (has_res_prop) {
            for (res_prop_no = 0; res_prop_no < uecho_message_getopc(res_msg); res_prop_no++) {
              res_prop = uecho_message_getproperty(res_msg, res_prop_no);
              if (!res_prop)
                continue;
              res_prop_bytes = uecho_property_getdata(res_prop);
              for (n = 0; n < uecho_property_getdatasize(res_prop); n++) {
                printf("%02X", res_prop_bytes[n]);
              }
              printf(" ");
            }
          }
          printf("(%s)", uecho_property_getname(prop));
          printf("\n");
          uecho_message_delete(req_msg);
          uecho_message_delete(res_msg);
        }
        prop_no++;
      }
      obj_no++;
    }

    if (uecho_node_next(node))
      printf("\n");
  }
}

int main(int argc, char* argv[])
{
  bool verbose_mode;
  bool debug_mode;
  uEchoController* ctrl;
  size_t found_node_cnt;
  int c;

  // Parse options

  verbose_mode = false;
  debug_mode = false;

  while ((c = getopt(argc, argv, "vhd")) != -1) {
    switch (c) {
    case 'v': {
      verbose_mode = true;
    } break;
    case 'd': {
      debug_mode = true;
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

  if (debug_mode) {
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

  found_node_cnt = uecho_controller_getnodecount(ctrl);
  if (0 < found_node_cnt) {
    print_founddevices(ctrl, verbose_mode);
  }

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
