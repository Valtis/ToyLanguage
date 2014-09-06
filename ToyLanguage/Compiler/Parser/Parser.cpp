#include "Parser.h"
#include "ParseError.h"

#include "../DataStructures/AstNodes/NodeCreation.h"

#include "../FunctionDefines.h"

Parser::Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens) : m_lines(tokens.first),
m_tokens(tokens.second), m_current_token(m_tokens.begin())
{
  token_to_string[TokenType::FUNCTION] = "function declaration";
  token_to_string[TokenType::WHILE] = "while";
  token_to_string[TokenType::IF] = "if";
  token_to_string[TokenType::TEXT] = "Text";
  token_to_string[TokenType::NUMBER] = "Number";
  token_to_string[TokenType::LPAREN] = "(";
  token_to_string[TokenType::RPAREN] = ")";
  token_to_string[TokenType::LBRACE] = "{";
  token_to_string[TokenType::RBRACE] = "}";
  token_to_string[TokenType::COMMA] = ",";
  token_to_string[TokenType::COLON] = ":";
  token_to_string[TokenType::SEMICOLON] = ";";
  token_to_string[TokenType::VARIABLE] = "variable declaration";
  token_to_string[TokenType::ASSIGNMENT] = "=";
  token_to_string[TokenType::PLUS] = "+";
  token_to_string[TokenType::QUOTE] = "\"";
  token_to_string[TokenType::IDENT] = "identifier";
  token_to_string[TokenType::MULTIPLICATION] = "*";


  m_inbuilt_functions.insert(FN_ADD);
  m_inbuilt_functions.insert(FN_SUB);
  m_inbuilt_functions.insert(FN_MUL);
  m_inbuilt_functions.insert(FN_DIV);
  m_inbuilt_functions.insert(FN_PRINT);
  m_inbuilt_functions.insert(FN_PRINTLN);
  m_inbuilt_functions.insert(FN_COMPARE);
  m_inbuilt_functions.insert(FN_IF);
  m_inbuilt_functions.insert(FN_LIST);
  m_inbuilt_functions.insert(FN_MAP);

}


Parser::~Parser()
{

}

std::unordered_map<std::string, Function> Parser::Parse()
{
  while (HasTokens())
  {
    ParseFunctionDeclaration();
  }


  if (m_functions.count("main") == 0)
  {
    throw UndefinedMainError("main-function not defined", m_tokens.back().LineNumber());
  }

  return m_functions;
}

void Parser::ParseFunctionDeclaration()
{

  Expect(TokenType::LPAREN);
  Expect(TokenType::FUNCTION);

  auto name_token = Expect(TokenType::IDENT);
  VerifyNoFunctionRedeclaration(name_token);
  Function f(name_token.Value(), name_token.LineNumber());


  ParseFunctionArguments(f);
  ParseFunctionBody(f);

  Expect(TokenType::RPAREN);

  m_functions[name_token.Value()] = f;
}

void Parser::ParseFunctionArguments(Function &f)
{
  Expect(TokenType::LPAREN);

  while (CurrentToken().Type() == TokenType::IDENT) {
    std::string name = CurrentToken().Value();
    CheckIfParameterShadowsFunction(name);
    if (f.HasParameter(name))
    {
      throw FunctionParameterRedeclarationError("Redeclaration of parameter '" + name + "' for function '" + f.Name() + "'",
        CurrentToken().LineNumber());
    }

    f.AddParameter(CurrentToken().Value());
    NextToken();
  }

  Expect(TokenType::RPAREN);
}



void Parser::CheckIfParameterShadowsFunction(std::string parameterName)
{
  if (m_functions.count(parameterName) != 0 || m_inbuilt_functions.count(parameterName) != 0)
  {
    throw ParameterShadowsFunctionError("Parameter '" + parameterName + "' shadows a function with same name",
      CurrentToken().LineNumber());
  }
}



