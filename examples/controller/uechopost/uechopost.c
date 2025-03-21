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
  printf(" -h : Print this message\n");
  printf(" -d : Enable debug output\n");
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
    printf("%02X(%ld)", uecho_property_getcode(prop), uecho_property_getdatasize(prop));
  }

  printf("\n");
}

void uechopost_print_objectresponse(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoProperty* prop;
  int opc, n, propDataSize, np;
  byte* propData;

  opc = uecho_message_getopc(msg);
  printf("%s %06X %02X ",
      uecho_message_getsourceaddress(msg),
      uecho_message_getsourceobjectcode(msg),
      uecho_message_getesv(msg));

  for (n = 0; n < opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    propDataSize = uecho_property_getdatasize(prop);
    if (propDataSize == 0) {
      printf("%02X%02X ", uecho_property_getcode(prop), propDataSize);
      continue;
    }
    propData = uecho_property_getdata(prop);
    printf("%02X%02X", uecho_property_getcode(prop), propDataSize);
    for (np = 0; np < propDataSize; np++) {
      printf("%02X", propData[np]);
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
  bool verboseMode;
  bool debugMode;
  uEchoController* ctrl;
  uEchoNode* dstNode;
  char* dstNodeAddr;
  uEchoObjectCode dstObjCode;
  uEchoObject* dstObj;
  uEchoMessage *msg, *resMsg;
  int esv;
  char *edata, *edt;
  size_t edtSize;
  int epc, pdc, edtByte;
  int n;

  byte* propData;
  bool isResponseRequired;
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

  if (argc < 4) {
    usage();
    return EXIT_FAILURE;
  }

  // Debug mode

  if (debugMode) {
    uecho_log_setlevel(UECHO_LOG_DEBUG);
  }

  // Start controller

  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;

  if (verboseMode) {
    uecho_controller_setmessagelistener(ctrl, uechopost_controlpoint_listener);
  }

  // Start controller and search objects

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  uecho_controller_search(ctrl);

  // Find destination node

  dstNodeAddr = argv[0];

  dstNode = NULL;
  for (n = 0; n < UECHOPOST_RESPONSE_RETRY_COUNT; n++) {
    uecho_sleep(UECHOPOST_MAX_RESPONSE_MTIME / UECHOPOST_RESPONSE_RETRY_COUNT);
    dstNode = uecho_controller_getnodebyaddress(ctrl, dstNodeAddr);
    if (dstNode)
      break;
  }

  if (!dstNode) {
    printf("Node (%s) is not found\n", dstNodeAddr);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }

  // Find destination object

  sscanf(argv[1], "%x", &dstObjCode);

  dstObj = uecho_node_getobjectbycode(dstNode, dstObjCode);

  if (!dstNode) {
    printf("Node (%s) doesn't has the specified object (%06X)\n", dstNodeAddr, dstObjCode);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }

  // Create Message

  msg = uecho_message_new();
  sscanf(argv[2], "%x", &esv);
  uecho_message_setesv(msg, esv);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(dstObj));

#if defined(DEBUG)
  printf("%s %06X %01X\n", dst_node_addr, dst_obj_code, esv);
#endif

  edata = edt = argv[3];
  edtSize = strlen(argv[3]);
  while ((edt - edata + (2 + 2)) <= edtSize) {
    sscanf(edt, "%02x%02x", &epc, &pdc);
    edt += (2 + 2);

#if defined(DEBUG)
    printf("[%02X] = %02X ", epc, pdc);
#endif

    if (pdc == 0) {
      uecho_message_setproperty(msg, epc, NULL, 0);
      continue;
    }

    if (edtSize < (edt - edata + (pdc * 2)))
      break;

    propData = (byte*)malloc(pdc);
    for (n = 0; n < pdc; n++) {
      sscanf(edt, "%02x", &edtByte);
#if defined(DEBUG)
      printf("%02X", edt_byte);
#endif
      propData[n] = edtByte & 0xFF;
      edt += 2;
    }
    uecho_message_setproperty(msg, epc, propData, pdc);
    free(propData);
  }
#if defined(DEBUG)
  printf("\n");
#endif

  // Send message

  isResponseRequired = uecho_message_isresponserequired(msg);
  if (isResponseRequired) {
    resMsg = uecho_message_new();
    if (uecho_controller_postmessage(ctrl, dstNode, msg, resMsg)) {
      uechopost_print_objectresponse(ctrl, resMsg);
    }
    uecho_message_delete(resMsg);
  }
  else {
    uecho_controller_sendmessage(ctrl, dstNode, msg);
  }

  uecho_message_delete(msg);

  // Stop controller

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
