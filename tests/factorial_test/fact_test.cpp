#include <gtest/gtest.h>

#include "ir_builder.hpp"

custom::Graph* buildFactorialGraph() {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* entry = builder.createBasicBlock(graph);
  custom::BasicBlock* loop = builder.createBasicBlock(graph);
  custom::BasicBlock* done = builder.createBasicBlock(graph);

  entry->add_succs_true(loop);
  loop->add_succs_false(loop);
  loop->add_succs_true(done);

  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, entry, std::vector<size_t>{custom::VRegs::v0}, 0);
  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, entry, std::vector<size_t>{custom::VRegs::v1}, 1);
  builder.createInstruction(custom::Opcode::CAST,
                            custom::Type::myu64,
                            entry,
                            std::vector<size_t>{custom::VRegs::v2},
                            std::vector<size_t>{});

  builder.createInstruction(custom::Opcode::CMP,
                            custom::Type::myu64,
                            loop,
                            std::vector<size_t>{},
                            std::vector<size_t>{custom::VRegs::v1, custom::VRegs::v2});
  builder.createInstruction(
      custom::Opcode::JMP, custom::Type::myu64, loop, std::vector<size_t>{}, std::vector<size_t>{});
  builder.createInstruction(custom::Opcode::MUL,
                            custom::Type::myu64,
                            loop,
                            std::vector<size_t>{custom::VRegs::v0},
                            std::vector<size_t>{custom::VRegs::v0, custom::VRegs::v1});
  builder.createInstruction(custom::Opcode::ADD,
                            custom::Type::myu64,
                            loop,
                            std::vector<size_t>{custom::VRegs::v1},
                            std::vector<size_t>{custom::VRegs::v1});
  builder.createInstruction(
      custom::Opcode::JMP, custom::Type::myu64, loop, std::vector<size_t>{}, std::vector<size_t>{});

  builder.createInstruction(custom::Opcode::RET,
                            custom::Type::myu64,
                            done,
                            std::vector<size_t>{},
                            std::vector<size_t>{custom::VRegs::v0});

  return graph;
}

TEST(FactorialGraphTest, BasicAssertions) {
  custom::Graph* graph = buildFactorialGraph();

  custom::BasicBlock* entry = graph->get_block(0);
  custom::BasicBlock* loop = graph->get_block(1);
  custom::BasicBlock* done = graph->get_block(2);

  ASSERT_EQ(entry->getInstruction(0)->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(entry->getInstruction(0)->getDestRegs(), std::vector<std::size_t>{0});
  ASSERT_EQ(entry->getInstruction(1)->getOpcode(), custom::Opcode::MOVI);
  ASSERT_EQ(entry->getInstruction(1)->getDestRegs(), std::vector<std::size_t>{1});
  ASSERT_EQ(entry->getInstruction(2)->getOpcode(), custom::Opcode::CAST);
  ASSERT_EQ(entry->getInstruction(2)->getDestRegs(), std::vector<std::size_t>{2});

  ASSERT_EQ(loop->getInstruction(0)->getOpcode(), custom::Opcode::CMP);
  ASSERT_EQ(loop->getInstruction(0)->getSrcRegs(), (std::vector<std::size_t>{1, 2}));
  ASSERT_EQ(loop->getInstruction(1)->getOpcode(), custom::Opcode::JMP);
  ASSERT_EQ(loop->getInstruction(2)->getOpcode(), custom::Opcode::MUL);
  ASSERT_EQ(loop->getInstruction(2)->getDestRegs(), std::vector<std::size_t>{0});
  ASSERT_EQ(loop->getInstruction(2)->getSrcRegs(), (std::vector<std::size_t>{0, 1}));
  ASSERT_EQ(loop->getInstruction(3)->getOpcode(), custom::Opcode::ADD);
  ASSERT_EQ(loop->getInstruction(3)->getDestRegs(), std::vector<std::size_t>{1});
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

  delete graph;
}
