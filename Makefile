CC = gcc
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS =

SRC_DIR = src/src
INCLUDE_DIR = src/include
BIN_DIR = bin

MAIN_SRC = src/main.c
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
MAIN_OBJ = $(BIN_DIR)/main.o
TARGET = $(BIN_DIR)/michiato

TEST_SRC = src/tests/test_michiato.c
TEST_OBJS = $(BIN_DIR)/test_michiato.o
TEST_TARGET = $(BIN_DIR)/test_michiato

.PHONY: all clean dirs run test

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

test: dirs $(TEST_TARGET)
	./$(TEST_TARGET)

$(BIN_DIR)/test_michiato.o: $(TEST_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
