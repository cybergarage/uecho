/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROPERTY_H_
#define _UECHO_PROPERTY_H_

#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoPropertyCodeMin = 0x80,
  uEchoPropertyCodeMax = 0xFF,
};

/****************************************
 * Data Type
 ****************************************/

typedef byte uEchoPropertyCode;

typedef enum {
  uEchoPropertyAttrNone = 0x00,
  uEchoPropertyAttrRead = 0x01,
  uEchoPropertyAttrWrite = 0x02,
  uEchoPropertyAttrAnno = 0x10,
  uEchoPropertyAttrReadWrite = uEchoPropertyAttrRead | uEchoPropertyAttrWrite,
  uEchoPropertyAttrReadAnno = uEchoPropertyAttrRead | uEchoPropertyAttrAnno,
} uEchoPropertyAttr;

#if !defined(_UECHO_PROPERTY_INTERNAL_H_)
typedef void uEchoProperty;
#endif

/****************************************
 * Function
 ****************************************/

uEchoProperty* uecho_property_new(void);
bool uecho_property_delete(uEchoProperty* prop);

bool uecho_property_setdata(uEchoProperty* prop, const byte* data, size_t count);
bool uecho_property_setintegerdata(uEchoProperty* prop, int data, size_t data_size);
bool uecho_property_setbytedata(uEchoProperty* prop, byte data);
bool uecho_property_adddata(uEchoProperty* prop, const byte* data, size_t count);
bool uecho_property_addbytedata(uEchoProperty* prop, const byte data);
bool uecho_property_cleardata(uEchoProperty* prop);

uEchoProperty* uecho_property_next(uEchoProperty* prop);
void uecho_property_remove(uEchoProperty* prop);

void uecho_property_setcode(uEchoProperty* prop, uEchoPropertyCode val);
uEchoPropertyCode uecho_property_getcode(uEchoProperty* prop);

byte uecho_property_getdatasize(uEchoProperty* prop);
byte* uecho_property_getdata(uEchoProperty* prop);
bool uecho_property_getintegerdata(uEchoProperty* prop, size_t data_size, int* data);
bool uecho_property_getbytedata(uEchoProperty* prop, byte* data);

void uecho_property_setattribute(uEchoProperty* prop, uEchoPropertyAttr val);
uEchoPropertyAttr uecho_property_getattribute(uEchoProperty* prop);
bool uecho_property_isreadable(uEchoProperty* prop);
bool uecho_property_iswritable(uEchoProperty* prop);
bool uecho_property_isreadonly(uEchoProperty* prop);
bool uecho_property_iswriteonly(uEchoProperty* prop);
bool uecho_property_isannounceable(uEchoProperty* prop);

uEchoProperty* uecho_property_copy(uEchoProperty* prop);
bool uecho_property_equals(uEchoProperty* prop1, uEchoProperty* prop2);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
