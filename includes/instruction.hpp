#ifndef INCLUDES_INSTRUCTION_HPP_
#define INCLUDES_INSTRUCTION_HPP_

#include <cstddef>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "opcode.hpp"
#include "type.hpp"

namespace custom {

class BasicBlock;

class Instruction {
 public:
  Instruction() = default;
  Instruction(Opcode Opcode,
              Type type,
              BasicBlock* basicBlock,
              Instruction* next_instr = nullptr,
              Instruction* prev_instr = nullptr);

  virtual ~Instruction() = default;

  Opcode getOpcode() const;
  Type getType() const;
  Instruction* get_next() const;
  Instruction* get_prev() const;
  std::size_t get_lin() const;
  std::size_t get_livenum() const;
  std::size_t get_id() const;
  ImmType get_imm() const;

  void set_imm(ImmType value);
  void set_id(std::size_t id);
  void set_next(Instruction* other);
  void set_prev(Instruction* other);
  void set_basic_block(BasicBlock* other);
  void set_lin(std::size_t num);
  void set_livenum(std::size_t num);

  virtual std::set<Instruction*> get_src_insts() {
    return _src_insts;
  }
  virtual void AddPhiUsage(Instruction* first, Instruction* second) {
    std::cerr << "No Phi Usage for this instruction provided" << std::endl;
  };

 protected:
  std::size_t _instr_id;

  // Intrusive linked list
  Instruction* _prev;
  Instruction* _next;

  // Common part
  Opcode _opcode;
  BasicBlock* _basic_block;
  Type _type;

  // Liveness analysis values
  std::size_t _lin;
  std::size_t _live_num;

  ImmType _imm;
  std::set<Instruction*> _src_insts = std::set<Instruction*>{};
};

}  // namespace custom

#endif  // INCLUDES_INSTRUCTION_HPP_
