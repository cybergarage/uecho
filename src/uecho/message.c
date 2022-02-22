/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <arpa/inet.h>

#include <uecho/message_internal.h>

#include <uecho/misc.h>
#include <uecho/net/socket.h>
#include <uecho/util/strings.h>

/****************************************
* uecho_message_new
****************************************/

uEchoMessage* uecho_message_new(void)
{
  uEchoMessage* msg;

  msg = (uEchoMessage*)malloc(sizeof(uEchoMessage));

  if (!msg)
    return NULL;

  uecho_message_setehd1(msg, uEchoEhd1);
  uecho_message_setehd2(msg, uEchoEhd2);
  uecho_message_settid(msg, 0);
  uecho_message_setsourceobjectcode(msg, uEchoObjectCodeUnknown);
  uecho_message_setdestinationobjectcode(msg, uEchoObjectCodeUnknown);
  uecho_message_setesv(msg, 0);

  msg->OPC = 0;
  msg->EP = NULL;

  msg->OPCSet = 0;
  msg->EPSet = NULL;

  msg->OPCGet = 0;
  msg->EPGet = NULL;

  msg->bytes = NULL;
  msg->srcAddr = NULL;

  return msg;
}

/****************************************
* uecho_message_delete
****************************************/

bool uecho_message_delete(uEchoMessage* msg)
{
  if (!msg)
    return false;

  uecho_message_clear(msg);

  free(msg);

  return true;
}

/****************************************
 * uecho_message_clearproperties
 ****************************************/

bool uecho_properties_clear(uEchoProperty*** EP, byte* OPC)
{
  int n;
  for (n = 0; n < (int)(*OPC); n++) {
    uecho_property_delete((*EP)[n]);
    (*EP)[n] = NULL;
  }

  if (*EP) {
    free(*EP);
  }

  *EP = NULL;
  *OPC = 0;

  return true;
}

bool uecho_message_clearproperties(uEchoMessage* msg)
{
  if (!msg)
    return false;
  uecho_properties_clear(&msg->EP, &msg->OPC);
  uecho_properties_clear(&msg->EPSet, &msg->OPCSet);
  uecho_properties_clear(&msg->EPGet, &msg->OPCGet);
  return true;
}

/****************************************
 * uecho_message_clear
 ****************************************/

bool uecho_message_clear(uEchoMessage* msg)
{
  if (!msg)
    return false;

  if (msg->bytes) {
    free(msg->bytes);
    msg->bytes = NULL;
  }

  if (msg->srcAddr) {
    free(msg->srcAddr);
    msg->srcAddr = NULL;
  }

  if (!uecho_message_clearproperties(msg))
    return false;

  return true;
}

/****************************************
 * uecho_message_settid
 ****************************************/

bool uecho_message_settid(uEchoMessage* msg, uEchoTID val)
{
  if (!msg)
    return false;

  if (uEchoTidMax < val) {
    val %= uEchoTidMax;
  }
  uint16_t nval = htons(val);
  msg->TID[0] = (nval & 0xFF00) >> 8;
  msg->TID[1] = nval & 0x00FF;
  return true;
}

/****************************************
 * uecho_message_gettid
 ****************************************/

uEchoTID uecho_message_gettid(uEchoMessage* msg)
{
  uint16_t nval = (msg->TID[0] << 8) + msg->TID[1];
  return ntohs(nval);
}

/****************************************
 * uecho_message_setsourceobjectcode
 ****************************************/

