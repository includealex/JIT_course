#include <gtest/gtest.h>

#include <iostream>

#include "basic_block.hpp"
#include "ir_builder.hpp"
#include "optimizer.hpp"

namespace custom {

TEST(OptimizerTest, Inling_call_split) {
  custom::IRBuilder builder;

  std::string foo_name = "foo";
  std::string bar_name = "bar";
  auto* foo = builder.createFunction(foo_name, Type::myu64, {});
  auto* bar = builder.createFunction(bar_name, Type::myu64, {});

  auto* foo_graph = foo->get_graph();
  custom::BasicBlock* A = builder.createBasicBlock(foo_graph);
  auto* v0 = builder.createMOVI(Type::myu64, A, 0);
  auto* v1 = builder.createCALL(Type::myu64, A, bar_name);
  auto* v2 = builder.createSUBI(Type::myu64, A, 3, 2);

  ASSERT_EQ(foo_graph->basic_blocks_num(), 1);
  custom::Optimizer optimizer;
  optimizer.apply_inline(foo_graph, &builder);
  ASSERT_EQ(A->get_last_inst()->getOpcode(), Opcode::CALL);
  ASSERT_EQ(foo_graph->basic_blocks_num(), 2);

  delete foo;
  delete bar;
}

TEST(OptimizerTest, InliningTailCall) {
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

  // auto* v3 = builder->createADD(Type::myu64, A, v0, v2);

  custom::Optimizer optimizer;
  optimizer.apply_inline(foo_graph, &builder);

  delete foo;
  delete bar;
}

}  // namespace custom
