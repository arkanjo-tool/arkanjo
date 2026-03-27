#include "parser.hpp"

Comparation::Comparation(string _path1, string _path2, double _sim) {
    if (_path1 > _path2)
        swap(_path1, _path2);
    path1 = _path1;
    path2 = _path2;
    similarity = _sim;
}

bool Comparation::operator<(const Comparation& com) const {
    if (similarity != com.similarity) {
        return similarity > com.similarity;
    }
    if (path1 == com.path1) {
        return path2 < com.path2;
    }
    return path1 < com.path1;
}

bool Comparation::operator==(const Comparation& com) const {
    return path1 == com.path1 && path2 == com.path2;
}

vector<string> Parser::parser_line(const string& line) {
    string at = "";
    vector<string> ret;
    for (auto c : line) {
        if (c <= 20 || c == ' ') { // an space on non-printable char
            if (!at.empty())
                ret.push_back(at);
            at = "";
        } else {
            at += c;
        }
    }
    if (!at.empty())
        ret.push_back(at);

    return ret;
}

bool Parser::is_an_file(const std::string& s) {
    return !s.empty() && s[0] == '/';
}

void removeANSI_inplace(std::string& s) {
    size_t write = 0;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\033' && i + 1 < s.size() && s[i + 1] == '[') {
            i += 2;
            while (i < s.size() && (s[i] < '@' || s[i] > '~')) {
                i++;
            }
        } else {
            s[write++] = s[i];
        }
    }

    s.resize(write);
}

double Parser::retrive_similarity(const std::string& s) {
    try {
        return std::stod(s);
    } catch (...) {
        return 0.0;
    }
}

void Parser::parser_block_stream(const std::string& path, const std::vector<std::string>& tokens, set<Comparation>& comparations) {
    if (tokens.size() < 2)
        return;

    const std::string& path_comp = tokens[0];

    if (!is_an_file(path_comp))
        return;

    double similarity = retrive_similarity(tokens[1]);

    if (similarity < similarity_cap)
        return;
    
    Comparation com(path, path_comp, similarity);
    comparations.insert(com);
}

void Parser::exec_from_stream(FILE* pipe) {
    std::string line;
    char chunk[256];

    std::string path;

    while (fgets(chunk, sizeof(chunk), pipe)) {
        line += chunk;

        if (!line.empty() && line.back() == '\n') {
            removeANSI_inplace(line);

            auto tokens = parser_line(line);
            if (tokens.size() > 2) {
                for (auto token : tokens) {
                    if (is_an_file(token)) {
                        path = token;
                        break;
                    }
                }
                line.clear();
                continue;
            }

            parser_block_stream(path, tokens, comparations);

            line.clear();
        }
    }
    fout << comparations.size() << '\n';
    for (const auto& com : comparations) {
        fout << com.path1 << ' ' << com.path2 << ' ';
        fout << fixed << setprecision(2) << com.similarity << '\n';
    }
}

Parser::Parser(const fs::path& output_file, double similarity_cap)
    : fout(output_file), similarity_cap(similarity_cap) {

    if (!fout) {
        throw std::runtime_error("Failed to open output file");
    }
}

Parser::~Parser() {
    if (fout.is_open())
        fout.close();
}