bool uecho_message_setsourceobjectcode(uEchoMessage* msg, int code)
{
  return uecho_integer2byte(code, msg->SEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_getsourceobjectcode
 ****************************************/

int uecho_message_getsourceobjectcode(uEchoMessage* msg)
{
  return uecho_byte2integer(msg->SEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_issourceobjectcode
 ****************************************/

bool uecho_message_issourceobjectcode(uEchoMessage* msg, int code)
{
  return (uecho_message_getsourceobjectcode(msg) == code);
}

/****************************************
 * uecho_message_setdestinationobjectcode
 ****************************************/

bool uecho_message_setdestinationobjectcode(uEchoMessage* msg, int code)
{
  return uecho_integer2byte(code, msg->DEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_getdestinationobjectcode
 ****************************************/

int uecho_message_getdestinationobjectcode(uEchoMessage* msg)
{
  return uecho_byte2integer(msg->DEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_issourceobjectcode
 ****************************************/

bool uecho_message_isdestinationobjectcode(uEchoMessage* msg, int code)
{
  return (uecho_message_getdestinationobjectcode(msg) == code);
}

/****************************************
 * uecho_message_setopc
 ****************************************/

bool uecho_message_setopcep(byte* OPC, uEchoProperty*** EP, byte count)
{
  int n;
  *OPC = count;
  if (*OPC <= 0)
    return true;
  *EP = (uEchoProperty**)malloc(sizeof(uEchoProperty*) * count);
  for (n = 0; n < (size_t)count; n++) {
    (*EP)[n] = uecho_property_new();
  }
  return true;
}

bool uecho_message_setopc(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->OPC, &msg->EP, count);
}

bool uecho_message_setopcset(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->OPCSet, &msg->EPSet, count);
}

bool uecho_message_setopcget(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->OPCGet, &msg->EPGet, count);
}

/****************************************
 * uecho_message_getopc
 ****************************************/

byte uecho_message_getopc(uEchoMessage* msg)
{
  return msg->OPC;
}

/****************************************
 * uecho_message_getopcset
 ****************************************/

byte uecho_message_getopcset(uEchoMessage* msg)
{
  return msg->OPCSet;
}

/****************************************
 * uecho_message_getopcget
 ****************************************/

byte uecho_message_getopcget(uEchoMessage* msg)
{
  return msg->OPCGet;
}

/****************************************
 * uecho_message_setehd1
 ****************************************/

void uecho_message_setehd1(uEchoMessage* msg, byte val)
{
  msg->EHD1 = val;
}

/****************************************
 * uecho_message_getehd1
 ****************************************/

byte uecho_message_getehd1(uEchoMessage* msg)
{
  return msg->EHD1;
}

/****************************************
 * uecho_message_setehd2
 ****************************************/

void uecho_message_setehd2(uEchoMessage* msg, byte val)
{
  msg->EHD2 = val;
}

/****************************************
 * uecho_message_getehd2
 ****************************************/

byte uecho_message_getehd2(uEchoMessage* msg)
{
  return msg->EHD2;
}

/****************************************
 * uecho_message_setesv
 ****************************************/

bool uecho_message_setesv(uEchoMessage* msg, uEchoEsv val)
{
  if (!msg)
    return false;

  msg->ESV = val;

  return true;
}

/****************************************
 * uecho_message_getesv
 ****************************************/

uEchoEsv uecho_message_getesv(uEchoMessage* msg)
{
  if (!msg)
    return 0;

  return msg->ESV;
}

/****************************************
 * uecho_message_requestesv2responseesv
 ****************************************/

bool uecho_message_requestesv2responseesv(uEchoEsv reqEsv, uEchoEsv* resEsv)
{
  *resEsv = 0;

  switch (reqEsv) {
  case uEchoEsvWriteRequestResponseRequired:
    *resEsv = uEchoEsvWriteResponse;
    return true;
  case uEchoEsvReadRequest:
    *resEsv = uEchoEsvReadResponse;
    return true;
  case uEchoEsvNotificationRequest:
    *resEsv = uEchoEsvNotification;
    return true;
  case uEchoEsvWriteReadRequest:
    *resEsv = uEchoEsvWriteReadResponse;
    return true;
  case uEchoEsvNotificationResponseRequired:
    *resEsv = uEchoEsvNotificationResponse;
    return true;
  }

  return false;
}

/****************************************
 * uecho_message_requestesv2errorresponseesv
 ****************************************/

bool uecho_message_requestesv2errorresponseesv(uEchoEsv reqEsv, uEchoEsv* resEsv)
{
  *resEsv = 0;

  switch (reqEsv) {
  case uEchoEsvWriteRequest:
    *resEsv = uEchoEsvWriteRequestError;
    return true;
  case uEchoEsvWriteRequestResponseRequired:
    *resEsv = uEchoEsvWriteRequestResponseRequiredError;
    return true;
  case uEchoEsvReadRequest:
    *resEsv = uEchoEsvReadRequestError;
    return true;
  case uEchoEsvNotificationRequest:
    *resEsv = uEchoEsvNotificationRequestError;
    return true;
  case uEchoEsvWriteReadRequest:
    *resEsv = uEchoEsvWriteReadRequestError;
    return true;
  }

  return false;
}

/****************************************
 * uecho_message_isresponserequired
 ****************************************/

bool uecho_message_isresponserequired(uEchoMessage* msg)
{
  uEchoEsv resEsv;

  if (!msg)
    return false;

  return uecho_message_requestesv2responseesv(msg->ESV, &resEsv);
}

/****************************************
 * uecho_message_isresponsemessage
 ****************************************/

bool uecho_message_isresponsemessage(uEchoMessage* msg, uEchoMessage* resMeg)
{
  if (!msg || !resMeg)
    return false;

  if (uecho_message_gettid(msg) != uecho_message_gettid(resMeg))
    return false;

  if (uecho_message_getesv(msg) == uecho_message_getesv(resMeg))
    return false;

  return true;
}

/****************************************
 * uecho_message_setsourceaddress
 ****************************************/

void uecho_message_setsourceaddress(uEchoMessage* msg, const char* addr)
{
  uecho_strloc(addr, &msg->srcAddr);
}

/****************************************
 * uecho_message_getsourceaddress
 ****************************************/

const char* uecho_message_getsourceaddress(uEchoMessage* msg)
{
  return msg->srcAddr;
}

/****************************************
 * uecho_message_issourceaddress
 ****************************************/

bool uecho_message_issourceaddress(uEchoMessage* msg, const char* addr)
{
  return uecho_streq(msg->srcAddr, addr);
}

/****************************************
 * uecho_message_iswriterequest
 ****************************************/

bool uecho_message_iswriterequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvWriteRequest) || (msg->ESV == uEchoEsvWriteRequestResponseRequired) || (msg->ESV == uEchoEsvWriteReadRequest))
    return true;
  return false;
}

/****************************************
 * uecho_message_isreadrequest
 ****************************************/

bool uecho_message_isreadrequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvReadRequest) || (msg->ESV == uEchoEsvWriteReadRequest))
    return true;
  return false;
}

