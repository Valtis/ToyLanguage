#include "SemanticAstVisitor.h"
#include "SemanticError.h"

#include "../FunctionDefines.h"
#include "../DataStructures/Function.h"
#include "../DataStructures/AstNodes/NodeCreation.h"
#include "../DataStructures/AstNodes/FunctionCallNode.h"


#define NOT_RESTRICTED -1

SemanticAstVisitor::SemanticAstVisitor(std::unordered_map<std::string, Function> f) : m_functions(f)
{

  std::unordered_map<std::string, int> transform = { { FN_ADD, 2 }, { FN_SUB, 2 }, { FN_MUL, 2 },
  { FN_DIV, 2 }, { FN_PRINT, 1 }, { FN_PRINTLN, 1 } };

  std::unordered_map<std::string, ChildCount> limits = { { FN_ADD, { 2, 2 } }, { FN_SUB, { 2, 2 } },
      { FN_MUL, { 2, 2 } }, { FN_DIV, { 2, 2 } }, { FN_PRINT, { 1, 1 } }, { FN_PRINTLN, { 1, 1 } }, 
      { FN_COMPARE, { 2, 2 } }, { FN_IF, { 2, 3 } }, { FN_LIST, { 1, NOT_RESTRICTED } }, { FN_MAP, { 2, NOT_RESTRICTED } }
      };

  // workaround for compiler error C2593 
  m_inbuilt_function_transform_count = transform;
  m_inbuilt_function_child_count = limits;
}


void SemanticAstVisitor::Visit(FunctionCallNode *node)
{
  if (m_inbuilt_function_transform_count.count(node->Name()) != 0)
  {
    TransformInbuiltFunctionsWithMaxChildrenLimitation(node);
  }


  if (m_inbuilt_function_child_count.count(node->Name()) != 0)
  {
    CheckInbuiltFunctionParameterCount(node);
  }
  else
  {
    CheckUserFunctionParameterCount(node);
  }

  if (node->Name() == FN_IF)
  {
    TransformIf(node);
  }
}

// functions like '+' have maximum child count internally as the VM pops always the same number of operands from stack (eg. 2 for '+')
// this function transforms calls like (+ 1 2 3 4) to (+ (+ (+ 1 2 ) 3 ) 4) to that this limit is respected
void SemanticAstVisitor::TransformInbuiltFunctionsWithMaxChildrenLimitation(FunctionCallNode *node)
{

  size_t child_limit = m_inbuilt_function_transform_count[node->Name()];

  auto parent = node->Parent();
  auto current_node = parent->GetChildByRawPointer(node);

  while (node->Children().size() > child_limit)
  {

    auto new_node = CreateFunctionCallNode(node->DeclarationLine(), node->Name());
    new_node->AddChild(current_node);


    auto right_child = node->Children()[child_limit];
    node->RemoveChild(right_child);
    new_node->AddChild(right_child);

    current_node = new_node;
  }


  parent->ReplaceChild(parent->GetChildByRawPointer(node), current_node);
}


// todo: clean up as this is becoming horrible
void SemanticAstVisitor::CheckInbuiltFunctionParameterCount(FunctionCallNode *node)
{
  ChildCount count = m_inbuilt_function_child_count[node->Name()]; 

  if ((count.max_count != NOT_RESTRICTED && node->Children().size() > count.max_count) || (count.min_count != NOT_RESTRICTED &&  node->Children().size() < count.min_count))
  {
    std::string error = "Invalid parameter count for function '" + node->Name() + "'. Expected ";
  
    if (count.max_count != count.min_count && count.min_count != NOT_RESTRICTED && count.max_count != NOT_RESTRICTED)
    {
      error += std::to_string(count.min_count) + "-" + std::to_string(count.max_count);
    }
    else
    {
      error += std::to_string(count.min_count);
    }
    error += " parameters but was " + std::to_string(node->Children().size());

    throw ParameterCountMismatchError(error, node->DeclarationLine());
  }

 /* // horrible hack. Clean this up
  if (node->Name() == FN_MAP)
  {
    std::string function_param_name = (node->Children()[1]->
    

  }*/

}



void SemanticAstVisitor::CheckUserFunctionParameterCount(FunctionCallNode *node)
{
  int expected_count = m_functions[node->Name()].ParameterCount();
  int actual_count = node->Children().size();
  if (expected_count != actual_count)
  {
    throw ParameterCountMismatchError("Invalid parameter count for function '" + node->Name() + "'.  Expected "
      + std::to_string(expected_count) + " but was " + std::to_string(actual_count), node->DeclarationLine());
  }

}




// adds metadata to the if-node so that code generator can set the jump points correctly
void SemanticAstVisitor::TransformIf(FunctionCallNode * node)
{
  auto boolean_node = node->Children()[0];
  auto then_node = node->Children()[1];
  
 
  
  auto boolean_meta_node = CreateIfBooleanNode();
  auto then_meta_node = CreateIfThenNode();

  node->RemoveChild(boolean_node);
  node->RemoveChild(then_node);

  boolean_meta_node->AddChild(boolean_node);
  then_meta_node->AddChild(then_node);

  
  node->AddChildToLeft(then_meta_node);
  node->AddChildToLeft(boolean_meta_node);
}
