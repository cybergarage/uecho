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

void uecho_post_print_usage()
{
  printf("echopost <address> <obj> <esv> <property (epc, pdc, edt) ...>\n");
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  const char *dstNodeAddr;
  uEchoObjectCode dstObjCode;
  uEchoEsv esv;
  char *edata, *edt;
  size_t edtSize;
  int epc, pdc, edtByte;
  int n;
  
  if (argc < 5) {
    uecho_post_print_usage();
    return EXIT_FAILURE;
  }

  dstNodeAddr = argv[1];
  sscanf(argv[2], "%x", &dstObjCode);
  sscanf(argv[3], "%x", &esv);

  printf("%s %06X %01X\n", dstNodeAddr, dstObjCode, esv);
  
  edata = edt = argv[4];
  edtSize = strlen(argv[4]);
  while ((edt - edata + (2 + 2)) <= edtSize) {
    sscanf(edt, "%02x%02x", &epc, &pdc);
    edt += (2 + 2);
    printf("[%02X] = %02X ", epc, pdc);
    if (edtSize < (edt - edata + (pdc * 2)))
      break;
    for (n=0; n<pdc; n++) {
      sscanf(edt, "%02x", &edtByte);
      printf("%02X", edtByte);
      edt += 2;
    }
  }
  printf("\n");
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  uecho_controller_searchallobjects(ctrl);
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
