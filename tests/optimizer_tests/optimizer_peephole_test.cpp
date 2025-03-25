#include <gtest/gtest.h>

#include <iostream>

#include "basic_block.hpp"
#include "ir_builder.hpp"
#include "optimizer.hpp"

namespace custom {

TEST(OptimizerTest, PeepholeWithZero) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createXORI(Type::myu64, A, 1, 0);
  auto* v1 = builder.createASHRI(Type::myu64, A, 2, 0);
  auto* v2 = builder.createSUBI(Type::myu64, A, 3, 0);

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  ASSERT_EQ(v0->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v1->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v2->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v0->get_imm(), 1);
  ASSERT_EQ(v1->get_imm(), 2);
  ASSERT_EQ(v2->get_imm(), 3);

  delete graph;
}

TEST(OptimizerTest, PeepholeSimilars) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createXORI(Type::myu64, A, 42, 42);
  auto* v1 = builder.createSUBI(Type::myu64, A, 42, 42);

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  ASSERT_EQ(v0->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v0->get_imm(), 0);
  ASSERT_EQ(v1->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v1->get_imm(), 0);

  delete graph;
}

TEST(OptimizerTest, PeepholeAshrWithBigNums) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createASHRI(Type::myu64, A, 1, 65);

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  ASSERT_EQ(v0->getOpcode(), Opcode::MOVI);
  ASSERT_EQ(v0->get_imm(), 1);

  delete graph;
}

}  // namespace custom
