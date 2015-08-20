# uEcho for C

## Making Devices

uEcho supports your original standard devices of ECHONET Lite specification easily.

## Creating Devices

To create your original device, use `uecho_node_new` as the following at first.

```
uEchoNode *node = uecho_node_new();
```

The new node has only a node profile class object, and it has no device object. The node profile object is updated automatically when new devices are added into the node or the any properties in the node are changed.
