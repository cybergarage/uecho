/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>
#include <uecho/device.h>

// See : APPENDIX Detailed Requirements for ECHONET Device objects
//       3.3.29 Requirements for mono functional lighting class

#define LIGHT_OBJECT_CODE 0x029101
#define LIGHT_PROPERTY_SWITCH_CODE 0x80
#define LIGHT_PROPERTY_SWITCH_ON 0x30
#define LIGHT_PROPERTY_SWITCH_OFF 0x31

void uecho_lighting_propertyrequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoProperty *prop)
{
  byte status;
  
  if (uecho_property_getdatasize(prop) != 1)
    return;
  
  if (uecho_property_getbytedata(prop, &status))
    return;

  // TODO : Set the status to hardware
  
  switch (status) {
    case LIGHT_PROPERTY_SWITCH_ON:
      break;
    case LIGHT_PROPERTY_SWITCH_OFF:
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
  uecho_object_setproperty(obj, LIGHT_PROPERTY_SWITCH_CODE, uEchoPropertyAttrReadWrite);
  prop[0] = LIGHT_PROPERTY_SWITCH_ON;
  uecho_object_setpropertydata(obj, LIGHT_PROPERTY_SWITCH_CODE, prop, 1);
  
  // Set property observer
  uecho_object_setpropertywriterequestlistener(obj, LIGHT_PROPERTY_SWITCH_CODE, uecho_lighting_propertyrequestlistener);
  
  return obj;
}
