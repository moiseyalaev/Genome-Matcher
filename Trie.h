#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <iostream>  // REMEMBER: GET RID OF + COUT

template <typename ValueType>
class Trie
{
public:
  Trie();
  ~Trie();
  void reset();
  void insert(const std::string &key, const ValueType &value);
  std::vector<ValueType> find(const std::string &key, bool exactMatchOnly) const;

  // C++11 syntax for preventing copying and assignment
  Trie(const Trie &) = delete;
  Trie &operator=(const Trie &) = delete;

private:
  struct Node
  {
    Node();

    std::vector<ValueType> m_values; // a vector with all the values
    std::vector<Node *> m_children;  // a vector of Node pointers to track children
  };

  Node *m_root;

  std::vector<ValueType> lookup(Node *curr, std::string key, bool exactMatchOnly) const;
  void eraseTrie(Node *curr);
};

template <typename ValueType>
Trie<ValueType>::Node::Node()
{
  for (int i = 0; i < 255; i++)
    m_children.push_back(nullptr);
}

template <typename ValueType>
Trie<ValueType>::Trie() // goal: O(1)
{
  m_root = new Node; // null byte when not fixed
}

template <typename ValueType>
Trie<ValueType>::~Trie() // goal: O(N)
{
  eraseTrie(m_root);
}

template <typename ValueType>
void Trie<ValueType>::reset() // goal: O(N)
{
  eraseTrie(m_root);
  m_root = new Node;
}

template <typename ValueType>
void Trie<ValueType>::insert(const std::string &key, const ValueType &value) // goal: O(LC) L =length of the inserted key,
{                                                                            // C = average number of children per node in your trie
  Node *curr = m_root;                                                       // node currently looking at

  for (char ch : key)
  {
    if (curr->m_children[ch] == nullptr) // input the char as the index    // if the input doesnt have a node yet, create one
      curr->m_children[ch] = new Node;

    curr = curr->m_children[ch];
  }

  curr->m_values.push_back(value); // update value at at the detemined node
}

template <typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string &key, bool exactMatchOnly) const
{

  if (m_root->m_children[key[0]] == nullptr) // if the first char of the string doesnt match, return an an empty vector
    return std::vector<ValueType>();

  return lookup(m_root->m_children[key[0]], key.substr(1), exactMatchOnly);
}

//================================================== Private functions ======================================

template <typename ValueType>
std::vector<ValueType> Trie<ValueType>::lookup(Node *curr, std::string key, bool exactMatchOnly) const
{
  if (curr == nullptr)
    return std::vector<ValueType>(); // return empty vector
  if (key.size() == 0)               // when the string is reached the bottom of trie
    return curr->m_values;           // return the vector of values

  std::vector<ValueType> finalValues;

  if (exactMatchOnly) // check for only exact matches
  {
    if (curr->m_children[key[0]] != nullptr)
    {
      std::vector<ValueType> currValues = lookup(curr->m_children[key[0]], key.substr(1), exactMatchOnly); // get the values from looking up the children
      finalValues.insert(finalValues.end(), currValues.begin(), currValues.end());
    }
    else
      return std::vector<ValueType>();
  }
  else
  {
    if (curr->m_children[key[0]] != nullptr)
    {
      std::vector<ValueType> currValues = lookup(curr->m_children[key[0]], key.substr(1), false); // get the values from looking up the children
      finalValues.insert(finalValues.end(), currValues.begin(), currValues.end());
    }
    for (int i = 0; i < 255; i++)
    {
     if ((curr->m_children[i] != nullptr) && (i != key[0])) 
     {
      std::vector<ValueType> currValues = lookup(curr->m_children[i], key.substr(1), true); // get the values from looking up the children
      finalValues.insert(finalValues.end(), currValues.begin(), currValues.end());
     }
    }
  }

  return finalValues;
}

template <typename ValueType>
void Trie<ValueType>::eraseTrie(Node *curr)
{
  if (curr == nullptr)
    return;

  if (curr->m_children.size() == 0)
  {
    delete curr;
    return;
  }

  for (Node *child : curr->m_children)
    eraseTrie(child);
}

#endif // TRIE_INCLUDED


