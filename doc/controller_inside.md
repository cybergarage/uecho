![logo](img/logo.png)

# Inside of uEcho Controller

## Node Profile Object

The controller is a special node which is specified in [ECHONETLite][enet] to control other nodes, and the controller must implement the following special node profile class object which has all children objects of the node [\[1\]][enet-spec].

- Class group code: 0x0E
- Class code: 0xF0
- Instance code: 0x01 (general node)

The `uecho` adds the node profile class objects automatically when the controller is created, and so the developer doesn't need to add the objects yourself.

## Controller Message Listeners

The `uecho` handles all messages from other nodes automatically. However, the developer can set the listener using `uecho_controller_setmessagelistener()` to listen to the internal messages from other nodes. 

![Controller Observers](img/controller_msg_listener.png)

After a node is received a message from other nodes, the node's listeners are called as the following sequences:

## References

- \[1\] [Part II ECHONET Lite Communication Middleware Specification][enet-spec]

[enet]:http://echonet.jp/english/
[enet-spec]:http://www.echonet.gr.jp/english/spec/index.htm