/****************************************
 * uecho_message_isnotifyrequest
 ****************************************/

bool uecho_message_isnotifyrequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvNotificationRequest) || (msg->ESV == uEchoEsvNotificationResponseRequired))
    return true;
  return false;
}

/****************************************
 * uecho_message_iswriteresponse
 ****************************************/

bool uecho_message_iswriteresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvWriteResponse) || (msg->ESV == uEchoEsvWriteReadResponse))
    return true;
  return false;
}

/****************************************
 * uecho_message_isreadresponse
 ****************************************/

bool uecho_message_isreadresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvReadResponse) || (msg->ESV == uEchoEsvWriteReadResponse))
    return true;
  return false;
}

/****************************************
 * uecho_message_isnotifyresponse
 ****************************************/

bool uecho_message_isnotifyresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if ((msg->ESV == uEchoEsvNotification) || (msg->ESV == uEchoEsvNotificationResponse))
    return true;
  return false;
}

/****************************************
 * uecho_message_addproperty
 ****************************************/

bool uecho_property_add(byte* OPC, uEchoProperty*** EP, uEchoProperty* prop)
{
  *OPC += 1;
  *EP = (uEchoProperty**)realloc(*EP, sizeof(uEchoProperty*) * (*OPC));
  (*EP)[(*OPC - 1)] = prop;
  return true;
}

bool uecho_message_addproperty(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->OPC, &msg->EP, prop);
}

