#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

class AstNode;


enum class NodeType { ROOT, FUNCTION_CALL, NUMBER };

union NodeValue
{
  double number;
  char *text;
};

typedef std::shared_ptr < AstNode > Ast_Node;

class AstNode
{
public:
  AstNode(NodeType type);


  ~AstNode();
    
  void AddChild(Ast_Node node)
  {
    node->m_parent = this;
    m_children.push_back(std::move(node));
  }

  void RemoveChild(Ast_Node node)
  {
    for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
      if ((*iter) == node)
      {
        m_children.erase(iter);
        return;
      }
    }
    
  }

  AstNode *Parent()
  {
    return m_parent;
  }

  std::vector<Ast_Node> Children()
  {
    return m_children;
  }

  void SetType(NodeType type)
  {
    m_type = type;
  }

  NodeType Type()
  {
    return m_type;
  }

  void ValueAsNumber(const std::string &number)
  {
    MaybeReleaseMemory();
    m_value.number = std::stod(number);
  }

  double ValueAsNumber()
  {
    return m_value.number;
  }

  void ValueAsText(const std::string &text)
  {
    MaybeReleaseMemory();
    
    m_value.text = new char[text.length() + 1];

#pragma warning( disable : 4996)
    strncpy(m_value.text, text.c_str(), text.length()+1);

    m_value.text[text.length()] = '\0';
    m_release_char = true;
  }

  std::string ValueAsText()
  {
    return std::string(m_value.text);
  }
  
private:

  void MaybeReleaseMemory();
  NodeType m_type;
  AstNode *m_parent;

  bool m_release_char;
  std::vector<Ast_Node> m_children;
  NodeValue m_value;

};

