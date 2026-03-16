#include <arkanjo/cli/parser_options.hpp>

#include <iostream>

option* build_longopts(const std::vector<CliOption>& options) {
    size_t n = options.size();
    option* opts = new option[n + 1];

    for (size_t i = 0; i < n; ++i) {
        if (options[i].has_arg == PositionalArgument) continue;
        
        opts[i].name = options[i].long_name;
        opts[i].has_arg = options[i].has_arg;
        opts[i].flag = nullptr;
        opts[i].val = options[i].short_name;
    }

    opts[n] = {0,0,0,0};

    return opts;
}

std::string build_shortopts(const option* long_opts) {
    std::string shortopts;

    for (; long_opts->name != nullptr; ++long_opts) {
        if (long_opts->flag == nullptr && long_opts->val < 256 && long_opts->val > 0) {
            shortopts += static_cast<char>(long_opts->val);

            if (long_opts->has_arg == required_argument)
                shortopts += ":";
            else if (long_opts->has_arg == optional_argument)
                shortopts += "::";
        }
    }

    return shortopts;
}

bool parse_options(int argc, char* argv[], const std::vector<CliOption>& options, ParsedOptions& ctx_options) {
    ctx_options.extra_args.clear();

    int option_index = 0;
    int opt;

    option* long_opts = build_longopts(options);
    std::string shortopts = build_shortopts(long_opts);

    while ((opt = getopt_long(argc, argv, shortopts.c_str(), long_opts, &option_index)) != -1) {
        if (opt == '?') {
            std::string cerr = "invalid option: ";
            if (optopt) {
                cerr += "-";
                cerr += char(optopt);
            } else {
                cerr += argv[optind - 1];
            }
            cerr += "\nTry '--help' for usage.";
            throw CLIError(cerr);
            return false;
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
    for (auto& kv : ctx_options.args)
        std::cout << kv.first << " = " << kv.second << "\n";
    for (auto& a : ctx_options.extra_args)
        std::cout << "[DEBUG] Extra arg: " << a << "\n";
#endif

    return true;
}