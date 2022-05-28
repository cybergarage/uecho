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

#include <uecho/_message.h>

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

  msg->opc = 0;
  msg->ep = NULL;

  msg->opc_set = 0;
  msg->ep_set = NULL;

  msg->opc_get = 0;
  msg->ep_get = NULL;

  msg->bytes = NULL;
  msg->from_addr = NULL;
  msg->to_addr = NULL;

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

bool uecho_properties_clear(uEchoProperty*** ep, byte* opc)
{
  int n;
  for (n = 0; n < (int)(*opc); n++) {
    uecho_property_delete((*ep)[n]);
    (*ep)[n] = NULL;
  }

  if (*ep) {
    free(*ep);
  }

  *ep = NULL;
  *opc = 0;

  return true;
}

bool uecho_message_clearproperties(uEchoMessage* msg)
{
  if (!msg)
    return false;
  uecho_properties_clear(&msg->ep, &msg->opc);
  uecho_properties_clear(&msg->ep_set, &msg->opc_set);
  uecho_properties_clear(&msg->ep_get, &msg->opc_get);
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

  if (msg->from_addr) {
    free(msg->from_addr);
    msg->from_addr = NULL;
  }

  if (msg->to_addr) {
    free(msg->to_addr);
    msg->to_addr = NULL;
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
  msg->tid[0] = (nval & 0xFF00) >> 8;
  msg->tid[1] = nval & 0x00FF;
  return true;
}

/****************************************
 * uecho_message_gettid
 ****************************************/

uEchoTID uecho_message_gettid(uEchoMessage* msg)
{
  uint16_t nval = (msg->tid[0] << 8) + msg->tid[1];
  return ntohs(nval);
}

/****************************************
 * uecho_message_setsourceobjectcode
 ****************************************/

bool uecho_message_setsourceobjectcode(uEchoMessage* msg, int code)
{
  return uecho_integer2byte(code, msg->seoj, uEchoEOJSize);
}

/****************************************
 * uecho_message_getsourceobjectcode
 ****************************************/

int uecho_message_getsourceobjectcode(uEchoMessage* msg)
{
  return uecho_byte2integer(msg->seoj, uEchoEOJSize);
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
  return uecho_integer2byte(code, msg->deoj, uEchoEOJSize);
}

/****************************************
 * uecho_message_getdestinationobjectcode
 ****************************************/

int uecho_message_getdestinationobjectcode(uEchoMessage* msg)
{
  return uecho_byte2integer(msg->deoj, uEchoEOJSize);
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

bool uecho_message_setopcep(byte* opc, uEchoProperty*** ep, byte count)
{
  int n;
  *opc = count;
  if (*opc <= 0)
    return true;
  *ep = (uEchoProperty**)malloc(sizeof(uEchoProperty*) * count);
  for (n = 0; n < (size_t)count; n++) {
    (*ep)[n] = uecho_property_new();
  }
  return true;
}

bool uecho_message_setopc(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->opc, &msg->ep, count);
}

bool uecho_message_setopcset(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->opc_set, &msg->ep_set, count);
}

bool uecho_message_setopcget(uEchoMessage* msg, byte count)
{
  if (!msg)
    return false;
  return uecho_message_setopcep(&msg->opc_get, &msg->ep_get, count);
}

/****************************************
 * uecho_message_getopc
 ****************************************/

byte uecho_message_getopc(uEchoMessage* msg)
{
  return msg->opc;
}

/****************************************
 * uecho_message_getopcset
 ****************************************/

byte uecho_message_getopcset(uEchoMessage* msg)
{
  return msg->opc_set;
}

/****************************************
 * uecho_message_getopcget
 ****************************************/

byte uecho_message_getopcget(uEchoMessage* msg)
{
  return msg->opc_get;
}

/****************************************
 * uecho_message_setehd1
 ****************************************/

void uecho_message_setehd1(uEchoMessage* msg, byte val)
{
  msg->ehd1 = val;
}

/****************************************
 * uecho_message_getehd1
 ****************************************/

byte uecho_message_getehd1(uEchoMessage* msg)
{
  return msg->ehd1;
}

/****************************************
 * uecho_message_setehd2
 ****************************************/

void uecho_message_setehd2(uEchoMessage* msg, byte val)
{
  msg->ehd2 = val;
}

/****************************************
 * uecho_message_getehd2
 ****************************************/

byte uecho_message_getehd2(uEchoMessage* msg)
{
  return msg->ehd2;
}

