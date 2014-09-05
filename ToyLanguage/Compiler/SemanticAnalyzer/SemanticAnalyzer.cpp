#include "SemanticAnalyzer.h"
#include "SemanticAstVisitor.h"
#include "../../Utility/AstFunctions.h"
#include "../../VM/VMFunction.h"

SemanticAnalyzer::SemanticAnalyzer(std::unordered_map<std::string, Function> functions) : m_functions(functions)
{

}

std::unordered_map<std::string, Function> SemanticAnalyzer::Analyze()
{

  GenerateUserFunctionIds();

  for (auto &pair : m_functions)
  {
    Function f = pair.second;
    SemanticAstVisitor visitor(m_functions);
    TraverseAst(f.RootNode(), &visitor);
    
  }
  return m_functions;
}


void SemanticAnalyzer::GenerateUserFunctionIds()
{
  int id = MAIN_FUNCTION_ID + 1;
  for (auto &name_function_pair : m_functions)
  {
    if (name_function_pair.first != "main")
    {
      name_function_pair.second.FunctionId(id++);
    } 
    else
    {
      name_function_pair.second.FunctionId(MAIN_FUNCTION_ID);
    }
  }
}
