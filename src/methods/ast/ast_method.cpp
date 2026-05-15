#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/methods/ast/ast_method.hpp>
#include <arkanjo/base/config.hpp>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/features/ast_feature.hpp>
#include <arkanjo/base/features/source_feature.hpp>

#include <thread>

using fm = FormatterManager;

struct ZSNode {
    std::string type;
    std::vector<ZSNode> children;
};

ZSNode from_tsnode(TSNode node) {
    ZSNode out;
    out.type = ts_node_type(node);

    uint32_t count = ts_node_named_child_count(node);

    for (uint32_t i = 0; i < count; i++) {
        out.children.push_back(from_tsnode(ts_node_named_child(node, i)));
    }

    return out;
}

int build_postorder(const ZSNode& node, PostOrderTree& tree) {
    int leftmost = -1;

    for (const auto& child : node.children) {
        int child_lmd = build_postorder(child, tree);

        if (leftmost == -1) {
            leftmost = child_lmd;
        }
    }

    int id = tree.labels.size();

    tree.labels.push_back(node.type);

    if (leftmost == -1) {
        leftmost = id;
    }

    tree.lmd.push_back(leftmost);

    return leftmost;
}

int ASTMethod::tree_distance(const PostOrderTree& a, const PostOrderTree& b) {
    size_t n = a.labels.size();
    size_t m = b.labels.size();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

    for (size_t i = 0; i <= n; i++) {
        dp[i][0] = i;
    }

    for (size_t j = 0; j <= m; j++) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= m; j++) {

            int rename_cost = a.labels[i - 1] == b.labels[j - 1] ? 0 : 1;

            dp[i][j] = std::min({
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + rename_cost
            });
        }
    }

    return dp[n][m];
}

double ASTMethod::similarity_score(const PostOrderTree& a, const PostOrderTree& b) {
    int dist = tree_distance(a, b);

    int max_size = std::max(a.labels.size(), b.labels.size());

    return 1.0 - (static_cast<double>(dist) / max_size);
}

ASTMethod::ASTMethod(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;

    if (similarity < 0) {
        std::cerr << "SIMILARITY SHOULD BE GREATER OR EQUAL 0 TO USE DUPLICATION FINDER BY AST COMMAND";
    }
}

void ASTMethod::save_duplications(std::vector<DuplicationEntry>& file_duplication_pairs) {
    std::string output_file_path = base_path / "output_parsed.txt";

    auto fout = std::ofstream(output_file_path);

    fout << file_duplication_pairs.size() << '\n';
    for (const auto& [similarity, path1, path2] : file_duplication_pairs) {
        fout << path1 << ' ' << path2 << ' ';
        fout << std::fixed << std::setprecision(2) << similarity << '\n';
    }

    fout.close();
}

std::vector<DuplicationEntry> ASTMethod::compare_range(
    const std::vector<PostOrderTree>& processed,
    size_t begin, size_t end
) {
    std::vector<DuplicationEntry> local;

    for (size_t i = begin; i < end; i++) {
        for (size_t j = i + 1; j < processed.size(); j++) {
            double sim = similarity_score(processed[i], processed[j]) * 100.0;

            if (sim >= similarity)
                local.push_back({sim, processed[i].path, processed[j].path});
        }
    }

    return local;
}

void ASTMethod::on_function(const FunctionData& fd) {
    auto ast = fd.get_feature<ASTFeature>();
    if (!ast)
        return;

    auto tree = from_tsnode(ast->root);

    PostOrderTree p;
    build_postorder(tree, p);

    fs::path path{fd.path};
    std::string filename = fd.function_name + path.extension().string();

    p.path = path / filename;

    processed.push_back(p);
}

void ASTMethod::execute() {
    unsigned int thread_count = std::thread::hardware_concurrency();
    const size_t threads = std::max<size_t>(1, thread_count);

    size_t n = processed.size();
    size_t chunk = (n + threads - 1) / threads;

    std::vector<std::thread> workers;
    std::vector<std::vector<DuplicationEntry>> results(threads);

    for (size_t t = 0; t < threads; t++) {
        size_t begin = t * chunk;
        size_t end = std::min(begin + chunk, n);

        if (begin >= end)
            break;

        workers.emplace_back([&, t, begin, end]() {
            results[t] = compare_range(processed, begin, end);
        });
    }

    for (auto& w : workers)
        w.join();

    std::vector<DuplicationEntry> duplications;

    for (auto& r : results) {
        duplications.insert(duplications.end(), r.begin(), r.end());
    }

    save_duplications(duplications);
}