bool uecho_message_addpropertyset(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->OPCSet, &msg->EPSet, prop);
}

bool uecho_message_addpropertyget(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->OPCGet, &msg->EPGet, prop);
}

/****************************************
 * uecho_message_getproperty
 ****************************************/

uEchoProperty* uecho_property_get(byte OPC, uEchoProperty** EP, size_t n)
{
  if ((OPC - 1) < n)
    return NULL;
  return EP[n];
}

uEchoProperty* uecho_message_getproperty(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->OPC, msg->EP, n);
}

uEchoProperty* uecho_message_getpropertyset(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->OPCSet, msg->EPSet, n);
}

uEchoProperty* uecho_message_getpropertyget(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->OPCGet, msg->EPGet, n);
}

/****************************************
 * uecho_message_getpropertybycode
 ****************************************/

uEchoProperty* uecho_property_getbycode(byte OPC, uEchoProperty** EP, uEchoPropertyCode code)
{
  uEchoProperty* prop;
  size_t n;
  for (n = 0; n < (size_t)(OPC); n++) {
    prop = EP[n];
    if (!prop)
      continue;
    if (prop->code == code)
      return prop;
  }
  return NULL;
}

uEchoProperty* uecho_message_getpropertybycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  if (!msg)
    return NULL;
  return uecho_property_getbycode(msg->OPC, msg->EP, code);
}

uEchoProperty* uecho_message_getpropertysetbycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  if (!msg)
    return NULL;
  return uecho_property_getbycode(msg->OPCSet, msg->EPSet, code);
}

uEchoProperty* uecho_message_getpropertygetbycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  if (!msg)
    return NULL;
  return uecho_property_getbycode(msg->OPCGet, msg->EPGet, code);
}

/****************************************
 * uecho_message_setproperty
 ****************************************/

bool uecho_property_set(byte* OPC, uEchoProperty*** EP, uEchoPropertyCode propCode, size_t propDataSize, const byte* propData)
{
  uEchoProperty* prop;
  prop = uecho_property_getbycode(*OPC, *EP, propCode);
  if (!prop) {
    prop = uecho_property_new();
    if (!prop)
      return false;
    uecho_property_setcode(prop, propCode);
    if (!uecho_property_add(OPC, EP, prop)) {
      uecho_property_delete(prop);
      return false;
    }
  }
  return uecho_property_setdata(prop, propData, propDataSize);
}

bool uecho_message_setproperty(uEchoMessage* msg, uEchoPropertyCode propCode, size_t propDataSize, const byte* propData)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->OPC, &msg->EP, propCode, propDataSize, propData);
}

bool uecho_message_setpropertyset(uEchoMessage* msg, uEchoPropertyCode propCode, size_t propDataSize, const byte* propData)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->OPCSet, &msg->EPSet, propCode, propDataSize, propData);
}

bool uecho_message_setpropertyget(uEchoMessage* msg, uEchoPropertyCode propCode, size_t propDataSize, const byte* propData)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->OPCGet, &msg->EPGet, propCode, propDataSize, propData);
}

/****************************************
 * uecho_message_parse
 ****************************************/

bool uecho_message_property_parse(byte OPC, uEchoProperty** EP, const byte* data, size_t dataLen, size_t* offset)
{
  uEchoProperty* prop;
  size_t n, count;

  // EP

  for (n = 0; n < (size_t)OPC; n++) {
    prop = EP[n];
    if (!prop)
      return false;

    // EPC

    if ((dataLen - 1) < (*offset))
      return false;
    uecho_property_setcode(prop, data[(*offset)++]);

    // PDC

    if ((dataLen - 1) < (*offset))
      return false;
    count = data[(*offset)++];

    // EDT

    if ((dataLen - 1) < ((*offset) + count - 1))
      return false;
    if (!uecho_property_setdata(prop, (data + (*offset)), count))
      return false;
    *offset += count;
  }

  return true;
}

