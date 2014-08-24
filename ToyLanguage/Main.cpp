#include <fstream>
#include <iostream>
#include "Compiler/Lexer.h"
#include "Compiler/Parser/Parser.h"
#include "Compiler/DataStructures/AstNode.h"
#include "Compiler/Parser/ParseError.h"

#include <regex>
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
  tokenToString[TokenType::QUOTE] = "Quote";
  tokenToString[TokenType::IDENT] = "Identifier";
  tokenToString[TokenType::MULTIPLICATION] = "Multiplication";





 


  std::pair < std::vector < std::string>, std::vector<Token>> tokens;
  try
  {
    Lexer lexer(file);
    tokens = lexer.AnalyzeText();

    std::cout << "Lexer output:\n";
    for (auto token : tokens.second) {
      std::cout << "  Type: " << tokenToString[token.Type()] << "\tValue: " << token.Value() << "\n";
    }
    std::cout << "\n";
    Parser parser(tokens);

    auto functions = parser.Parse();
  }
  catch (const InvalidTokenError &ex)
  {
    std::cout << "Lexer error at line " + std::to_string(ex.LineNumber()+1) + "\n";
    std::cout << ex.what() << "\n";
  }
  catch (const ParseError &ex)
  {
    std::cout << "Parse error at line " + std::to_string(ex.LineNumber()+1) + "\n";
    std::cout << "  >>> " << tokens.first[ex.LineNumber()] << "\n";
    std::cout << ex.what() << "\n";
  }
  catch (const std::logic_error &ex)
  {
    std::cout << "Internal error\n";
    std::cout << ex.what() << "\n";
  }


  system("pause");
}
