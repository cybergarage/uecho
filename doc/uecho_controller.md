# uEcho for C

## Making Controller

The controller is a special node to control other nodes, it can find the nodes in the local area network and send messages into the found devices.

## Control Devices

### 1. Creating Controller

To create a controller, use `uecho_controller_new` as the following.

```
uEchoController *ctrl = uecho_controller_new();
```

### 2. Starting Controller

```
uecho_controller_start(ctrl);
```

### 3. Searching Devices

```
uecho_controller_searchallobjects(ctrl);
```

```
for ()
```

### 4. Sending Messages

```
msg = uecho_message_new();
uecho_message_setdestinationobjectcode(msg, 0xXXXXXX);
uecho_message_setesv(msg, 0xXX);
uecho_message_setproperty(msg, epc, ..., ...);
....
```

```
uEchoObject *dstObj .....
uecho_controller_sendmessage(ctrl, dstObj, msg);
```

Basically, all messages of ECHONETLite is async

```
uecho_controller_postmessage(ctrl, dstObj, msg, resMsg)
```