bool uecho_message_isreadwritemessage(uEchoMessage* msg)
{
  if (msg->ESV == uEchoEsvWriteReadRequest)
    return true;
  if (msg->ESV == uEchoEsvWriteReadResponse)
    return true;
  if (msg->ESV == uEchoEsvWriteReadRequestError)
    return true;
  return false;
}

bool uecho_message_parse(uEchoMessage* msg, const byte* data, size_t dataLen)
{
  size_t offset;

  if (!msg)
    return false;

  if (!uecho_message_clear(msg))
    return false;

  if (dataLen < uEchoMessageMinLen)
    return false;

  // Check Headers

  if (data[0] != uEchoEhd1)
    return false;

  if (data[1] != uEchoEhd2)
    return false;

  // TID

  msg->TID[0] = data[2];
  msg->TID[1] = data[3];

  // SEOJ

  msg->SEOJ[0] = data[4];
  msg->SEOJ[1] = data[5];
  msg->SEOJ[2] = data[6];

  // DEOJ

  msg->DEOJ[0] = data[7];
  msg->DEOJ[1] = data[8];
  msg->DEOJ[2] = data[9];

  // ESV

  uecho_message_setesv(msg, data[10]);

  if (uecho_message_isreadwritemessage(msg)) {
    // OPCSet
    uecho_message_setopcset(msg, data[11]);
    offset = 12;
    if (!uecho_message_property_parse(msg->OPCSet, msg->EPSet, data, dataLen, &offset))
      return false;
    // OPCGet
    if ((dataLen - 1) < offset)
      return false;
    uecho_message_setopcget(msg, data[offset]);
    offset += 1;
    if (!uecho_message_property_parse(msg->OPCGet, msg->EPGet, data, dataLen, &offset))
      return false;
  }
  else {
    // OPC
    uecho_message_setopc(msg, data[11]);
    offset = 12;
    if (!uecho_message_property_parse(msg->OPC, msg->EP, data, dataLen, &offset))
      return false;
  }

  return true;
}

/****************************************
 * uecho_message_parsepacket
 ****************************************/

bool uecho_message_parsepacket(uEchoMessage* msg, uEchoDatagramPacket* dgmPkt)
{
  if (!msg || !dgmPkt)
    return false;

  if (!uecho_message_parse(msg, uecho_socket_datagram_packet_getdata(dgmPkt), uecho_socket_datagram_packet_getlength(dgmPkt)))
    return false;

  uecho_message_setsourceaddress(msg, uecho_socket_datagram_packet_getremoteaddress(dgmPkt));

  return true;
}

/****************************************
 * uecho_message_size
 ****************************************/

size_t uecho_message_opcsize(byte OPC, uEchoProperty** EP)
{
  uEchoProperty* prop;
  size_t msgLen, n;

  msgLen = 0;
  for (n = 0; n < (size_t)(OPC); n++) {
    prop = EP[n];
    if (!prop)
      continue;
    msgLen += 2;
    msgLen += uecho_property_getdatasize(prop);
  }

  return msgLen;
}

size_t uecho_message_size(uEchoMessage* msg)
{
  size_t msgLen;

  if (!msg)
    return 0;

  msgLen = uEchoMessageMinLen;

  if (uecho_message_isreadwritemessage(msg)) {
    msgLen += uecho_message_opcsize(msg->OPCSet, msg->EPSet);
    msgLen += 1;
    msgLen += uecho_message_opcsize(msg->OPCGet, msg->EPGet);
  }
  else {
    msgLen += uecho_message_opcsize(msg->OPC, msg->EP);
  }
  
  return msgLen;
}

/****************************************
 * uecho_message_getbytes
 ****************************************/

bool uecho_message_setopcbytes(uEchoMessage* msg, byte OPC, uEchoProperty** EP, size_t *offset)
{
  uEchoProperty* prop;
  size_t n, count;
  for (n = 0; n < (size_t)OPC; n++) {
    prop = EP[n];
    if (!prop)
      return false;
    count = uecho_property_getdatasize(prop);
    msg->bytes[(*offset)++] = uecho_property_getcode(prop);
    msg->bytes[(*offset)++] = count;
    if (count <= 0)
      continue;
    memcpy((msg->bytes + (*offset)), uecho_property_getdata(prop), count);
    *offset += count;
  }
  return true;
}

