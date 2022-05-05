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

const int UECHO_TEST_SEARCH_WAIT_MTIME = 2000;

void usage()
{
  printf("Usage : uechosearch [options]\n");
  printf(" -v : Enable verbose output\n");
  printf(" -h : Print this message\n");
}

uEchoMessage *create_readpropertymessagebycode(int obj_code, byte prop_code)
{
  uEchoMessage* msg;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, obj_code);
  uecho_message_setproperty(msg, prop_code, NULL, 0);
  return msg;
}

uEchoMessage *create_readpropertymessage(uEchoProperty* prop)
{
  return create_readpropertymessagebycode(
    uecho_object_getcode(uecho_property_getparentobject(prop)),
    uecho_property_getcode(prop)
  );
}

void print_founddevices(uEchoController* ctrl, bool verbose)
{
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
  byte *res_prop_bytes;
  const char *manufacture_name;

  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    printf("%-15s ", uecho_node_getaddress(node));

    if (!verbose) {
      obj_no = 0;
      for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
        printf("[%d] %06X ", obj_no, uecho_object_getcode(obj));
        obj_no++;
      }
      printf("\n");
      continue;
    }

    printf("\n");
    obj_no = 0;
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf("[%d] %06X (%s)\n", obj_no, uecho_object_getcode(obj), uecho_object_getname(obj));

      prop_no = 0;
      for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        if (uecho_property_isreadrequired(prop)) {
          req_msg = create_readpropertymessage(prop);
          res_msg = uecho_message_new();
          printf("[%d] [%d] %02X (%s) ", obj_no, prop_no, uecho_property_getcode(prop), uecho_property_getname(prop));
          if (uecho_controller_postmessage(ctrl, node, req_msg, res_msg)) {
            for (res_prop_no=0; res_prop_no<uecho_message_getopc(res_msg); res_prop_no++) {
              res_prop = uecho_message_getproperty(res_msg, res_prop_no);
              if (!res_prop)
                continue;
              res_prop_bytes = uecho_property_getdata(res_prop);
              for (n=0; n<uecho_property_getdatasize(res_prop); n++) {
                printf("%02X", res_prop_bytes[n]);
              }
            }
          }
          printf("\n");
          uecho_message_delete(req_msg);
          uecho_message_delete(res_msg);
        }
        prop_no++;
      }
      obj_no++;
    }
  }
}

int main(int argc, char* argv[])
{
  bool verbose_mode;
  uEchoController* ctrl;
  size_t found_node_cnt;
  int c;

  // Parse options

  verbose_mode = false;

  while ((c = getopt(argc, argv, "vh")) != -1) {
    switch (c) {
    case 'v': {
      verbose_mode = true;
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

  // Start controller

  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  uecho_controller_search(ctrl);
  uecho_sleep(UECHO_TEST_SEARCH_WAIT_MTIME);

  found_node_cnt = uecho_controller_getnodecount(ctrl);
  if (0 < found_node_cnt) {
    print_founddevices(ctrl, verbose_mode);
  }

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
