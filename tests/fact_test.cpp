#include <gtest/gtest.h>
#include "../includes/ir_builder.hpp"

custom::Graph* buildFactorialGraph() {
    custom::Graph* graph = custom::IRBuilder::createGraph();
    custom::BasicBlock* entry = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* loop = custom::IRBuilder::createBasicBlock(graph);
    custom::BasicBlock* done = custom::IRBuilder::createBasicBlock(graph);

    entry->add_succs_true(loop);
    loop->push_preds_back(entry);
    loop->add_succs_false(loop);
    loop->add_succs_true(done);
    done->push_preds_back(loop);

    const std::size_t v0 = 0;
    const std::size_t v1 = 1;
    const std::size_t v2 = 2;

    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::myu64, entry, v0, {});
    custom::IRBuilder::createInstruction(custom::Opcode::MOV, custom::Type::myu64, entry, v1, {});
    custom::IRBuilder::createInstruction(custom::Opcode::CAST, custom::Type::myu64, entry, v2, {0});

    custom::IRBuilder::createInstruction(custom::Opcode::CMP, custom::Type::myu64, loop, -1, {v1, v2});
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::myu64, loop, -1, {});
    custom::IRBuilder::createInstruction(custom::Opcode::MUL, custom::Type::myu64, loop, v0, {v0, v1});
    custom::IRBuilder::createInstruction(custom::Opcode::ADD, custom::Type::myu64, loop, v1, {v1});
    custom::IRBuilder::createInstruction(custom::Opcode::JMP, custom::Type::myu64, loop, -1, {});

    custom::IRBuilder::createInstruction(custom::Opcode::RET, custom::Type::myu64, done, -1, {v0});

    return graph;
}

TEST(FactorialGraphTest, BasicAssertions) {
    custom::Graph* graph = buildFactorialGraph();

    custom::BasicBlock* entry = graph->get_block(0);
    custom::BasicBlock* loop = graph->get_block(1);
    custom::BasicBlock* done = graph->get_block(2);

    ASSERT_EQ(entry->getInstruction(0)->getOpcode(), custom::Opcode::MOV);
    ASSERT_EQ(entry->getInstruction(0)->getDestReg(), 0);
    ASSERT_EQ(entry->getInstruction(1)->getOpcode(), custom::Opcode::MOV);
    ASSERT_EQ(entry->getInstruction(1)->getDestReg(), 1);
    ASSERT_EQ(entry->getInstruction(2)->getOpcode(), custom::Opcode::CAST);
    ASSERT_EQ(entry->getInstruction(2)->getDestReg(), 2);

    ASSERT_EQ(loop->getInstruction(0)->getOpcode(), custom::Opcode::CMP);
    ASSERT_EQ(loop->getInstruction(0)->getSrcRegs(), (std::vector<std::size_t>{1, 2}));
    ASSERT_EQ(loop->getInstruction(1)->getOpcode(), custom::Opcode::JMP);
    ASSERT_EQ(loop->getInstruction(2)->getOpcode(), custom::Opcode::MUL);
    ASSERT_EQ(loop->getInstruction(2)->getDestReg(), 0);
    ASSERT_EQ(loop->getInstruction(2)->getSrcRegs(), (std::vector<std::size_t>{0, 1}));
    ASSERT_EQ(loop->getInstruction(3)->getOpcode(), custom::Opcode::ADD);
    ASSERT_EQ(loop->getInstruction(3)->getDestReg(), 1);
    ASSERT_EQ(loop->getInstruction(3)->getSrcRegs(), std::vector<std::size_t>{1});
    ASSERT_EQ(loop->getInstruction(4)->getOpcode(), custom::Opcode::JMP);

    ASSERT_EQ(done->getInstruction(0)->getOpcode(), custom::Opcode::RET);
    ASSERT_EQ(done->getInstruction(0)->getSrcRegs(), std::vector<std::size_t>{0});

    ASSERT_EQ(entry->get_succs_true(), loop);
    ASSERT_EQ(loop->get_succs_false(), loop);
    ASSERT_EQ(loop->get_succs_true(), done);

    ASSERT_EQ(entry->get_id(), 0);
    ASSERT_EQ(loop->get_id(), 1);
    ASSERT_EQ(done->get_id(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
