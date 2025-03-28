/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_H_
#define _UECHO_MESSAGE_H_

#include <uecho/const.h>
#include <uecho/esv.h>
#include <uecho/property.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef unsigned int uEchoTID;

#if !defined(_UECHO_MESSAGE_INTERNAL_H_)
typedef void uEchoMessage;
#endif

/****************************************
 * Function
 ****************************************/

uEchoMessage* uecho_message_new(void);
bool uecho_message_delete(uEchoMessage* msg);
bool uecho_message_clear(uEchoMessage* msg);

bool uecho_message_parse(uEchoMessage* msg, const byte* data, size_t dataLen);

byte uecho_message_getehd1(uEchoMessage* msg);
byte uecho_message_getehd2(uEchoMessage* msg);

bool uecho_message_settid(uEchoMessage* msg, uEchoTID val);
uEchoTID uecho_message_gettid(uEchoMessage* msg);

byte uecho_message_getopc(uEchoMessage* msg);
byte uecho_message_getopcset(uEchoMessage* msg);
byte uecho_message_getopcget(uEchoMessage* msg);

bool uecho_message_setsourceobjectcode(uEchoMessage* msg, int code);
int uecho_message_getsourceobjectcode(uEchoMessage* msg);
bool uecho_message_issourceobjectcode(uEchoMessage* msg, int code);

bool uecho_message_setdestinationobjectcode(uEchoMessage* msg, int code);
int uecho_message_getdestinationobjectcode(uEchoMessage* msg);
bool uecho_message_isdestinationobjectcode(uEchoMessage* msg, int code);

bool uecho_message_setesv(uEchoMessage* msg, uEchoEsv val);
uEchoEsv uecho_message_getesv(uEchoMessage* msg);

bool uecho_message_addproperty(uEchoMessage* msg, uEchoProperty* prop);
bool uecho_message_setproperty(uEchoMessage* msg, uEchoPropertyCode propCode, const byte* propData, size_t propDataSize);
uEchoProperty* uecho_message_getproperty(uEchoMessage* msg, size_t n);
uEchoProperty* uecho_message_getpropertybycode(uEchoMessage* msg, uEchoPropertyCode code);

bool uecho_message_addpropertyset(uEchoMessage* msg, uEchoProperty* prop);
bool uecho_message_setpropertyset(uEchoMessage* msg, uEchoPropertyCode propCode, const byte* propData, size_t propDataSize);
uEchoProperty* uecho_message_getpropertyset(uEchoMessage* msg, size_t n);
uEchoProperty* uecho_message_getpropertysetbycode(uEchoMessage* msg, uEchoPropertyCode code);

bool uecho_message_addpropertyget(uEchoMessage* msg, uEchoProperty* prop);
bool uecho_message_setpropertyget(uEchoMessage* msg, uEchoPropertyCode propCode, const byte* propData, size_t propDataSize);
uEchoProperty* uecho_message_getpropertyget(uEchoMessage* msg, size_t n);
uEchoProperty* uecho_message_getpropertygetbycode(uEchoMessage* msg, uEchoPropertyCode code);

size_t uecho_message_size(uEchoMessage* msg);
byte* uecho_message_getbytes(uEchoMessage* msg);

void uecho_message_setsourceaddress(uEchoMessage* msg, const char* addr);
const char* uecho_message_getsourceaddress(uEchoMessage* msg);
bool uecho_message_issourceaddress(uEchoMessage* msg, const char* addr);

void uecho_message_setdestinationaddress(uEchoMessage* msg, const char* addr);
const char* uecho_message_getdestinationaddress(uEchoMessage* msg);
bool uecho_message_isdestinationaddress(uEchoMessage* msg, const char* addr);

bool uecho_message_set(uEchoMessage* msg, uEchoMessage* srcMsg);
uEchoMessage* uecho_message_copy(uEchoMessage* msg);
bool uecho_message_equals(uEchoMessage* msg1, uEchoMessage* msg2);

bool uecho_message_isresponserequired(uEchoMessage* msg);
bool uecho_message_isresponsemessage(uEchoMessage* msg, uEchoMessage* resMeg);

/****************************************
 * Function
 ****************************************/

uEchoMessage* uecho_message_search_new(void);

bool uecho_message_isnotification(uEchoMessage* msg);

bool uecho_message_iswriterequest(uEchoMessage* msg);
bool uecho_message_isreadrequest(uEchoMessage* msg);
bool uecho_message_isnotifyrequest(uEchoMessage* msg);

bool uecho_message_iswriteresponse(uEchoMessage* msg);
bool uecho_message_isreadresponse(uEchoMessage* msg);
bool uecho_message_isnotifyresponse(uEchoMessage* msg);

bool uecho_message_issearchrequest(uEchoMessage* msg);
bool uecho_message_issearchresponse(uEchoMessage* msg);

bool uecho_message_isrequest(uEchoMessage* msg);
bool uecho_message_isresponse(uEchoMessage* msg);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_H_ */
