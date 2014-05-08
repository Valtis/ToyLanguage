#include "Token.h"
Token::Token() : m_type(TokenType::INVALID_TOKEN), m_token_value(""), m_line_number(-1)
{

}

Token::Token(TokenType type, std::string token_value, int line_number) : m_type(type), m_token_value(token_value), m_line_number(line_number)
{
}


Token::~Token()
{
}
