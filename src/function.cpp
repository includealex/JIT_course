#include "function.hpp"

namespace custom {

Function::Function(std::string name, Type rettype, std::vector<Instruction*> params)
    : _name(name), _rettype(rettype), _params(params) {
  _graph = new Graph;
}

Function::~Function() {
  delete _graph;
}

std::string Function::get_name() const {
  return _name;
}

Graph* Function::get_graph() const {
  return _graph;
}

Type Function::get_rettype() const {
  return _rettype;
}

std::vector<Instruction*> Function::get_params() const {
  return _params;
}

void Function::set_params(std::vector<Instruction*> new_params) {
  _params = new_params;
}

bool Function::is_inlinable() const {
  return _inlinable;
}
void Function::set_noinline() {
  _inlinable = false;
}

}  // namespace custom