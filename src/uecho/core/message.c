/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <arpa/inet.h>

#include <uecho/net/socket.h>
#include <uecho/core/message.h>
#include <uecho/misc.h>

/****************************************
* uecho_message_new
****************************************/

uEchoMessage *uecho_message_new(void)
{
	uEchoMessage *msg;

	msg = (uEchoMessage *)malloc(sizeof(uEchoMessage));

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
  
  return msg;
}

/****************************************
* uecho_message_delete
****************************************/

void uecho_message_delete(uEchoMessage *msg)
{
  uecho_message_clear(msg);

	free(msg);
}

/****************************************
 * uecho_message_clear
 ****************************************/

void uecho_message_clear(uEchoMessage *msg)
{
  int n;
  
  if (0 < msg->OPC) {
    for (n=0; n<(int)(msg->OPC); n++) {
      uecho_property_delete(msg->EP[n]);
      msg->EP[n] = NULL;
    }
  }
  
  if (msg->EP) {
    free(msg->EP);
    msg->EP = NULL;
  }

  if (msg->bytes) {
    free(msg->bytes);
    msg->bytes = NULL;
  }
}

/****************************************
 * uecho_message_settid
 ****************************************/

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val)
{
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

uEchoTID uecho_message_gettid(uEchoMessage *msg)
{
  uint16_t nval = (msg->TID[0] << 8) + msg->TID[1];
  return ntohs(nval);
}

/****************************************
 * uecho_message_setsourceobjectcode
 ****************************************/

bool uecho_message_setsourceobjectcode(uEchoMessage *msg, uEchoObjectCode code)
{
  return uecho_integer2byte(code, msg->SEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_setdestinationobjectcode
 ****************************************/

bool uecho_message_setdestinationobjectcode(uEchoMessage *msg, uEchoObjectCode code)
{
  return uecho_integer2byte(code, msg->DEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_getsourceobjectcode
 ****************************************/

uEchoObjectCode uecho_message_getsourceobjectcode(uEchoMessage *msg)
{
  return uecho_byte2integer(msg->SEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_getdestinationobjectcode
 ****************************************/

uEchoObjectCode uecho_message_getdestinationobjectcode(uEchoMessage *msg)
{
  return uecho_byte2integer(msg->DEOJ, uEchoEOJSize);
}

/****************************************
 * uecho_message_setopc
 ****************************************/

bool uecho_message_setopc(uEchoMessage *msg, byte count)
{
  int n;
  
  uecho_message_clear(msg);
  
  msg->OPC = count;
  
  if (msg->OPC <= 0)
    return true;
  
  msg->EP = (uEchoProperty**)malloc(sizeof(uEchoProperty*) * count);
  for (n=0; n<(int)(msg->OPC); n++) {
    msg->EP[n] = uecho_property_new();
  }
  
  return true;
}

/****************************************
 * uecho_message_getopc
 ****************************************/

byte uecho_message_getopc(uEchoMessage *msg)
{
    return msg->OPC;
}

/****************************************
 * uecho_message_setehd1
 ****************************************/

void uecho_message_setehd1(uEchoMessage *msg, byte val)
{
  msg->EHD1 = val;
}

/****************************************
 * uecho_message_getehd1
 ****************************************/

byte uecho_message_getehd1(uEchoMessage *msg)
{
  return msg->EHD1;
}

/****************************************
 * uecho_message_setehd2
 ****************************************/

void uecho_message_setehd2(uEchoMessage *msg, byte val)
{
  msg->EHD2 = val;
}

/****************************************
 * uecho_message_getehd2
 ****************************************/

byte uecho_message_getehd2(uEchoMessage *msg)
{
  return msg->EHD2;
}

/****************************************
 * uecho_message_setesv
 ****************************************/

void uecho_message_setesv(uEchoMessage *msg, uEchoEsvType val)
{
  msg->ESV = val;
}

/****************************************
 * uecho_message_getesv
 ****************************************/

uEchoEsvType uecho_message_getesv(uEchoMessage *msg)
{
  return msg->ESV;
}

/****************************************
 * uecho_message_getproperty
 ****************************************/

uEchoProperty *uecho_message_getproperty(uEchoMessage *msg, size_t n)
{
  if ((msg->OPC - 1) < n)
    return NULL;
  return msg->EP[n];
}

/****************************************
 * uecho_message_parse
 ****************************************/

bool uecho_message_parse(uEchoMessage *msg, const byte *data, size_t dataLen)
{
  uEchoProperty *prop;
  size_t n, offset, count;
  
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
  for (n = 0; n<(int)(msg->OPC); n++) {
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

bool uecho_message_parsepacket(uEchoMessage *msg, uEchoDatagramPacket *dgmPkt)
{
  if (!msg || !dgmPkt)
    return false;
  
  return uecho_message_parse(
        msg,
        uecho_socket_datagram_packet_getdata(dgmPkt),
        uecho_socket_datagram_packet_getlength(dgmPkt));
}

/****************************************
 * uecho_message_size
 ****************************************/

size_t uecho_message_size(uEchoMessage *msg)
{
  uEchoProperty *prop;
  size_t msgLen, n;
  
  msgLen = uEchoMessageMinLen;

  for (n = 0; n<(size_t)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    msgLen += 2;
    msgLen += uecho_property_getdatasize(prop);
  }
  
  return msgLen;
}

/****************************************
 * uecho_message_getbytes
 ****************************************/

byte *uecho_message_getbytes(uEchoMessage *msg)
{
  uEchoProperty *prop;
  size_t n, offset, count;
  
  if (msg->bytes) {
    free(msg->bytes);
  }

  msg->bytes = (byte *)malloc(uecho_message_size(msg));

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
  for (n = 0; n<(size_t)(msg->OPC); n++) {
    prop = uecho_message_getproperty(msg, n);
    count = uecho_property_getdatasize(prop);
    msg->bytes[offset++] = uecho_property_getcode(prop);
    msg->bytes[offset++] = count;
    memcpy((msg->bytes + offset), uecho_property_getdata(prop), count);
    offset += count;
  }
  
  return msg->bytes;
}

/****************************************
 * uecho_message_equals
 ****************************************/

bool uecho_message_equals(uEchoMessage *msg1, uEchoMessage *msg2)
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
