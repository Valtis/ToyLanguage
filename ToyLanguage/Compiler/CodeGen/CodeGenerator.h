#pragma once
#include <unordered_map>

#include "../DataStructures/Function.h"
#include "../../VM/VMFunction.h"



class CodeGenerator
{
public:
  CodeGenerator(const std::unordered_map<std::string, Function> functions);

  std::unordered_map<int, VMFunction> GenerateCode();
  
private:
  VMFunction GenerateCodeForFunction(const std::string &name, Function &f);
  const std::unordered_map<std::string, Function> m_functions;

};