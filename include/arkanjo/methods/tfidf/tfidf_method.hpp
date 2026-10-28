/**
 * @file tfidf_method.hpp
 * @brief Main duplication detection tool interface
 *
 * Provides the primary interface for the code duplication detection tool,
 * handling the heavy preprocessing of the input codebase to enable
 * fast query responses during operation.
 *
 * Defines the preprocessor/setup step of the tool,
 * where we do a heavy preprocessing of the input codebase
 * to enable fast query response later.
 */

#pragma once

#include <arkanjo/base/similarity.hpp>
#include <arkanjo/methods/method.hpp>
#include <arkanjo/methods/tfidf/parser.hpp>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

/**
 * @brief Main code duplication detection tool
 *
 * Orchestrates the preprocessing pipeline that analyzes the entire codebase
 * to identify and catalog potential code duplications, creating the foundation
 * for efficient similarity queries during the tool's operation phase.
 */
class TfidfMethod : public IMethod {
  private:
    static constexpr const char* SAVING_MESSAGE = "Saving results..."; ///< Status message displayed when saving analysis results

    fs::path base_path;  ///< Root directory path of the codebase to analyze
    Similarity similarity; ///< Similarity threshold for considering code segments duplicates

  public:
    /**
     * @brief Constructs the duplication finder tool
     * @param base_path_ Root path of the codebase to analyze
     * @param similarity_ Minimum similarity threshold to consider as duplicate
     */
    TfidfMethod(const fs::path& base_path_, Similarity similarity_);

    /**
     * @brief Executes the full duplication analysis pipeline
     *
     * Performs the complete preprocessing and analysis of the codebase,
     * including file scanning, similarity comparison, and result saving.
     */
    void execute_by_feature(const fs::path& path, const std::string feature_name = "");

    void on_function(const FunctionData& fd) override;

    void execute() override;
};
