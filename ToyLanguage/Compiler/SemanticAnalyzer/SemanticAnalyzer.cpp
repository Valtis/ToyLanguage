#include "SemanticAnalyzer.h"
#include <functional>
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"
#include "SemanticError.h"
#include "../../VM/VMFunction.h"
#define BIND(NAME__) std::bind(&SemanticAnalyzer::NAME__, this, std::placeholders::_1)

SemanticAnalyzer::SemanticAnalyzer(std::unordered_map<std::string, Function> functions) : m_functions(functions)
{
  // workaround for compiler error C2593 (thanks Microsoft)
  std::unordered_map<std::string, int> inbuilt_functions = { { FN_ADD, 2 }, { FN_SUB, 2 }, { FN_MUL, 2 }, { FN_DIV, 2 }, { FN_PRINT, 1 }, { FN_PRINTLN, 1 } };
  m_inbuilt_functions = inbuilt_functions;
}

std::unordered_map<std::string, Function> SemanticAnalyzer::Analyze()
{

  GenerateUserFunctionIds();

  for (auto &pair : m_functions)
  {
    Function f = pair.second;
    TraverseAst(f.RootNode(), BIND(TransformInbuiltFunctions));
    TraverseAst(f.RootNode(), BIND(CheckUserDefinedFunctionParameterCount));
    
  }
  return m_functions;
}


// transform AST so that arithmetic functions have only 2 children (eg. (- 2 3 4 5) is transformed to (-(-(- 2 3) 4 ) 5).
void SemanticAnalyzer::TransformInbuiltFunctions(Ast_Node &node)
{

  // name - maximum number of children in the leaf node

  if (node->Type() != NodeType::FUNCTION_CALL)
  {
    return;
  }

  if (m_inbuilt_functions.count(node->ValueAsText()) != 0)
  {
    size_t child_limit = m_inbuilt_functions[node->ValueAsText()];
    
    if (node->Children().size() < child_limit)
    {
      throw ParameterCountMismatchError("Function " + node->ValueAsText() + " requires at least " + std::to_string(child_limit) + " arguments", node->DeclarationLine());
    }

    auto parent = node->Parent();

    Ast_Node current_node = node;

    while (node->Children().size() > child_limit)
    {

      auto new_node = std::make_shared<AstNode>(NodeType::FUNCTION_CALL, node->DeclarationLine());
      new_node->ValueAsText(node->ValueAsText());
      new_node->AddChild(current_node);


      auto right_child = node->Children()[child_limit];
      node->RemoveChild(right_child);
      new_node->AddChild(right_child);

      current_node = new_node;
    }


    parent->ReplaceChild(node, current_node);
  }
}

void SemanticAnalyzer::CheckUserDefinedFunctionParameterCount(Ast_Node &node)
{
  if (IsUserFunction(node))
  {
    int expected_count = m_functions[node->ValueAsText()].ParameterCount();
    int actual_count = node->Children().size();
    if (expected_count != actual_count)
    {
      throw ParameterCountMismatchError("Invalid parameter count for function '" + node->ValueAsText() + "'.  Expected " + std::to_string(expected_count) + " but was " + std::to_string(actual_count), node->DeclarationLine());
    }
  }
}


void SemanticAnalyzer::GenerateUserFunctionIds()
{
  m_user_function_ids["main"] == MAIN_FUNCTION_ID;
  int id = 1;
  for (auto name_function_pair : m_functions)
  {
    if (name_function_pair.first != "main")
    {
      m_user_function_ids[name_function_pair.first] = id++;
    }
  }
}

bool SemanticAnalyzer::IsUserFunction(Ast_Node & node)
{
  return node->Type() == NodeType::FUNCTION_CALL && m_inbuilt_functions.count(node->ValueAsText()) == 0;
}
