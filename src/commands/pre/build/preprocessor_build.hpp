/**
 * @file preprocessor_build.hpp
 * @brief Codebase preprocessing interface
 *
 * Defines the preprocessing stage that prepares the codebase for
 * efficient duplication detection by parsing, analyzing, and organizing
 * source code into a queryable format.
 *
 * Defines the preprocessor/setup step of the tool, where we do a heavy
 * preprocessing of the input codebase to enable fast query response later.
 */

#pragma once

#include "duplication_finder_diff.hpp"
#include "duplication_finder_tool.hpp"
#include "function_breaker.hpp"
#include "parser.hpp"
#include <arkanjo/commands/pre/preprocessor.hpp>
#include <arkanjo/commands/command_base.hpp>
#include <string>
#include <tuple>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Codebase preprocessing orchestrator
 *
 * Manages the complete preprocessing pipeline including:
 * - Code structure analysis
 * - Function extraction
 * - Duplication detection
 * - Results organization
 *
 * Creates the foundation for fast query responses during the main operation phase.
 */
class PreprocessorBuild : public Preprocessor, public CommandBase<PreprocessorBuild> {
  private:
    static constexpr const char* TEMPLATE_COUNT =
      "The total number of functions that are "
      "similar to the found one is {count}. "
      "More info about them are listed below.";
    // User interaction messages
    static constexpr const char* PROJECT_PATH_MESSAGE = "Enter your project path:";                                  ///< Project path prompt
    static constexpr const char* MINIMUM_SIMILARITY_MESSAGE = "Enter minimum similarity desired on using the tool:"; ///< Similarity threshold prompt

    // Processing stage messages
    static constexpr const char* INITIAL_MESSAGE = "Initiating Preprocessing";                                           ///< Initialization message
    static constexpr const char* BREAKER_MESSAGE = "Reading codebase... (this may take a while)";                        ///< Code analysis message
    static constexpr const char* DUPLICATION_MESSAGE = "Finding duplication in the codebase... (this may take a while)"; ///< Duplication detection message
    static constexpr const char* SAVING_MESSAGE = "Saving results...";                                                   ///< Results saving message
    static constexpr const char* END_MESSAGE = "Finished preprocessing";                                                 ///< Completion message

    // Duplication finder selection messages
    static constexpr const char* MESSAGE_DUPLICATION_FINDER_TYPE_1 = "Enter the number of the duplication finder technique you want to use:";
    static constexpr const char* MESSAGE_DUPLICATION_FINDER_TYPE_2 = "1) NLP text similarity using gensim";
    static constexpr const char* MESSAGE_DUPLICATION_FINDER_TYPE_3 = "2) Count proportion of equal lines using diff command";
    static constexpr const char* INVALID_CODE_DUPLICATION_FINDER = "Valid options are '1' or '2' only. Stopping Program...";

    /**
     * @brief Reads preprocessing parameters from user/config
     * @return tuple<string,double,bool>
     *         - Project path
     *         - Similarity threshold
     *         - Duplication finder selection flag
     */
    std::tuple<std::string, double, bool> read_parameters();

    /**
     * @brief Executes full preprocessing pipeline
     * @param path Project path to process
     * @param similarity Similarity threshold
     * @param use_duplication_finder_by_tool Flag to select duplication detection method
     */
    void preprocess(const fs::path& path, double similarity, bool use_duplication_finder_by_tool);

  public:
    PreprocessorBuild();

    /**
     * @brief Constructs preprocessor build with optional forcing
     * @param force_preprocess Whether to force preprocessing even if cached results exist
     */
    PreprocessorBuild(bool force_preprocess);

    /**
     * @brief Testing constructor with direct parameter specification
     * @param force_preprocess Whether to force preprocessing
     * @param path Direct project path specification
     * @param similarity Direct similarity threshold specification
     * @note Exposed for testing purposes only
     */
    PreprocessorBuild(bool force_preprocess, const fs::path& path, double similarity);

    COMMAND_DESCRIPTION("Creates the foundation for fast query responses during the main operation phase.")

    bool validate(const ParsedOptions& options) override;

    bool run(const ParsedOptions& options) override;
};
