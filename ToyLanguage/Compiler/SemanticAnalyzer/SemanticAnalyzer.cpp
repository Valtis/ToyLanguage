#include "SemanticAnalyzer.h"
#include <functional>
#include <map>
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"
#include "SemanticError.h"
#define BIND(NAME__) std::bind(&SemanticAnalyzer::NAME__, this, std::placeholders::_1)

std::unordered_map<std::string, Function> SemanticAnalyzer::Analyze(std::unordered_map<std::string, Function> functions)
{
  for (auto &pair : functions)
  {
    Function f = pair.second;
    TraverseAst(f.RootNode(), BIND(TransformInbuiltFunctions));
  }
  return functions;
}


// transform AST so that arithmetic functions have only 2 children (eg. (- 2 3 4 5) is transformed to (-(-(- 2 3) 4 ) 5).
void SemanticAnalyzer::TransformInbuiltFunctions(Ast_Node &node)
{

  // name - maximum number of children in the leaf node
  std::map<std::string, int> functions = { { FN_ADD, 2 }, { FN_SUB, 2 }, { FN_MUL, 2 }, { FN_DIV, 2 }, { FN_PRINT, 1 }, { FN_PRINTLN, 1 } };

  if (node->Type() != NodeType::FUNCTION_CALL)
  {
    return;
  }

  if (functions.count(node->ValueAsText()) != 0)
  {


    size_t child_limit = functions[node->ValueAsText()];
    

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


