#include "SemanticAnalyzer.h"
#include <functional>
#include <set>
#include "../../Utility/AstFunctions.h"

#define BIND(NAME__) std::bind(&SemanticAnalyzer::NAME__, this, std::placeholders::_1)

std::unordered_map<std::string, Function> SemanticAnalyzer::Analyze(std::unordered_map<std::string, Function> functions)
{
  for (auto &pair : functions)
  {
    Function f = pair.second;
    TransformAst(f.RootNode(), BIND(TransformArithmeticInstructions));
  }
  return functions;
}


// transform ast so that arithmetic functions have only 2 children (eg. (- 2 3 4 5) is transformed to (-(-(- 2 3) 4 ) 5).
void SemanticAnalyzer::TransformArithmeticInstructions(Ast_Node &node)
{
  std::set<std::string> arithmetic_functions = { "+", "-", "/", "*" };
  if (node->Type() != NodeType::FUNCTION_CALL)
  {
    return;
  }

  if (arithmetic_functions.count(node->ValueAsText()) != 0)
  {
    auto parent = node->Parent();

    Ast_Node current_node = node;

    while (node->Children().size() > 2)
    {

      auto new_node = std::make_shared<AstNode>(NodeType::FUNCTION_CALL);
      new_node->ValueAsText(node->ValueAsText());
      new_node->AddChild(current_node);


      auto right_child = node->Children()[2];
      node->RemoveChild(right_child);
      new_node->AddChild(right_child);

      current_node = new_node;
    }


    parent->ReplaceChild(node, current_node);
  }
}

