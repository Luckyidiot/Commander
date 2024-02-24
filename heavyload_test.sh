#!/bin/bash

# Put a burden on the server to test :: Continuously make connection
echo "Heavy load...."
for((;;)); do
    ./client 127.0.0.1 hello
done