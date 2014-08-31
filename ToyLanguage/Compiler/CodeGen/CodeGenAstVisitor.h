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

  void Visit(RootNode *node) override;
  void Visit(NumberNode *node) override;
  void Visit(VariableReadNode *node) override;
  void Visit(FunctionCallNode *node) override;
  void Visit(IfBooleanNode *node) override;
  void Visit(IfThenNode *node) override;


private:

  void GenerateJumpsForIf(FunctionCallNode *node);

  // maps inbuilt functions to vm instructions
  std::unordered_map<std::string, Instruction> m_inbuilt_functions;
  


  // maps function names to the ids that are actually used by the vm
  std::unordered_map<std::string, int> m_function_names_to_ids;

  VMFunction *m_function;
};