/****************************************
 * uecho_message_setesv
 ****************************************/

bool uecho_message_setesv(uEchoMessage* msg, uEchoEsv val)
{
  if (!msg)
    return false;

  msg->esv = val;

  return true;
}

/****************************************
 * uecho_message_getesv
 ****************************************/

uEchoEsv uecho_message_getesv(uEchoMessage* msg)
{
  if (!msg)
    return 0;

  return msg->esv;
}

/****************************************
 * uecho_message_requestesv2responseesv
 ****************************************/

bool uecho_message_requestesv2responseesv(uEchoEsv req_esv, uEchoEsv* res_esv)
{
  *res_esv = 0;

  switch (req_esv) {
  case uEchoEsvWriteRequestResponseRequired:
    *res_esv = uEchoEsvWriteResponse;
    return true;
  case uEchoEsvReadRequest:
    *res_esv = uEchoEsvReadResponse;
    return true;
  case uEchoEsvNotificationRequest:
    *res_esv = uEchoEsvNotification;
    return true;
  case uEchoEsvWriteReadRequest:
    *res_esv = uEchoEsvWriteReadResponse;
    return true;
  case uEchoEsvNotificationResponseRequired:
    *res_esv = uEchoEsvNotificationResponse;
    return true;
  }

  return false;
}

/****************************************
 * uecho_message_requestesv2errorresponseesv
 ****************************************/

bool uecho_message_requestesv2errorresponseesv(uEchoEsv req_esv, uEchoEsv* res_esv)
{
  *res_esv = 0;

  switch (req_esv) {
  case uEchoEsvWriteRequest:
    *res_esv = uEchoEsvWriteRequestError;
    return true;
  case uEchoEsvWriteRequestResponseRequired:
    *res_esv = uEchoEsvWriteRequestResponseRequiredError;
    return true;
  case uEchoEsvReadRequest:
    *res_esv = uEchoEsvReadRequestError;
    return true;
  case uEchoEsvNotificationRequest:
    *res_esv = uEchoEsvNotificationRequestError;
    return true;
  case uEchoEsvWriteReadRequest:
    *res_esv = uEchoEsvWriteReadRequestError;
    return true;
  }

  return false;
}

/****************************************
 * uecho_message_isresponserequired
 ****************************************/

bool uecho_message_isresponserequired(uEchoMessage* msg)
{
  uEchoEsv res_esv;

  if (!msg)
    return false;

  return uecho_message_requestesv2responseesv(msg->esv, &res_esv);
}

/****************************************
 * uecho_message_isresponsemessage
 ****************************************/

bool uecho_message_isresponsemessage(uEchoMessage* msg, uEchoMessage* res_meg)
{
  if (!msg || !res_meg)
    return false;

  if (uecho_message_gettid(msg) != uecho_message_gettid(res_meg))
    return false;

  if (uecho_message_getesv(msg) == uecho_message_getesv(res_meg))
    return false;

  return true;
}

/****************************************
 * uecho_message_setsourceaddress
 ****************************************/

void uecho_message_setsourceaddress(uEchoMessage* msg, const char* addr)
{
  uecho_strloc(addr, &msg->from_addr);
}

/****************************************
 * uecho_message_getsourceaddress
 ****************************************/

const char* uecho_message_getsourceaddress(uEchoMessage* msg)
{
  return msg->from_addr;
}

/****************************************
 * uecho_message_issourceaddress
 ****************************************/

bool uecho_message_issourceaddress(uEchoMessage* msg, const char* addr)
{
  return uecho_streq(msg->from_addr, addr);
}

/****************************************
 * uecho_message_setdestinationaddress
 ****************************************/

void uecho_message_setdestinationaddress(uEchoMessage* msg, const char* addr)
{
  uecho_strloc(addr, &msg->to_addr);
}

/****************************************
 * uecho_message_getdestinationaddress
 ****************************************/

const char* uecho_message_getdestinationaddress(uEchoMessage* msg)
{
  return msg->to_addr;
}

/****************************************
 * uecho_message_isdestinationaddress
 ****************************************/

bool uecho_message_isdestinationaddress(uEchoMessage* msg, const char* addr)
{
  return uecho_streq(msg->to_addr, addr);
}

/****************************************
 * uecho_message_iswriterequest
 ****************************************/

bool uecho_message_iswriterequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_iswriterequest(msg->esv);
}

/****************************************
 * uecho_message_isreadrequest
 ****************************************/

bool uecho_message_isreadrequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_isreadrequest(msg->esv);
}

