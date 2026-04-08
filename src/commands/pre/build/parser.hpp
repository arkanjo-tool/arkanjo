/**
 * @file parser.hpp
 * @brief Code duplication results parser
 *
 * Handles parsing and transformation of output from third-party
 * code duplication detection tools into a more usable format.
 *
 * The parser is responsible to read the output to the code duplication detection tool
 * that exists in the third_party folder and parse/transform it in a way that is easier
 * to work with later.
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

/**
 * @brief Structure representing a code comparison result
 *
 * Stores information about two code paths and their similarity score,
 * with comparison operators for sorting and deduplication.
 */
struct Comparation {
    std::string path1;      ///< Path to first code file being compared
    std::string path2;      ///< Path to second code file being compared
    double similarity; ///< Similarity score between the files (0-100)

    /**
     * @brief Default constructor
     */
    Comparation() {}

    /**
     * @brief Parameterized constructor
     * @param _path1 First file path
     * @param _path2 Second file path
     * @param _sim Similarity score
     */
    Comparation(std::string _path1, std::string _path2, double _sim);

    /**
     * @brief Comparison operator for sorting
     * @param com Other Comparation to compare with
     * @return bool True if this should be ordered before com
     */
    bool operator<(const Comparation& com) const;

    /**
     * @brief Equality comparison operator
     * @param com Other Comparation to compare with
     * @return bool True if comparations are identical
     */
    bool operator==(const Comparation& com) const;
};

/**
 * @brief Parses and transforms duplication detection tool output
 *
 * Processes raw output from third-party duplication detection tools,
 * filters results by similarity threshold, and produces cleaned output.
 */
class Parser {
  private:
    std::string PROJECT_PATH_MESSAGE = "Enter your project path:";                                  ///< Project path prompt
    std::string MINIMUM_SIMILARITY_MESSAGE = "Enter minimum similarity desired on using the tool:"; ///< Similarity threshold prompt

    std::ofstream fout;          ///< Output file stream
    double similarity_cap; ///< Minimum similarity threshold
    std::set<Comparation> comparations;

    /**
     * @brief Splits a line into components
     * @param line Input line to parse
     * @return vector<string> Tokenized line components
     */
    std::vector<std::string> parser_line(const std::string& line);

    /**
     * @brief Checks if string represents a valid file path
     * @param s String to check
     * @return bool True if string is a file path
     */
    bool is_an_file(const std::string& s);

    /**
     * @brief Extracts similarity score from string
     * @param s String containing similarity value
     * @return double Parsed similarity score
     */
    double retrive_similarity(const std::string& s);

    /**
     * @brief Processes a block of comparison results
     * @param tokens Tokenized line components
     * @param comparations Set to store parsed comparisons
     */
    void parser_block_stream(const std::string& path, const std::vector<std::string>& tokens, std::set<Comparation>& comparations);

  public:
    /**
     * @brief Constructs parser with configuration
     * @param output_file Path for cleaned output
     * @param similarity_cap Minimum similarity threshold (0-100)
     */
    explicit Parser(const fs::path& output_file, double similarity_cap);

    /**
     * @brief Main parsing execution method using stream
     */
    void exec_from_stream(FILE* pipe);

    ~Parser();
};
