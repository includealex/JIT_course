#include "optimizer.hpp"

#include <set>

namespace custom {

void Optimizer::constant_fold(Graph* graph, IRBuilder* builder) {
  RPO rpo;
  rpo.run_rpo(graph->get_root());
  auto rpo_ids = rpo.get_rpo_ids_arr();

  //   std::vector<BasicBlock*> bb_to_remove;
  for (auto& id : rpo_ids) {
    auto cur_block = graph->get_block(id);
    std::unordered_map<size_t, uint64_t> constant_values;
    std::vector<std::pair<size_t, uint64_t>> new_instructions;
    std::vector<Instruction*> to_remove;

    for (auto* instr = cur_block->get_first_inst(); instr != nullptr; instr = instr->get_next()) {
      if (instr->getOpcode() == Opcode::MOVI || instr->getOpcode() == Opcode::CONST) {
        if (!instr->getDestRegs().empty()) {
          size_t dst = instr->getDestRegs()[0];
          constant_values[dst] = instr->getImmediateValue();
          to_remove.push_back(instr);
        }
      } else if (instr->getOpcode() == Opcode::SUB || instr->getOpcode() == Opcode::ASHR ||
                 instr->getOpcode() == Opcode::XOR) {
        const auto& destRegs = instr->getDestRegs();
        const auto& srcRegs = instr->getSrcRegs();

        if (destRegs.size() == 1 && srcRegs.size() == 2) {
          size_t dst = destRegs[0];
          size_t src1 = srcRegs[0];
          size_t src2 = srcRegs[1];

          if (constant_values.count(src1) && constant_values.count(src2)) {
            uint64_t folded_value = 0;

            if (instr->getOpcode() == Opcode::SUB) {
              folded_value = constant_values[src1] - constant_values[src2];
            } else if (instr->getOpcode() == Opcode::ASHR) {
              folded_value = constant_values[src1] >> constant_values[src2];
            } else if (instr->getOpcode() == Opcode::XOR) {
              folded_value = constant_values[src1] ^ constant_values[src2];
            }

            constant_values[dst] = folded_value;
            to_remove.push_back(instr);
            new_instructions.emplace_back(dst, folded_value);
          }
        }
      }
    }

    for (const auto& [dst, value] : new_instructions) {
      builder->createInstruction(Opcode::MOVI, Type::myu64, cur_block, {dst}, value);
    }

    for (auto& tmp_instr : to_remove) {
      cur_block->remove_instruction(tmp_instr);
    }

    // FIXME: fix logic dedicated to bb_removal.
    //     if (cur_block->instructions_amount() == 1) {
    //       BasicBlock* successor = cur_block->get_succs_true();

    //       if (successor && !cur_block->get_succs_false() && successor->get_preds().size() == 1) {
    //         Instruction* instr_to_move = cur_block->get_first_inst();
    //         bb_to_remove.push_back(cur_block);
    //         if (instr_to_move) {
    //             successor->pushback_instr(instr_to_move);
    //         }
    //         else if (!instr_to_move) {
    //             std::cout << "WARNING! Something happened to first instruction of basic block."
    //             << std::endl;
    //         }
    //         break;
    //       }
    //     }
    //   }

    //   for (auto& tmp_bb: bb_to_remove) {
    //     auto tmp_id = tmp_bb->get_id();

    //     auto block_to_remove = graph->get_block(tmp_id);
    //     if (block_to_remove) {
    //         graph->remove_block(block_to_remove);
    //     }
  }
}

void Optimizer::peephole(Graph* graph, IRBuilder* builder) {
  RPO rpo;
  rpo.run_rpo(graph->get_root());
  auto rpo_ids = rpo.get_rpo_ids_arr();

  for (auto& id : rpo_ids) {
    auto cur_block = graph->get_block(id);
    std::unordered_map<size_t, uint64_t> constant_values;
    std::unordered_map<size_t, uint64_t> ashr_chain;
    std::vector<Instruction*> to_remove;
    std::vector<std::pair<size_t, uint64_t>> new_instructions;

    for (auto* instr = cur_block->get_first_inst(); instr != nullptr; instr = instr->get_next()) {
      auto opcode = instr->getOpcode();
      const auto& destRegs = instr->getDestRegs();
      const auto& srcRegs = instr->getSrcRegs();

      if (opcode == Opcode::MOVI && destRegs.size() == 1) {
        constant_values[destRegs[0]] = instr->getImmediateValue();
      } else if (opcode == Opcode::ASHR && destRegs.size() == 1 && srcRegs.size() == 2) {
        size_t dst = destRegs[0];
        size_t src1 = srcRegs[0];
        size_t src2 = srcRegs[1];

        if (constant_values.count(src2) && constant_values[src2] == 0) {
          // ASHR with 0
          to_remove.push_back(instr);
          constant_values[dst] = constant_values[src1];
        } else if (ashr_chain.count(src1) && constant_values.count(src2)) {
          // Merge ASHR shifts
          uint64_t new_shift = ashr_chain[src1] + constant_values[src2];
          ashr_chain[dst] = new_shift;
          to_remove.push_back(instr);
          new_instructions.emplace_back(dst, new_shift);
        } else if (constant_values.count(src2)) {
          ashr_chain[dst] = constant_values[src2];
        }
      } else if (opcode == Opcode::SUB && destRegs.size() == 1 && srcRegs.size() == 2) {
        size_t dst = destRegs[0];
        size_t src1 = srcRegs[0];
        size_t src2 = srcRegs[1];

        if (constant_values.count(src2) && constant_values[src2] == 0) {
          // SUB with 0
          to_remove.push_back(instr);
          constant_values[dst] = constant_values[src1];
        } else if (src1 == src2) {
          // SUB x, x → MOVI 0
          to_remove.push_back(instr);
          new_instructions.emplace_back(dst, 0);
          constant_values[dst] = 0;
        }
      } else if (opcode == Opcode::XOR && destRegs.size() == 1 && srcRegs.size() == 2) {
        size_t dst = destRegs[0];
        size_t src1 = srcRegs[0];
        size_t src2 = srcRegs[1];

        if (constant_values.count(src2) && constant_values[src2] == 0) {
          // XOR with 0
          to_remove.push_back(instr);
          constant_values[dst] = constant_values[src1];
        } else if (src1 == src2) {
          // XOR x, x → MOVI 0
          to_remove.push_back(instr);
          new_instructions.emplace_back(dst, 0);
          constant_values[dst] = 0;
        }
      }
    }

    for (const auto& [dst, value] : new_instructions) {
      builder->createInstruction(Opcode::MOVI, Type::myu64, cur_block, {dst}, value);
    }

    for (auto& tmp_instr : to_remove) {
      cur_block->remove_instruction(tmp_instr);
    }
  }
}

}  // namespace custom
