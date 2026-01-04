CXX = g++
CXXFLAGS = -std=c++17 -g

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -I/opt/homebrew/include
  LDFLAGS = -L/opt/homebrew/lib 
else
  LDFLAGS = 
endif

CXXFLAGS += -Ithird-party 

SRC_BASE = base/utils.cpp base/windows_utils.cpp base/linux_utils.cpp base/apple_utils.cpp base/config.cpp base/path.cpp base/similarity_table.cpp base/function.cpp
SRC_PRE = pre/function_breaker_util.cpp pre/function_breaker_c.cpp pre/function_breaker_java.cpp pre/function_breaker.cpp pre/parser.cpp pre/duplication_finder_tool.cpp pre/duplication_finder_diff.cpp pre/preprocessor.cpp
SRC_E2E = tests/e2e/test.cpp
SRC_EXTRA = finder/similar_function_finder.cpp counter/counter_duplication_code_trie.cpp counter/counter_duplication_code.cpp explorer/similarity_explorer.cpp big_clone/big_clone_formater.cpp big_clone/big_clone_tailor_evaluator.cpp rand/random_selector.cpp orchestrator.cpp

all: exec preprocessor test

exec:
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_EXTRA) -o exec $(LDFLAGS)

preprocessor:
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) pre/preprocessor_main.cpp -o preprocessor $(LDFLAGS)

test:
	$(CXX) $(CXXFLAGS) $(SRC_BASE) $(SRC_PRE) $(SRC_E2E) -o test $(LDFLAGS)

clean:
	rm -f exec preprocessor test
