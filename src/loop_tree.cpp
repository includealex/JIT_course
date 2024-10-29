#include "loop_tree.hpp"
#include <unordered_set>

namespace custom {

void LoopTree::build_tree(Graph* graph) {
    custom::DFS df;
    df.run_dfs(graph->get_root());

    std::unordered_map<size_t, LTNode*> loop_nodes;
    std::unordered_set<size_t> unique_headers;
    std::vector<size_t> headers_ids;
    std::set<size_t> looped_element_ids;
    auto loops = df.get_loops();

    for (auto& loop : loops) {
        auto header = loop.get_header();
        headers_ids.push_back(header->get_id());
        for (auto& latch : loop.get_latches()) {
            auto res = df.get_basic_blocks_between(header, latch);
            for (auto& el : res) {
                loop.add_block(el);
                looped_element_ids.insert(el->get_id());
            }
        }
    }

    std::vector<size_t> root_succs_ids;
    for (auto& el : headers_ids) {
        if (looped_element_ids.find(el) != looped_element_ids.end()) {
            root_succs_ids.push_back(el);
        }
    }

    root = new LTNode(-1);

    for (auto& loop : loops) {
        std::size_t header_id = loop.get_header()->get_id();
        
        if (loop_nodes.find(header_id) == loop_nodes.end()) {
            LTNode* node = new LTNode(header_id);
            node->set_header(header_id);

            std::vector<std::size_t> latches_id;
            for (auto& block : loop.get_latches()) {
                latches_id.push_back(block->get_id());
            }
            node->set_latches_id(latches_id);

            std::vector<std::size_t> blocks_id;
            for (auto& block : loop.get_blocks()) {
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
    }

    for (auto& [header_id, node] : loop_nodes) {
        for (auto& [possible_parent_id, possible_parent_node] : loop_nodes) {
            if (header_id != possible_parent_id) {
                auto& parent_blocks = possible_parent_node->get_blocks_id();
                if (std::find(parent_blocks.begin(), parent_blocks.end(), header_id) != parent_blocks.end()) {
                    if (std::find(possible_parent_node->succs.begin(), possible_parent_node->succs.end(), node) == possible_parent_node->succs.end()) {
                        possible_parent_node->succs.push_back(node);
                    }
                    break;
                }
            }
        }
    }

    for (auto& root_id : root_succs_ids) {
        if (loop_nodes.find(root_id) != loop_nodes.end()) {
            if (std::find(root->succs.begin(), root->succs.end(), loop_nodes[root_id]) == root->succs.end()) {
                root->succs.push_back(loop_nodes[root_id]);
            }
        }
    }

    auto num_of_blocks = graph->basic_blocks_num();
    std::vector<size_t> all_block_ids(num_of_blocks);
    std::iota(std::begin(all_block_ids), std::end(all_block_ids), 0);

    for (auto& el : looped_element_ids) {
        auto it = std::find(all_block_ids.begin(), all_block_ids.end(), el);
        if (it != all_block_ids.end()) {
            all_block_ids.erase(it);
        }
    }

    root->set_blocks_id(all_block_ids);
    remove_duplicate_level_nodes(root);
    clean_outer_loop_blocks(root);
    print_tree();
}

LoopTree::~LoopTree() {
    delete_tree(root);
}

void LoopTree::delete_tree(LTNode* node) {
    if (node == nullptr) return;

    for (auto* child : node->succs) {
        delete_tree(child);
    }

    delete node;
}

void LoopTree::clean_outer_loop_blocks(LTNode* node) {
    if (!node) return;

    std::unordered_set<size_t> inner_headers;
    std::unordered_set<size_t> inner_blocks;

    for (const auto& child : node->succs) {
        inner_headers.insert(child->idx);
        const auto& child_blocks = child->get_blocks_id();
        inner_blocks.insert(child_blocks.begin(), child_blocks.end());
    }

    auto& blocks = node->get_blocks_id();
    blocks.erase(
        std::remove_if(
            blocks.begin(),
            blocks.end(),
            [&](size_t block_id) {
                return inner_headers.count(block_id) > 0 || inner_blocks.count(block_id) > 0;
            }
        ),
        blocks.end()
    );

    for (auto& child : node->succs) {
        clean_outer_loop_blocks(child);
    }
}

void LoopTree::remove_duplicate_level_nodes(LTNode* parent) {
    if (!parent) return;

    for (auto it = parent->succs.begin(); it != parent->succs.end();) {
        LTNode* node = *it;

        bool is_duplicate = false;

        for (const auto& sibling : parent->succs) {
            if (sibling == node) continue;

            auto& sibling_blocks = sibling->get_blocks_id();
            if (std::find(sibling_blocks.begin(), sibling_blocks.end(), node->idx) != sibling_blocks.end()) {
                is_duplicate = true;
                break;
            }
        }

        if (is_duplicate) {
            it = parent->succs.erase(it);
        } else {
            remove_duplicate_level_nodes(node);
            ++it;
        }
    }
}

} // namespace custom
