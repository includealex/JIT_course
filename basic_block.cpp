#include "basic_block.hpp"
#include "instruction.hpp"
#include "graph.hpp"

namespace custom {

void BasicBlock::pushback_instr(Instruction* inst) {
    inst->set_basic_block(this);

    if (_last_inst) {
        _last_inst->set_next(inst);
        inst->set_prev(_last_inst);
        _last_inst = inst;
    } else {
        _first_inst = inst;
        _last_inst = inst;
        inst->set_prev(nullptr);
    }

    inst->set_next(nullptr);


    // TODO: something weird here.
    if (isPhiInstruction(inst)) {
        if (!_first_Phi) {
            _first_Phi = inst;
        }
    }

    _instructions.push_back(inst);
}

size_t BasicBlock::getInstructionCount() const {
    return _instructions.size();
}

Instruction* BasicBlock::getInstruction(size_t index) const {
    if (index < _instructions.size()) {
        return _instructions[index];
    }
    return nullptr;
}

bool BasicBlock::isPhiInstruction(Instruction* inst) {
    return inst->getOpcode() == Opcode::PHI;
}

std::vector<BasicBlock*> BasicBlock::get_preds() {
    return _preds;
}

std::vector<BasicBlock*> BasicBlock::get_succs() {
    return _succs;
}

BasicBlock* BasicBlock::get_succs(std::size_t idx) {
    return _succs[idx];
}

std::vector<Instruction*> BasicBlock::get_instructions() {
    return _instructions;
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

void BasicBlock::push_succs_back(BasicBlock* el) {
    _succs.push_back(el);
}

} // namespace custom

