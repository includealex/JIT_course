#ifndef INCLUDES_INSTRUCTION_HPP_
#define INCLUDES_INSTRUCTION_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "basic_block.hpp"
#include "opcode.hpp"
#include "type.hpp"

namespace custom {

class BasicBlock;

class Instruction {
 public:
  Instruction(Opcode opcode,
              Type type,
              BasicBlock* basicBlock,
              const std::vector<std::size_t>& destRegs = {},
              const std::vector<std::size_t>& srcRegs = {});

  Instruction(Opcode opcode,
              Type type,
              BasicBlock* basicBlock,
              const std::vector<std::size_t>& destRegs = {},
              const std::size_t value = 0);

  virtual ~Instruction() = default;

  Opcode getOpcode() const;
  Type getType() const;
  Instruction* get_next() const;
  Instruction* get_prev() const;
  std::size_t get_lin() const;
  std::size_t get_livenum() const;
  std::size_t get_id() const;
  std::size_t getImmediateValue() const;
  const std::vector<std::size_t>& getDestRegs() const;
  const std::vector<std::size_t>& getSrcRegs() const;

  void set_id(std::size_t id);
  void set_next(Instruction* other);
  void set_prev(Instruction* other);
  void set_basic_block(BasicBlock* other);
  void set_lin(std::size_t num);
  void set_livenum(std::size_t num);

 private:
  std::size_t _instr_id;

  // Intrusive linked list
  Instruction* _prev;
  Instruction* _next;

  // Common part
  Opcode _opcode;
  BasicBlock* _basic_block;
  Type _type;

  // Register information
  std::vector<std::size_t> _destRegs;
  std::vector<std::size_t> _srcRegs;

  // Liveness analysis values
  std::size_t _lin;
  std::size_t _live_num;

  std::size_t _imm;
};

}  // namespace custom

#endif  // INCLUDES_INSTRUCTION_HPP_
