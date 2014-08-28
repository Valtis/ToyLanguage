#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
class AstNode;


enum class NodeType { ROOT, FUNCTION_CALL, NUMBER, VARIABLE };

union NodeValue
{
  double number;
  char *text;
  int integer;
};

typedef std::shared_ptr < AstNode > Ast_Node;

class AstNode
{
public:
  AstNode(NodeType type, int line);


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

  void ReplaceChild(Ast_Node old_node, Ast_Node new_node)
  {
    for (size_t i = 0; i < m_children.size(); ++i)
    {
      if (m_children[i] == old_node)
      {
        new_node->m_parent = this;
        m_children[i] = new_node;
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

  void ValueAsInteger(int value)
  {
    MaybeReleaseMemory();
    m_value.integer = value;
  }

  int ValueAsInteger()
  {
    return m_value.integer;
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

  std::string ValueAsText() const
  {
    return std::string(m_value.text);
  }

  int DeclarationLine() const
  {
    return m_declaration_line;
  }
  
private:

  void MaybeReleaseMemory();
  NodeType m_type;
  AstNode *m_parent;

  bool m_release_char;
  std::vector<Ast_Node> m_children;
  NodeValue m_value;
  int m_declaration_line;

};

