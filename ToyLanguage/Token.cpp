#include "Token.h"


Token::Token(TokenType type, std::string token_value) : m_type(type), m_token_value(token_value)
{
}


Token::~Token()
{
}
