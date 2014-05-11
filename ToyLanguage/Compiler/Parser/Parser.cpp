#include "Parser.h"
#include "ParseError.h"
#include <list>
#include "ShuntingYard.h"
#include "ShuntingYardError.h"

Parser::Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens) : m_lines(tokens.first), m_tokens(tokens.second), m_current_token(m_tokens.begin())
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
  token_to_string[TokenType::VOID_TOKEN] = "void";
  token_to_string[TokenType::INT_TOKEN] = "int";
  token_to_string[TokenType::QUOTE] = "\"";
  token_to_string[TokenType::IDENT] = "identifier";
  token_to_string[TokenType::MULTIPLICATION] = "*";
}


Parser::~Parser()
{

}

std::unordered_map<std::string, Function> Parser::Parse()
{
  while (true)
  {
    switch (m_current_token->Type())
    {
    case TokenType::FUNCTION:
      ParseFunction();
      break;
    default:
      InvalidTokenError("Expected function declaration");
    }
    if (HasTokens())
    {
      NextToken();
    }
    else 
    {
      break;
    }
  }


  if (m_functions.count("main") == 0)
  {
    throw ParseError("main-function not defined");
  }
  return m_functions;

}

void Parser::ParseFunction()
{
  Expect(TokenType::IDENT);

  if (m_functions.count(m_current_token->Value()) != 0)
  {
    RaiseFunctionRedeclarationError();
  }
   
  Function function(m_current_token->Value(), m_current_token->LineNumber());
  m_root_node = Ast_Node{ new AbstractSyntaxTreeNode };

  Expect(TokenType::LPAREN);

  Expect(TokenType::RPAREN);
  Expect(TokenType::COLON);

  ExpectType();


  ParseBlock(m_root_node);
  
  function.SetRootNode(m_root_node);
  m_functions[function.Name()] = function;
}

void Parser::ParseBlock(Ast_Node parent_node)
{

  Expect(TokenType::LBRACE);

  Ast_Node node{ new AbstractSyntaxTreeNode{} };
  NextToken();
  while (m_current_token->Type() != TokenType::RBRACE)
  {

    switch (m_current_token->Type())
    {
    case TokenType::VARIABLE:
      ParseVariableDeclaration(node);
      break;
    case TokenType::IDENT:
       ParseExpression(node);

      break;
    case TokenType::WHILE:
      break;
    case TokenType::IF:
      break;
    default:
      InvalidTokenError("Expected variable declaration, identifier or statement");
    }
    NextToken();
  }

  parent_node->AddChild(node);

}

void Parser::ParseVariableDeclaration(Ast_Node parent_node)
{
  Expect(TokenType::IDENT);
  
  if (parent_node->VariableExists(m_current_token->Value()))
  {
    RaiseVariableRedeclarationError(parent_node->GetVariable(m_current_token->Value()).DeclarationLine());
  }

  Variable v{m_current_token->Value(), m_current_token->LineNumber()};
  

  Expect(TokenType::COLON);
  ExpectNonVoidType();
  
  switch (m_current_token->Type())
  {
  case TokenType::INT_TOKEN:
    v.SetValue(VariableType::INTEGER, 0);
    break;
  default:
    throw std::logic_error("Internal error - variable token provided by lexer has invalid type");
  }
  
  parent_node->AddVariable(v.Name(), v);
  Expect(TokenType::ASSIGNMENT);

  Ast_Node declarationNode(new AbstractSyntaxTreeNode(OperationType::VARIABLE_DECLARATION));
  Ast_Node assignmentNode(new AbstractSyntaxTreeNode{ OperationType::ASSIGNMENT });
  Ast_Node child(new AbstractSyntaxTreeNode{ OperationType::VARIABLE });
  child->SetValue(v);
  
  assignmentNode->AddChild(child);


  declarationNode->AddChild(assignmentNode);

  // check that assignment is allowed (for example int <--- str raises error)
  parent_node->AddChild(declarationNode);
  NextToken();
  ParseExpression(assignmentNode);

}



