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

const int UECHOPOST_MAX_RESPONSE_MTIME = 5000;
const int UECHOPOST_RESPONSE_RETRY_COUNT = 100;

void uecho_post_print_usage()
{
  printf("echopost <address> <obj> <esv> <property (epc, pdc, edt) ...>\n");
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  const char *dstNodeAddr;
  uEchoObjectCode dstObjCode;
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
    uecho_post_print_usage();
    return EXIT_FAILURE;
  }

  // Start controller
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  // Find destination node
  
  uecho_controller_searchallobjects(ctrl);
  
  dstNodeAddr = argv[1];

  dstNode = NULL;
  for (int n=0; n<UECHOPOST_RESPONSE_RETRY_COUNT; n++) {
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
  
  sscanf(argv[2], "%x", &dstObjCode);
  
  dstObj = uecho_node_getobjectbycode(dstNode, dstObjCode);
  
  if (!dstNode) {
    printf("Node (%s) doesn't has the specified object (%06X)\n", dstNodeAddr, dstObjCode);
    uecho_controller_delete(ctrl);
    return EXIT_FAILURE;
  }
  
  // Create Message
  
  msg = uecho_message_new();
  uecho_message_setdestinationobjectcode(msg, dstObjCode);
  sscanf(argv[3], "%x", &esv);
  uecho_message_setesv(msg, esv);

#if defined(DEBUG)
  printf("%s %06X %01X\n", dstNodeAddr, dstObjCode, esv);
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
  
  uecho_controller_sendmessage(ctrl, dstObj, msg);
  
  uecho_message_delete(msg);
  
  // Stop controller
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
