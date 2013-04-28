Mew Ophaswongse
April 28, 2013 - MiniRSA Chat Program

The server and the client have two RSA instances - one for its own keys (public and private) and one for the recipient's public key. After connecting, the program prompts the user to enter these keys, which are then stored in the RSA instances. When a message is sent, rsaSender is used to encrypt. When a message is received, rsaReceiver is used to decrypt. This ensures that the message will only be read by the intended recipient.
Throughout the program, the user can enter escape keywords: '.keys' to change the public keys of the recipient,
'.crack', to guess the private key from the a given public key, and '.quit' to exit the program.

When I was creating my server and client, I ran into a problem that only one message can be sent/received at a time and one has to wait for the other's response (which is probably not what we want for a "chat program").
I found a solution on http://www.thiyagaraaj.com/tutorials/unix-network-programming-example-programs/tcp-chat-programming-in-unix-using-c-programmin, which uses fd_set to do synchronous I/O. Each program then monitors two file descriptors, 0 and the connecting fd. When the fd is set to 0, this means the program has to ask user for input. Otherwise, it has to read message from the other side.

To generate keys, use the 'keys' program.

I used Git for version control. For test run, please see attached screenshots.