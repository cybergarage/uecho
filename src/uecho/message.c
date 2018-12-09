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

  msg->EP = NULL;
  msg->OPC = 0;
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

bool uecho_message_clearproperties(uEchoMessage* msg)
{
  int n;

  if (!msg)
    return false;

  for (n = 0; n < (int)(msg->OPC); n++) {
    uecho_property_delete(msg->EP[n]);
    msg->EP[n] = NULL;
  }

  if (msg->EP) {
    free(msg->EP);
    msg->EP = NULL;
  }

  msg->OPC = 0;

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

bool uecho_message_setopc(uEchoMessage* msg, byte count)
{
  int n;

  if (!msg)
    return false;

  uecho_message_clear(msg);

  msg->OPC = count;

  if (msg->OPC <= 0)
    return true;

  msg->EP = (uEchoProperty**)malloc(sizeof(uEchoProperty*) * count);
  for (n = 0; n < (int)(msg->OPC); n++) {
    msg->EP[n] = uecho_property_new();
  }

  return true;
}

/****************************************
 * uecho_message_getopc
 ****************************************/

byte uecho_message_getopc(uEchoMessage* msg)
{
  return msg->OPC;
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

bool uecho_message_addproperty(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg)
    return false;

  msg->OPC++;

  msg->EP = (uEchoProperty**)realloc(msg->EP, sizeof(uEchoProperty*) * msg->OPC);
  msg->EP[(msg->OPC - 1)] = prop;

  return true;
}

/****************************************
 * uecho_message_addproperty
 ****************************************/

bool uecho_message_setproperty(uEchoMessage* msg, uEchoPropertyCode propCode, size_t propDataSize, const byte* propData)
{
  uEchoProperty* prop;

  if (!msg)
    return false;

  prop = uecho_message_getpropertybycode(msg, propCode);
  if (!prop) {
    prop = uecho_property_new();
    if (!prop)
      return false;
    uecho_property_setcode(prop, propCode);
    if (!uecho_message_addproperty(msg, prop)) {
      uecho_property_delete(prop);
      return false;
    }
  }

  return uecho_property_setdata(prop, propData, propDataSize);
}

/****************************************
 * uecho_message_getproperty
 ****************************************/

uEchoProperty* uecho_message_getproperty(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return false;
  if ((msg->OPC - 1) < n)
    return NULL;
  return msg->EP[n];
}

/****************************************
 * uecho_message_getpropertybycode
 ****************************************/

uEchoProperty* uecho_message_getpropertybycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  uEchoProperty* prop;
  size_t n;

  if (!msg)
    return NULL;

  for (n = 0; n < (size_t)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    if (!prop)
      continue;
    if (uecho_property_getcode(prop) == code)
      return prop;
  }

  return NULL;
}

/****************************************
 * uecho_message_parse
 ****************************************/

bool uecho_message_parse(uEchoMessage* msg, const byte* data, size_t dataLen)
{
  uEchoProperty* prop;
  size_t n, offset, count;

  if (!msg)
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

  // OPC

  uecho_message_setopc(msg, data[11]);

  // EP

  offset = 12;
  for (n = 0; n < (int)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    if (!prop)
      return false;

    // EPC

    if ((dataLen - 1) < offset)
      return false;
    uecho_property_setcode(prop, data[offset++]);

    // PDC

    if ((dataLen - 1) < offset)
      return false;
    count = data[offset++];

    // EDT

    if ((dataLen - 1) < (offset + count - 1))
      return false;
    if (!uecho_property_setdata(prop, (data + offset), count))
      return false;
    offset += count;
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

size_t uecho_message_size(uEchoMessage* msg)
{
  uEchoProperty* prop;
  size_t msgLen, n;

  if (!msg)
    return 0;

  msgLen = uEchoMessageMinLen;

  for (n = 0; n < (size_t)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    if (!prop)
      continue;
    msgLen += 2;
    msgLen += uecho_property_getdatasize(prop);
  }

  return msgLen;
}

/****************************************
 * uecho_message_getbytes
 ****************************************/

byte* uecho_message_getbytes(uEchoMessage* msg)
{
  uEchoProperty* prop;
  size_t n, offset, count;

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
  msg->bytes[11] = msg->OPC;

  offset = 12;
  for (n = 0; n < (size_t)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    if (!prop)
      continue;
    count = uecho_property_getdatasize(prop);
    msg->bytes[offset++] = uecho_property_getcode(prop);
    msg->bytes[offset++] = count;
    if (count <= 0)
      continue;
    memcpy((msg->bytes + offset), uecho_property_getdata(prop), count);
    offset += count;
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
