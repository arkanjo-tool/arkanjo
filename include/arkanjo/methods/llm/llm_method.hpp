/**
 * @file llm_method.hpp
 * @brief LLM-embedding-based code duplication detection
 *
 * Implements a duplication detector that computes semantic similarity between
 * function bodies using Hugging Face embeddings (model:
 * jinaai/jina-embeddings-v2-base-code), delegating the embedding and pairwise
 * comparison to a Python helper script located in the third-party folder.
 *
 * Defines the preprocessor/setup step of the tool, where we do a heavy
 * preprocessing of the input codebase to enable fast query response later.
 */

#pragma once

#include <cstdio>
#include <optional>
#include <string>
#include <tuple>
#include <vector>
#include <filesystem>
#include <arkanjo/methods/method.hpp>

namespace fs = std::filesystem;

/**
 * @brief Semantic duplication detector using LLM embeddings.
 *
 * Materializes each function body as a file under the source feature directory
 * (same as the other methods), then invokes a Python script that embeds every
 * function with a Hugging Face model and prints the pairwise similarities, which
 * are parsed back into duplication entries.
 */
class LLMMethod : public IMethod {
  private:
    static constexpr const char* SAVING_MESSAGE = "Saving results..."; ///< Status message for saving output

    fs::path base_path;  ///< Root path of the codebase cache to analyze
    double similarity; ///< Similarity threshold for considering duplicates (0-100)
    std::optional<int> max_seq_length; ///< Override for the Python script's --max-seq-length (nullopt = use script default)
    std::optional<int> batch_size;     ///< Override for the Python script's --batch-size (nullopt = use script default)
    std::optional<std::string> model;  ///< Override for the Python script's --model (nullopt = use script default)

    /**
     * @brief Builds the command that runs the Python embedding script.
     * @param source_dir Directory containing the materialized function bodies.
     * @return The full shell command string passed to popen.
     */
    std::string build_command(const fs::path& source_dir) const;

    /**
     * @brief Parses a single "path1<TAB>path2<TAB>similarity" output line.
     * @param line Raw line read from the script's stdout.
     * @param entry Output parameter receiving the parsed entry.
     * @return bool True if the line was a valid result line, false otherwise.
     */
    bool parse_line(const std::string& line, DuplicationEntry& entry) const;

    /**
     * @brief Reads and parses all result lines from the script's stdout.
     * @param pipe Open read pipe returned by popen.
     * @return vector<DuplicationEntry> Parsed pairs filtered by the threshold.
     */
    std::vector<DuplicationEntry> read_results(FILE* pipe) const;

    /**
     * @brief Saves duplication results to output.
     * @param file_duplication_pairs Pairs to save.
     */
    void save_duplications(std::vector<DuplicationEntry>& file_duplication_pairs) override;

  public:
    /**
     * @brief Constructs the LLM detector.
     * @param base_path_ Root path of the codebase cache.
     * @param similarity_ Minimum similarity threshold (0-100) to consider as duplicate.
     * @param max_seq_length_ Optional override for the embedding script's max sequence length.
     * @param batch_size_ Optional override for the embedding script's batch size.
     * @param model_ Optional override for the embedding model name (Hugging Face id).
     */
    LLMMethod(const fs::path& base_path_, double similarity_,
              std::optional<int> max_seq_length_ = std::nullopt,
              std::optional<int> batch_size_ = std::nullopt,
              std::optional<std::string> model_ = std::nullopt);

    void on_function(const FunctionData& fd) override;

    void execute() override;
};
