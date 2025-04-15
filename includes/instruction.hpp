#ifndef INCLUDES_INSTRUCTION_HPP_
#define INCLUDES_INSTRUCTION_HPP_

#include <algorithm>
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

  BasicBlock* getBB() const;
  Opcode getOpcode() const;
  Type getType() const;
  Instruction* get_next() const;
  Instruction* get_prev() const;
  std::size_t get_lin() const;
  std::size_t get_livenum() const;
  std::size_t get_id() const;
  ImmType get_imm() const;
  ImmType get_second_imm() const;

  void setOpcode(Opcode opc);
  void set_imm(ImmType value);
  void set_second_imm(ImmType value);
  void set_id(std::size_t id);
  void set_next(Instruction* other);
  void set_prev(Instruction* other);
  void set_basic_block(BasicBlock* other);
  void set_lin(std::size_t num);
  void set_livenum(std::size_t num);

  void remove_src_instr(Instruction* instr);

  virtual std::vector<Instruction*> get_src_insts();
  void add_src_inst(Instruction* instr);

  virtual void AddPhiUsage(Instruction* first, Instruction* second) {
    std::cerr << "No Phi Usage for this instruction provided" << std::endl;
  };

  void add_user();
  void sub_user();
  std::size_t get_users();
  void add_user_inst(Instruction* instr);
  std::vector<Instruction*> get_users_instrs() const;

  std::string get_called_name() const;
  void set_called_name(std::string name);

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

  ImmType _imm = IMMPOISON;
  ImmType _second_imm = IMMPOISON;
  std::vector<Instruction*> _src_insts = std::vector<Instruction*>{};
  std::size_t _n_users = 0;
  std::string _called_name = "";

  std::vector<Instruction*> _users_instrs;
};

}  // namespace custom

#endif  // INCLUDES_INSTRUCTION_HPP_
