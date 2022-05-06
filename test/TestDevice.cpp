/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#include "TestDevice.h"

#undef UECHO_TEST_VERBOSE

void uecho_test_printrequest(uEchoMessage* msg)
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

void uecho_test_object_messagelitener(uEchoObject* obj, uEchoMessage* msg)
{
#if defined(UECHO_TEST_VERBOSE)
  uecho_test_printrequest(msg);
#endif
}

bool uecho_test_property_requesthandler(uEchoObject* obj, uEchoProperty* prop, uEchoEsv esv, size_t pdc, byte* edt)
{
  byte status;

#if defined(UECHO_TEST_VERBOSE)
  printf("%02X %02X (%d)\n",
      esv,
      uecho_property_getcode(prop),
      pdc);
#endif

  if (pdc != 1)
    return false;

  status = edt[0];

#if defined(UECHO_TEST_VERBOSE)
  switch (status) {
  case UECHO_TEST_PROPERTY_SWITCH_ON:
    printf("POWER = %02X\n", status);
    break;
  case UECHO_TEST_PROPERTY_SWITCH_OFF:
    printf("POWER = %02X\n", status);
    break;
  }
#endif

  return true;
}

uEchoObject* uecho_test_createtestdevice()
{
  uEchoObject* obj = uecho_device_new();

  uecho_object_setmanufacturercode(obj, UECHO_TEST_MANCODE);
  uecho_object_setcode(obj, UECHO_TEST_OBJECTCODE);

  byte prop[32];

  // Operation status property
  uecho_object_setproperty(obj, UECHO_TEST_PROPERTY_SWITCHCODE, uEchoPropertyAttrReadWrite);
  prop[0] = UECHO_TEST_PROPERTY_SWITCH_ON;
  uecho_object_setpropertydata(obj, UECHO_TEST_PROPERTY_SWITCHCODE, prop, 1);

  // Set object listner

  uecho_object_setmessagelistener(obj, uecho_test_object_messagelitener);

  // Set property observer

  uecho_object_setpropertywriterequesthandler(obj, UECHO_TEST_PROPERTY_SWITCHCODE, uecho_test_property_requesthandler);

  return obj;
}

uEchoNode* uecho_test_createtestnode()
{
  uEchoNode* node = uecho_node_new();
  uEchoObject* dev = uecho_test_createtestdevice();
  uecho_node_addobject(node, dev);
  return node;
}