void Parser::ParseFunctionBody(Function &f)
{
  Expect(TokenType::LPAREN);

  auto root_node = CreateRootNode(CurrentToken().LineNumber());

  if (CurrentToken().Type() == TokenType::IDENT)
  {
    ParseFunctionCall(f, root_node);
  }

  Expect(TokenType::RPAREN);

  f.SetRootNode(root_node);
}



void Parser::ParseFunctionCall(Function & f, AstPtr node)
{
  if (m_functions.count(CurrentToken().Value()) == 0 && m_inbuilt_functions.count(CurrentToken().Value()) == 0 &&
    CurrentToken().Value() != f.Name())
  {
    throw UndefinedFunctionCall("Call to undefined function " + CurrentToken().Value(), CurrentToken().LineNumber());
  }

  auto call_node = CreateFunctionCallNode(CurrentToken().LineNumber(), CurrentToken().Value());
  node->AddChild(call_node);
  NextToken();

  ParseFunctionCallArguments(f, call_node);
}


void Parser::ParseFunctionCallArguments(Function & f, AstPtr call_node)
{
  // REFACTOOOOR
  while (CurrentToken().Type() != TokenType::RPAREN)
  {
    // function call with parameters
    if (CurrentToken().Type() == TokenType::LPAREN)
    {
      NextToken();
      ParseFunctionCall(f, call_node);
    }
    else if (CurrentToken().Type() == TokenType::IDENT)
    {
      // function name in parameter list; pass as a parameter
      if (m_functions.count(CurrentToken().Value()) != 0)
      {
        auto node = CreateFunctionParameterNode(CurrentToken().LineNumber(), CurrentToken().Value());
        call_node->AddChild(node);       
      }
      else if (f.HasParameter(CurrentToken().Value()))
      {
        auto variable_node = CreateVariableReadNode(CurrentToken().LineNumber(), f.ParameterID(CurrentToken().Value()));
        call_node->AddChild(variable_node);
      }
    }
    else if (CurrentToken().Type() == TokenType::NUMBER)
    {
      auto number_node = CreateNumberNode(CurrentToken().LineNumber(), std::stod(CurrentToken().Value()));
      call_node->AddChild(number_node);
    }
    else
    {
      throw UnexpectedTokenError("Unexpected token " + CurrentToken().Value(), CurrentToken().LineNumber());
    }
    NextToken();
  }

}


Token Parser::CurrentToken()
{
  if (m_current_token == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file", m_tokens.back().LineNumber());
  }
  return *m_current_token;
}


Token Parser::NextToken()
{
  ++m_current_token;
  return CurrentToken();
}

Token Parser::PeekToken()
{
  if (m_current_token + 1 == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file", m_tokens.back().LineNumber());
  }
  return *(m_current_token + 1);
}


bool Parser::HasTokens()
{
  return m_current_token != m_tokens.end();
}

Token Parser::Expect(TokenType type)
{

  if (m_current_token == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file. Expected token'" + token_to_string[type] + "' ", m_tokens.back().LineNumber());
  }

  if (m_current_token->Type() != type)
  {
    throw UnexpectedTokenError("Expected '" + token_to_string[type] + "', actual '" + m_current_token->Value() + "' (type: " + token_to_string[m_current_token->Type()] + ")", m_current_token->LineNumber());
  }
  auto token = CurrentToken();
  ++m_current_token;

  return token;
}

void Parser::VerifyNoFunctionRedeclaration(Token &name_token)
{
  if (m_functions.count(name_token.Value()) != 0) {
    throw FunctionRedeclarationError("Redeclaration of function '" + name_token.Value() + "'. Previous declaration at line " +
      std::to_string(m_functions[name_token.Value()].DeclarationLine()), name_token.LineNumber());
  }
  else if (m_inbuilt_functions.count(name_token.Value()) != 0)
  {
    throw FunctionRedeclarationError("Declaration of function '" + name_token.Value() + "' shadows inbuilt function", name_token.LineNumber());
  }


}