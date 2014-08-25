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
  void TraverseAst(const Ast_Node &node, VMFunction &f);
  void GeneratePushNumberInstruction(const Ast_Node & node, VMFunction & vm_function);
  void GenerateFunctionCallInstruction(const Ast_Node & node, VMFunction & vm_function);
  void AddArithmeticInstruction(size_t instruction_child_count, VMFunction & vm_function, Instruction instruction);
};