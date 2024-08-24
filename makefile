CC = gcc
CFLAGS = -Wall -Iinclude -I/usr/include/libmongoc-1.0 -I/usr/include/postgresql -I/usr/include/libbson-1.0
LDFLAGS = -lpq -lmongoc-1.0 -lbson-1.0 -lrabbitmq
SRC_DIR = src
OBJ_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = bin/federated_database_system

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ) $(EXEC)
