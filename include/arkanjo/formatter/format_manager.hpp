#pragma once

#include <arkanjo/formatter/template_renderer.hpp>
#include <arkanjo/formatter/base.hpp>

#define DEFINE_COLOR_HELPER(name)      \
inline std::string name(const std::string& text) { \
    auto fmt = FormatterManager::get_formatter();  \
    return fmt->colorize(text, Utils::name); \
}

// Manager global
class FormatterManager {
public:
    static void set_formatter(std::shared_ptr<IFormatter> f) {
        formatter() = std::move(f);
    }

    static std::shared_ptr<IFormatter> get_formatter() {
        auto f = formatter();
        if (!f) {
            static auto fallback =
                std::make_shared<ConsoleFormatter>();
            return fallback;
        }
        return f;
    }

    static void set_format(Format f) {
        current_format() = f;
    }

    static Format get_format() {
        return current_format();
    }

    template <typename T>
    static void write(
        const std::string& template_str,
        const std::vector<T>& data,
        enum Format effective = Format::AUTO,
        RowColorFn color_fn = nullptr,
        std::ostream& out = std::cout
    ) {
        if (effective == Format::AUTO) {
            effective = current_format();
        }

        if (effective != current_format()) {
            return;
        }

        json arr = json::array();
        if (effective == Format::JSON) {
            for (const auto& x : data) arr.push_back(x);
            out << arr.dump(2) << "\n";
            return;
        }

        size_t i = 0;
        auto formatter = get_formatter();
        for (const auto& item : data) {
            std::string line = TemplateRenderer::render(template_str, item, formatter);

            if (color_fn && formatter) {
                line = formatter->colorize(line, color_fn(i));
            }

            out << line << "\n";
            ++i;
        }
        if (data.size() <= 0) {
            out << template_str << "\n";
        }
    }

    template <typename T>
    static void write(
        const std::string& template_str,
        const T& item,
        Format effective = Format::AUTO,
        RowColorFn color_fn = nullptr,
        std::ostream& out = std::cout
    ) {
        write<T>(template_str, std::vector<T>{item}, effective, color_fn, out);
    }

    static void write(const std::string& str, std::ostream& out = std::cout) {
        write(str, std::vector<char>{}, Format::TEXT, nullptr, out);
    }

    static void warn(const std::string& str) {
        auto fmt = get_formatter();

        Utils::COLOR bold = fmt->style().at("bold");
        Utils::COLOR warn_color = fmt->style().at("warning");

        std::string warning = fmt->colorize("Warning: ", bold);
        warning = fmt->colorize(warning, warn_color);

        write(warning + str, std::cerr);
    }

private:
    static std::shared_ptr<IFormatter>& formatter() {
        static std::shared_ptr<IFormatter> f =
            std::make_shared<ConsoleFormatter>();
        return f;
    }

    static Format& current_format() {
        static Format fmt = Format::TEXT;
        return fmt;
    }
};

DEFINE_COLOR_HELPER(RED)
DEFINE_COLOR_HELPER(GREEN)
DEFINE_COLOR_HELPER(YELLOW)
DEFINE_COLOR_HELPER(BLUE)
DEFINE_COLOR_HELPER(CYAN)
DEFINE_COLOR_HELPER(MAGENTA)
DEFINE_COLOR_HELPER(GRAY)
DEFINE_COLOR_HELPER(WHITE)
DEFINE_COLOR_HELPER(BOLD)
DEFINE_COLOR_HELPER(UNDERLINE)
