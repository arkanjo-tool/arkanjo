make: 
	g++ base/utils.cpp base/config.cpp base/path.cpp base/similarity_table.cpp pre/function_breaker_util.cpp pre/function_breaker_c.cpp pre/function_breaker_java.cpp pre/function_breaker.cpp pre/parser.cpp pre/duplication_finder_tool.cpp pre/duplication_finder_diff.cpp pre/preprocessor.cpp finder/similar_function_finder.cpp counter/counter_duplication_code_trie.cpp counter/counter_duplication_code.cpp explorer/similarity_explorer.cpp big_clone/big_clone_formater.cpp base/function.cpp big_clone/big_clone_tailor_evaluator.cpp rand/random_selector.cpp orchestrator.cpp -o exec -ljsoncpp -g

preprocessor:
	g++ base/utils.cpp base/config.cpp pre/function_breaker_util.cpp pre/function_breaker_c.cpp pre/function_breaker_java.cpp pre/function_breaker.cpp pre/parser.cpp pre/duplication_finder_tool.cpp pre/duplication_finder_diff.cpp pre/preprocessor.cpp pre/preprocessor_main.cpp -o preprocessor -g -ljsoncpp

test:
	g++ base/utils.cpp base/config.cpp pre/function_breaker_util.cpp pre/function_breaker_c.cpp pre/function_breaker_java.cpp pre/function_breaker.cpp pre/parser.cpp pre/duplication_finder_tool.cpp pre/duplication_finder_diff.cpp pre/preprocessor.cpp tests/e2e/test.cpp -o test -g -ljsoncpp