/****************************************
 * uecho_message_isnotifyrequest
 ****************************************/

bool uecho_message_isnotifyrequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_isnotifyrequest(msg->esv);
}

/****************************************
 * uecho_message_iswriteresponse
 ****************************************/

bool uecho_message_iswriteresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_iswriteresponse(msg->esv);
}

/****************************************
 * uecho_message_isreadresponse
 ****************************************/

bool uecho_message_isreadresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_isreadresponse(msg->esv);
}

/****************************************
 * uecho_message_isnotifyresponse
 ****************************************/

bool uecho_message_isnotifyresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  return uecho_esv_isnotifyresponse(msg->esv);
}

/****************************************
 * uecho_message_isrequest
 ****************************************/

bool uecho_message_isrequest(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if (uecho_message_isreadrequest(msg))
    return true;
  if (uecho_message_iswriterequest(msg))
    return true;
  if (uecho_message_isnotifyrequest(msg))
    return true;
  return false;
}

/****************************************
 * uecho_message_isresponse
 ****************************************/

bool uecho_message_isresponse(uEchoMessage* msg)
{
  if (!msg)
    return false;
  if (uecho_message_isreadresponse(msg))
    return true;
  if (uecho_message_iswriteresponse(msg))
    return true;
  if (uecho_message_isnotifyresponse(msg))
    return true;
  return false;
}

/****************************************
 * uecho_message_addproperty
 ****************************************/

bool uecho_property_add(byte* opc, uEchoProperty*** ep, uEchoProperty* prop)
{
  *opc += 1;
  *ep = (uEchoProperty**)realloc(*ep, sizeof(uEchoProperty*) * (*opc));
  (*ep)[(*opc - 1)] = prop;
  return true;
}

bool uecho_message_addproperty(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->opc, &msg->ep, prop);
}

bool uecho_message_addpropertyset(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->opc_set, &msg->ep_set, prop);
}

bool uecho_message_addpropertyget(uEchoMessage* msg, uEchoProperty* prop)
{
  if (!msg || !prop)
    return false;
  return uecho_property_add(&msg->opc_get, &msg->ep_get, prop);
}

/****************************************
 * uecho_message_getproperty
 ****************************************/

uEchoProperty* uecho_property_get(byte opc, uEchoProperty** ep, size_t n)
{
  if (!ep || !(*ep) || ((opc - 1) < n))
    return NULL;
  return ep[n];
}

uEchoProperty* uecho_message_getproperty(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->opc, msg->ep, n);
}

uEchoProperty* uecho_message_getpropertyset(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->opc_set, msg->ep_set, n);
}

uEchoProperty* uecho_message_getpropertyget(uEchoMessage* msg, size_t n)
{
  if (!msg)
    return NULL;
  return uecho_property_get(msg->opc_get, msg->ep_get, n);
}

/****************************************
 * uecho_message_getpropertybycode
 ****************************************/

uEchoProperty* uecho_property_getbycode(byte opc, uEchoProperty** ep, uEchoPropertyCode code)
{
  uEchoProperty* prop;
  size_t n;
  for (n = 0; n < (size_t)(opc); n++) {
    prop = ep[n];
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
  return uecho_property_getbycode(msg->opc, msg->ep, code);
}

uEchoProperty* uecho_message_getpropertysetbycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  if (!msg)
    return NULL;
  return uecho_property_getbycode(msg->opc_set, msg->ep_set, code);
}

uEchoProperty* uecho_message_getpropertygetbycode(uEchoMessage* msg, uEchoPropertyCode code)
{
  if (!msg)
    return NULL;
  return uecho_property_getbycode(msg->opc_get, msg->ep_get, code);
}

/****************************************
 * uecho_message_setproperty
 ****************************************/

bool uecho_property_set(byte* opc, uEchoProperty*** ep, uEchoPropertyCode prop_code, const byte* prop_data, size_t prop_data_size)
{
  uEchoProperty* prop;
  prop = uecho_property_getbycode(*opc, *ep, prop_code);
  if (!prop) {
    prop = uecho_property_new();
    if (!prop)
      return false;
    uecho_property_setcode(prop, prop_code);
    if (!uecho_property_add(opc, ep, prop)) {
      uecho_property_delete(prop);
      return false;
    }
  }
  return uecho_property_setdata(prop, prop_data, prop_data_size);
}

bool uecho_message_setproperty(uEchoMessage* msg, uEchoPropertyCode prop_code, const byte* prop_data, size_t prop_data_size)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->opc, &msg->ep, prop_code, prop_data, prop_data_size);
}

