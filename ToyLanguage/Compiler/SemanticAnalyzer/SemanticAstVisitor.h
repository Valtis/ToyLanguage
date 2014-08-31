#pragma once 
#include <unordered_map>
#include <string>
#include "../DataStructures/AstNodes/AstVisitor.h"
class Function;

class SemanticAstVisitor : public AstVisitor
{
public:
  SemanticAstVisitor(std::unordered_map<std::string, Function> functions);
 
  void Visit(FunctionCallNode *node) override;

private:

  void TransformInbuiltFunctionsWithMaxChildrenLimitation(FunctionCallNode *node);
  void CheckInbuiltFunctionParameterCount(FunctionCallNode *node);

  void CheckUserFunctionParameterCount(FunctionCallNode *node);
  void TransformIf(FunctionCallNode * node);
  std::unordered_map<std::string, Function> m_functions;
  

  struct ChildCount
  {
    int min_count;
    int max_count;
  };

  std::unordered_map<std::string, ChildCount> m_inbuilt_function_child_count;
  std::unordered_map<std::string, int> m_inbuilt_function_transform_count;
};