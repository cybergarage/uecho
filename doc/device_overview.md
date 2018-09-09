![logo](img/logo.png)

# Overview of uEcho Device

## Making Devices

uEcho supports your original standard devices of [ECHONET Lite][enet] specification easily. This document explains to create your original  [ECHONET Lite][enet] device step by step.

## Creating Devices

### 1. Creating Node

To create your original device, use `uecho_node_new()` as the following at first.

```
uEchoNode *node = uecho_node_new();
```

The new node has only a node profile class object, and it has no device object. The node profile object is updated automatically when new devices are added into the node or the any properties in the node are changed.

### 2. Creating Device Object

The new node has no device object. To add your device objects, create a new device object using `uecho_object_new()` or `uecho_device_new()`. `uecho_object_new()` create a null object, but `uecho_device_new()` create a object which added some mandatory properties of ECHONET device object super class requirements.


Next, according to ECHONET standard device specification [\[1\]][enet-spec], set the manufacturer code, the object code and other standard properties code into the created device object. Then, add the device object into the node using `uecho_node_addobject()` as the following:

```
uEchoObject *obj;
obj = uecho_device_new();
uecho_object_setmanufacturercode(obj, 0xXXXXXX);
uecho_object_setcode(obj, 0xXXXXXX);
....
uecho_object_setproperty(obj, 0xXX, uEchoPropertyAttrReadWrite);
uecho_object_setpropertydata(obj, 0xXX, ....., ....);
....
uecho_node_addobject(node, obj);
```

### 3. Setting Observers

To implement the device, developer has only to handle write requests from other nodes because eEcho handles other read and notification requests automatically. To handle the write requests, use `uecho_object_setpropertywriterequestlistener()` as the following:

```
void object_propertywriterequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoProperty *prop)
{
  size_t dataSize;
  byte *data;

  dataSize = uecho_property_getdatasize(prop);
  data = uecho_property_getdata(prop);

  ....
}
....
{
  uEchoObject *obj;
  byte propCode;
  ....
  uecho_object_setpropertywriterequestlistener(obj, propCode, object_propertywriterequestlistener)  
}
```

`uecho_object_setpropertywriterequestlistener()` sets the handler for all write request ESV types, Write (0x60) , Write Response Required (0x61) and Write & read Request (0x6E). To set handlers for each ESV, use `uecho_object_setpropertyrequeslistener()`.

### 4. Start Node

Finally, start the node to use `uecho_node_start` as the following:

```
uEchoNode *node;
....
uecho_node_start(node);
```

## Next Steps

Let's check the following documentations to know the device functions of uEcho in more detail.

- [uEcho Examples](./uecho_examples.md)
- [Inside of uEcho Device](./uecho_device_inside.md)

## References

- \[1\] [Detailed Requirements for ECHONET Device objects][enet-spec]

[enet]:http://echonet.jp/english/
[enet-spec]:http://www.echonet.gr.jp/english/spec/index.htm
