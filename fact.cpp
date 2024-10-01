#include <cassert>
#include <iostream>

#include "ir_builder.hpp"

custom::Graph* buildFactorialGraph() {
    custom::Graph* graph = custom::IRBuilder::createGraph();

    custom::BasicBlock* entry = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* loop = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* done = custom::IRBuilder::createBasicBlock(graph);

    entry->push_succs_back(loop);
    loop->push_preds_back(entry);
    loop->push_succs_back(loop);
    loop->push_succs_back(done);
    done->push_preds_back(loop);

    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::u64, entry);  // movi.u64 v0, 1
    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::u64, entry);  // movi.u64 v1, 2
    custom::IRBuilder::createInstruction(custom::Opcode::CAST, custom::Type::u64, entry);  // u32tou64 v2, a0

    custom::IRBuilder::createInstruction(custom::Opcode::CMP, custom::Type::u64, loop);   // cmp.u64 v1, v2
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::u64, loop);   // ja done
    custom::IRBuilder::createInstruction(custom::Opcode::MUL, custom::Type::u64, loop);   // mul.u64 v0, v0, v1
    custom::IRBuilder::createInstruction(custom::Opcode::ADD, custom::Type::u64, loop);   // addi.u64 v1, v1, 1
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::u64, loop);   // jmp loop

    custom::IRBuilder::createInstruction(custom::Opcode::RET, custom::Type::u64, done);   // ret.u64 v0

    return graph;
}

void testFactorialGraph() {
    custom::Graph* graph = buildFactorialGraph();

    custom::BasicBlock* entry = graph->get_block(0);
    custom::BasicBlock* loop = graph->get_block(1);
    custom::BasicBlock* done = graph->get_block(2);

    assert(entry->getInstructionCount() == 3);
    assert(entry->getInstruction(0)->getOpcode() == custom::Opcode::MOV);
    assert(entry->getInstruction(1)->getOpcode() == custom::Opcode::MOV);
    assert(entry->getInstruction(2)->getOpcode() == custom::Opcode::CAST);

    assert(loop->getInstructionCount() == 5);
    assert(loop->getInstruction(0)->getOpcode() == custom::Opcode::CMP);
    assert(loop->getInstruction(1)->getOpcode() == custom::Opcode::JMP);
    assert(loop->getInstruction(2)->getOpcode() == custom::Opcode::MUL);
    assert(loop->getInstruction(3)->getOpcode() == custom::Opcode::ADD);
    assert(loop->getInstruction(4)->getOpcode() == custom::Opcode::JMP);

    assert(done->getInstructionCount() == 1);
    assert(done->getInstruction(0)->getOpcode() == custom::Opcode::RET);

    assert(entry->get_succs(0) == loop);
    assert(loop->get_succs(0) == loop);
    assert(loop->get_succs(1) == done);

    // Check data flow (SSA form would require further analysis, but we'll skip that here)
    
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testFactorialGraph();

    return 0;
}


