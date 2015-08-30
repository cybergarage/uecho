/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdio.h>
#include <uecho/uecho.h>

const int UECHOPOST_MAX_RESPONSE_MTIME = 5000;
const int UECHOPOST_RESPONSE_RETRY_COUNT = 100;

char *gDstNodeAddr;
uEchoObjectCode gDstObjCode;
bool gDstMsgReceived;

void uechopost_print_messages(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoProperty *prop;
  size_t opc, n;
  
  opc = uecho_message_getopc(msg);
  printf("%s %1X %1X %02X %03X %03X %02X %ld ",
         uecho_message_getsourceaddress(msg),
         uecho_message_getehd1(msg),
         uecho_message_getehd2(msg),
         uecho_message_gettid(msg),
         uecho_message_getsourceobjectcode(msg),
         uecho_message_getdestinationobjectcode(msg),
         uecho_message_getesv(msg),
         opc);
  
  for (n=0; n<opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X(%d)", uecho_property_getcode(prop), uecho_property_getdatasize(prop));
  }
  
  printf("\n");
}

void uechopost_print_objectresponse(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoProperty *prop;
  size_t opc, n, propDataSize, np;
  byte *propData;
  
  opc = uecho_message_getopc(msg);
  printf("Response (%s:%03X) : ESV = %02X, OPC = %ld, ",
         uecho_message_getsourceaddress(msg),
         uecho_message_getdestinationobjectcode(msg),
         uecho_message_getesv(msg),
         opc);
  
  for (n=0; n<opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    propDataSize = uecho_property_getdatasize(prop);
    if (propDataSize == 0) {
      printf("%02X(%ld) ", uecho_property_getcode(prop), propDataSize);
      continue;
    }
    propData = uecho_property_getdata(prop);
    printf("%02X(%ld:", uecho_property_getcode(prop), propDataSize);
    for (np=0; np<propDataSize; np++) {
      printf("%02X", propData[np]);
    }
    printf(") ");
  }
  
  printf("\n");
}

void uechopost_print_usage()
{
  printf("echopost <address> <obj> <esv> <property (epc, pdc, edt) ...>\n");
}

void uechopost_controlprint_listener(uEchoController *ctrl, uEchoMessage *msg)
{
#if defined(DEBUG)
  uechopost_print_messages(ctrl, msg);
#endif
  
  if (uecho_message_issourceaddress(msg, gDstNodeAddr) && uecho_message_issourceobjectcode(msg, gDstObjCode)) {
    uechopost_print_objectresponse(ctrl, msg);
    gDstMsgReceived = true;
  }
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  uEchoNode *dstNode;
  uEchoObject *dstObj;
  uEchoMessage *msg;
  uEchoEsv esv;
  char *edata, *edt;
  size_t edtSize;
  int epc, pdc, edtByte;
  int n;
  byte *propData;
  
  if (argc < 5) {
    uechopost_print_usage();
    return EXIT_FAILURE;
  }

  // Start controller
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  uecho_controller_setmessagelistener(ctrl, uechopost_controlprint_listener);

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  // Find destination node
  
  uecho_controller_searchallobjects(ctrl);
  
  gDstNodeAddr = argv[1];

  dstNode = NULL;
  for (int n=0; n<UECHOPOST_RESPONSE_RETRY_COUNT; n++) {
    uecho_sleep(UECHOPOST_MAX_RESPONSE_MTIME / UECHOPOST_RESPONSE_RETRY_COUNT);
    dstNode = uecho_controller_getnodebyaddress(ctrl, gDstNodeAddr);
    if (dstNode)
      break;
  }

  if (!dstNode) {
    printf("Node (%s) is not found\n", gDstNodeAddr);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }

  // Find destination object
  
  sscanf(argv[2], "%x", &gDstObjCode);
  
  dstObj = uecho_node_getobjectbycode(dstNode, gDstObjCode);
  
  if (!dstNode) {
    printf("Node (%s) doesn't has the specified object (%06X)\n", gDstNodeAddr, gDstObjCode);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }
  
  // Create Message
  
  msg = uecho_message_new();
  uecho_message_setdestinationobjectcode(msg, gDstObjCode);
  sscanf(argv[3], "%x", &esv);
  uecho_message_setesv(msg, esv);

#if defined(DEBUG)
  printf("%s %06X %01X\n", gDstNodeAddr, gDstObjCode, esv);
#endif
  
  edata = edt = argv[4];
  edtSize = strlen(argv[4]);
  while ((edt - edata + (2 + 2)) <= edtSize) {
    sscanf(edt, "%02x%02x", &epc, &pdc);
    edt += (2 + 2);

#if defined(DEBUG)
    printf("[%02X] = %02X ", epc, pdc);
#endif
    
    if (pdc == 0) {
      uecho_message_setproperty(msg, epc, 0, NULL);
      continue;
    }
    
    if (edtSize < (edt - edata + (pdc * 2)))
      break;

    propData = (byte *)malloc(pdc);
    for (n=0; n<pdc; n++) {
      sscanf(edt, "%02x", &edtByte);
#if defined(DEBUG)
      printf("%02X", edtByte);
#endif
      propData[n] = edtByte & 0xFF;
      edt += 2;
    }
    uecho_message_setproperty(msg, epc, pdc, propData);
    free(propData);
  }
#if defined(DEBUG)
  printf("\n");
#endif

  // Send message
  
  gDstMsgReceived = false;
  uecho_controller_sendmessage(ctrl, dstObj, msg);
  
  uecho_message_delete(msg);
  
  // Wait response message
  
  for (int n=0; n<UECHOPOST_RESPONSE_RETRY_COUNT; n++) {
    uecho_sleep(UECHOPOST_MAX_RESPONSE_MTIME / UECHOPOST_RESPONSE_RETRY_COUNT);
    if (gDstMsgReceived)
      break;
  }

  // Stop controller
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
