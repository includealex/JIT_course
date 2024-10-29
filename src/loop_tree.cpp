#include "loop_tree.hpp"

namespace custom {

void LoopTree::build_tree (Graph* graph) {
    custom::DFS df;
    df.run_dfs(graph->get_root());

    // Filling loops
    std::vector<size_t> headers_ids;
    std::set<size_t> looped_element_ids;
    for (auto loop : df.get_loops()) {
        auto header = loop.get_header();
        headers_ids.push_back(header->get_id());
        for (auto& latch: loop.get_latches()) {
            auto res = df.get_basic_blocks_between(header, latch);
            for (auto& el: res) {
                loop.add_block(el);
                // if (el->get_id() != header->get_id())
                looped_element_ids.insert(el->get_id());
            }
        }
    }

    std::vector<size_t> root_succs_ids;
    for (auto& el: headers_ids) {
        auto it = std::find(looped_element_ids.begin(), looped_element_ids.end(), el);
        if (it == looped_element_ids.end()) {
            root_succs_ids.push_back(el);
        }
    }

    auto num_of_blocks = graph->basic_blocks_num();

    std::vector<size_t> v(num_of_blocks);
    std::iota(std::begin(v), std::end(v), 0);

    for (auto& el: looped_element_ids) {
        auto it = std::find(v.begin(), v.end(), el);
        if (it != v.end()) {
            v.erase(it);
        }
    }

    LTNode* root = new LTNode(0);
    // contains root_succs_ids and block
    std::vector<std::size_t> root_arr(v.begin(), v.end());
    root->set_blocks_id(root_arr);
    // root should point to loop with header id from root_succs_ids vector.


}

} // namespace custom