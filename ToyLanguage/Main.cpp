#include <fstream>
#include <iostream>
#include "Compiler/Lexer.h"
#include "Compiler/Parser/Parser.h"
#include "Compiler/DataStructures/AstNode.h"
#include "Compiler/Parser/ParseError.h"
#include "Compiler/SemanticAnalyzer/SemanticAnalyzer.h"
#include "Compiler/SemanticAnalyzer/SemanticError.h"
#include "Compiler/CodeGen/CodeGenerator.h"

#include "VM/VM.h"
#include <regex>
void TraverseAst(const Ast_Node &node, int level = 0);
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

 /*   std::cout << "Lexer output:\n";
    for (auto token : tokens.second) {
      std::cout << "  Type: " << tokenToString[token.Type()] << "\tValue: " << token.Value() << "\tLine " << token.LineNumber() << "\n";
    }*/
    std::cout << "\n";
    Parser parser(tokens);

    auto functions = parser.Parse();



    std::cout << "Before semantic analysis:----------------\n\n";
    for (auto f : functions)
    {
      std::cout << "  Function: " << f.second.Name() << "\tDeclared at line: " << f.second.DeclarationLine() << "\t\n";
      TraverseAst(f.second.RootNode());
      std::cout << "\n\n";

    }


    SemanticAnalyzer analyzer;
    functions = analyzer.Analyze(functions);
      
    
    std::cout << "\nAfter semantic analysis:----------------\n\n";
    for (auto f : functions)
    {
      std::cout << "  Function: " << f.second.Name() << "\tDeclared at line: " << f.second.DeclarationLine() << "\t\n";
      TraverseAst(f.second.RootNode());
      std::cout << "\n\n";

    }




    CodeGenerator generator;
    auto code = generator.GenerateCode(functions);
    
    

    VM vm;
    std::cout << "\nInitializing VM...\n";
    vm.Initialize(code);
    std::cout << "Executing program...\n";
    vm.Execute();
  }
  catch (const InvalidTokenError &ex)
  {
    std::cout << "Lexer error at line " + std::to_string(ex.LineNumber()) + "\n";
    std::cout << ex.what() << "\n";
  }
  catch (const UnexpectedEOFError &ex)
  {
    std::cout << ex.what() << "\n";
  }
  catch (const ParseError &ex)
  {
    std::cout << "Parse error at line " + std::to_string(ex.LineNumber()) + "\n";
    std::cout << "  >>> " << tokens.first[ex.LineNumber() - 1] << "\n";
    std::cout << ex.what() << "\n";
  }
  catch (const SemanticError &ex)
  {
    std::cout << "Semantic error at line " + std::to_string(ex.LineNumber()) + "\n";
    std::cout << "  >>> " << tokens.first[ex.LineNumber() - 1] << "\n";
    std::cout << ex.what() << "\n";
  }
  catch (const std::exception &ex)
  {
    std::cout << "Error\n";
    std::cout << ex.what() << "\n";
  }


  system("pause");
}



std::string node_name(NodeType type)
{
  switch (type)
  {
  case NodeType::FUNCTION_CALL:
    return "Function call";
  case NodeType::NUMBER:
    return "Number";
  case NodeType::ROOT:
    return "Root";
  default:
    return "TBD";
  }
}

std::string node_value(const Ast_Node &node)
{
  switch (node->Type())
  {
  case NodeType::NUMBER:
    return std::to_string(node->ValueAsNumber());
  case NodeType::FUNCTION_CALL:
    return node->ValueAsText();
  default:
    return "";
  }
}

void TraverseAst(const Ast_Node &node, int level)
{
  if (node == nullptr)
  {
    return;
  }

  std::string spaces = "    ";
  for (int i = 0; i < level; ++i)
  {
    spaces += "  ";
  }
  std::cout << spaces << "Node type: " << node_name(node->Type()) << "\t" << spaces << "Node value: " << node_value(node) << "\n";

  for (auto child : node->Children())
  {
    TraverseAst(child, level + 1);
  }
}