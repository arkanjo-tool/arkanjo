/**
 * @file ast_method.hpp
 */

#pragma once

#include <arkanjo/methods/method.hpp>

#include <vector>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <atomic>
#include <tree_sitter/api.h>

namespace fs = std::filesystem;

struct ZSNode {
  TSSymbol symbol;
  std::vector<ZSNode> children;
};

struct PostOrderTree {
  /**
   * Tree-sitter grammar symbol for each node.
   */
  std::vector<TSSymbol> labels;

  /**
   * Left-most descendant index for each node.
   *
   * Stored for future compatibility with Zhang-Shasha style tree edit distance
   * algorithms.
   *
   * Currently not used by the similarity computation.
   */
  std::vector<int> lmd;

  fs::path path;
};

class ASTMethod : public IMethod {
  private:
    static constexpr const char* SAVING_MESSAGE = "Saving results..."; ///< Status message for saving output

    fs::path base_path;  ///< Root path of the codebase to analyze
    double similarity; ///< Similarity threshold for considering duplicates

    std::vector<PostOrderTree> processed;

    std::ofstream output_file;

    std::mutex processed_mutex;
    std::mutex output_mutex;

    std::atomic<size_t> total_matches{0};

    ZSNode from_tsnode(TSNode node);

    /**
     * @brief Computes the edit distance between two post-order trees.
     *
     * This implementation applies the Levenshtein distance algorithm
     * over the post-order label sequences of both trees.
     *
     * Supported operations:
     * - insertion
     * - deletion
     * - substitution
     *
     * A substitution has zero cost when labels are equal,
     * otherwise the replacement cost is one.
     *
     * @param a First post-order tree.
     * @param b Second post-order tree.
     *
     * @return Edit distance between both label sequences.
     */
    int tree_distance(const PostOrderTree& a, const PostOrderTree& b);

    /**
     * @brief Computes a normalized similarity score between two trees.
     *
     * The similarity is derived from the edit distance between the
     * post-order label sequences and normalized by the size of the
     * largest tree.
     *
     * Similarity range:
     * - 1.0 -> identical trees
     * - 0.0 -> completely different trees
     *
     * @param a First post-order tree.
     * @param b Second post-order tree.
     *
     * @return Normalized similarity score in the range [0.0, 1.0].
     */
    double similarity_score(const PostOrderTree& a, const PostOrderTree& b);

    /**
     * @brief Compares a subset of trees against all remaining trees.
     *
     * Computes the similarity score between PostOrderTree objects and
     * returns all pairs whose similarity is greater than or equal to
     * the configured threshold.
     *
     * The range [begin, end) allows the comparison workload to be
     * distributed across multiple threads.
     *
     * Returned tuple format:
     * (similarity_percentage, path1, path2)
     *
     * @param processed Vector containing processed post-order trees.
     * @param begin Initial comparison index.
     * @param end Final comparison index.
     *
     * @return Vector containing detected similar tree pairs.
     */
    void compare_range(
      const std::vector<PostOrderTree>& processed,
      size_t begin, size_t end,
      std::vector<DuplicationEntry>& local
    );

    void save_duplications(std::vector<DuplicationEntry>& file_duplication_pairs) override;
    void flush_duplications(std::vector<DuplicationEntry>& local);
    void header_duplications();
  public:
    /**
     * @brief Constructs preprocessor with configuration
     * @param base_path_ Root path of codebase
     * @param similarity_ Similarity threshold (0.0-100.0)
     */
    ASTMethod(const fs::path& base_path_, double similarity_);

    void on_function(const FunctionData& fd) override;

    /**
     * @brief Executes the preprocessing pipeline
     */
    void execute() override;
};
