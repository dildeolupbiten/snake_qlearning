CC = gcc

CFLAGS = -Iinclude -Wall -Wextra -pedantic

SRC_DIR = src
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(SRC:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

TARGET = snake_qlearning

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

