#include <arkanjo/cli/parser_options.hpp>

#include <iostream>

void parse_options(int argc, char* argv[], const std::string& short_opts, struct option* long_opts, ParsedOptions& ctx_options) {
    ctx_options.extra_args.clear();
    
    int option_index = 0;
    int opt;

    while ((opt = getopt_long(argc, argv, short_opts.c_str(), long_opts, &option_index)) != -1) {
        if (opt == '?') {
            continue;
        }

        std::string name;

        if (opt == 0) { // long option
            name = long_opts[option_index].name;
        } else { // short option
            name = std::string(1, static_cast<char>(opt));

            for (int i = 0; long_opts[i].name != nullptr; i++) {
                if (long_opts[i].val == opt) {
                    name = long_opts[i].name;
                    break;
                }
            }
        }

        ctx_options.args[name] = (optarg ? optarg : "true");
    }

    for (int i = optind; i < argc; i++)
        ctx_options.extra_args.push_back(argv[i]);

#ifdef DEBUG
    std::cout << "[DEBUG] Parsed options:\n";
    for (auto& kv : ctx_options.args) std::cout << kv.first << " = " << kv.second << "\n";
    for (auto& a : ctx_options.extra_args) std::cout << "[DEBUG] Extra arg: " << a << "\n";
#endif
}