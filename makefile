CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11

INCLUDE = -I./include

PYTHON := python3
SCRIPT := test/test_valgrind.py

SRC_DIR = src
TEST_DIR = test
BIN_DIR = bin

SRC_BLOCK = $(SRC_DIR)/block.c
SRC_POINTER = $(SRC_DIR)/pointer.c
SRC_MAPPINGS = $(SRC_DIR)/mappings.c
SRC_CONTIGUOUS = $(SRC_DIR)/contiguous_pointer.c

TEST_BLOCK = $(TEST_DIR)/test_block.c
TEST_POINTER = $(TEST_DIR)/test_pointer.c
TEST_MAPPINGS = $(TEST_DIR)/test_mappings.c
TEST_CONTIGUOUS = $(TEST_DIR)/test_contiguous.c

BIN_BLOCK = $(BIN_DIR)/test_block
BIN_POINTER = $(BIN_DIR)/test_pointer
BIN_MAPPINGS = $(BIN_DIR)/test_map
BIN_CONTIGUOUS = $(BIN_DIR)/test_cont

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

all: $(BIN_BLOCK) $(BIN_POINTER) $(BIN_MAPPINGS) $(BIN_CONTIGUOUS)

$(BIN_BLOCK): $(SRC_BLOCK) $(TEST_BLOCK)
	$(CC) $(CFLAGS) $(TEST_BLOCK) -o $(BIN_BLOCK)

$(BIN_POINTER): $(SRC_BLOCK) $(SRC_POINTER) $(TEST_POINTER)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC_BLOCK) $(TEST_POINTER) -o $(BIN_POINTER)

$(BIN_MAPPINGS): $(SRC_MAPPINGS) $(TEST_MAPPINGS)
	$(CC) $(CFLAGS) $(TEST_MAPPINGS) -o $(BIN_MAPPINGS)

$(BIN_CONTIGUOUS): $(SRC_BLOCK) $(SRC_POINTER) $(SRC_MAPPINGS) $(SRC_CONTIGUOUS) $(TEST_CONTIGUOUS)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC_BLOCK) $(SRC_POINTER) $(SRC_MAPPINGS) $(TEST_CONTIGUOUS) -o $(BIN_CONTIGUOUS)

run_block: $(BIN_BLOCK)
	@$(VALGRIND) ./$(BIN_BLOCK) 2> $(BIN_DIR)/valgrind_block.txt
	@$(PYTHON) $(SCRIPT) bin/valgrind_block.txt 1

run_pointer: $(BIN_POINTER)
	@$(VALGRIND) ./$(BIN_POINTER) 2> $(BIN_DIR)/valgrind_pointer.txt
	@$(PYTHON) $(SCRIPT) bin/valgrind_pointer.txt 3

run_mappings: $(BIN_MAPPINGS)
	@$(VALGRIND) ./$(BIN_MAPPINGS) 2> $(BIN_DIR)/valgrind_map.txt
	@$(PYTHON) $(SCRIPT) bin/valgrind_map.txt 1

run_contiguous: $(BIN_CONTIGUOUS)
	@$(VALGRIND) ./$(BIN_CONTIGUOUS) 2> $(BIN_DIR)/valgrind_cont.txt
	@$(PYTHON) $(SCRIPT) bin/valgrind_cont.txt 3

clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean run_block run_pointer run_mappings run_contiguous
