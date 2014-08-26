#pragma once
#include <unordered_map>

#include "../DataStructures/AstNode.h"
#include "../DataStructures/Function.h"

class SemanticAnalyzer {
public:
  std::unordered_map<std::string, Function> Analyze(std::unordered_map<std::string, Function> functions);
  void TransformArithmeticInstructions(Ast_Node &node);
};