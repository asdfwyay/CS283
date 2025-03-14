1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

A remote client determines when an output is fully received from the server when calling `recv()` yields a specified character (in our case `EOF (0x04)`) that marks the end of a message. The client uses a while loop to continuously call `recv()` until this character is sent to ensure it receives the entire message from the server.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

In a networked shell protocol that communicates using a reliable stream protocol such as TCP, messages can be separated by delimiters. In our implementation, the null character `\0` is used to indicate the end of a message sent from the client to the server, while the EOF character `EOF (0x04)` is used to indicate the end of a message sent from the server to the client. The protocol would ensure that neither end writes to the socket file descriptor until it receives its respective end of message character. If not handled correctly, server-to-client and client-to-server messages would be conflated, resulting in undefined behavior.

3. Describe the general differences between stateful and stateless protocols.

When a client sends a message to a server in a **stateful protocol**, it expects to receive a response back from the server. Furthermore, stateful protocols require servers to keep track of client states, which determine the responses of the server. TCP is an example of a stateful protocol.

On the other hand, in a **stateless protocol**, there is no obligation for the server to keep track of informatinoa bout each individual client; rather, the response it gives depends on the state of the server itself. Furthermore, clients need not resend a request if it does not receive a response from the server. UDP is an example of a stateless protocol.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is unreliable because it does not check if packets are lost nor does it care, as subsequent packets would be sent without hesitation. However, as a stateless protocol, UDP has the benefit of being faster than stateful protocols such as TCP because there would be no delays introduced due to resending a lost packet.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Linux has implementation for several network protocols such as TCP and UDP built into its kernel. These can be accessed using the sockets API, which allows programs to perform network communication.