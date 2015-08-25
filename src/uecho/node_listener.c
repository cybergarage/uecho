/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>
#include <uecho/core/server.h>

/****************************************
 * uecho_object_notifymessage
 ****************************************/

void uecho_object_notifymessageproperty(uEchoObject *obj, uEchoProperty *msgProp)
{
}

/****************************************
 * uecho_object_notifymessage
 ****************************************/

void uecho_object_notifymessage(uEchoObject *obj, uEchoMessage *msg)
{
  uEchoProperty *msgProp, *nodeProp;
  uEchoPropertyCode msgPropCode;
  int msgPropSize;
  byte *msgPropData;
  uEchoEsv msgEsv;
  int msgOpc, n;
  
  if (!obj)
    return;
  
  msgEsv = uecho_message_getesv(msg);
  msgOpc = uecho_message_getopc(msg);
  
  // Write properties
  
  if (uecho_message_iswriterequest(msg)) {
    for (n=0; n<msgOpc; n++) {
      msgProp = uecho_message_getproperty(msg, n);
      if (!msgProp)
        continue;
      
      msgPropCode = uecho_property_getcode(msgProp);
      nodeProp = uecho_object_getproperty(obj, msgPropCode);
      if (!nodeProp)
        continue;
      
      if (!uecho_property_iswritable(nodeProp))
        continue;
      
      msgPropSize = uecho_property_getdatasize(msgProp);
      msgPropData = uecho_property_getdata(msgProp);
      
      uecho_property_setdata(nodeProp, msgPropData, msgPropSize);
    }
  }
  
  // Notify observers
  
  for (n=0; n<msgOpc; n++) {
    msgProp = uecho_message_getproperty(msg, n);
    if (!msgProp)
      continue;
    uecho_object_notifymessageproperty(obj, msgProp);
  }
}

/****************************************
* uecho_node_servermessagelistener
****************************************/

void uecho_node_servermessagelistener(uEchoServer *server, uEchoMessage *msg)
{
  uEchoNode *node;
  uEchoObjectCode destObjCode;
  uEchoObject *nodeDestObj;
  
  node = (uEchoNode *)uecho_server_getuserdata(server);
  if (!node)
    return;
  
  destObjCode = uecho_message_getdestinationobjectcode(msg);
  nodeDestObj = uecho_node_getobjectbycode(node, destObjCode);

  if (!nodeDestObj)
    return;

  uecho_object_notifymessage(nodeDestObj, msg);
}
