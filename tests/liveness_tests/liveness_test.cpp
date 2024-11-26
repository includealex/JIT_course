#include "liveness.hpp"

#include <gtest/gtest.h>

#include "ir_builder.hpp"

namespace custom {

TEST(LivenessTest, FirstExample) {
  custom::IRBuilder builder;
  custom::Liveness liveness;

  custom::Graph* graph = builder.createGraph();
  custom::BasicBlock* A = builder.createBasicBlock(graph);
  custom::BasicBlock* B = builder.createBasicBlock(graph);
  custom::BasicBlock* C = builder.createBasicBlock(graph);
  custom::BasicBlock* D = builder.createBasicBlock(graph);
  custom::BasicBlock* E = builder.createBasicBlock(graph);

  A->add_succs_true(B);
  B->add_succs_true(C);
  B->add_succs_false(D);
  C->add_succs_true(B);
  D->add_succs_true(E);

  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v0}, 0);
  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v1}, 1);
  builder.createInstruction(
      custom::Opcode::MOVI, custom::Type::myu64, A, std::vector<size_t>{custom::VRegs::v2}, 2);

  builder.createInstruction(custom::Opcode::PHI,
                            custom::Type::myu64,
                            B,
                            std::vector<size_t>{custom::VRegs::v3},
                            std::vector<size_t>{custom::VRegs::v7, custom::VRegs::v0});
  builder.createInstruction(custom::Opcode::PHI,
                            custom::Type::myu64,
                            B,
                            std::vector<size_t>{custom::VRegs::v4},
                            std::vector<size_t>{custom::VRegs::v8, custom::VRegs::v1});
  builder.createInstruction(custom::Opcode::CMP,
                            custom::Type::mybool,
                            B,
                            std::vector<size_t>{custom::VRegs::v5},
                            std::vector<size_t>{custom::VRegs::v4, custom::VRegs::v0});
  builder.createInstruction(custom::Opcode::NEZ,
                            custom::Type::mybool,
                            B,
                            std::vector<size_t>{custom::VRegs::v6},
                            std::vector<size_t>{custom::VRegs::v5});

  builder.createInstruction(custom::Opcode::MUL,
                            custom::Type::myu64,
                            C,
                            std::vector<size_t>{custom::VRegs::v7},
                            std::vector<size_t>{custom::VRegs::v3, custom::VRegs::v4});
  builder.createInstruction(custom::Opcode::SUB,
                            custom::Type::myu64,
                            C,
                            std::vector<size_t>{custom::VRegs::v8},
                            std::vector<size_t>{custom::VRegs::v4, custom::VRegs::v0});

  builder.createInstruction(custom::Opcode::ADD,
                            custom::Type::myu64,
                            D,
                            std::vector<size_t>{custom::VRegs::v9},
                            std::vector<size_t>{custom::VRegs::v2, custom::VRegs::v3});
  builder.createInstruction(custom::Opcode::RET,
                            custom::Type::myvoid,
                            D,
                            std::vector<size_t>{custom::VRegs::v10},
                            std::vector<size_t>{});

  liveness.set_live_ints(graph);

  delete graph;
}

}  // namespace custom
