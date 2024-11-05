#include "rpo.hpp"

namespace custom {

void RPO::run_rpo(BasicBlock* block, BasicBlock* excluded_block) {
    _rpo_ids.clear();
    int id_to_excl = (excluded_block != nullptr) ? excluded_block->get_id() : -1;
    get_rpo_ids(block, id_to_excl);
    clear_all_markers(block);
}

void RPO::get_rpo_ids(BasicBlock* block, std::size_t exclude_id) {
    if (block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    std::unordered_set<BasicBlock*> visited;
    stack.push(block);

    while (!stack.empty()) {
        BasicBlock* curr = stack.top();

        if (curr->is_rpo_marker() || curr->get_id() == exclude_id) {
            stack.pop();
            continue;
        }

        if (visited.find(curr) != visited.end()) {
            _rpo_ids.push_back(curr->get_id());
            curr->set_rpo_marker(true);
            stack.pop();
        } 
        else {
            visited.insert(curr);
            stack.push(curr);
            for (BasicBlock* succ : {curr->get_succs_true(), curr->get_succs_false()}) {
                if (succ != nullptr && !succ->is_rpo_marker()) {
                    stack.push(succ);
                }
            }
        }
    }
}

void RPO::clear_all_markers(BasicBlock* block) {
    clear_rpo_markers(block);
}

void RPO::clear_rpo_markers(BasicBlock* block) {
    if (block == nullptr) {
        return;
    }

    std::stack<BasicBlock*> stack;
    stack.push(block);

    while (!stack.empty()) {
        BasicBlock* curr = stack.top();
        stack.pop();

        curr->set_rpo_marker(false);

        for (BasicBlock* succ : {curr->get_succs_true(), curr->get_succs_false()}) {
            if (succ != nullptr && succ->is_rpo_marker()) {
                stack.push(succ);
            }
        }
    }
}

std::vector<std::size_t> RPO::get_rpo_ids_arr() {
    return _rpo_ids;
}

} // namespace custom
