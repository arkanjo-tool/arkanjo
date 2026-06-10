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

#include <arkanjo/methods/diff/diff_method.hpp>
#include <arkanjo/methods/tool/tool_method.hpp>
#include <arkanjo/methods/ast/ast_method.hpp>
#include <arkanjo/methods/llm/llm_method.hpp>

#include "function_breaker.hpp"
#include <arkanjo/commands/pre/preprocessor.hpp>
#include <arkanjo/commands/command_base.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <filesystem>

namespace fs = std::filesystem;

using MethodFactory = std::function<std::unique_ptr<IMethod>(
  const std::string&, float, std::optional<int>, std::optional<int>,
  std::optional<std::string>
)>;

struct MethodInfo {
  MethodFactory create;
  std::string_view description;
};

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
    static constexpr const char* ERROR_PATH_MESSAGE = "Provided path does not exist.";                 ///< Error message for invalid paths

    // Duplication finder selection messages
    static constexpr const char* MESSAGE_DUPLICATION_FINDER_TYPE = "Enter the number of the duplication finder technique you want to use:";
    static constexpr const char* INVALID_CODE_DUPLICATION_FINDER = "Invalid option entered. Stopping Program...";

    bool mode_verbose = false; ///< Detailed execution information
    int minimum_lines = 0;     ///< Minimum clone size in original lines

    const std::vector<MethodInfo> MethodsType = {
      {
        [](const std::string& base_path, float similarity,
           std::optional<int> /*max_seq_length*/, std::optional<int> /*batch_size*/,
           std::optional<std::string> /*model*/) {
          return std::make_unique<ToolMethod>(base_path, similarity);
        },
        "NLP text similarity using gensim"
      },
      {
        [](const std::string& base_path, float similarity,
           std::optional<int> /*max_seq_length*/, std::optional<int> /*batch_size*/,
           std::optional<std::string> /*model*/) {
          return std::make_unique<DiffMethod>(base_path, similarity);
        },
        "Count proportion of equal lines using diff command"
      },
      {
        [](const std::string& base_path, float similarity,
           std::optional<int> /*max_seq_length*/, std::optional<int> /*batch_size*/,
           std::optional<std::string> /*model*/) {
          return std::make_unique<ASTMethod>(base_path, similarity);
        },
        "Compare linearized structural sequences extracted from Tree-sitter ASTs"
      },
      {
        [](const std::string& base_path, float similarity,
           std::optional<int> max_seq_length, std::optional<int> batch_size,
           std::optional<std::string> model) {
          return std::make_unique<LLMMethod>(base_path, similarity, max_seq_length, batch_size, model);
        },
        "Embedding-based similarity using a code language model"
      }
    };

    /**
     * @brief Reads preprocessing parameters from user/config
     * @param options Parsed command line options. std::nullopt represents default behavior.
     * @return tuple
     *         - Project path
     *         - Similarity threshold
     *         - Duplication finder selection index
     *         - Optional LLM max sequence length override
     *         - Optional LLM batch size override
     *         - Optional LLM model name override
     */
    std::tuple<std::string, double, size_t, std::optional<int>, std::optional<int>,
               std::optional<std::string>, Granularity>
    read_parameters(const std::optional<ParsedOptions>& options);

    /**
     * @brief Executes full preprocessing pipeline
     * @param path Project path to process
     * @param similarity Similarity threshold
     * @param use_duplication_finder_index Flag to select duplication detection method
     * @param llm_max_seq_length Optional LLM max sequence length override (ignored by non-LLM methods)
     * @param llm_batch_size Optional LLM batch size override (ignored by non-LLM methods)
     * @param llm_model Optional LLM model name override (ignored by non-LLM methods)
     */
    void preprocess(const fs::path& path, double similarity, size_t use_duplication_finder_index,
                    std::optional<int> llm_max_seq_length = std::nullopt,
                    std::optional<int> llm_batch_size = std::nullopt,
                    std::optional<std::string> llm_model = std::nullopt,
                    Granularity granularity = Granularity::Function);

  public:
    static constexpr CliOption options_[] = {
      {"verbose", 0, NoArgument, "Enable verbose output"},
      {"path", 0, PositionalArgument, "Project path to preprocess."},
      {"minimum-lines", 0, RequiredArgument, "Minimum clone size in original lines."},
      {"llm-max-seq-length", 0, RequiredArgument,
        "Override the LLM detector's max token sequence length per function "
        "(longer bodies are truncated). Only used by the LLM duplication "
        "finder; ignored by other methods."},
      {"llm-batch-size", 0, RequiredArgument,
        "Override the LLM detector's embedding batch size. Lower this on "
        "machines with less RAM. Only used by the LLM duplication finder; "
        "ignored by other methods."},
      {"llm-model", 0, RequiredArgument,
        "Override the LLM detector's embedding model (Hugging Face "
        "sentence-transformers id). Only used by the LLM duplication finder; "
        "ignored by other methods."},
      {"granularity", 0, RequiredArgument,
        "Comparison granularity: 'function' (default) compares individual "
        "functions; 'file' keeps each file whole and compares files against "
        "each other. Applies to every duplication finder method."},
      OPTION_END
    };
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
