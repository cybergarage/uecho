# Inside of uEcho Device

## Node and Object Observers

Basically uEcho handles all messages from other nodes automatically.
To implement the device object, use some observer functions such as `uecho_node_setmessagelistener`, `uecho_object_setmessagelistener` and `uecho_object_setpropertyrequeslistener`.

![Node Observers](img/node_msg_observer.png)

`uecho_node_setmessagelistener` can get all message for the node from other nodes, thus the message might be invalid.

uEcho verifies the messages form other nodes using the objects and properties information of the node, and returns an error response when the message is invalid automatically. `uecho_object_setmessagelistener` can get only valid messages for the object from other nodes.

`uecho_object_setpropertyrequeslistener` can get only valid request message for the object property from other nodes.
