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

    for (size_t idx = 0; idx < graph->basic_blocks_num(); ++idx) {
        auto el = graph->get_block(idx);
        DFS tmp_dfs;
        tmp_dfs.run_dfs(root, el);
        auto tmp_root_ids_arr = tmp_dfs.get_dfs_ids_arr();
        std::sort(tmp_root_ids_arr.begin(), tmp_root_ids_arr.end());

        std::vector<size_t> difference;
        std::set_difference(
            root_ids_arr.begin(), root_ids_arr.end(),
            tmp_root_ids_arr.begin(), tmp_root_ids_arr.end(),
            std::back_inserter(difference)
        );
        auto it = std::find(difference.begin(), difference.end(), idx);
        difference.erase(it);

        premap.insert({idx, difference});
    }

    return premap;
}

void DominTree::build_tree(Graph* graph) {
    auto premap = get_premap(graph);

    std::unordered_map<size_t, std::vector<size_t>> aftermap;
    for (auto& kev : premap) {
        int id = kev.first;
        std::vector<size_t> start_near_succs = kev.second;

        std::set<size_t> far_succs;
        for (auto& elem: start_near_succs) {
            auto it = premap.find(elem);
            auto sz = it->second.size();
            for (size_t idx = 0; idx < sz; ++idx) {
                far_succs.insert(it->second[idx]);
            }
        }

        for (auto& elem: far_succs) {
            auto it = std::find(start_near_succs.begin(), start_near_succs.end(), elem);
            start_near_succs.erase(it);
        }

        aftermap.insert({id, start_near_succs});
    }

    _domin_tree_root = convert_to_tree(aftermap, 0);
}

DTNode* DominTree::prebuild_tree(size_t idx, std::unordered_map<size_t, DTNode*>& nodeMap,
                const std::unordered_map<size_t, std::vector<size_t>>& aftermap) {
    if (nodeMap.find(idx) != nodeMap.end()) {
        return nodeMap[idx];
    }

    DTNode* node = new DTNode(idx);
    nodeMap[idx] = node;

    if (aftermap.find(idx) != aftermap.end()) {
        for (size_t succIdx : aftermap.at(idx)) {
            DTNode* succNode = prebuild_tree(succIdx, nodeMap, aftermap);
            node->succs.push_back(succNode);
        }
    }
    
    return node;
}

DTNode* DominTree::convert_to_tree(const std::unordered_map<size_t, std::vector<size_t>>& aftermap, size_t rootIdx) {
    std::unordered_map<size_t, DTNode*> nodeMap;
    return prebuild_tree(rootIdx, nodeMap, aftermap);
}

DTNode* DominTree::find_node_by_index(size_t target_idx) {
    if (!_domin_tree_root) {
        return nullptr;
    }

    std::stack<DTNode*> node_stack;
    node_stack.push(_domin_tree_root);

    while (!node_stack.empty()) {
        DTNode* current_node = node_stack.top();
        node_stack.pop();

        if (current_node->idx == target_idx) {
            return current_node;
        }

        for (auto succ : current_node->succs) {
            node_stack.push(succ);
        }
    }

    return nullptr;
}

} // namespace custom
