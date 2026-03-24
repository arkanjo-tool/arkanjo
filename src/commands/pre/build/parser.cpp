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

vector<string> Parser::parser_line(string line) {
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

bool Parser::is_an_file(string s) {
    return !s.empty() && s[0] == '/';
}

string Parser::remove_formatation_from_similarity(string s) {
    for (int i = 0; i < 4; i++) {
        s.pop_back();
    }
    reverse(s.begin(), s.end());
    while (s.back() != 'm') {
        s.pop_back();
    }
    s.pop_back();
    reverse(s.begin(), s.end());
    return s;
}

double Parser::retrive_similarity(string s) {
    s = remove_formatation_from_similarity(s);
    // char* cs = s.data();
    float similarity = stod(s);
    return similarity;
}

void Parser::parser_block(string path, set<Comparation>& comparations) {
    string line;
    while (getline(fin, line)) {
        vector<string> tokens = parser_line(line);
        if (tokens.empty()) {
            break;
        }
        if (int32_t(tokens.size()) != 2 || !is_an_file(tokens[0])) {
            continue;
        }
        string path_compared = tokens[0];

        double similarity = retrive_similarity(tokens[1]);
        Comparation com(path, path_compared, similarity);
        if (similarity >= similarity_cap_)
            comparations.insert(com);
    }
}

void Parser::exec() {
    string line;
    set<Comparation> comparations;

    while (getline(fin, line)) {
        vector<string> tokens = parser_line(line);
        if (tokens.empty())
            continue;

        string path;
        for (auto token : tokens) {
            if (is_an_file(token)) {
                path = token;
                break;
            }
        }
        // Removing some formatation that shows on terminal
        for (int i = 0; i < 4; i++)
            path.pop_back();
        parser_block(path, comparations);
    }
    fout << comparations.size() << '\n';
    for (auto com : comparations) {
        fout << com.path1 << ' ' << com.path2 << ' ';
        fout << fixed << setprecision(2) << com.similarity << '\n';
    }
}

Parser::Parser(const fs::path& input_file, const fs::path& output_file, double similarity_cap) {
    fin = ifstream(input_file.string());
    fout = ofstream(output_file.string());
    similarity_cap_ = similarity_cap;

    exec();

    fin.close();
    fout.close();
}
