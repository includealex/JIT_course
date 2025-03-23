#include "optimizer.hpp"

#include <gtest/gtest.h>

#include "basic_block.hpp"
#include "ir_builder.hpp"

namespace custom {

TEST(OptimizerTest, ConstantFolding) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 1);
  auto* v1 = builder.createMOVI(Type::myu64, A, 2);
  auto* v3 = builder.createSUB(Type::myu64, A, v0, v1);

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph, &builder);

  // TODO: add assertions

  delete graph;
}

TEST(OptimizerTest, ConstantFoldingSubgraph) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);

  A->add_succs_true(B);

  auto* v0 = builder.createMOVI(Type::myu64, A, 1);
  auto* v1 = builder.createMOVI(Type::myu64, A, 2);
  auto* v2 = builder.createSUB(Type::myu64, A, v1, v0);

  auto* v3 = builder.createXOR(Type::myu64, B, v2, v1);

  custom::Optimizer optimizer;

  optimizer.constant_fold(graph, &builder);

  // TODO: add assertions

  delete graph;
}

TEST(OptimizerTest, PeepholeOptimization) {
  custom::IRBuilder builder;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);

  auto* v0 = builder.createMOVI(Type::myu64, A, 8);
  auto* v1 = builder.createMOVI(Type::myu64, A, 0);
  auto* v2 = builder.createASHR(Type::myu64, A, v0, v1);
  auto* v3 = builder.createMOVI(Type::myu64, A, 5);

  auto* v4 = builder.createSUB(Type::myu64, A, v3, v1);
  auto* v5 = builder.createSUB(Type::myu64, A, v3, v3);
  auto* v6 = builder.createXOR(Type::myu64, A, v3, v1);
  auto* v7 = builder.createXOR(Type::myu64, A, v3, v3);

  custom::Optimizer optimizer;
  optimizer.peephole(graph, &builder);

  // TODO: add assertions

  delete graph;
}

}  // namespace custom
