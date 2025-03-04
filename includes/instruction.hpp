#ifndef INCLUDES_INSTRUCTION_HPP_
#define INCLUDES_INSTRUCTION_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "basic_block.hpp"

namespace custom {

class BasicBlock;

enum class Opcode {
  ADD = 0,
  MUL = 1,
  JMP = 2,
  RET = 3,
  CONST = 4,
  MOV = 5,
  MOVI = 6,
  CAST = 7,
  CMP = 8,
  NEQ = 9,
  SUB = 10,
  PHI = 11,
  ASHR = 12,
  XOR = 13
};

enum class Type {
  myu32 = 0,
  myu64 = 1,
  myvoid = 2,
  myfloat = 3,
  mydouble = 4,
  myint32 = 5,
  myint64 = 6,
  mybool = 7,
};

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

  std::size_t get_id() {
    return _instr_id;
  }

  std::size_t getImmediateValue() {
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
