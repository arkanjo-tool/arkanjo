#pragma once

#include <arkanjo/utils/utils.hpp>
#include <arkanjo/formatter/base.hpp>

struct Placeholder {
    std::string key;
    Utils::COLOR style = Utils::COLOR::NONE;
};

class TemplateRenderer {
private:
    static Utils::COLOR resolve_style(const std::string& name, const StyleMap& theme) {
        auto it = theme.find(name);
        return (it != theme.end()) ? it->second : Utils::COLOR::NONE;
    }

    static Placeholder parse_placeholder(const std::string& raw, const StyleMap& theme) {
        Placeholder p;

        auto pos = raw.find(':');

        if (pos == std::string::npos) {
            p.key = raw;
        } else {
            p.key = raw.substr(0, pos);
            std::string style_name = raw.substr(pos + 1);

            p.style = resolve_style(style_name, theme);
        }

        return p;
    }

public:
    static std::string render(const std::string& tpl, const json& ctx, std::shared_ptr<IFormatter> formatter) {
        std::string result;
        result.reserve(tpl.size());

        size_t pos = 0;
        while (pos < tpl.size()) {
            size_t start = tpl.find('{', pos);

            if (start == std::string::npos) {
                result.append(tpl, pos, tpl.size() - pos);
                break;
            }

            result.append(tpl, pos, start - pos);

            size_t end = tpl.find('}', start);
            if (end == std::string::npos) {
                result.append(tpl, start, tpl.size() - start);
                break;
            }

            std::string raw = tpl.substr(start + 1, end - start - 1);
            auto ph = parse_placeholder(raw, formatter->style());

            std::string value;

            if (ctx.contains(ph.key)) {
                const auto& v = ctx[ph.key];
                value = v.is_string() ? v.get<std::string>() : v.dump();
            } else {
                value = "{" + ph.key + "}";
            }

            result.append(formatter->colorize(value, ph.style));

            pos = end + 1;
        }

        return result;
    }
};
