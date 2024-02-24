PRINCIPLE

Step 1: Client sends a binary file to Server via socket and port
Step 2: Server accept the connection and create a new socket using accept()
Step 3: The newly created socket will communicate with a specific client for receiving the binary file.


Each new request comes will trigger the select() which immediately return and start executing the rest.




client_function.c
    . Implement the binary file sending mechanics

server_function.c
    . Doubting if the Receive_fileExe() works as expected because it may overwirte the current data
      but not append to current.
