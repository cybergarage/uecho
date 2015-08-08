/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/message.h>
#include <uecho/net/socket.h>

/****************************************
* uecho_message_new
****************************************/

uEchoMessage *uecho_message_new()
{
	uEchoMessage *msg;

	msg = (uEchoMessage *)malloc(sizeof(uEchoMessage));

  if (!msg)
    return NULL;

  uecho_message_setehd1(msg, uEchoEhd1);
  uecho_message_setehd2(msg, uEchoEhd2);

  uecho_message_settid(msg, 0);
    
  msg->SEOJ = uecho_object_new();
  msg->DEOJ = uecho_object_new();
    
  uecho_message_setesv(msg, 0);

  msg->EP = NULL;
  msg->OPC = 0;
  
  msg->bytes = NULL;
  
  return msg;
}

/****************************************
* uecho_message_delete
****************************************/

void uecho_message_delete(uEchoMessage *msg) {
  uecho_message_clear(msg);

  uecho_object_delete(msg->SEOJ);
  uecho_object_delete(msg->DEOJ);
  
	free(msg);
}

/****************************************
 * uecho_message_clear
 ****************************************/

void uecho_message_clear(uEchoMessage *msg) {
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

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val) {
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

uEchoTID uecho_message_gettid(uEchoMessage *msg) {
  uint16_t nval = (msg->TID[0] << 8) + msg->TID[1];
  return ntohs(nval);
}

/****************************************
 * uecho_message_setopc
 ****************************************/

bool uecho_message_setopc(uEchoMessage *msg, byte count) {
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

byte uecho_message_getopc(uEchoMessage *msg) {
    return msg->OPC;
}

/****************************************
 * uecho_message_getopc
 ****************************************/

uEchoProperty *uecho_message_getproperty(uEchoMessage *msg, size_t n) {
  if ((msg->OPC - 1) < n)
    return NULL;
  return msg->EP[n];
}

/****************************************
 * uecho_message_parse
 ****************************************/

bool uecho_message_parse(uEchoMessage *msg, const byte *data, size_t dataLen) {
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
  
  uecho_object_setclassgroupcode(msg->SEOJ, data[4]);
  uecho_object_setclasscode(msg->SEOJ, data[5]);
  uecho_object_setinstancecode(msg->SEOJ, data[6]);
  
  // DEOJ
  
  uecho_object_setclassgroupcode(msg->DEOJ, data[7]);
  uecho_object_setclasscode(msg->DEOJ, data[8]);
  uecho_object_setinstancecode(msg->DEOJ, data[9]);
  
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
  return uecho_message_parse(msg, uecho_socket_datagram_packet_getdata(dgmPkt), uecho_socket_datagram_packet_getlength(dgmPkt));
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
    if (!prop)
      continue;
    
    msgLen += 2;
    msgLen += uecho_property_getcount(prop);
  }
  
  return msgLen;
}

/****************************************
 * uecho_message_getbytes
 ****************************************/

byte *uecho_message_getbytes(uEchoMessage *msg)
{
  if (msg->bytes) {
    free(msg->bytes);
  }

  msg->bytes = (byte *)malloc(uecho_message_size(msg));

  return msg->bytes;
}
