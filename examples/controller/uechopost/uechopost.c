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
#include <uecho/uecho.h>
#include <unistd.h>

const int UECHOPOST_MAX_RESPONSE_MTIME = 5000;
const int UECHOPOST_RESPONSE_RETRY_COUNT = 100;

void usage()
{
  printf("Usage : uechopost [options] <address> <obj> <esv> <property (epc, pdc, edt) ...>\n");
  printf(" -v : Enable verbose output\n");
  printf(" -n : Disable unicast server\n");
  printf(" -h : Print this message\n");
}

void uechopost_print_messages(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoProperty* prop;
  size_t opc, n;

  opc = uecho_message_getopc(msg);
  printf("%s %2X %2X %04X %06X %06X %02X %ld ",
      uecho_message_getsourceaddress(msg),
      uecho_message_getehd1(msg),
      uecho_message_getehd2(msg),
      uecho_message_gettid(msg),
      uecho_message_getsourceobjectcode(msg),
      uecho_message_getdestinationobjectcode(msg),
      uecho_message_getesv(msg),
      opc);

  for (n = 0; n < opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X(%d)", uecho_property_getcode(prop), uecho_property_getdatasize(prop));
  }

  printf("\n");
}

void uechopost_print_objectresponse(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoProperty* prop;
  int opc, n, prop_data_size, np;
  byte* prop_data;

  opc = uecho_message_getopc(msg);
  printf("%s %06X %02X ",
      uecho_message_getsourceaddress(msg),
      uecho_message_getsourceobjectcode(msg),
      uecho_message_getesv(msg));

  for (n = 0; n < opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    prop_data_size = uecho_property_getdatasize(prop);
    if (prop_data_size == 0) {
      printf("%02X%02X ", uecho_property_getcode(prop), prop_data_size);
      continue;
    }
    prop_data = uecho_property_getdata(prop);
    printf("%02X%02X", uecho_property_getcode(prop), prop_data_size);
    for (np = 0; np < prop_data_size; np++) {
      printf("%02X", prop_data[np]);
    }
    printf(" ");
  }

  printf("\n");
}

void uechopost_controlpoint_listener(uEchoController* ctrl, uEchoMessage* msg)
{
  uechopost_print_messages(ctrl, msg);
}

int main(int argc, char* argv[])
{
  bool verbose_mode;
  uEchoController* ctrl;
  uEchoNode* dst_node;
  char* dst_node_addr;
  uEchoObjectCode dst_obj_code;
  uEchoObject* dst_obj;
  uEchoMessage *msg, *res_msg;
  int esv;
  char *edata, *edt;
  size_t edt_size;
  int epc, pdc, edt_byte;
  int n;

  byte* prop_data;
  bool is_response_required;
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

  if (argc < 4) {
    usage();
    return EXIT_FAILURE;
  }

  // Start controller

  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;

  if (verbose_mode) {
    uecho_controller_setmessagelistener(ctrl, uechopost_controlpoint_listener);
  }

  // Start controller and search objects

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  uecho_controller_search(ctrl);

  // Find destination node

  dst_node_addr = argv[0];

  dst_node = NULL;
  for (n = 0; n < UECHOPOST_RESPONSE_RETRY_COUNT; n++) {
    uecho_sleep(UECHOPOST_MAX_RESPONSE_MTIME / UECHOPOST_RESPONSE_RETRY_COUNT);
    dst_node = uecho_controller_getnodebyaddress(ctrl, dst_node_addr);
    if (dst_node)
      break;
  }

  if (!dst_node) {
    printf("Node (%s) is not found\n", dst_node_addr);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }

  // Find destination object

  sscanf(argv[1], "%x", &dst_obj_code);

  dst_obj = uecho_node_getobjectbycode(dst_node, dst_obj_code);

  if (!dst_node) {
    printf("Node (%s) doesn't has the specified object (%06X)\n", dst_node_addr, dst_obj_code);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }

  // Create Message

  msg = uecho_message_new();
  sscanf(argv[2], "%x", &esv);
  uecho_message_setesv(msg, esv);

#if defined(DEBUG)
  printf("%s %06X %01X\n", dst_node_addr, dst_obj_code, esv);
#endif

  edata = edt = argv[3];
  edt_size = strlen(argv[3]);
  while ((edt - edata + (2 + 2)) <= edt_size) {
    sscanf(edt, "%02x%02x", &epc, &pdc);
    edt += (2 + 2);

#if defined(DEBUG)
    printf("[%02X] = %02X ", epc, pdc);
#endif

    if (pdc == 0) {
      uecho_message_setproperty(msg, epc, 0, NULL);
      continue;
    }

    if (edt_size < (edt - edata + (pdc * 2)))
      break;

    prop_data = (byte*)malloc(pdc);
    for (n = 0; n < pdc; n++) {
      sscanf(edt, "%02x", &edt_byte);
#if defined(DEBUG)
      printf("%02X", edt_byte);
#endif
      prop_data[n] = edt_byte & 0xFF;
      edt += 2;
    }
    uecho_message_setproperty(msg, epc, pdc, prop_data);
    free(prop_data);
  }
#if defined(DEBUG)
  printf("\n");
#endif

  // Send message

  is_response_required = uecho_message_isresponserequired(msg);
  if (is_response_required) {
    res_msg = uecho_message_new();
    if (uecho_controller_postmessage(ctrl, dst_obj, msg, res_msg)) {
      uechopost_print_objectresponse(ctrl, res_msg);
    }
    uecho_message_delete(res_msg);
  }
  else {
    uecho_controller_sendmessage(ctrl, dst_obj, msg);
  }

  uecho_message_delete(msg);

  // Stop controller

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