byte* uecho_message_getbytes(uEchoMessage* msg)
{
  size_t offset;

  if (msg->bytes) {
    free(msg->bytes);
  }

  msg->bytes = (byte*)malloc(uecho_message_size(msg));

  msg->bytes[0] = msg->EHD1;
  msg->bytes[1] = msg->EHD2;
  msg->bytes[2] = msg->TID[0];
  msg->bytes[3] = msg->TID[1];
  msg->bytes[4] = msg->SEOJ[0];
  msg->bytes[5] = msg->SEOJ[1];
  msg->bytes[6] = msg->SEOJ[2];
  msg->bytes[7] = msg->DEOJ[0];
  msg->bytes[8] = msg->DEOJ[1];
  msg->bytes[9] = msg->DEOJ[2];
  msg->bytes[10] = msg->ESV;

  if (uecho_message_isreadwritemessage(msg)) {
    msg->bytes[11] = msg->OPCSet;
    offset = 12;
    uecho_message_setopcbytes(msg, msg->OPCSet, msg->EPSet, &offset);
    msg->bytes[offset] = msg->OPCGet;
    offset += 1;
    uecho_message_setopcbytes(msg, msg->OPCGet, msg->EPGet, &offset);
  }
  else {
    msg->bytes[11] = msg->OPC;
    offset = 12;
    uecho_message_setopcbytes(msg, msg->OPC, msg->EP, &offset);
  }

  return msg->bytes;
}

/****************************************
 * uecho_message_set
 ****************************************/

bool uecho_message_set(uEchoMessage* msg, uEchoMessage* srcMsg)
{
  uEchoProperty *prop, *srcProp;
  size_t srcMsgOpc, n;

  if (!msg || !srcMsg)
    return false;

  uecho_message_clear(msg);

  uecho_message_setehd1(msg, uecho_message_getehd1(srcMsg));
  uecho_message_setehd2(msg, uecho_message_getehd2(srcMsg));
  uecho_message_settid(msg, uecho_message_gettid(srcMsg));
  uecho_message_setsourceobjectcode(msg, uecho_message_getsourceobjectcode(srcMsg));
  uecho_message_setdestinationobjectcode(msg, uecho_message_getdestinationobjectcode(srcMsg));
  uecho_message_setesv(msg, uecho_message_getesv(srcMsg));
  uecho_message_setsourceaddress(msg, uecho_message_getsourceaddress(srcMsg));

  srcMsgOpc = uecho_message_getopc(srcMsg);
  for (n = 0; n < srcMsgOpc; n++) {
    srcProp = uecho_message_getproperty(srcMsg, n);
    if (!srcProp)
      continue;
    prop = uecho_property_copy(srcProp);
    if (!srcProp)
      continue;
    uecho_message_addproperty(msg, prop);
  }

  return true;
}

/****************************************
 * uecho_message_copy
 ****************************************/

uEchoMessage* uecho_message_copy(uEchoMessage* srcMsg)
{
  uEchoMessage* newMsg;

  newMsg = uecho_message_new();
  if (!newMsg)
    return NULL;

  uecho_message_set(newMsg, srcMsg);

  return newMsg;
}

/****************************************
 * uecho_message_equals
 ****************************************/

bool uecho_message_equals(uEchoMessage* msg1, uEchoMessage* msg2)
{
  size_t msgSize;
  byte *msg1Bytes, *msg2Bytes;

  msgSize = uecho_message_size(msg1);
  if (msgSize != uecho_message_size(msg2))
    return false;

  msg1Bytes = uecho_message_getbytes(msg1);
  msg2Bytes = uecho_message_getbytes(msg2);

  return (memcmp(msg1Bytes, msg2Bytes, msgSize) == 0) ? true : false;
}
