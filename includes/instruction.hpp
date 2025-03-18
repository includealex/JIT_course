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

class Instruction final {
 public:
  Instruction(Opcode opcode,
              Type type,
              BasicBlock* basicBlock,
              const std::vector<std::size_t>& destRegs = {},
              const std::vector<std::size_t>& srcRegs = {})
      : _prev(nullptr),
        _next(nullptr),
        _opcode(opcode),
        _basic_block(basicBlock),
        _type(type),
        _destRegs(destRegs),
        _srcRegs(srcRegs) {}

  Instruction(Opcode opcode,
              Type type,
              BasicBlock* basicBlock,
              const std::vector<std::size_t>& destRegs = {},
              const std::size_t value = 0) {
    _prev = nullptr;
    _next = nullptr;
    _opcode = opcode;
    _basic_block = basicBlock;
    _type = type;
    _destRegs = destRegs;
    _val_to_set = value;
  }

  Opcode getOpcode() const {
    return _opcode;
  }

  Type getType() const {
    return _type;
  }

  const std::vector<std::size_t>& getDestRegs() const {
    return _destRegs;
  }

  const std::vector<std::size_t>& getSrcRegs() const {
    return _srcRegs;
  }

  void set_basic_block(BasicBlock* other) {
    _basic_block = other;
  }

  void set_next(Instruction* other) {
    _next = other;
  }

  void set_prev(Instruction* other) {
    _prev = other;
  }

  Instruction* get_next() const {
    return _next;
  }

  Instruction* get_prev() const {
    return _prev;
  }

  void set_id(std::size_t id) {
    _instr_id = id;
  }

  std::size_t get_id() const {
    return _instr_id;
  }

  std::size_t getImmediateValue() const {
    return _val_to_set;
  }

  // Liveness analysis values
  std::size_t lin;
  std::size_t live_num;

 private:
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

  std::size_t _instr_id;
  std::size_t _val_to_set;
};

}  // namespace custom

#endif  // INCLUDES_INSTRUCTION_HPP_
