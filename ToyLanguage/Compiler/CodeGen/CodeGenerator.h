#pragma once
#include <unordered_map>

#include "../DataStructures/Function.h"
#include "../DataStructures/AstNode.h"
#include "../../VM/VMFunction.h"



class CodeGenerator
{
public:
  CodeGenerator::CodeGenerator(const std::unordered_map<std::string, Function> functions, const std::unordered_map<std::string, int> function_name_ids);

  std::unordered_map<int, VMFunction> GenerateCode();
  
private:
  VMFunction GenerateCodeForFunction(const std::string &name, Function &f);
  void Generator(const Ast_Node &node, VMFunction &vm_function, const std::string &current_function_name);
  void GeneratePushNumberInstruction(const Ast_Node & node, VMFunction & vm_function);
  void GenerateFunctionCallInstruction(const Ast_Node & node, VMFunction & vm_function);
  void GenerateReturnOrStopInstruction(const std::string &current_function_name, VMFunction & vm_function);
  void GenerateVariableReadInstruction(const Ast_Node & node, VMFunction & vm_function);
  std::unordered_map<std::string, int> m_function_names_to_ids;
  const std::unordered_map<std::string, Function> m_functions;

};