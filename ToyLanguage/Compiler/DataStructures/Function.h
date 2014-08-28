#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "AstNode.h"

class Function
{
public:
  Function();
  Function(std::string name, int declaration_line);
  ~Function();

  void SetRootNode(std::shared_ptr<AstNode> node);
  std::shared_ptr<AstNode> RootNode() { return m_root_node; }
  std::string Name() const { return m_name;  }

  int DeclarationLine() const { return m_declaration_line;  }


  bool HasParameter(std::string parameter)
  {
    return std::find(m_parameters.begin(), m_parameters.end(), parameter) != m_parameters.end();
  }

  void AddParameter(std::string parameter)
  {
    m_parameters.push_back(parameter);
  }

  int ParameterID(std::string name)
  {
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
      if (m_parameters[i] == name)
      {
        return i;
      }
    }

    return -1;
  }


  int ParameterCount()
  {
    return m_parameters.size();
  }
  
private:
  int m_declaration_line;
  
  std::vector<std::string> m_parameters;

  std::string m_name;
  Ast_Node m_root_node;

};