bool uecho_message_setpropertyset(uEchoMessage* msg, uEchoPropertyCode prop_code, const byte* prop_data, size_t prop_data_size)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->opc_set, &msg->ep_set, prop_code, prop_data, prop_data_size);
}

bool uecho_message_setpropertyget(uEchoMessage* msg, uEchoPropertyCode prop_code, const byte* prop_data, size_t prop_data_size)
{
  if (!msg)
    return false;
  return uecho_property_set(&msg->opc_get, &msg->ep_get, prop_code, prop_data, prop_data_size);
}

/****************************************
 * uecho_message_parse
 ****************************************/

bool uecho_message_property_parse(byte opc, uEchoProperty** ep, const byte* data, size_t data_len, size_t* offset)
{
  uEchoProperty* prop;
  size_t n, count;

  // EP

  for (n = 0; n < (size_t)opc; n++) {
    prop = ep[n];
    if (!prop)
      return false;

    // EPC

    if ((data_len - 1) < (*offset))
      return false;
    uecho_property_setcode(prop, data[(*offset)++]);

    // PDC

    if ((data_len - 1) < (*offset))
      return false;
    count = data[(*offset)++];

    // EDT

    if ((data_len - 1) < ((*offset) + count - 1))
      return false;
    if (!uecho_property_setdata(prop, (data + (*offset)), count))
      return false;
    *offset += count;
  }

  return true;
}

bool uecho_message_isreadwritemessage(uEchoMessage* msg)
{
  if (msg->esv == uEchoEsvWriteReadRequest)
    return true;
  if (msg->esv == uEchoEsvWriteReadResponse)
    return true;
  if (msg->esv == uEchoEsvWriteReadRequestError)
    return true;
  return false;
}

bool uecho_message_parse(uEchoMessage* msg, const byte* data, size_t data_len)
{
  size_t offset;

  if (!msg)
    return false;

  if (!uecho_message_clear(msg))
    return false;

  if (data_len < uEchoMessageMinLen)
    return false;

  // Check Headers

  if (data[0] != uEchoEhd1)
    return false;

  if (data[1] != uEchoEhd2)
    return false;

  // TID

  msg->tid[0] = data[2];
  msg->tid[1] = data[3];

  // SEOJ

  msg->seoj[0] = data[4];
  msg->seoj[1] = data[5];
  msg->seoj[2] = data[6];

  // DEOJ

  msg->deoj[0] = data[7];
  msg->deoj[1] = data[8];
  msg->deoj[2] = data[9];

  // ESV

  uecho_message_setesv(msg, data[10]);

  if (uecho_message_isreadwritemessage(msg)) {
    // OPCSet
    uecho_message_setopcset(msg, data[11]);
    offset = 12;
    if (!uecho_message_property_parse(msg->opc_set, msg->ep_set, data, data_len, &offset))
      return false;
    // OPCGet
    if ((data_len - 1) < offset)
      return false;
    uecho_message_setopcget(msg, data[offset]);
    offset += 1;
    if (!uecho_message_property_parse(msg->opc_get, msg->ep_get, data, data_len, &offset))
      return false;
  }
  else {
    // OPC
    uecho_message_setopc(msg, data[11]);
    offset = 12;
    if (!uecho_message_property_parse(msg->opc, msg->ep, data, data_len, &offset))
      return false;
  }

  return true;
}

/****************************************
 * uecho_message_parsepacket
 ****************************************/

bool uecho_message_parsepacket(uEchoMessage* msg, uEchoDatagramPacket* dgm_pkt)
{
  if (!msg || !dgm_pkt)
    return false;

  if (!uecho_message_parse(msg, uecho_socket_datagram_packet_getdata(dgm_pkt), uecho_socket_datagram_packet_getlength(dgm_pkt)))
    return false;

  uecho_message_setsourceaddress(msg, uecho_socket_datagram_packet_getlocaladdress(dgm_pkt));
  uecho_message_setsourceaddress(msg, uecho_socket_datagram_packet_getremoteaddress(dgm_pkt));

  return true;
}

/****************************************
 * uecho_message_size
 ****************************************/

size_t uecho_message_opcsize(byte opc, uEchoProperty** ep)
{
  uEchoProperty* prop;
  size_t msg_len, n;

  msg_len = 0;
  for (n = 0; n < (size_t)(opc); n++) {
    prop = ep[n];
    if (!prop)
      continue;
    msg_len += 2;
    msg_len += uecho_property_getdatasize(prop);
  }

  return msg_len;
}

