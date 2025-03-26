#include "optimizer.hpp"

#include <set>

#include "instruction_ext.hpp"

namespace custom {

void Optimizer::replace_movi_with_constants(BasicBlock* block) {
  for (Instruction* instr = block->get_first_inst(); instr; instr = instr->get_next()) {
    auto src_ins_vec = instr->get_src_insts();

    if (src_ins_vec.size() == 0) {
      continue;
    }

    if (src_ins_vec.size() == 1) {
      auto first_src_instr = src_ins_vec[0];

      if (first_src_instr->getOpcode() != Opcode::MOVI) {
        continue;
      }

      instr->remove_src_instr(first_src_instr);
      auto tmp = instr->get_imm();
      instr->set_imm(first_src_instr->get_imm());
      instr->set_second_imm(tmp);

      first_src_instr->sub_user();
      continue;
    }

    if (src_ins_vec.size() != 2) {
      std::cerr << "Unexpected number of src instructions" << std::endl;
    }

    auto first_src_instr = src_ins_vec[0];
    auto second_src_instr = src_ins_vec[1];

    if (first_src_instr == second_src_instr) {
      if (first_src_instr->getOpcode() != Opcode::MOVI) {
        continue;
      }

      instr->remove_src_instr(first_src_instr);
      instr->set_imm(first_src_instr->get_imm());
      instr->set_second_imm(first_src_instr->get_imm());
      first_src_instr->sub_user();
      continue;
    }

    if (first_src_instr->getOpcode() == Opcode::MOVI) {
      instr->set_imm(first_src_instr->get_imm());
      first_src_instr->sub_user();
      instr->remove_src_instr(first_src_instr);
    }

    if (second_src_instr->getOpcode() == Opcode::MOVI) {
      instr->set_second_imm(second_src_instr->get_imm());
      second_src_instr->sub_user();
      instr->remove_src_instr(second_src_instr);
    }
  }
}

void Optimizer::remove_unused_movi(BasicBlock* block) {
  std::vector<Instruction*> to_remove;
  for (Instruction* instr = block->get_first_inst(); instr; instr = instr->get_next()) {
    if (instr->getOpcode() != Opcode::MOVI) {
      continue;
    }

    if (instr->get_users() == 0) {
      to_remove.push_back(instr);
    }
  }

  for (auto& el : to_remove) {
    if (el == nullptr) {
      continue;
    }

    block->remove_instruction(el);
  }
}

void Optimizer::replace_arith(BasicBlock* block) {
  for (Instruction* instr = block->get_first_inst(); instr; instr = instr->get_next()) {
    if (instr == nullptr) {
      continue;
    }

    auto cur_opc = instr->getOpcode();
    if ((cur_opc != Opcode::SUB) && (cur_opc != Opcode::SUBI) && (cur_opc != Opcode::ASHR) &&
        (cur_opc != Opcode::ASHRI) && (cur_opc != Opcode::XOR) && (cur_opc != Opcode::XORI)) {
      continue;
    }

    auto imm1 = instr->get_imm();
    auto imm2 = instr->get_second_imm();

    if (imm1 == IMMPOISON || imm2 == IMMPOISON) {
      continue;
    }

    ImmType res = IMMPOISON;
    switch (cur_opc) {
      case Opcode::SUB:
        res = imm1 - imm2;
        break;
      case Opcode::SUBI:
        res = imm1 - imm2;
        break;
      case Opcode::XOR:
        res = imm1 ^ imm2;
        break;
      case Opcode::XORI:
        res = imm1 ^ imm2;
        break;
      case Opcode::ASHR:
        res = (imm1 >> imm2) | -((imm1 & 0x80000000) >> imm2);
        ;
        break;
      case Opcode::ASHRI:
        res = (imm1 >> imm2) | -((imm1 & 0x80000000) >> imm2);
        ;
        break;
      default:
        break;
    }

    instr->set_imm(res);
    instr->set_second_imm(IMMPOISON);
    instr->setOpcode(Opcode::MOVI);
  }
}

void Optimizer::constant_fold(Graph* graph, IRBuilder* builder) {
  auto n = graph->basic_blocks_num();
  for (std::size_t i = 0; i < n; ++i) {
    auto* cur_block = graph->get_block(i);

    if (cur_block == nullptr) {
      continue;
    }

    replace_movi_with_constants(cur_block);
    remove_unused_movi(cur_block);
    replace_arith(cur_block);
  }
}

void Optimizer::peephole_xor_with_zero(Instruction* instr) {
  if (instr->getOpcode() != Opcode::XORI) {
    return;
  }

  if (instr->get_second_imm() != 0) {
    return;
  }

  if (instr->get_imm() != IMMPOISON) {
    instr->setOpcode(Opcode::MOVI);
  } else {
    instr->setOpcode(Opcode::MOV);
  }

  instr->set_second_imm(IMMPOISON);

  return;
}

void Optimizer::peephole_xor_with_same(Instruction* instr) {
  if (instr->getOpcode() == Opcode::XORI) {
    auto fimm = instr->get_imm();
    auto simm = instr->get_second_imm();
    if ((fimm == IMMPOISON) || (simm == IMMPOISON)) {
      return;
    }

    if (fimm == simm) {
      instr->setOpcode(Opcode::MOVI);
      instr->set_imm(0);
      instr->set_second_imm(IMMPOISON);
    }
  }

  if (instr->getOpcode() == Opcode::XOR) {
    auto src_insts = instr->get_src_insts();
    auto finst = src_insts[0];
    auto sinst = src_insts[1];

    if (finst == sinst) {
      instr->setOpcode(Opcode::MOVI);
      instr->set_imm(0);
      instr->remove_src_instr(finst);
    }
  }
}

void Optimizer::peephole_sub_with_same(Instruction* instr) {
  if (instr->getOpcode() == Opcode::SUBI) {
    auto fimm = instr->get_imm();
    auto simm = instr->get_second_imm();
    if ((fimm == IMMPOISON) || (simm == IMMPOISON)) {
      return;
    }

    if (fimm == simm) {
      instr->setOpcode(Opcode::MOVI);
      instr->set_imm(0);
      instr->set_second_imm(IMMPOISON);
    }
  }

  if (instr->getOpcode() == Opcode::SUB) {
    auto src_insts = instr->get_src_insts();
    auto finst = src_insts[0];
    auto sinst = src_insts[1];

    if (finst == sinst) {
      instr->setOpcode(Opcode::MOVI);
      instr->set_imm(0);
      instr->remove_src_instr(finst);
    }
  }
}

void Optimizer::peephole_sub_with_zero(Instruction* instr) {
  if (instr->getOpcode() != Opcode::SUBI) {
    return;
  }

  if (instr->get_second_imm() != 0) {
    return;
  }

  if (instr->get_imm() != IMMPOISON) {
    instr->setOpcode(Opcode::MOVI);
  } else {
    instr->setOpcode(Opcode::MOV);
  }

  instr->set_second_imm(IMMPOISON);

  return;
}

void Optimizer::peephole_ashr_with_zero(Instruction* instr) {
  if (instr->getOpcode() != Opcode::ASHRI) {
    return;
  }

  if (instr->get_second_imm() != 0) {
    return;
  }

  if (instr->get_imm() != IMMPOISON) {
    instr->setOpcode(Opcode::MOVI);
  } else {
    instr->setOpcode(Opcode::MOV);
  }

  instr->set_second_imm(IMMPOISON);

  return;
}

void Optimizer::peephole_ashr_with_big(Instruction* instr) {
  if (instr->getOpcode() != Opcode::ASHRI) {
    return;
  }

  if (instr->get_second_imm() > 64) {
    instr->set_second_imm(0);
    peephole_ashr_with_zero(instr);
  }
}

void Optimizer::peephole(Graph* graph, IRBuilder* builder) {
  auto n = graph->basic_blocks_num();
  for (std::size_t i = 0; i < n; ++i) {
    auto* cur_block = graph->get_block(i);

    if (cur_block == nullptr) {
      continue;
    }

    for (Instruction* cur_instr = cur_block->get_first_inst(); cur_instr;
         cur_instr = cur_instr->get_next()) {
      if (cur_instr == nullptr) {
        continue;
      }

      peephole_xor_with_zero(cur_instr);
      peephole_xor_with_same(cur_instr);
      peephole_sub_with_zero(cur_instr);
      peephole_sub_with_same(cur_instr);
      peephole_ashr_with_zero(cur_instr);
      peephole_ashr_with_big(cur_instr);
    }
  }
}

void Optimizer::apply_inline(Graph* graph, IRBuilder* builder) {
  for (auto& single_block : graph->get_blocks()) {
    // FIXME: recursive calls?
    // Now no recursion is possible
    for (auto& call_instr : single_block->find_calls()) {
      if (call_instr != nullptr) {
        auto* cur_graph = call_instr->getBB()->get_graph();
        auto foo_name = call_instr->get_called_name();
        auto* callee_func = builder->getFunction(foo_name);
        if (!callee_func->is_inlinable()) {
          continue;
        }

        auto callee_graph = callee_func->get_graph();

        auto* splitted = cur_graph->split_BasicBlock(call_instr, single_block);
        single_block->add_succs_true(callee_graph->get_root());

        for (auto& tmp_block : callee_graph->get_blocks()) {
          cur_graph->addBasicBlock(tmp_block);
        }

        if (callee_func->get_params().size() != 0) {
          if (call_instr->get_src_insts().size() == 2) {
            callee_func->set_params(call_instr->get_src_insts());
          }
        }

        for (auto& tmp_block : callee_graph->get_ret_blocks()) {
          tmp_block->add_succs_true(splitted);
        }

        graph->addBasicBlock(splitted);
      }
    }
  }
}

}  // namespace custom
