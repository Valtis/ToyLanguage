#include "Parser.h"
#include "ParseError.h"

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
    FunctionRedeclarationError();
  }
   
  Function function(m_current_token->Value(), m_current_token->LineNumber());
  m_root_node = Ast_Node{ new AbstractSyntaxTreeNode };

  Expect(TokenType::LPAREN);

  if (PeekNextToken().Type() == TokenType::VARIABLE)
  {
    ParseVariableList();
  }

  Expect(TokenType::RPAREN);
  Expect(TokenType::COLON);

  ExpectType();


  ParseBlock(m_root_node);


/*  while (m_current_node->Parent() != nullptr)
  {
    m_current_node = m_current_node->Parent();
  }*/
  
  function.SetRootNode(m_root_node);
  m_functions[function.Name()] = function;
}


void Parser::ParseVariableList()
{
  while (true)
  {
    Expect(TokenType::VARIABLE);
    Expect(TokenType::IDENT);
    Expect(TokenType::COLON);
    ExpectNonVoidType();
    if (PeekNextToken().Type() != TokenType::COMMA)
      break;
    NextToken();

  }

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
      // ParseStatement();

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
    VariableRedeclarationError();
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

  std::shared_ptr<AbstractSyntaxTreeNode> declarationNode(new AbstractSyntaxTreeNode(OperationType::VARIABLE_DECLARATION));
  std::shared_ptr<AbstractSyntaxTreeNode> assignmentNode(new AbstractSyntaxTreeNode{ OperationType::ASSIGNMENT });
  std::shared_ptr<AbstractSyntaxTreeNode> child(new AbstractSyntaxTreeNode{ OperationType::VALUE });
  child->SetValue(v);
  
  assignmentNode->AddChild(child);
  NextToken();
  ParseExpression(assignmentNode);

  declarationNode->AddChild(assignmentNode);
  // check that assignment is allowed (for example int <--- str raises error)
  parent_node->AddChild(declarationNode);
}


void Parser::ParseStatement() 
{
  Expect(TokenType::IDENT);
  
  NextToken();
  switch (m_current_token->Type())
  {
  case TokenType::LPAREN:
    ParseFunctionCallParameters();
    break;
  case TokenType::ASSIGNMENT:
    break;
  case TokenType::PLUS:
    break;
  case TokenType::SEMICOLON:
    return;
  default:
    InvalidTokenError("Expected expression");
  }

  Expect(TokenType::SEMICOLON);
}

void Parser::ParseFunctionCallParameters()
{
  /*while (true)
  {
    NextToken();

    if (m_current_token->Type() == TokenType::RPAREN)
    {
      return;
    }

    ParseExpression();
  }*/
}

void Parser::ParseExpression(Ast_Node parent_node)
{
  Ast_Node node, root;

  bool expectOperator = false;
  // todo - add operator presedence code 
  while (m_current_token->Type() != TokenType::SEMICOLON)
  {
  
    
    NextToken();
  }

  return parent_node->AddChild(root);
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


void Parser::InvalidTokenError(std::string expected)
{
  throw ParseError("Unexpected token " + GetTokenErrorInfo() + expected);
}

std::string Parser::GetTokenErrorInfo()
{
  int line_number = m_current_token->LineNumber();
  return "'" + m_current_token->Value() + "' at line " + std::to_string(line_number) + "\n\n---> "
    + m_lines[line_number - 1] + "\n\n";
}

void Parser::FunctionRedeclarationError()
{

  int declaration_line = m_functions[m_current_token->Value()].DeclarationLine();
  throw ParseError(
    "Redefinition of function " + GetTokenErrorInfo() +
    "Previous declaration at line " + std::to_string(declaration_line) + "\n\n---> " + m_lines[declaration_line - 1]);
}

void Parser::VariableRedeclarationError()
{
  int declaration_line = m_root_node->GetVariable(m_current_token->Value()).DeclarationLine();
  throw ParseError(
    "Redefinition of variable " + GetTokenErrorInfo() +
    "Previous declaration at line " + std::to_string(declaration_line) + "\n\n---> " + m_lines[declaration_line - 1] + "\n");
}

void Parser::UndeclaredVariableError()
{
  throw ParseError("Identifier " + GetTokenErrorInfo() + "has not been declared");
}
