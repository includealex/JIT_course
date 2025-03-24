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

void Optimizer::peephole(Graph* graph, IRBuilder* builder) {
  return;
}

}  // namespace custom
