#ifndef INCLUDES_FUNCTION_HPP_
#define INCLUDES_FUNCTION_HPP_

#include "basic_block.hpp"
#include "graph.hpp"
#include "instruction_ext.hpp"

namespace custom {

class Function final {
 public:
  Function(std::string name, Type rettype, std::vector<Instruction*> params);
  ~Function();

  void set_params(std::vector<Instruction*>);

  std::string get_name() const;
  Graph* get_graph() const;
  Type get_rettype() const;
  std::vector<Instruction*> get_params() const;

 private:
  std::string _name;
  Type _rettype;
  std::vector<Instruction*> _params;

  Graph* _graph;
};

}  // namespace custom

#endif  // INCLUDES_FUNCTION_HPP_