size_t uecho_message_size(uEchoMessage* msg)
{
  size_t msg_len;

  if (!msg)
    return 0;

  msg_len = uEchoMessageMinLen;

  if (uecho_message_isreadwritemessage(msg)) {
    msg_len += uecho_message_opcsize(msg->opc_set, msg->ep_set);
    msg_len += 1;
    msg_len += uecho_message_opcsize(msg->opc_get, msg->ep_get);
  }
  else {
    msg_len += uecho_message_opcsize(msg->opc, msg->ep);
  }

  return msg_len;
}

/****************************************
 * uecho_message_getbytes
 ****************************************/

bool uecho_message_setopcbytes(uEchoMessage* msg, byte opc, uEchoProperty** ep, size_t* offset)
{
  uEchoProperty* prop;
  size_t n, count;
  for (n = 0; n < (size_t)opc; n++) {
    prop = ep[n];
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

  msg->bytes[0] = msg->ehd1;
  msg->bytes[1] = msg->ehd2;
  msg->bytes[2] = msg->tid[0];
  msg->bytes[3] = msg->tid[1];
  msg->bytes[4] = msg->seoj[0];
  msg->bytes[5] = msg->seoj[1];
  msg->bytes[6] = msg->seoj[2];
  msg->bytes[7] = msg->deoj[0];
  msg->bytes[8] = msg->deoj[1];
  msg->bytes[9] = msg->deoj[2];
  msg->bytes[10] = msg->esv;

  if (uecho_message_isreadwritemessage(msg)) {
    msg->bytes[11] = msg->opc_set;
    offset = 12;
    uecho_message_setopcbytes(msg, msg->opc_set, msg->ep_set, &offset);
    msg->bytes[offset] = msg->opc_get;
    offset += 1;
    uecho_message_setopcbytes(msg, msg->opc_get, msg->ep_get, &offset);
  }
  else {
    msg->bytes[11] = msg->opc;
    offset = 12;
    uecho_message_setopcbytes(msg, msg->opc, msg->ep, &offset);
  }

  return msg->bytes;
}

/****************************************
 * uecho_message_set
 ****************************************/

bool uecho_message_set(uEchoMessage* msg, uEchoMessage* src_msg)
{
  uEchoProperty *prop, *src_prop;
  size_t src_msg_opc, n;

  if (!msg || !src_msg)
    return false;

  uecho_message_clear(msg);

  uecho_message_setehd1(msg, uecho_message_getehd1(src_msg));
  uecho_message_setehd2(msg, uecho_message_getehd2(src_msg));
  uecho_message_settid(msg, uecho_message_gettid(src_msg));
  uecho_message_setsourceobjectcode(msg, uecho_message_getsourceobjectcode(src_msg));
  uecho_message_setdestinationobjectcode(msg, uecho_message_getdestinationobjectcode(src_msg));
  uecho_message_setesv(msg, uecho_message_getesv(src_msg));
  uecho_message_setsourceaddress(msg, uecho_message_getsourceaddress(src_msg));
  uecho_message_setdestinationaddress(msg, uecho_message_getdestinationaddress(src_msg));

  src_msg_opc = uecho_message_getopc(src_msg);
  for (n = 0; n < src_msg_opc; n++) {
    src_prop = uecho_message_getproperty(src_msg, n);
    if (!src_prop)
      continue;
    prop = uecho_property_copy(src_prop);
    if (!src_prop)
      continue;
    uecho_message_addproperty(msg, prop);
  }

  return true;
}

/****************************************
 * uecho_message_copy
 ****************************************/

uEchoMessage* uecho_message_copy(uEchoMessage* src_msg)
{
  uEchoMessage* new_msg;

  new_msg = uecho_message_new();
  if (!new_msg)
    return NULL;

  uecho_message_set(new_msg, src_msg);

  return new_msg;
}

/****************************************
 * uecho_message_equals
 ****************************************/

bool uecho_message_equals(uEchoMessage* msg1, uEchoMessage* msg2)
{
  size_t msg_size;
  byte *msg1_bytes, *msg2_bytes;

  msg_size = uecho_message_size(msg1);
  if (msg_size != uecho_message_size(msg2))
    return false;

  msg1_bytes = uecho_message_getbytes(msg1);
  msg2_bytes = uecho_message_getbytes(msg2);

  return (memcmp(msg1_bytes, msg2_bytes, msg_size) == 0) ? true : false;
}
