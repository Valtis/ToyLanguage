#include <fstream>
#include <iostream>
#include "Compiler/Lexer.h"
#include "Compiler/Parser/Parser.h"
#include "Compiler/DataStructures/AstNodes/RootNode.h"
#include "Compiler/DataStructures/AstNodes/NumberNode.h"
#include "Compiler/DataStructures/AstNodes/VariableReadNode.h"
#include "Compiler/DataStructures/AstNodes/FunctionCallNode.h"


#include "Compiler/DataStructures/AstNodes/AstVisitor.h"
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
    

    /*
    std::cout << "Before semantic analysis:----------------\n\n";
    for (auto f : functions)
    {
      std::cout << "  Function: " << f.second.Name() << "\tDeclared at line: " << f.second.DeclarationLine() << "\t\n";
      TraverseAst(f.second.RootNode());
      std::cout << "\n\n";

    }*/
    

    SemanticAnalyzer analyzer(functions);
    functions = analyzer.Analyze();
      
    
  /*  std::cout << "\nAfter semantic analysis:----------------\n\n";
    for (auto f : functions)
    {
      std::cout << "  Function: " << f.second.Name() << "\tDeclared at line: " << f.second.DeclarationLine() << "\t\n";
      TraverseAst(f.second.RootNode());
      std::cout << "\n\n";

    }

    std::cout << "\nName-id-pairs:\n";
    for (auto nameid : analyzer.UserFunctionIds())
    {
      std::cout << "Function name: " << nameid.first << "\tID: " << nameid.second << "\n";
    }*/

    


    CodeGenerator generator(functions, analyzer.UserFunctionIds());
    auto code = generator.GenerateCode();
  

    VM vm;
 //   std::cout << "\nInitializing VM...\n";
    vm.Initialize(code);
 //   std::cout << "Executing program...\n";
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






class AstNamePrinter : public AstVisitor
{
public:
  void visit(VariableReadNode *node)
  {
    std::cout << "Variable";
  }

  void visit(RootNode *node)
  {
    std::cout << "Root";
  }  

  void visit(NumberNode *node)
  {
    std::cout << "Number";
  }  
  
  void visit(FunctionCallNode *node)
  {
    std::cout << "Function call";
  }
};

class AstValuePrinter : public AstVisitor
{
public:
  void visit(VariableReadNode *node)
  {
    std::cout << node->VariableId();
  }

  void visit(RootNode *node)
  {
    
  }

  void visit(NumberNode *node)
  {
    std::cout << node->Value();
  }

  void visit(FunctionCallNode *node)
  {
    std::cout << node->Name();
  }
};


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


  std::string name;
  AstNamePrinter name_printer;
  AstValuePrinter value_printer;

  std::cout << spaces << "Node type: "; 
  node->accept(&name_printer);
  std::cout << "\t" << spaces << "Node value: "; 
  
  node->accept(&value_printer);
  std::cout << "\n";

  for (auto child : node->Children())
  {
    TraverseAst(child, level + 1);
  }
}