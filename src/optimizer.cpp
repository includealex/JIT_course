#include "optimizer.hpp"

namespace custom {

void Optimizer::constant_fold(Graph* graph) {
  RPO rpo;
  rpo.run_rpo(graph->get_root());
  auto rpo_ids = rpo.get_rpo_ids_arr();

  for (auto& id : rpo_ids) {
    auto block = graph->get_block(id);
    std::unordered_map<size_t, uint64_t> constant_values;
    std::vector<std::pair<size_t, uint64_t>> new_instructions;
    std::vector<Instruction*> to_remove;

    for (auto* instr = block->get_first_inst(); instr != nullptr; instr = instr->get_next()) {
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
      IRBuilder::createInstruction(Opcode::MOVI, Type::myu64, block, {dst}, value);
    }

    for (auto* instr : to_remove) {
      block->remove_instruction(instr);
    }
  }
}

}  // namespace custom
