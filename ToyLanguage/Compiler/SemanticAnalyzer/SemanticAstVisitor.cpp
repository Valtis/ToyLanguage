#include "SemanticAstVisitor.h"
#include "SemanticError.h"

#include "../FunctionDefines.h"
#include "../DataStructures/Function.h"
#include "../DataStructures/AstNodes/NodeCreation.h"
#include "../DataStructures/AstNodes/FunctionCallNode.h"


SemanticAstVisitor::SemanticAstVisitor(std::unordered_map<std::string, Function> f) : m_functions(f)
{

  std::unordered_map<std::string, int> i = { { FN_ADD, 2 }, { FN_SUB, 2 }, { FN_MUL, 2 }, { FN_DIV, 2 }, { FN_PRINT, 1 }, { FN_PRINTLN, 1 } };
  // workaround for compiler error C2593 
  m_inbuilt_function_min_child_count = i;
}



void SemanticAstVisitor::visit(RootNode *node)
{

}

void SemanticAstVisitor::visit(NumberNode *node)
{

}

void SemanticAstVisitor::visit(VariableReadNode *node)
{

}

void SemanticAstVisitor::visit(FunctionCallNode *node)
{
  if (m_inbuilt_function_min_child_count.count(node->Name()) != 0)
  {
    CheckInbuiltFunctionChildCount(node);
  }
  else
  {
    CheckUserFunctionParameterCount(node);
  }
}

// transform AST so that arithmetic functions have only 2 children (eg. (- 2 3 4 5) is transformed to (-(-(- 2 3) 4 ) 5).
void SemanticAstVisitor::CheckInbuiltFunctionChildCount(FunctionCallNode *node)
{

  size_t child_limit = m_inbuilt_function_min_child_count[node->Name()];

  if (node->Children().size() < child_limit)
  {
    throw ParameterCountMismatchError("Function " + node->Name() + " requires at least " + std::to_string(child_limit) + " arguments", node->DeclarationLine());
  }

  auto parent = node->Parent();
  auto current_node = parent->GetChildByRawPointer(node);

  while (node->Children().size() > child_limit)
  {

    auto new_node = CreateFunctionCallNode(node->DeclarationLine(), node->Name());
    new_node->AddChild(current_node);


    auto right_child = node->Children()[child_limit];
    node->RemoveChild(right_child);
    new_node->AddChild(right_child);

    current_node = new_node;
  }


  parent->ReplaceChild(parent->GetChildByRawPointer(node), current_node);
}



void SemanticAstVisitor::CheckUserFunctionParameterCount(FunctionCallNode *node)
{
  int expected_count = m_functions[node->Name()].ParameterCount();
  int actual_count = node->Children().size();
  if (expected_count != actual_count)
  {
    throw ParameterCountMismatchError("Invalid parameter count for function '" + node->Name() + "'.  Expected " 
      + std::to_string(expected_count) + " but was " + std::to_string(actual_count), node->DeclarationLine());
  }

}
