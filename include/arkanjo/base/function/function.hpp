/**
 * @file function.hpp
 * @brief Function representation for a codebase
 *
 * Represents a function extracted from a codebase.
 * Stores the function location, source content, header content,
 * and metadata required to identify and manipulate the function.
 *
 * Loading data from external sources is handled by FunctionLoader.
 */

#pragma once

#include <limits>
#include <string>
#include <vector>

#include <arkanjo/base/path.hpp>

static constexpr uint32_t INVALID_LINE = std::numeric_limits<uint32_t>::max();

struct FunctionLocation {
    uint32_t declaration{INVALID_LINE}; ///< Line number of function declaration
    uint32_t begin{INVALID_LINE};       ///< Line number where function starts
    uint32_t end{INVALID_LINE};         ///< Line number where function ends

    [[nodiscard]]
    constexpr uint32_t size() const noexcept {
        if (declaration == INVALID_LINE || end == INVALID_LINE)
            return 0;

        if (end < declaration)
            return 0;

        return end - declaration + 1;
    }
};

/**
 * @brief Represents a function extracted from a codebase.
 *
 * Provides access to function source content, header content,
 * file location, and metadata.
 */
class Function {
    friend class FunctionLoader;

  private:
    Path path_;                        ///< Path object containing function location info
    std::string name_;                 ///< Function name used as identifier

    std::vector<std::string> content_; ///< Function source code content
    std::vector<std::string> header_;  ///< Function header content

    FunctionLocation location_;

  public:
    Function() = default;

    /**
     * @brief Constructs a Function object
     * @param _path Path object containing function location info
     */
    explicit Function(const Path& _path);

    /**
     * @brief Gets the function path information.
     * @return Path associated with this function.
     */
    const Path& path() const;

    /**
     * @brief Gets the function name
     * @return Function name identifier
     */
    const std::string& name() const;

    /**
     * @brief Gets the function location inside the source file.
     *
     * Contains declaration, begin, and end line numbers.
     */
    const FunctionLocation& scope_location() const;

    /**
     * @brief Gets the function's header content
     * @return Reference to the header lines.
     */
    const std::vector<std::string>& header() const;

    /**
     * @brief Builds combined content of header and source
     * @return Vector containing the merged header and source lines.
     */
    std::vector<std::string> build_all_content() const;

    /**
     * @brief Checks whether the function contains a pattern.
     *
     * @param Text pattern to search.
     * @return if the pattern is found.
     */
    bool contains_given_pattern(const std::string& pattern) const;
};
