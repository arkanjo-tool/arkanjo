CXX = g++
MODE ?= release

PROJECT := arkanjo
BIN_PATH := bin

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

TARGET_MAIN := $(PROJECT)
TARGET_PRE := $(PROJECT)-preprocessor
TARGET_TEST := $(PROJECT)-test

CXXFLAGS = -std=c++17 -Ithird-party -Iinclude
WARNINGS = -Wall -Wextra
LDFLAGS =

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -I/opt/homebrew/include
  LDFLAGS += -L/opt/homebrew/lib 
endif

ifeq ($(MODE),debug)
  CXXFLAGS += -g -O0 $(WARNINGS)
else ifeq ($(MODE),release)
  CXXFLAGS += -O2 -DNDEBUG $(WARNINGS)
endif

SRC_BASE = \
	src/utils/utils.cpp \
	src/utils/windows_utils.cpp \
	src/utils/linux_utils.cpp \
	src/utils/apple_utils.cpp \
	src/base/config.cpp \
	src/base/path.cpp \
	src/base/similarity_table.cpp \
	src/base/function.cpp

SRC_PRE = \
	src/commands/pre/function_breaker_util.cpp \
	src/commands/pre/function_breaker_c.cpp \
	src/commands/pre/function_breaker_java.cpp \
	src/commands/pre/function_breaker.cpp \
	src/commands/pre/parser.cpp \
	src/commands/pre/duplication_finder_tool.cpp \
	src/commands/pre/duplication_finder_diff.cpp \
	src/commands/pre/preprocessor.cpp

SRC_E2E = tests/e2e/test.cpp

SRC_EXTRA = \
	src/commands/finder/similar_function_finder.cpp \
	src/commands/counter/counter_duplication_code_trie.cpp \
	src/commands/counter/counter_duplication_code.cpp \
	src/commands/explorer/similarity_explorer.cpp \
	src/commands/big_clone/big_clone_formater.cpp \
	src/commands/big_clone/big_clone_tailor_evaluator.cpp \
	src/commands/rand/random_selector.cpp \
	src/orchestrator.cpp

.PHONY: all main preprocessor test clean install uninstall

all: main preprocessor test

main: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_EXTRA) -o $(BIN_PATH)/$(TARGET_MAIN) $(LDFLAGS)

release:
	$(MAKE) MODE=release

preprocessor: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) src/commands/pre/preprocessor_main.cpp -o $(BIN_PATH)/$(TARGET_PRE) $(LDFLAGS)

test: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_E2E) -o $(BIN_PATH)/$(TARGET_TEST) $(LDFLAGS)

clean:
	rm -f $(BIN_PATH)/$(TARGET_MAIN) \
		$(BIN_PATH)/$(TARGET_PRE) \
		$(BIN_PATH)/$(TARGET_TEST)

install: main preprocessor
	install -d $(BINDIR)
	install -m 755 $(BIN_PATH)/$(TARGET_MAIN) $(BINDIR)
	install -m 755 $(BIN_PATH)/$(TARGET_PRE) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET_MAIN)
	rm -f $(BINDIR)/$(TARGET_PRE)

$(BIN_PATH):
	mkdir -p $@