void Parser::ParseExpression(Ast_Node parent_node)
{
  std::vector<Token> tokens;
  while (m_current_token->Type() != TokenType::SEMICOLON)
  {
    auto type = m_current_token->Type();
    if (type != TokenType::IDENT && type != TokenType::NUMBER && !IsOperator(type))
    {
      InvalidTokenError("Expected value literal, variable, function call or operator");
    }

    tokens.push_back(*m_current_token);
    NextToken();
  }

  ShuntingYard yard;
  try
  {
    parent_node->AddChild(yard.CreateAst(tokens, parent_node));
  }
  catch (const ShuntingYardUndeclaredVariableError &ex)
  {
    RaiseUndeclaredVariableError(ex.GetToken());
  }
  catch (const ShuntingYardMissingParenthesisError &ex)
  {
    RaiseMissingParenthesisError(ex.GetToken());
  }
  catch (const ShuntingYardMissingOperandError &ex)
  {
    RaiseMissingOperandError(ex.GetToken());
  }
}


bool Parser::IsOperator(TokenType type)
{
  return type == TokenType::MULTIPLICATION || type == TokenType::DIVISION || type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::ASSIGNMENT
    || type != TokenType::LPAREN || type != TokenType::RPAREN;
}

void Parser::NextToken()
{
  ++m_current_token;
  if (m_current_token == m_tokens.end())
  {
    throw ParseError("Unexpected end of file");
  }
}

Token Parser::PeekNextToken()
{
  if (m_current_token + 1 == m_tokens.end())
  {
    throw ParseError("Unexpected end of file");
  }
  return *(m_current_token + 1);
}


bool Parser::HasTokens()
{
  return m_current_token + 1 != m_tokens.end();
}

void Parser::Expect(TokenType type)
{
  NextToken();
  if (m_current_token->Type() != type)
  {
    InvalidTokenError("Expected '" + token_to_string[type] + "'\n");
  }
}

void Parser::ExpectType()
{
  NextToken();
  auto type = m_current_token->Type();
  if (type != TokenType::VOID_TOKEN && type != TokenType::INT_TOKEN)
  {
    InvalidTokenError("Expected value type");
  }
}

void Parser::ExpectNonVoidType()
{
  NextToken();
  auto type = m_current_token->Type();
  if (type != TokenType::INT_TOKEN)
  {
    InvalidTokenError("Expected non-void value type");
  }
}



void Parser::InvalidTokenError(const Token &token, std::string expected)
{
  throw ParseError("Unexpected token " + GetTokenErrorInfo(token) + expected);
}

void Parser::InvalidTokenError(std::string expected)
{
  InvalidTokenError(*m_current_token, expected);
}

std::string Parser::GetTokenErrorInfo(const Token &token)
{
  int line_number = token.LineNumber();
  return "'" + token.Value() + "' " + GetLineInfo(line_number) + "\n\n";
}

std::string Parser::GetLineInfo(int token_line)
{
  return "at line " + std::to_string(token_line) + "\n\n---> "
    + m_lines[token_line - 1];
}


void Parser::RaiseFunctionRedeclarationError()
{

  int declaration_line = m_functions[m_current_token->Value()].DeclarationLine();
  throw ParseError(
    "Redefinition of function " + GetTokenErrorInfo(*m_current_token) +
    "Previous declaration " + GetLineInfo(declaration_line));
}

void Parser::RaiseVariableRedeclarationError( int initial_declaration )
{
 
  throw UndeclaredVariableError(
    "Redefinition of variable " + GetTokenErrorInfo(*m_current_token) +
    "Previous declaration " + GetLineInfo(initial_declaration) + "\n");
}

void Parser::RaiseUndeclaredVariableError(Token token)
{
  throw UndeclaredVariableError("Identifier " + GetTokenErrorInfo(token) + "has not been declared");
}


void Parser::RaiseMissingParenthesisError(Token token)
{
  throw MissingParenthesisError("Mismatched parenthesis " + GetTokenErrorInfo(token));
}

void Parser::RaiseMissingOperandError(Token token)
{
  throw MissingOperandError("Missing operand " + GetLineInfo(token.LineNumber()));
}