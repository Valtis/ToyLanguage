#include <fstream>
#include <iostream>
#include "Lexer.h"

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

  for (auto t : tokens)
  {
    std::string str = "TokenType: " + tokenToString[t.Type()];
    for (int i = str.length(); i < 30; ++i)
    {
      str += " ";
    }
    str += "Value: " + t.Value();

    std::cout << str << "\n";
  }


  system("pause");

}