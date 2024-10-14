#include <cassert>
#include <iostream>

#include "ir_builder.hpp"

custom::Graph* buildFactorialGraph() {
    custom::Graph* graph = custom::IRBuilder::createGraph();

    custom::BasicBlock* entry = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* loop = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* done = custom::IRBuilder::createBasicBlock(graph);

    entry->push_succs_back(loop);  // entry -> loop
    loop->push_preds_back(entry);
    loop->push_succs_back(loop);   // loop -> loop
    loop->push_succs_back(done);   // loop -> done
    done->push_preds_back(loop);

    const std::size_t v0 = 0;
    const std::size_t v1 = 1;
    const std::size_t v2 = 2;

    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::myu64, entry, v0, {});         // movi.myu64 v0, 1
    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::myu64, entry, v1, {});         // movi.myu64 v1, 2
    custom::IRBuilder::createInstruction(custom::Opcode::CAST, custom::Type::myu64, entry, v2, {0});       // myu32tomyu64 v2, a0

    // Instructions in 'loop' block
    custom::IRBuilder::createInstruction(custom::Opcode::CMP, custom::Type::myu64, loop, -1, {v1, v2});    // cmp.myu64 v1, v2
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::myu64, loop, -1, {});          // ja done
    custom::IRBuilder::createInstruction(custom::Opcode::MUL, custom::Type::myu64, loop, v0, {v0, v1});    // mul.myu64 v0, v0, v1
    custom::IRBuilder::createInstruction(custom::Opcode::ADD, custom::Type::myu64, loop, v1, {v1});        // addi.myu64 v1, v1, 1
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::myu64, loop, -1, {});          // jmp loop

    // Instructions in 'done' block
    custom::IRBuilder::createInstruction(custom::Opcode::RET, custom::Type::myu64, done, -1, {v0});        // ret.myu64 v0

    return graph;
}

void testFactorialGraph() {
    custom::Graph* graph = buildFactorialGraph();

    custom::BasicBlock* entry = graph->get_block(0);
    custom::BasicBlock* loop = graph->get_block(1);
    custom::BasicBlock* done = graph->get_block(2);

    assert(entry->getInstructionCount() == 3);
    assert(entry->getInstruction(0)->getOpcode() == custom::Opcode::MOV);
    assert(entry->getInstruction(0)->getDestReg() == 0);  // v0
    assert(entry->getInstruction(1)->getOpcode() == custom::Opcode::MOV);
    assert(entry->getInstruction(1)->getDestReg() == 1);  // v1
    assert(entry->getInstruction(2)->getOpcode() == custom::Opcode::CAST);
    assert(entry->getInstruction(2)->getDestReg() == 2);  // v2

    assert(loop->getInstructionCount() == 5);
    assert(loop->getInstruction(0)->getOpcode() == custom::Opcode::CMP);
    assert(loop->getInstruction(0)->getSrcRegs() == (std::vector<std::size_t>{1, 2})); // v1, v2
    assert(loop->getInstruction(1)->getOpcode() == custom::Opcode::JMP);
    assert(loop->getInstruction(2)->getOpcode() == custom::Opcode::MUL);
    assert(loop->getInstruction(2)->getDestReg() == 0);  // v0
    assert(loop->getInstruction(2)->getSrcRegs() == (std::vector<std::size_t>{0, 1})); // v0, v1
    assert(loop->getInstruction(3)->getOpcode() == custom::Opcode::ADD);
    assert(loop->getInstruction(3)->getDestReg() == 1);  // v1
    assert(loop->getInstruction(3)->getSrcRegs() == std::vector<std::size_t>{1});    // v1
    assert(loop->getInstruction(4)->getOpcode() == custom::Opcode::JMP);

    assert(done->getInstructionCount() == 1);
    assert(done->getInstruction(0)->getOpcode() == custom::Opcode::RET);
    assert(done->getInstruction(0)->getSrcRegs() == std::vector<std::size_t>{0});    // v0

    // Test control flow between basic blocks
    assert(entry->get_succs(0) == loop);
    assert(loop->get_succs(0) == loop); // loop to loop
    assert(loop->get_succs(1) == done); // loop to done

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testFactorialGraph();

    return 0;
}
