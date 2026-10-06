/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>

#include <uecho/_controller.h>
#include <uecho/profile.h>
#include <uecho/util/timer.h>

#include "TestDevice.h"

static uEchoMessage* uecho_test_response(uEchoMessage* request, uEchoEsv esv)
{
  uEchoMessage* response = uecho_message_copy(request);
  uecho_message_setesv(response, esv);
  uecho_message_setsourceobjectcode(response, uecho_message_getdestinationobjectcode(request));
  uecho_message_setdestinationobjectcode(response, uecho_message_getsourceobjectcode(request));
  uecho_message_setsourceaddress(response, "192.0.2.1");
  return response;
}

BOOST_AUTO_TEST_CASE(ControllerResponseIdentity)
{
  uEchoController* controller = uecho_controller_new();
  uEchoMessage* request = uecho_message_new();
  uEchoMessage* result = uecho_message_new();
  uecho_message_settid(request, 42);
  uecho_message_setsourceobjectcode(request, uEchoNodeProfileObject);
  uecho_message_setdestinationobjectcode(request, 0x001101);
  uecho_message_setdestinationaddress(request, "192.0.2.1");
  uecho_message_setesv(request, uEchoEsvReadRequest);
  uecho_message_setproperty(request, 0x80, NULL, 0);
  uecho_controller_setpostrequestmessage(controller, request);
  uecho_controller_setpostresponsemessage(controller, result);
  uEchoMessage* response = uecho_test_response(request, uEchoEsvReadResponse);
  BOOST_CHECK(uecho_controller_ispostresponsemessage(controller, response));
  uecho_message_setsourceaddress(response, "192.0.2.2");
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK(!uecho_controller_ispostresponsereceived(controller));
  uecho_message_setsourceaddress(response, "192.0.2.1");
  uecho_message_setsourceobjectcode(response, 0x001102);
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_message_setsourceobjectcode(response, 0x001101);
  uecho_message_setdestinationobjectcode(response, 0x001101);
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_message_setdestinationobjectcode(response, uEchoNodeProfileObject);
  uecho_message_settid(response, 43);
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_message_settid(response, 42);
  uecho_message_setesv(response, uEchoEsvWriteResponse);
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_message_setesv(response, uEchoEsvReadResponse);
  uecho_property_setcode(uecho_message_getproperty(response, 0), 0x81);
  BOOST_CHECK(!uecho_controller_ispostresponsemessage(controller, response));
  uecho_property_setcode(uecho_message_getproperty(response, 0), 0x80);
  byte first = 0x30, second = 0x31;
  uecho_message_setproperty(response, 0x80, &first, 1);
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK(uecho_controller_ispostresponsereceived(controller));
  uecho_message_setproperty(response, 0x80, &second, 1);
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK_EQUAL(uecho_property_getdata(uecho_message_getproperty(result, 0))[0], first);
  uecho_controller_setpostrequestmessage(controller, NULL);
  uecho_controller_setpostresponsemessage(controller, NULL);
  uecho_message_delete(response);
  uecho_message_delete(result);
  uecho_message_delete(request);
  uecho_controller_delete(controller);
}

BOOST_AUTO_TEST_CASE(ControllerResponseServicesAndSetGetCopy)
{
  for (uEchoEsv esv : { uEchoEsv(uEchoEsvWriteRequest), uEchoEsv(uEchoEsvWriteRequestResponseRequired), uEchoEsv(uEchoEsvReadRequest), uEchoEsv(uEchoEsvNotificationRequest), uEchoEsv(uEchoEsvWriteReadRequest), uEchoEsv(uEchoEsvNotificationResponseRequired) }) {
    uEchoMessage* request = uecho_message_new();
    uecho_message_settid(request, 1);
    uecho_message_setsourceobjectcode(request, uEchoNodeProfileObject);
    uecho_message_setdestinationobjectcode(request, 0x001101);
    uecho_message_setesv(request, esv);
    if (esv == uEchoEsvWriteReadRequest) {
      byte value = 0x30;
      uecho_message_setpropertyset(request, 0x80, &value, 1);
      uecho_message_setpropertyget(request, 0x81, NULL, 0);
    }
    else
      uecho_message_setproperty(request, 0x80, NULL, 0);
    uEchoEsv success, error;
    if (uecho_message_requestesv2responseesv(esv, &success)) {
      uEchoMessage* response = uecho_test_response(request, success);
      BOOST_CHECK(uecho_message_isresponsemessage(request, response));
      uecho_message_delete(response);
    }
    if (uecho_message_requestesv2errorresponseesv(esv, &error)) {
      uEchoMessage* response = uecho_test_response(request, error);
      BOOST_CHECK(uecho_message_isresponsemessage(request, response));
      uecho_message_delete(response);
    }
    uEchoMessage* unrelated = uecho_test_response(request, uEchoEsvWriteRequest);
    BOOST_CHECK(!uecho_message_isresponsemessage(request, unrelated));
    uecho_message_delete(unrelated);
    uecho_message_delete(request);
  }
  uEchoMessage* request = uecho_message_new();
  uecho_message_setsourceobjectcode(request, uEchoNodeProfileObject);
  uecho_message_setdestinationobjectcode(request, 0x001100);
  uecho_message_setesv(request, uEchoEsvReadRequest);
  uEchoMessage* response = uecho_test_response(request, uEchoEsvReadResponse);
  uecho_message_setsourceobjectcode(response, 0x001101);
  BOOST_CHECK(uecho_message_isresponsemessage(request, response));
  uecho_message_setsourceobjectcode(response, 0x001201);
  BOOST_CHECK(!uecho_message_isresponsemessage(request, response));
  uecho_message_delete(response);
  uecho_message_delete(request);
}

