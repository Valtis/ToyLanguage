#pragma once
#include <unordered_map>
#include <string>


#include "../DataStructures/AstNodes/AstVisitor.h"
#include "../../VM/ByteCode.h"

class VMFunction;
class CodeGenAstVisitor : public AstVisitor
{
public:
  CodeGenAstVisitor(VMFunction *f, std::unordered_map<std::string, int> function_names_to_ids);

  void visit(RootNode *node) override;
  void visit(NumberNode *node) override;
  void visit(VariableReadNode *node) override;
  void visit(FunctionCallNode *node) override;

private:

  // maps inbuilt functions to vm instructions
  std::unordered_map<std::string, Instruction> m_inbuilt_functions;
  
  // maps function names to the ids that are actually used by the vm
  std::unordered_map<std::string, int> m_function_names_to_ids;

  VMFunction *m_function;
};
