#include "domin_tree.hpp"

namespace custom {

std::unordered_map<size_t, std::vector<size_t>> DominTree::get_premap(Graph* graph) {
    size_t root_idx = 0;
    auto root = graph->get_block(root_idx);
    DFS root_dfs;
    root_dfs.run_dfs(root);
    auto root_ids_arr = root_dfs.get_dfs_ids_arr();
    std::sort(root_ids_arr.begin(), root_ids_arr.end());

    std::unordered_map<size_t, std::vector<size_t>> premap;
    premap.insert({root_idx, root_ids_arr});

    for (size_t idx = 1; idx < graph->basic_blocks_num(); ++idx) {
        auto el = graph->get_block(idx);
        DFS tmp_dfs;
        tmp_dfs.run_dfs_excluded_block(root, el);
        auto tmp_root_ids_arr = tmp_dfs.get_dfs_ids_arr();
        std::sort(tmp_root_ids_arr.begin(), tmp_root_ids_arr.end());

        std::vector<size_t> difference;
        std::set_difference(
            root_ids_arr.begin(), root_ids_arr.end(),
            tmp_root_ids_arr.begin(), tmp_root_ids_arr.end(),
            std::back_inserter(difference)
        );

        premap.insert({idx, difference});
    }

    return premap;
}

void DominTree::build_tree(Graph* graph) {
    auto premap = get_premap(graph);

    // Root initialising
    node.cur = graph->get_block(0);

    // TODO: add some implementation of tree from hmap
    for (auto& el : premap) {
        for (size_t i = 0; i < el.second.size(); ++i) {
            std::cout << el.second[i] << std::endl;
        }
    }

    return;
}

} // namespace custom