BOOST_AUTO_TEST_CASE(ControllerFailedPostClearsPendingState)
{
  uEchoController* controller = uecho_controller_new();
  uEchoNode* peer = uecho_node_new();
  uecho_node_setaddress(peer, "");
  uEchoMessage* request = uecho_message_search_new();
  uEchoMessage* response = uecho_message_new();
  BOOST_CHECK(!uecho_controller_postmessage(controller, peer, request, response));
  BOOST_CHECK(!uecho_controller_ispostresponsewaiting(controller));
  uecho_message_delete(request);
  uecho_message_delete(response);
  uecho_node_delete(peer);
  uecho_controller_delete(controller);
}

BOOST_AUTO_TEST_CASE(ControllerDiscoveryLimits)
{
  uEchoController* controller = uecho_controller_new();
  uEchoMessage* response = uecho_message_search_new();
  uecho_message_setesv(response, uEchoEsvReadResponse);
  uecho_message_setsourceaddress(response, "192.0.2.1");
  byte entry[] = { 2, 0x00, 0x11, 0x01 };
  uecho_message_setproperty(response, uEchoNodeProfileClassSelfNodeInstanceListS, entry, sizeof(entry));
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), 0);
  entry[0] = 1;
  uecho_message_setsourceobjectcode(response, 0x001101);
  uecho_message_setproperty(response, uEchoNodeProfileClassSelfNodeInstanceListS, entry, sizeof(entry));
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), 0);
  uecho_message_setsourceobjectcode(response, uEchoNodeProfileObjectReadOnly);
  uecho_controller_servermessagelistener(controller, response);
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), 1);
  BOOST_CHECK(uecho_node_hasobjectbycode(uecho_controller_getnodebyaddress(controller, "192.0.2.1"), 0x001101));
  uecho_message_setsourceobjectcode(response, uEchoNodeProfileObject);
  for (size_t n = 1; n <= UECHO_CONTROLLER_MAX_OBJECTS_PER_NODE + 8; n++) {
    unsigned int code = 0x001000 + n;
    entry[1] = (code >> 16) & 0xFF;
    entry[2] = (code >> 8) & 0xFF;
    entry[3] = code & 0xFF;
    uecho_message_setproperty(response, uEchoNodeProfileClassSelfNodeInstanceListS, entry, sizeof(entry));
    uecho_controller_servermessagelistener(controller, response);
  }
  uEchoNode* peer = uecho_controller_getnodebyaddress(controller, "192.0.2.1");
  BOOST_REQUIRE(peer);
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(peer), UECHO_CONTROLLER_MAX_OBJECTS_PER_NODE);
  BOOST_CHECK(uecho_node_hasobjectbycode(peer, 0x001001));
  for (size_t n = 2; n <= UECHO_CONTROLLER_MAX_NODES + 8; n++) {
    char address[32];
    snprintf(address, sizeof(address), "192.0.2.%zu", n);
    uecho_message_setsourceaddress(response, address);
    byte empty[] = { 0 };
    uecho_message_setproperty(response, uEchoNodeProfileClassSelfNodeInstanceListS, empty, sizeof(empty));
    uecho_controller_servermessagelistener(controller, response);
  }
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), UECHO_CONTROLLER_MAX_NODES);
  uecho_message_delete(response);
  uecho_controller_delete(controller);
}

BOOST_AUTO_TEST_CASE(ControllerRun)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_REQUIRE(!uecho_controller_getuserdata(ctrl));

  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_stop(ctrl));

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoOptionNone), false);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_enableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), true);

  uecho_controller_disableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerUdpOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_disableudpserver(ctrl);
  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), false);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), false);

  uecho_controller_enableudpserver(ctrl);
  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerRequest)
{
  // Create Controller (Disable UDP Server)

  uEchoController* ctrl = uecho_controller_new();
  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_isrunning(ctrl));

  // Add Device

  uEchoNode* node = uecho_controller_getlocalnode(ctrl);
  uEchoObject* dev = uecho_test_createtestdevice();
  BOOST_REQUIRE(uecho_node_addobject(node, dev));

  // Search (NotificationRequest instead of ReadRequest)

  BOOST_REQUIRE(uecho_controller_searchwithesv(ctrl, uEchoEsvNotificationRequest));

  // Find device

  uEchoObject* foundObj = uecho_test_findlocaldevice(ctrl);
  BOOST_REQUIRE(foundObj);
  if (!foundObj)
    return;

  uEchoNode* foundNode = uecho_object_getparentnode(foundObj);
  BOOST_REQUIRE(foundNode);
  if (!foundNode)
    return;

  // Post Message (ReadRequest)

  uEchoMessage* msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));

  uEchoMessage* res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  uEchoProperty* prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 1);
  byte* propData = uecho_property_getdata(prop);
  BOOST_REQUIRE(propData);
  BOOST_REQUIRE_EQUAL(propData[0], UECHO_TEST_PROPERTY_SWITCH_DEFAULT);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Write Message (WriteRequest:OFF)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequestResponseRequired);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  byte postByte = UECHO_TEST_PROPERTY_SWITCH_OFF;
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, &postByte, 1));

  res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvWriteResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Message (ReadRequest)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));

  res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 1);
  propData = uecho_property_getdata(prop);
  BOOST_REQUIRE(propData);
  BOOST_REQUIRE_EQUAL(propData[0], UECHO_TEST_PROPERTY_SWITCH_OFF);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Teminate

  BOOST_REQUIRE(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);
}
