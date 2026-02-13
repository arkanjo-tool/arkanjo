CXX = g++
MODE ?= debug

CXXFLAGS = -std=c++17 -Ithird-party -Iinclude

WARNINGS = -Wall -Wextra

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -I/opt/homebrew/include
  LDFLAGS = -L/opt/homebrew/lib 
endif

ifeq ($(MODE),debug)
  CXXFLAGS += -g -O0 $(WARNINGS)
else ifeq ($(MODE),release)
  CXXFLAGS += -O2 -DNDEBUG $(WARNINGS)
endif

BIN_PATH := bin

SRC_BASE = base/utils.cpp base/windows_utils.cpp base/linux_utils.cpp base/apple_utils.cpp base/config.cpp base/path.cpp base/similarity_table.cpp base/function.cpp
SRC_PRE = pre/function_breaker_util.cpp pre/function_breaker_c.cpp pre/function_breaker_java.cpp pre/function_breaker.cpp pre/parser.cpp pre/duplication_finder_tool.cpp pre/duplication_finder_diff.cpp pre/preprocessor.cpp
SRC_E2E = tests/e2e/test.cpp
SRC_EXTRA = finder/similar_function_finder.cpp counter/counter_duplication_code_trie.cpp counter/counter_duplication_code.cpp explorer/similarity_explorer.cpp big_clone/big_clone_formater.cpp big_clone/big_clone_tailor_evaluator.cpp rand/random_selector.cpp orchestrator.cpp

.PHONY: all exec preprocessor test clean

all: exec preprocessor test

exec: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_EXTRA) -o $(BIN_PATH)/exec $(LDFLAGS)

preprocessor: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) pre/preprocessor_main.cpp -o $(BIN_PATH)/preprocessor $(LDFLAGS)

test: $(BIN_PATH)
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_E2E) -o $(BIN_PATH)/test $(LDFLAGS)

clean:
	rm -f $(BIN_PATH)/exec $(BIN_PATH)/preprocessor $(BIN_PATH)/test

$(BIN_PATH):
	mkdir -p $@