#pragma once
#include <unordered_map>

#include "../DataStructures/AstNodes/AstNode.h"
#include "../DataStructures/Function.h"

class SemanticAnalyzer {
public:
  SemanticAnalyzer(std::unordered_map<std::string, Function> functions);
  std::unordered_map<std::string, Function> Analyze();

private:
  void GenerateUserFunctionIds();
  
  std::unordered_map<std::string, Function>  m_functions;
};