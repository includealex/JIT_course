#ifndef INCLUDES_OPCODE_HPP_
#define INCLUDES_OPCODE_HPP_

namespace custom {

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
  XOR = 13,
  MULI = 14,
  SUBI = 15,
  ADDI = 16,
  ASHRI = 17,
  XORI = 18,
  RETI = 19,
  JA = 20,
  PARAM = 21
};

}  // namespace custom

#endif  // INCLUDES_OPCODE_HPP_