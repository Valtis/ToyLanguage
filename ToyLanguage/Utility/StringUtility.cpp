#include "StringUtility.h"
#include <cstring>
using namespace std;

vector<string> Utility::Tokenize(const string &text, const string &delimiters)
{
  vector<string> tokens;
  if (text.size() == 0)
  {
    return tokens;
  }

  char *retVal;
  const unsigned int textbufferSize = text.length() + 1;
  char *textBuffer = new char[textbufferSize];
  for (unsigned int i = 0; i < textbufferSize; ++i)
  {
    textBuffer[i] = '\0';
  }
#ifdef _MSC_VER
  strncpy_s(textBuffer, textbufferSize, text.c_str(), text.length());
  char *nextToken = nullptr;
  retVal = strtok_s(textBuffer, delimiters.c_str(), &nextToken);
#else
  strncpy(textBuffer, text.c_str(), text.length());
  retVal = strtok(textBuffer, delimiters.c_str());
#endif



  while (retVal != nullptr)
  {
    tokens.push_back(retVal);
#ifdef _MSC_VER
    retVal = strtok_s(nullptr, delimiters.c_str(), &nextToken);
#else
    retVal = strtok(nullptr, delimiters.c_str());
#endif
  }

  delete [] textBuffer;
  return tokens;
}

string Utility::Trim(const string &line)
{
  const char *WHITE_SPACE_TOKENS = " \n\t\r";

  int beginning = line.find_first_not_of(WHITE_SPACE_TOKENS);

  if (beginning == string::npos)
  {
    return "";
  }

  string ret = line.substr(beginning);

  int end = ret.find_last_not_of(WHITE_SPACE_TOKENS);

  if (end == string::npos)
  {
    end = ret.length();
  }

  return ret.substr(0, end + 1);
}