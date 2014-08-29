#pragma once 
#include <unordered_map>
#include <string>
#include "../DataStructures/AstNodes/AstVisitor.h"
class Function;

class SemanticAstVisitor : public AstVisitor
{
public:
  SemanticAstVisitor(std::unordered_map<std::string, Function> functions);

  void visit(RootNode *node) override;
  void visit(NumberNode *node) override;
  void visit(VariableReadNode *node) override;
  void visit(FunctionCallNode *node) override;

private:

  void CheckInbuiltFunctionChildCount(FunctionCallNode *node);
  void CheckUserFunctionParameterCount(FunctionCallNode *node);

  std::unordered_map<std::string, Function> m_functions;
  std::unordered_map<std::string, int> m_inbuilt_function_min_child_count;
};