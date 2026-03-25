#include <arkanjo/utils/utils.hpp>

#include <arkanjo/base/config.hpp>

void Utils::ensure_file_is_open(const std::ifstream& file, const fs::path& file_name) {
    if (!file.is_open()) {
        std::cerr << "Attempted to open file: " << file_name << " ";
        std::cerr << "but a Error ocurred. Check if the file exist." << "\n";
        exit(0);
    }
}

std::vector<std::string> Utils::read_file_generic(const fs::path& string_path) {
    std::ifstream filein;
    std::string line;
    std::vector<std::string> ret;
    filein.open(string_path);
    ensure_file_is_open(filein, string_path);
    while (getline(filein, line)) {
        ret.push_back(line);
    }
    filein.close();
    return ret;
}

void Utils::write_file_generic(const fs::path& file_path, const std::vector<std::string>& content) {
    std::ofstream fileout;
    fs::create_directories(file_path.parent_path());
    fileout.open(file_path);

    for (const auto& line : content) {
        fileout << line << '\n';
    }
    fileout.close();
}

json Utils::read_json(const fs::path& string_path) {
    std::ifstream json_file(string_path.string(), std::ifstream::binary);
    ensure_file_is_open(json_file, string_path);
    json j_read;
    json_file >> j_read;
    return j_read;
}

std::string Utils::format_colored_message(const std::string& message, COLOR color) {
    if (Config::config().theme == config::Theme::Dark) {
        return COLOR_TOKENS_UTILS_DARK[color] + message + COLOR_TOKENS_UTILS_DARK[RESET];
    } else {
        return COLOR_TOKENS_UTILS_LIGTH[color] + message + COLOR_TOKENS_UTILS_LIGTH[RESET];
    }
}

bool Utils::is_empty_char(char c) {
    if (c == ' ') {
        return true;
    }
    if (c <= 20) {
        return true;
    }
    return false;
}

bool Utils::is_special_char(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return !(std::isalnum(uc) || c == '_');
}

std::vector<std::string> Utils::split_string(const std::string& s, char delimiter) {
    std::string cur_token;
    std::vector<std::string> ret;
    for (auto c : s) {
        if (c == delimiter) {
            if (!cur_token.empty()) {
                ret.push_back(cur_token);
            }
            cur_token = "";
        } else {
            cur_token.push_back(c);
        }
    }
    if (!cur_token.empty()) {
        ret.push_back(cur_token);
    }
    return ret;
}

Wrapped wrapped(const std::string& text, size_t spaces, bool use_first_line) {
    return {text, spaces, use_first_line};
}

std::ostream& operator<<(std::ostream& os, const Wrapped& w) {
    std::string indent(w.spaces, ' ');
    bool first_line = w.use_first_line;
    size_t max_line = 80 - w.spaces;
    std::string word;
    std::string current_line;
    std::istringstream stream(w.text);
    while (stream >> word) {
        if (current_line.empty()) {
            current_line = word;
        } else if (current_line.length() + 1 + word.length() > max_line) {
            if (first_line) {
                os << current_line << "\n";
                first_line = false;
            } else {
                os << indent << current_line << "\n";
            }
            current_line = word;
        } else {
            current_line += " " + word;
        }
    }
    if (!current_line.empty()) {
        if (first_line) {
            os << current_line << "\n";
        } else {
            os << indent << current_line << "\n";
        }
    }
    return os;
}

std::string Utils::to_uppercase(const std::string input) {
    std::string output = input;
    for (char &c : output) {
        c = std::toupper((unsigned char) c);
    }
    return output;
}

std::string Utils::hash(const std::string& content) {
    std::hash<std::string> hasher;
    size_t hash = hasher(content);

    std::stringstream ss;
    ss << std::hex << hash;

    std::string hash_str = ss.str();
    if (hash_str.length() > 12) {
        hash_str = hash_str.substr(0, 12);
    }

    return hash_str;
}

std::uintmax_t Utils::folder_size(const fs::path& folder) {
    std::uintmax_t size = 0;

    if (!fs::exists(folder)) return 0;

    std::error_code ec;
    for (auto& p : fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) continue;

        if (fs::is_regular_file(p.status(ec)) && !ec) {
            size += fs::file_size(p, ec);
        }
    }

    return size;
}

std::string Utils::format_size(std::uintmax_t bytes) {
    double size = static_cast<double>(bytes);
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;

    while (size >= 1024 && unit < 4) {
        size /= 1024;
        ++unit;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << size << units[unit];
    return out.str();
}