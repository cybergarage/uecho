/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>
#include <uecho/device.h>

// See : APPENDIX Detailed Requirements for ECHONET Device objects
//       3.3.29 Requirements for mono functional lighting class

#define LIGHT_OBJECT_CODE 0x029101
#define LIGHT_PROPERTY_POWER_CODE 0x80
#define LIGHT_PROPERTY_POWER_ON 0x30
#define LIGHT_PROPERTY_POWER_OFF 0x31

void uecho_lighting_printrequest(uEchoMessage *msg)
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
    printf("%02X", uecho_property_getcode(prop));
  }
  
  printf("\n");
}

void uecho_lighting_object_messagelitener(uEchoObject *obj, uEchoMessage *msg)
{
  uecho_lighting_printrequest(msg);
}

void uecho_lighting_propertyrequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoProperty *prop)
{
  byte status;
  
  printf("ESV = %02X : %02X(%d)\n", esv, uecho_property_getcode(prop), uecho_property_getdatasize(prop));
  
  if (uecho_property_getdatasize(prop) != 1)
    return;
  
  if (!uecho_property_getbytedata(prop, &status))
    return;

  // TODO : Set the status to hardware
  
  switch (status) {
    case LIGHT_PROPERTY_POWER_ON:
      printf("POWER = %02X\n", status);
      break;
    case LIGHT_PROPERTY_POWER_OFF:
      printf("POWER = %02X\n", status);
      break;
  }
}

uEchoObject *uecho_create_lighting_deviceobject(void)
{
  uEchoObject *obj;
  byte prop[32];
  
  obj = uecho_device_new();
  
  // TODO : Set your manufacture code
  
  uecho_object_setmanufacturercode(obj, 0x000000);

  // Mono functional lighting class
  
  uecho_object_setcode(obj, LIGHT_OBJECT_CODE);

  // Operation status property
  
  uecho_object_setproperty(obj, LIGHT_PROPERTY_POWER_CODE, uEchoPropertyAttrReadWrite);
  prop[0] = LIGHT_PROPERTY_POWER_ON;
  uecho_object_setpropertydata(obj, LIGHT_PROPERTY_POWER_CODE, prop, 1);
  
  // Set property observer
  
  uecho_object_setpropertywriterequestlistener(obj, LIGHT_PROPERTY_POWER_CODE, uecho_lighting_propertyrequestlistener);
  
  return obj;
}
