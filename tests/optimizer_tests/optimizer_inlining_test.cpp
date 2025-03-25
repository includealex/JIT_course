#include <gtest/gtest.h>

#include <iostream>

#include "basic_block.hpp"
#include "ir_builder.hpp"
#include "optimizer.hpp"

namespace custom {

TEST(OptimizerTest, InliningGetterTailCall) {
  custom::IRBuilder builder;

  auto* foo = builder.createFunction("foo", Type::myu64, {});
  auto* bar = builder.createFunction("bar", Type::myu64, {});

  auto* foo_graph = foo->get_graph();
  custom::BasicBlock* A = builder.createBasicBlock(foo_graph);
  auto* v0 = builder.createMOVI(Type::myu64, A, 0);
  auto* v1 = builder.createCALL(Type::myu64, A, "bar");

  auto* bar_graph = bar->get_graph();
  custom::BasicBlock* B = builder.createBasicBlock(bar_graph);
  auto* v2 = builder.createRETI(Type::myu64, B, 42);
  auto* v3 = builder.createSUBI(Type::myu64, A, 18, 9);

  custom::Optimizer optimizer;
  optimizer.apply_inline(foo_graph, &builder);

  ASSERT_EQ(foo_graph->basic_blocks_num(), 3);
}

TEST(OptimizerTest, InliningGetterTailCallTwoImms) {
  custom::IRBuilder builder;

  auto* a0 = builder.createPARAM(Type::myu32);
  auto* a1 = builder.createPARAM(Type::myu32);

  auto* foo = builder.createFunction("foo", Type::myu64, {});
  auto* bar = builder.createFunction("bar", Type::myu64, {a0, a1});

  auto* foo_graph = foo->get_graph();
  custom::BasicBlock* A = builder.createBasicBlock(foo_graph);
  auto* v0 = builder.createMOVI(Type::myu64, A, 0);
  auto* v1 = builder.createMOVI(Type::myu64, A, 17);
  auto* v2 = builder.createCALL(Type::myu64, A, "bar", v0, v1);

  auto* bar_graph = bar->get_graph();
  custom::BasicBlock* B = builder.createBasicBlock(bar_graph);
  auto* v3 = builder.createRETI(Type::myu64, B, 42);
  auto* v4 = builder.createSUBI(Type::myu64, A, 18, 9);

  custom::Optimizer optimizer;
  optimizer.apply_inline(foo_graph, &builder);

  ASSERT_EQ(foo_graph->basic_blocks_num(), 3);
}

}  // namespace custom
