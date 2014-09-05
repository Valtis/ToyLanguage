#include "CodeGenerator.h"
#include "CodeGenAstVisitor.h"
#include "../../VM/ByteCode.h"
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"
#include "../DataStructures/AstNodes/AstNode.h"

CodeGenerator::CodeGenerator(const std::unordered_map<std::string, Function> functions) : m_functions(functions)
{

}

std::unordered_map<int, VMFunction> CodeGenerator::GenerateCode()
{

  std::unordered_map< int, VMFunction > vm_functions;
  
  for (auto pair : m_functions)
  {
    Function f = pair.second;
    vm_functions[f.FunctionId()] = GenerateCodeForFunction(pair.first, f);
  }
  return vm_functions;
}

VMFunction CodeGenerator::GenerateCodeForFunction(const std::string &current_function_name, Function &f)
{

  VMFunction vm_function(current_function_name, f.ParameterCount());
  
  CodeGenAstVisitor visitor(&vm_function, m_functions);

  TraverseAst(f.RootNode(), &visitor);

  return vm_function;
}
