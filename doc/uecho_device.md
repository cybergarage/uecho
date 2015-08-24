# uEcho for C

## Making Devices

uEcho supports your original standard devices of ECHONET Lite specification easily.

## Creating Devices

### 1. Creating Node

To create your original device, use `uecho_node_new` as the following at first.

```
uEchoNode *node = uecho_node_new();
```

The new node has only a node profile class object, and it has no device object. The node profile object is updated automatically when new devices are added into the node or the any properties in the node are changed.

### 2. Adding Device Object

The new node has no device object. To add your device objects, create a new device object using `uecho_device_new()` and add the device object into the node using `uecho_node_addobject()` as the following.

```
obj = uecho_device_new();
uecho_object_setcode(obj, 0xXXXXXX);

uecho_node_addobject(node, obj);
```

### 3. Setting Observers

![](img/node_msg_observer.png)

### 4. Start Node

```
uecho_node_start(node);
```
