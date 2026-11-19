#include "cli/formatter/theme.hpp"

#include <arkanjo/base/config/config.hpp>
#include <string>

namespace formatter {

std::string format_colored_message(const std::string& message, Color color) {
    if (color == Color::NONE)
        return message;

    const auto& colorTokens = Config::config().theme == config::Theme::Dark
        ? kColorTokensUtilsDark
        : kColorTokensUtilsLight;

    const auto index = static_cast<std::size_t>(color);

    std::string result;
    result.reserve(
        colorTokens[index].size() + message.size() +
        colorTokens[static_cast<std::size_t>(Color::RESET)].size()
    );

    result.append(colorTokens[index]);
    result.append(message);
    result.append(colorTokens[static_cast<std::size_t>(Color::RESET)]);

    return result;
}

} // namespace formatter
