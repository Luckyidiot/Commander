COMPILER = gcc

CLIENT_DIR = clientDir
SERVER_DIR = serverDir

CLIENT_MAIN = myClient.c
SERVER_MAIN = myServer.c

CLIENT_EXTERN = client_function.c 
SERVER_EXTERN = server_function.c server_execution.c

CLIENT_SRC = $(foreach var,$(CLIENT_EXTERN),$(CLIENT_DIR)/$(var)) $(CLIENT_MAIN)
SERVER_SRC = $(foreach var,$(SERVER_EXTERN),$(SERVER_DIR)/$(var)) $(SERVER_MAIN)

#CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)
#SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)

EXE = server client
all: $(EXE)

server: $(SERVER_SRC)
	$(COMPILER) -Wall $(SERVER_SRC) -o $@
client: $(CLIENT_SRC)
	$(COMPILER) -Wall $(CLIENT_SRC) -o $@
#%.o: %.c
#	$(COMPILER) -Wall -c $^ -o $@

clean:
	rm -f $(EXE)
