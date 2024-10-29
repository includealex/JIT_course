#include "loop_tree.hpp"

namespace custom {

void LoopTree::build_tree (Graph* graph) {
    custom::DFS df;
    df.run_dfs(graph->get_root());

    // Filling loops
    std::vector<size_t> headers_ids;
    std::set<size_t> looped_element_ids;
    auto loops = df.get_loops();
    for (auto& loop : loops) {
        auto header = loop.get_header();
        headers_ids.push_back(header->get_id());
        for (auto& latch: loop.get_latches()) {
            auto res = df.get_basic_blocks_between(header, latch);
            for (auto& el: res) {
                loop.add_block(el);
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


    LTNode* root = new LTNode(0);

    for (auto& loop : loops) {
        std::size_t header_id = loop.get_header()->get_id();
        std::vector<std::size_t> latches_id;
        for(auto& block : loop.get_latches()) {
            latches_id.push_back(block->get_id());
        }
        std::vector<std::size_t> blocks_id;
        for(auto& block : loop.get_blocks()) {
            blocks_id.push_back(block->get_id());
        }

        for (auto& el : latches_id) {
            auto it = std::find(blocks_id.begin(), blocks_id.end(), el);
            if (it != blocks_id.end()) {
                blocks_id.erase(it);
            }
        }
        auto h_it = std::find(blocks_id.begin(), blocks_id.end(), header_id);
        if (h_it != blocks_id.end()) {
            blocks_id.erase(h_it);
        }
        std::cout << "For header with id: " << header_id << std::endl;
        for (auto& el : latches_id)
            std::cout << "latch id is: " << el << std::endl;
        for (auto& el : blocks_id)
            std::cout << "block id is: " << el << std::endl;
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
    // contains root_succs_ids and block
    std::vector<std::size_t> root_arr(v.begin(), v.end());
    root->set_blocks_id(root_arr);

}

} // namespace custom