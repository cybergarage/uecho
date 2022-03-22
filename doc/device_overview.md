![logo](img/logo.png)

# Overview of The uEcho Device

## Making Devices

The `uecho` supports your original standard devices of [ECHONET Lite][enet] specification easily. This document explains to create your original  [ECHONET Lite][enet] device step by step.

## Creating Devices

### 1. Creating Node

To create your original device, use `uecho_node_new()` as the following at first.

```
#include <uecho/uecho.h>

uEchoNode *node = uecho_node_new();
```

The new node has only a node profile class object, and it has no device object. The node profile object is updated automatically when new devices are added into the node or the any properties in the node are changed.

### 2. Creating Device Object

The new node has no device object. To add your device objects, create a new device object using `uecho_device_new()`.  `uecho_device_new()` create a new device object which is added some mandatory properties of ECHONET device object super class requirements [\[1\]][enet-spec].

Next, according to ECHONET standard device specification [\[1\]][enet-spec], set the manufacturer code using `uecho_object_setmanufacturercode()` and the object code using `uecho_object_setcode()`.
The created device has only mandatory properties, and so you should add other stadard properties of ECHONET standard device specification [\[1\]][enet-spec] or your extra properties using `uecho_object_setproperty()` and `uecho_object_setpropertydata()`.
Then, add the device object into the node using `uecho_node_addobject()` as the following:

```
uEchoObject *obj;
obj = uecho_device_new();

uecho_object_setmanufacturercode(obj, 0xXXXXXX);
uecho_object_setcode(obj, 0xXXXXXX);

uecho_object_setproperty(obj, 0xXX, uEchoPropertyAttrReadWrite);
uecho_object_setpropertydata(obj, 0xXX, ....., ....);

uecho_node_addobject(node, obj);
```

### 3. Setting 

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
  byte prop_code;
  ....
  uecho_object_setpropertywriterequesthandler(obj, prop_code, object_propertywriterequesthandler)  
}
```

The `uecho` updates the target property by the request property data if the hander grants the request, and returns a response when the request message requires the response. Otherwise, the `uecho` returns an error response.

The `uecho_object_setpropertywriterequesthandler()` sets the handler for all write request types, Write (0x60) , Write Response Required (0x61) and Write & read Request (0x6E). To set handlers for each (ECHONET Lite Service) of [ECHONET Lite][enet], use `uecho_object_setpropertyrequeslistener()`, and The `uecho_object_setpropertywriterequesthandler()` is a sugar function of the `uecho_object_setpropertyrequeslistener()`.

### 4. Start Node

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
