#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/base/preprocess_state.hpp>

#include <arkanjo/utils/utils.hpp>

PathId Similarity_Table::find_id_path(const Path& path) {
    auto [it, inserted] = path_id.try_emplace(path, paths.size());

    if (inserted) {
        paths.push_back(path);
        similarity_graph.emplace_back();
    }

    return it->second;
}

void Similarity_Table::read_comparation(std::ifstream& table_file) {
    std::string string_path1, string_path2;
    double similarity;
    table_file >> string_path1 >> string_path2 >> similarity;

    PathId id1 = find_id_path(Path(string_path1));
    PathId id2 = find_id_path(Path(string_path2));

    if (id1 > id2) {
        std::swap(id1, id2);
    }

    similarity_graph[id1].push_back(std::make_pair(id2, similarity));
    similarity_graph[id2].push_back(std::make_pair(id1, similarity));
    similarity_table[std::make_pair(id1, id2)] = similarity;
}

void Similarity_Table::read_file_table(std::ifstream& table_file) {
    int number_comparations;
    table_file >> number_comparations;
    for (int i = 0; i < number_comparations; i++) {
        read_comparation(table_file);
    }
}

Similarity_Table::Cache_Compatibility Similarity_Table::init_similarity_table() {
    auto res = Cache_Compatibility::Success;

    auto params = Preprocess_State::read_current_run_params();

    if (!Preprocess_State::is_cache_compatible(params.version)) {
        res = Cache_Compatibility::Fail;
    }

    std::ifstream table_file;
    const fs::path similarity_table_file_name = Config::config().base_path / Config::config().name_container / SIMILARITY_TABLE_FILE_NAME;
    table_file.open(similarity_table_file_name);
    Utils::ensure_file_is_open(table_file, similarity_table_file_name);

    read_file_table(table_file);

    table_file.close();
    return res;
}

Similarity_Table::Similarity_Table(double _similarity_threshold)
    : similarity_threshold{_similarity_threshold} { }

Similarity_Table::Similarity_Table()
    :  similarity_threshold{DEFAULT_SIMILARITY} { }

Similarity_Table::Cache_Compatibility Similarity_Table::load() {
    return init_similarity_table();
}

void Similarity_Table::update_similarity(double new_similarity_threshold) {
    similarity_threshold = new_similarity_threshold;
}

double Similarity_Table::get_similarity(const Path& path1, const Path& path2) {
    PathId id1 = find_id_path(path1);
    PathId id2 = find_id_path(path2);

    if (id1 == id2) {
        return MAXIMUM_SIMILARITY;
    }
    if (id1 > id2) {
        std::swap(id1, id2);
    }
    std::pair<PathId, PathId> aux = std::make_pair(id1, id2);
    auto it = similarity_table.find(aux);
    if (it != similarity_table.end())
        return it->second;
    return MINIMUM_SIMILARITY;
}

const Path& Similarity_Table::get_path(PathId id) const {
    return paths[id];
}

bool Similarity_Table::is_above_threshold(double similarity) const {
    return similarity_threshold <= similarity + EPS_ERROR_MARGIN;
}

double Similarity_Table::get_threshold_similarity() const {
    return similarity_threshold;
}

bool Similarity_Table::is_similar(const Path& path1, const Path& path2) {
    double similarity = get_similarity(path1, path2);
    return is_above_threshold(similarity);
}

const std::vector<Path>& Similarity_Table::get_path_list() const {
    return paths;
}

std::vector<Path> Similarity_Table::get_similar_path_to_the_reference(const Path& reference) {
    int reference_id = find_id_path(reference);
    std::vector<Path> ret;
    for (auto [neighbor_id, similarity] : similarity_graph[reference_id]) {
        if (is_above_threshold(similarity)) {
            ret.push_back(paths[neighbor_id]);
        }
    }
    return ret;
}

std::vector<SimilarPair> Similarity_Table::get_all_similar_pairs() {
    std::vector<SimilarPair> similar_pairs;
    for (const auto& [ids, similarity] : similarity_table) {
        const Path& path1 = paths[ids.first];
        const Path& path2 = paths[ids.second];

        if (!is_similar(path1, path2))
            continue;

        Function function(path1);
        function.load();

        similar_pairs.push_back({
            similarity, 
            function.number_of_lines(), 
            ids.first, 
            ids.second
        });
    }
    return similar_pairs;
}

// TODO: Stable sort is currently used to preserve output compatibility.
// Define an explicit ordering for ties and switch back to std::sort.
void Similarity_Table::sort_pairs_by_similarity(std::vector<SimilarPair>& similar_pairs) const {
    std::stable_sort(
        similar_pairs.begin(),
        similar_pairs.end(),
        [](const SimilarPair& pair1, const SimilarPair& pair2) {
            return pair1.similarity > pair2.similarity;
        });
}

void Similarity_Table::sort_pairs_by_line_number(std::vector<SimilarPair>& similar_pairs) const {
    std::stable_sort(
        similar_pairs.begin(),
        similar_pairs.end(),
        [](const SimilarPair& pair1, const SimilarPair& pair2) {
            return pair1.number_lines > pair2.number_lines;
        });
}

int Similarity_Table::get_number_lines_in_pair(const Path& path1, const Path& path2) {
    Function f1(path1);
    f1.load();
    Function f2(path2);
    f2.load();
    return f1.number_of_lines() + f2.number_of_lines();
}

std::vector<Cluster> Similarity_Table::get_clusters() {
    int n = paths.size();

    std::vector<bool> visited(n, false);
    std::vector<Cluster> clusters;

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;

        std::vector<int> stack;
        std::vector<int> component;

        stack.push_back(i);
        visited[i] = true;

        while (!stack.empty()) {
            int current = stack.back();
            stack.pop_back();

            component.push_back(current);

            for (auto [neighbor, similarity] : similarity_graph[current]) {
                if (!visited[neighbor] && is_above_threshold(similarity)) {
                    visited[neighbor] = true;
                    stack.push_back(neighbor);
                }
            }
        }

        if (component.size() > 1) {
            clusters.push_back({component});
        }
    }

    return clusters;
}

std::vector<ClusterInfo> Similarity_Table::get_clusters_info(bool sorted) {
    auto raw_clusters = get_clusters();
    std::vector<ClusterInfo> clusters_info;

    for (const auto& cluster : raw_clusters) {
        ClusterInfo info;

        for (int id : cluster.members) {
            info.paths.push_back(paths[id]);
        }

        for (size_t i = 0; i < info.paths.size(); i++) {
            for (size_t j = i + 1; j < info.paths.size(); j++) {
                int lines = get_number_lines_in_pair(info.paths[i], info.paths[j]);
                if (lines > 0) {
                    info.total_pairs++;
                    info.total_lines += lines;
                }
            }
        }

        if (info.total_pairs > 0) {
            clusters_info.push_back(info);
        }
    }

    if (sorted) {
        std::sort(clusters_info.begin(), clusters_info.end(), 
            [](const ClusterInfo& a, const ClusterInfo& b) {
                return a.score() > b.score();
            });
    }

    return clusters_info;
}