# Overview of uEcho Device

## Making Devices

The `uecho` framework enables you to easily create custom standard devices compliant with the [ECHONET Lite][enet] specification. This document explains how to create your own [ECHONET Lite][enet] device step by step.

## Creating Devices

### 1. Creating Node

All [ECHONET Lite][enet] devices belong to a node. To create your custom device, first use `uecho_node_new()` as follows:

```
#include <uecho/uecho.h>

uEchoNode *node = uecho_node_new();
```

The new node contains only a node profile class object and has no device objects. The node profile object is automatically updated when new devices are added to the node or when any properties in the node are changed.

### 2. Creating Device Object

To add device objects to the created node, create a new device object using `uecho_device_new()`. This function creates a new device object with the mandatory properties required by the ECHONET device object super class [\[1\]][enet-spec].

Next, according to the ECHONET standard device specification [\[1\]][enet-spec], set the manufacturer code using `uecho_object_setmanufacturercode()` and the object code using `uecho_object_setcode()`.
The created device contains only mandatory properties, so you should add other standard properties from the ECHONET standard device specification [\[1\]][enet-spec] or your custom properties using `uecho_object_setproperty()` and `uecho_object_setpropertydata()`.
Then, add the device object to the node using `uecho_node_addobject()` as follows:

```
uEchoObject *obj;
obj = uecho_device_new();

uecho_object_setmanufacturercode(obj, 0xXXXXXX);
uecho_object_setcode(obj, 0xXXXXXX);

uecho_object_setproperty(obj, 0xXX, uEchoPropertyAttrReadWrite);
uecho_object_setpropertydata(obj, 0xXX, ....., ....);

uecho_node_addobject(node, obj);
```

### 3. Handling Request Messages 

To implement the device, you have only to handle write requests from other nodes because The `uecho` handles other standard read and notification requests automatically. To grant the write requests and get the property data, use `uecho_object_setpropertywriterequesthandler()` as the following:

```
bool object_propertywriterequesthandler(uEchoObject* obj, uEchoProperty* prop, uEchoEsv esv, size_t pdc, byte *edt)
{
  if ((pdc != 1) || !edt) {
　  ....
    return false;
  }
  ....
  return true;
}
....
{
  uEchoObject *obj;
  byte propCode;
  ....
  uecho_object_setpropertywriterequesthandler(obj, propCode, object_propertywriterequesthandler)  
}
```

The `uecho` updates the target property by the request property data if the hander grants the request, and returns a response when the request message requires the response. Otherwise, the `uecho` returns an error response.

The `uecho_object_setpropertywriterequesthandler()` sets the handler for all write request types, Write (0x60) , Write Response Required (0x61) and Write & read Request (0x6E). To set handlers for each (ECHONET Lite Service) of [ECHONET Lite][enet], use `uecho_object_setpropertyrequeslistener()`, and The `uecho_object_setpropertywriterequesthandler()` is a sugar function of the `uecho_object_setpropertyrequeslistener()`.

### 4. Starting Node

Finally, start the node to use `uecho_node_start` as the following:

```
uEchoNode *node;
....
uecho_node_start(node);
```

## Next Steps

Let's check the following documentation to know the device functions of the `uecho` in more detail.

- [uEcho Examples](./examples.md)
- [Inside of uEcho Device](./device_inside.md)

## References

- \[1\] [Detailed Requirements for ECHONET Device objects][enet-spec]

[enet]:http://echonet.jp/english/
[enet-spec]:http://www.echonet.gr.jp/english/spec/index.htm
