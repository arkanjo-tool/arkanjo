/**
 * @file similarity_table.hpp
 * @brief Similarity relationships storage and analysis
 *
 * Stores and analyzes similarity relationships between code functions,
 * including similarity probabilities and threshold-based filtering.
 *
 * Similarity Table is a abstraction that store the pair of functions
 * that are similar to each other and the similarity probability between
 * them.
 */

#pragma once

#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/config.hpp>

struct PathId {
  int value;

  explicit PathId(int v = -1) : value(v) {}

  bool operator==(const PathId& other) const {
    return value == other.value;
  }

  bool operator!=(const PathId& other) const {
    return value != other.value;
  }

  bool operator<(const PathId& other) const {
    return value < other.value;
  }

  operator int() const { return value; }
};

/**
 * {
 * 1: [{ key, weight }, { key, weight }]
 * 2: [{ key, weight }]
 * 3: []
 * ...
 * }
 */
template<typename Key, typename Weight>
using AdjacencyList = std::vector<std::vector<std::pair<Key, Weight>>>;

/**
 * @struct Cluster
 * @brief Represents a cluster of similar functions in the similarity graph.
 */
struct Cluster {
    std::vector<int> members; ///< List of node indices (Path IDs) in the cluster 
};

struct ClusterInfo {
    std::vector<Path> paths;
    int total_lines = 0;
    int total_pairs = 0;

    double score() const {
        double w_files = 0.3;
        double w_lines = 0.4;
        double w_density = 0.3;
        double d = total_pairs > 0 ? (double)total_lines / total_pairs : 0;
        return w_files * paths.size() + w_lines * total_lines + w_density * d;
    }
};

/**
 * @brief Represents a similarity graph between functions (paths).
 *
 * Each node corresponds to a function (identified by a Path).
 * Stores pairs of similar functions with their similarity scores.
 *
 * Internally:
 * - `paths` stores all known functions.
 * - `path_id` maps a Path to its unique node ID.
 * - `similarity_graph` is an adjacency list representation:
 *      node -> [(neighbor_id, similarity), ...]
 * - `similarity_table` stores pairwise similarity for fast lookup.
 *
 * Graph interpretation:
 * - Nodes = functions
 * - Edges = similarity >= threshold (filtered at query time)
 *
 * @note The graph is undirected.
 */
class Similarity_Table {
  private:
    static constexpr const char* SIMILARITY_TABLE_FILE_NAME = "output_parsed.txt"; ///< Default similarity data file
    static constexpr const double DEFAULT_SIMILARITY = 100.00; ///< Default similarity threshold
    static constexpr const double EPS_ERROR_MARGIN = 1e-6;     ///< Floating-point comparison margin
    static constexpr const double MAXIMUM_SIMILARITY = 100.00; ///< Maximum possible similarity score
    static constexpr const double MINIMUM_SIMILARITY = 0.00;   ///< Minimum possible similarity score

    double similarity_threshold;    ///< Current similarity threshold
    std::vector<Path> paths;        ///< List of all known paths
    std::map<Path, PathId> path_id; ///< Path to ID mapping

    /// Adjacency list representing the similarity graph.
    /// Each index corresponds to a function ID, and stores
    /// (neighbor_id, similarity_score).
    AdjacencyList<PathId, double> similarity_graph;
    std::map<std::pair<PathId, PathId>, double> similarity_table; ///< Similarity score lookup table

    /**
     * @brief Finds or assigns ID for a path
     * @param path Path to look up
     * @return int Unique ID for the path
     */
    PathId find_id_path(const Path& path);

    /**
     * @brief Reads single comparison from file
     * 
     * Format: string_path1 >> string_path2 >> similarity
     * 
     * @param table_file Input file stream
     */
    void read_comparation(std::ifstream& table_file);

    /**
     * @brief Reads entire similarity table from file
     * @param table_file Input file stream
     */
    void read_file_table(std::ifstream& table_file);

    /**
     * @brief Initializes similarity table from file
     */
    void init_similarity_table();

    /**
     * @brief Checks if similarity meets threshold
     * @param similarity Similarity score to check
     * @return bool True if score meets threshold
     */
    bool is_above_threshold(double similarity) const;

    /**
     * @brief Sorts path pairs by line count
     * @param similar_path_pairs Pairs to sort
     * @return Sorted pairs with line counts
     */
    std::vector<std::tuple<int, Path, Path>> sort_pairs_by_line_number(const std::vector<std::pair<Path, Path>>& similar_path_pairs) const;

  public:
    /**
     * @brief Constructs with custom similarity threshold
     * @param _similarity_threshold Initial threshold value
     */
    explicit Similarity_Table(double _similarity_threshold);

    /**
     * @brief Constructs with default similarity threshold
     */
    explicit Similarity_Table();

    void load();

    /**
     * @brief Updates similarity threshold
     * @param new_similarity_threshold New threshold value
     */
    void update_similarity(double new_similarity_threshold);

    /**
     * @brief Gets similarity between two paths
     * @param path1 First path to compare
     * @param path2 Second path to compare
     * @return double Similarity score
     */
    double get_similarity(const Path& path1, const Path& path2);

    /**
     * @brief Checks if two paths are similar
     * @param path1 First path to compare
     * @param path2 Second path to compare
     * @return bool True if paths are similar
     */
    bool is_similar(const Path& path1, const Path& path2);

    /**
     * @brief Gets list of all known paths
     * @return vector<Path> All paths in table
     */
    const std::vector<Path>& get_path_list() const;

    int get_number_lines_in_pair(const Path& path1, const Path& path2);

    /**
     * @brief Gets paths similar to reference path
     * @param reference Path to compare against
     * @return vector<Path> Similar paths
     */
    std::vector<Path> get_similar_path_to_the_reference(const Path& reference);

    /**
     * @brief Gets all similar path pairs with scores, sorted
     * @return vector<tuple<double,Path,Path>> Similar pairs with scores
     */
    std::vector<std::tuple<double, Path, Path>> get_all_path_pairs_and_similarity_sorted_by_similarity();

    /**
     * @brief Gets all similar path pairs, sorted by similarity
     * @return vector<pair<Path,Path>> Similar path pairs
     */
    std::vector<std::pair<Path, Path>> get_all_similar_path_pairs_sorted_by_similarity();

    /**
     * @brief Gets all similar path pairs, sorted by line count
     * @return vector<pair<Path,Path>> Similar path pairs
     */
    std::vector<std::pair<Path, Path>> get_all_similar_path_pairs_sorted_by_line_number();

    /**
     * @brief Generate clusters of similar functions using DFS on the similarity graph.
     * Only edges with similarity >= threshold are considered.
     * @return vector of Cluster objects
     */
    std::vector<Cluster> get_clusters();

    /**
     * @brief Returns detailed information about all clusters found in the similarity table.
     *
     * @return std::vector<ClusterInfo>
     */
    std::vector<ClusterInfo> get_clusters_info(bool sorted);
};
