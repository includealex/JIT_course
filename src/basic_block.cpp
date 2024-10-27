#include "basic_block.hpp"
#include "instruction.hpp"
#include "graph.hpp"

namespace custom {

void BasicBlock::pushback_instr(Instruction* inst) {
    inst->set_basic_block(this);

    if (_last_inst) {
        auto prev_id = _last_inst->get_id();
        _last_inst->set_next(inst);
        inst->set_prev(_last_inst);
        inst->set_id(prev_id + 1);
        _last_inst = inst;
    } 
    else {
        _first_inst = inst;
        _last_inst = inst;
        inst->set_prev(nullptr);
        size_t begin_id = 0;
        inst->set_id(begin_id);
    }

    inst->set_next(nullptr);
}

std::size_t BasicBlock::instructions_amount() const{
    return _last_inst->get_id() + 1;
}

Instruction* BasicBlock::getInstruction(size_t index) const {
    auto cur_sz = instructions_amount();
    if (index >= cur_sz) {
        std::cout << "trying to get non-existant instruction" << std::endl;
        return nullptr;
    }

    Instruction* cur = _first_inst;
    for(size_t i = 0; i < index; ++i) {
        cur = cur->get_next();
    }

    return cur;
}

bool BasicBlock::isPhiInstruction(Instruction* inst) {
    return inst->getOpcode() == Opcode::PHI;
}

std::vector<BasicBlock*> BasicBlock::get_preds() {
    return _preds;
}

BasicBlock* BasicBlock::get_succs_true() {
    return _succs_true;
}

BasicBlock* BasicBlock::get_succs_false() {
    return _succs_false;
}

Instruction* BasicBlock::get_first_Phi() {
    return _first_Phi;
}

Instruction* BasicBlock::get_first_inst() {
    return _first_inst;
}

Instruction* BasicBlock::get_last_inst() {
    return _last_inst;
}

Graph* BasicBlock::get_graph() {
    return _graph;
}

void BasicBlock::set_graph(Graph* other) {
    _graph = other;
}

void BasicBlock::push_preds_back(BasicBlock* el) {
    _preds.push_back(el);
}

void BasicBlock::add_succs_false(BasicBlock* el) {
    _succs_false = el;
}

void BasicBlock::add_succs_true(BasicBlock* el) {
    _succs_true = el;
}

void BasicBlock::set_id(std::size_t id) {
    _basic_block_id = id;
}

std::size_t BasicBlock::get_id() {
    return _basic_block_id;
}

void BasicBlock::set_dfs_marker(bool value) {
    _markers.set_dfs_m(value);
}

bool BasicBlock::is_dfs_marker() {
    return _markers.is_dfs_m();
}

void BasicBlock::set_loop_gray_marker(bool value) {
    _markers.set_loop_gray_m(value);
}

bool BasicBlock::is_loop_gray_marker() {
    return _markers.is_loop_gray_m();
}

void BasicBlock::set_loop_black_marker(bool value) {
    _markers.set_loop_black_m(value);
}

bool BasicBlock::is_loop_black_marker() {
    return _markers.is_loop_black_m();
}

BasicBlock::~BasicBlock() {
    Instruction* current = _first_inst;
    while (current) {
        Instruction* next = current->get_next();
        delete current;
        current = next;
    }
}

} // namespace custom

