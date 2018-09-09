![logo](img/logo.png)

# Overview of uEcho Controller

The controller is a special node of [ECHONETLite][enet] to control other nodes, it can find the nodes in the local area network and send any messages into the found devices.

## Creating Controller

### 1. Starting Controller

To start a controller, create a controller using `uecho_controller_new` and start the controller using `uecho_controller_start` as the following:

```
uEchoController *ctrl = uecho_controller_new();
uecho_controller_start(ctrl);
```

### 2. Searching Nodes

Next, use `uecho_controller_searchallobjects` to search other nodes in the local area network as the following:

```
uEchoController *ctrl;
....
uecho_controller_searchallobjects(ctrl);
```

### 3. Getting Nodes and Objects

After the searching, use `uecho_controller_getnodes` and `uecho_node_next` to get all found nodes. [ECHONETLite](http://www.echonet.gr.jp/english/index.htm) node can have multiple objects, use `uecho_node_getobjects` and `uecho_object_next` to get all objects in the node.

```
uEchoController *ctrl;
....
uEchoNode *node;
uEchoObject *obj;

for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
  for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
    printf("%s %06X\n", uecho_node_getaddress(node), uecho_object_getcode(obj));
  }
}
```

### 4. Creating Control Message

To control the found objects, create the control message using uecho_message_new() as the following.

```
uEchoMessage *msg;
msg = uecho_message_new();
uecho_message_setdestinationobjectcode(msg, 0xXXXXXX);
uecho_message_setesv(msg, 0xXX);
uecho_message_setproperty(msg, epc, ..., ...);
....
```

To create the message, developer should only set the following message objects using the message functions.

- DEOJ : Destination ECHONET Lite object specification
- ESV : ECHONET Lite service
- EPC : ECHONET Lite Property
- PDC : Property data counter
- EDT : Property value data

The uEcho controller sets the following message objects automatically when the message is sent.

- EHD1 : ECHONET Lite message header 1
- EHD2 : ECHONET Lite message header 2
- TID￼  : Transaction ID
- SEOJ : Source ECHONET Lite object specification
- OPC  : Number of processing properties

### 5. Sending Messages

To send the created message, use `uecho_controller_sendmessage` as the following:

```
uEchoController *ctrl;
uEchoObject *dstObj;
uEchoMessage *msg;
....
uecho_controller_sendmessage(ctrl, dstObj, msg);
```

Basically, all messages of [ECHONETLite](http://www.echonet.gr.jp/english/index.htm) is async. To handle the async response of the message request, use `uecho_controller_postmessage` as the following:

```
uEchoController *ctrl;
uEchoObject *dstObj;
uEchoMessage *msg, *resMsg;
....
resMsg = uecho_message_new();
if (uecho_controller_postmessage(ctrl, dstObj, msg, resMsg)) {
  ....  
}
uecho_message_delete(resMsg);
```

## Next Steps

Let's check the following documentations to know the controller functions of uEcho in more detail.

- [uEcho Examples](./uecho_examples.md)
- [Inside of uEcho Controller](./uecho_controller_inside.md)

[enet]:http://echonet.jp/english/
