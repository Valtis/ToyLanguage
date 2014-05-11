#include <fstream>
#include <iostream>
#include "Compiler/Lexer.h"
#include "Compiler/Parser/Parser.h"
#include "Compiler/DataStructures/AbstractSyntaxTreeNode.h"
#include "Compiler/Parser/ParseError.h"
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
    std::cout << "function() ast\n";
    auto root = functions["function"].RootNode();
    Traverse(root);

    std::cout << "Main() ast\n";
    root = functions["main"].RootNode();
    Traverse(root);
  }
  catch (const ParseError &ex)
  {
    std::cout << "Parse error\n";
    std::cout << ex.what() << "\n";
  }
  catch (const std::logic_error &ex)
  {
    std::cout << "Internal error\n";
    std::cout << ex.what() << "\n";
  }


  system("pause");

}

void PrintNode(const Ast_Node &node)
{
  if (node == nullptr)
  {
    std::cout << "Null children (should not happen)\n";
    return;
  }


  if (node->Type() == OperationType::BLOCK)
  {
    // std::cout << "Block";
  }
  else if (node->Type() == OperationType::ADD)
  {
    std::cout << "ADD";
  }
  else if (node->Type() == OperationType::MUL)
  {
    std::cout << "MUL";
  }
  else if (node->Type() == OperationType::ASSIGNMENT)
  {
    std::cout << "ASSIGN";
  }
  else if (node->Type() == OperationType::DIV)
  {
    std::cout << "DIV";
  }
  else if (node->Type() == OperationType::SUB)
  {
    std::cout << "SUB";
  }
  else if (node->Type() == OperationType::VARIABLE_DECLARATION)
  {
    //  std::cout << "Variable declaration";
  }
  else if (node->Type() == OperationType::VARIABLE)
  {
    std::cout << "PUSH " << node->Value().Name();// << node->Value().Value().value.integer_value;
  }
  else if (node->Type() == OperationType::CONSTANT)
  {
    std::cout << "PUSH ";
    switch (node->Value().VariableData().type)
    {
    case VariableType::INTEGER:
      std::cout << node->Value().VariableData().value.integer_value;
      break;
    case VariableType::STRING:
      for (int i = 0; i < node->Value().VariableData().value.string_value.length; ++i)
      {
        std::cout << "STRING!";
      }
      break;
    default:
      break;

    }
  }
  else if (node->Type() == OperationType::FUNCTION_CALL)
  {
    std::cout << "CALL " << node->Value().Name();
  }

  std::cout << "\n";
}

void Traverse(const Ast_Node &node)
{
  auto children = node->Children();

  for (auto child : children)
  {
    Traverse(child);
  }
  PrintNode(node);
}