# Configuration

CC			= gcc
CFLAGS		= -g -std=gnu99 -Wall -Iinclude -fPIC -pthread 
TARGET		= client/mycal server/mycalserver

# Rules
make: $(TARGET)

$(TARGET): client/client.c server/server.c
	$(CC) $(CFLAGS) -o client/mycal cJSON.c client/client.c
	$(CC) $(CFLAGS) -o server/mycalserver cJSON.c server/server.c 

clean:
	@echo "Removing  executables"
	@rm -f client/mycal server/mycalserver