/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdlib.h>

#include <uecho/device.h>
#include <uecho/node.h>

// See : APPENDIX Detailed Requirements for ECHONET Device objects
//       3.3.29 Requirements for mono functional lighting class

#define LIGHT_OBJECT_CODE 0x029101
#define LIGHT_PROPERTY_POWER_CODE 0x80
#define LIGHT_PROPERTY_POWER_ON 0x30
#define LIGHT_PROPERTY_POWER_OFF 0x31

#undef UECHO_PLATFORM_RASPBIAN
#define RASPBERRY_PI_LIGHT_GPIO_NO "3"

/****************************************
 * uecho_light_printrequest
 ****************************************/

void uecho_light_printrequest(uEchoMessage* msg)
{
  uEchoProperty* prop;
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

  for (n = 0; n < opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X", uecho_property_getcode(prop));
  }

  printf("\n");
}

/****************************************
 * uecho_lighting_object_messagelitener
 ****************************************/

void uecho_lighting_object_messagelitener(uEchoObject* obj, uEchoMessage* msg)
{
  uecho_light_printrequest(msg);
}

/****************************************
 * uecho_lighting_propertyrequestlistener
 ****************************************/

bool uecho_lighting_propertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoProperty* prop)
{
  byte status;

  printf("ESV = %02X : %02X (%d), ", esv, uecho_property_getcode(prop), uecho_property_getdatasize(prop));

  if ((uecho_property_getdatasize(prop) != 1) || !uecho_property_getbytedata(prop, &status)) {
    printf("Bad Request\n");
    return false;
  }

  // TODO : Set the status to hardware

  switch (status) {
  case LIGHT_PROPERTY_POWER_ON:
    printf("POWER = ON\n");
#if defined(UECHO_PLATFORM_RASPBIAN)
    system("echo \"1\" > /sys/class/gpio/gpio" RASPBERRY_PI_LIGHT_GPIO_NO "/value");
#endif
    break;
  case LIGHT_PROPERTY_POWER_OFF:
    printf("POWER = OFF\n");
#if defined(UECHO_PLATFORM_RASPBIAN)
    system("echo \"0\" > /sys/class/gpio/gpio" RASPBERRY_PI_LIGHT_GPIO_NO "/value");
#endif
    break;
  default:
    printf("POWER = %02X\n", status);
    break;
  }

  return true;
}

/****************************************
 * uecho_light_new
 ****************************************/

uEchoObject* uecho_light_new(void)
{
  uEchoObject* obj;
  byte prop[32];

  obj = uecho_device_new();

  // TODO : Set your manufacture code

  uecho_object_setmanufacturercode(obj, 0xFFFFF0);

  // Mono functional lighting class

  uecho_object_setcode(obj, LIGHT_OBJECT_CODE);

  // Operation status property

  uecho_object_setproperty(obj, LIGHT_PROPERTY_POWER_CODE, uEchoPropertyAttrReadWrite);
  prop[0] = LIGHT_PROPERTY_POWER_ON;
  uecho_object_setpropertydata(obj, LIGHT_PROPERTY_POWER_CODE, prop, 1);

  // Set property observer

  uecho_object_setpropertywriterequesthandler(obj, LIGHT_PROPERTY_POWER_CODE, uecho_lighting_propertyrequestlistener);

  // Set Raspbian GPIO

#if defined(UECHO_PLATFORM_RASPBIAN)
  system("echo \"" RASPBERRY_PI_LIGHT_GPIO_NO "\" > /sys/class/gpio/export");
  system("echo \"out\" > /sys/class/gpio/gpio" RASPBERRY_PI_LIGHT_GPIO_NO "/direction");
#endif

  return obj;
}

/****************************************
 * uecho_light_delete
 ****************************************/

bool uecho_light_delete(uEchoObject* obj)
{
#if defined(UECHO_PLATFORM_RASPBIAN)
  system("echo \"" RASPBERRY_PI_LIGHT_GPIO_NO "\" > /sys/class/gpio/unexport");
#endif

  return uecho_object_delete(obj);
}
