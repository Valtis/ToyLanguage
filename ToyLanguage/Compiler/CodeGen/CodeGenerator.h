#pragma once
#include <unordered_map>

#include "../DataStructures/Function.h"
#include "../../VM/VMFunction.h"



class CodeGenerator
{
public:
  CodeGenerator(const std::unordered_map<std::string, Function> functions, const std::unordered_map<std::string, int> function_name_ids);

  std::unordered_map<int, VMFunction> GenerateCode();
  
private:
  VMFunction GenerateCodeForFunction(const std::string &name, Function &f);

  std::unordered_map<std::string, int> m_function_names_to_ids;
  const std::unordered_map<std::string, Function> m_functions;

};