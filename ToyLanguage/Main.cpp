#include <fstream>
#include <iostream>
#include "Compiler/Lexer.h"
#include "Compiler/Parser.h"
#include "Compiler/DataStructures/AbstractSyntaxTreeNode.h"

void Traverse(const Ast_Node &node);
int main()
{
  auto file = std::ifstream("file.txt");
  if (!file.is_open())
  {
    std::cout << "Could not open file\n";

    system("pause");
    return -1;
  }
  Lexer lexer{ file };

  std::unordered_map<TokenType, std::string> tokenToString;
  tokenToString[TokenType::FUNCTION] = "Function";
  tokenToString[TokenType::TEXT] = "Text";
  tokenToString[TokenType::NUMBER] = "Number";
  tokenToString[TokenType::LPAREN] = "Left parenthesis";
  tokenToString[TokenType::RPAREN] = "Right parenthesis";
  tokenToString[TokenType::LBRACE] = "Left brace";
  tokenToString[TokenType::RBRACE] = "Right brace";
  tokenToString[TokenType::COLON] = "Colon";
  tokenToString[TokenType::SEMICOLON] = "Semicolon";
  tokenToString[TokenType::VARIABLE] = "Variable";
  tokenToString[TokenType::ASSIGNMENT] = "Assignment";
  tokenToString[TokenType::PLUS] = "Plus";
  tokenToString[TokenType::VOID_TOKEN] = "Void";
  tokenToString[TokenType::INT_TOKEN] = "Int";
  tokenToString[TokenType::QUOTE] = "Quote";
  tokenToString[TokenType::IDENT] = "Identifier";
  tokenToString[TokenType::MULTIPLICATION] = "Multiplication";





  auto tokens = lexer.AnalyzeText();

  /* for (auto t : tokens.second)
   {
   std::string str = "TokenType: " + tokenToString[t.Type()];
   for (int i = str.length(); i < 30; ++i)
   {
   str += " ";
   }
   str += "Value: " + t.Value();

   std::cout << str << "\n";
   }
   */
  Parser parser(tokens);
  try
  {
    auto functions = parser.Parse();
    auto root = functions["function"].RootNode();
    Traverse(root);


  }
  catch (const std::exception &ex)
  {
    std::cout << ex.what() << "\n";
  }


  system("pause");

}

void PrintNodeType(const Ast_Node &node)
{
  if (node == nullptr)
  {
    std::cout << "Null children (should not happen)\n";
    return;
  }
  if (node->Type() == OperationType::BLOCK)
  {
    std::cout << "Block";
  }
  else if (node->Type() == OperationType::ADD)
  {
    std::cout << "Add";
  }
  else if (node->Type() == OperationType::MUL)
  {
    std::cout << "Multiply";
  }
  else if (node->Type() == OperationType::ASSIGNMENT)
  {
    std::cout << "Assignment";
  }
  else if (node->Type() == OperationType::VARIABLE_DECLARATION)
  {
    std::cout << "Variable declaration";
  }
  else if (node->Type() == OperationType::VALUE)
  {
    std::cout << "Value";
  }

  std::cout << "\n";
}

void Traverse(const Ast_Node &node)
{

  std::cout << "\n\n\nEntered node of type: ";
  PrintNodeType(node);
  if (node == nullptr)
  {
    return;
  }

//  enum class OperationType { BLOCK, FUNCTION_CALL, ADD, SUB, MUL, DIV, ASSIGNMENT, VARIABLE_DECLARATION, VALUE };
  auto children = node->Children();
  std::cout << "\n";
  if (children.size() == 0)
  {
    std::cout << "Node has no children\n";
    std::cout << "Name: " << node->Value().Name() << "  Value: " << node->Value().Value().value.integer_value << "\n";
  }

  int i = 0;
  std::cout << "Node has children \n";
  for (auto child : children)
  {
    std::cout << ++i << ": ";
    PrintNodeType(child);
  }

  i = 0;
  for (auto child : children)
  {
    std::cout << "Entering children number: " << ++i  << "\n";
    Traverse(child);
  }


}