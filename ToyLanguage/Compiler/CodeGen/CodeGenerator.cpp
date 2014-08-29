#include "CodeGenerator.h"
#include "CodeGenAstVisitor.h"
#include "../../VM/ByteCode.h"
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"
#include "../DataStructures/AstNodes/AstNode.h"

CodeGenerator::CodeGenerator(const std::unordered_map<std::string, Function> functions, 
  std::unordered_map<std::string, int> function_name_ids) : m_functions(functions), m_function_names_to_ids(function_name_ids)
{

}

std::unordered_map<int, VMFunction> CodeGenerator::GenerateCode()
{

  std::unordered_map< int, VMFunction > vm_functions;
  
  for (auto pair : m_functions)
  {
    Function f = pair.second;
    vm_functions[m_function_names_to_ids[pair.first]] = GenerateCodeForFunction(pair.first, f);
  }
  return vm_functions;
}

VMFunction CodeGenerator::GenerateCodeForFunction(const std::string &current_function_name, Function &f)
{

  VMFunction vm_function(current_function_name, f.ParameterCount());
  
  CodeGenAstVisitor visitor(&vm_function, m_function_names_to_ids);

  TraverseAst(f.RootNode(), &visitor);

  return vm_function;
}
