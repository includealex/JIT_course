#include "loop_tree.hpp"

namespace custom {
void LoopTree::build_tree(Graph* graph) {
    custom::DFS df;
    df.run_dfs(graph->get_root());

    std::unordered_map<size_t, LTNode*> loop_nodes;
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
        if (it != looped_element_ids.end()) {
            root_succs_ids.push_back(el);
        }
    }

    root = new LTNode(-1);

    for (auto& loop : loops) {
        std::size_t header_id = loop.get_header()->get_id();
        
        LTNode* node = new LTNode(header_id);
        node->set_header(header_id);

        std::vector<std::size_t> latches_id;
        for(auto& block : loop.get_latches()) {
            latches_id.push_back(block->get_id());
        }
        node->set_latches_id(latches_id);

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

        node->set_blocks_id(blocks_id);
        loop_nodes[header_id] = node;
    }

    for (auto& [header_id, node] : loop_nodes) {
        for (auto& [possible_parent_id, possible_parent_node] : loop_nodes) {
            if (header_id != possible_parent_id) {
                auto& parent_blocks = possible_parent_node->get_blocks_id();
                if (std::find(parent_blocks.begin(), parent_blocks.end(), header_id) != parent_blocks.end()) {
                    possible_parent_node->succs.push_back(node);
                    break;
                }
            }
        }
    }

    for (auto& root_id : root_succs_ids) {
        root->succs.push_back(loop_nodes[root_id]);
    }

    auto num_of_blocks = graph->basic_blocks_num();
    std::vector<size_t> all_block_ids(num_of_blocks);
    std::iota(std::begin(all_block_ids), std::end(all_block_ids), 0);

    for (auto& el: looped_element_ids) {
        auto it = std::find(all_block_ids.begin(), all_block_ids.end(), el);
        if (it != all_block_ids.end()) {
            all_block_ids.erase(it);
        }
    }

    root->set_blocks_id(all_block_ids);
    print_tree();
}

LoopTree::~LoopTree() {
    delete_tree(root);
    // delete root;
}

void LoopTree::delete_tree(LTNode* node) {
    if (node == nullptr) return;

    for (auto* child : node->succs) {
        delete_tree(child);
    }

    delete node;
}

} // namespace custom