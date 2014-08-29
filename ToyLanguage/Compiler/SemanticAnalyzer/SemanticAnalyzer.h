#pragma once
#include <unordered_map>

#include "../DataStructures/AstNodes/AstNode.h"
#include "../DataStructures/Function.h"

class SemanticAnalyzer {
public:
  SemanticAnalyzer(std::unordered_map<std::string, Function> functions);
  std::unordered_map<std::string, Function> Analyze();

  std::unordered_map<std::string, int> UserFunctionIds()
  {
    return m_user_function_ids;
  }
private:
  void GenerateUserFunctionIds();
  
  std::unordered_map<std::string, Function>  m_functions;
  // function name - parameter count
  std::unordered_map<std::string, int> m_inbuilt_functions;

  // function name_id
  std::unordered_map<std::string, int> m_user_function_ids;

};