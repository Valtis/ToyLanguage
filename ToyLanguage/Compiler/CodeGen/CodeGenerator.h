#pragma once
#include <unordered_map>

#include "../DataStructures/Function.h"
#include "../DataStructures/AstNode.h"
#include "../../VM/VMFunction.h"



class CodeGenerator
{
public:
  std::unordered_map<int, VMFunction> GenerateCode(const std::unordered_map<std::string, Function> functions);
  
private:
  VMFunction GenerateCodeForFunction(Function &f);
  void Generator(const Ast_Node &node, VMFunction &vm_function);
  void GeneratePushNumberInstruction(const Ast_Node & node, VMFunction & vm_function);
  void GenerateFunctionCallInstruction(const Ast_Node & node, VMFunction & vm_function);

  std::unordered_map<std::string, int> m_function_names_to_ids;
};