#include "CTree.h"

std::pair<bool, byteVector> CTree::find(const byteVector& key) {
  // Shared lock allows several readers read simultaniously
  std::shared_lock<std::shared_mutex> lock(m_Mutex);
  if (!m_Size) return {false, {}};

  CNode* current = m_Root;
  while (current) {
    if (key == current->m_Key) return {true, current->m_Val};
    else current = key < current->m_Key ? current->m_Left : current->m_Right;
  }
  return {false, {}};
}

bool CTree::insert(const byteVector& key, const byteVector& value) {
  // Unique lock allows exclusive access for writers
  std::unique_lock<std::shared_mutex> lock(m_Mutex);
  if (m_Size == 0) {
    m_Root = new CNode(key, value, nullptr, color::BLACK);
    ++m_Size;
    return true;
  }

  CNode** current = &m_Root;
  CNode* parent = nullptr;

  while (*current) {
    parent = *current;
    if (key == (*current)->m_Key) return false;
    else current = key < (*current)->m_Key ? &(*current)->m_Left : &(*current)->m_Right;    
  }
  
  // Inserted node is always RED which may lead to the need of rebalancing
  *current = new CNode(key, value, parent, color::RED);

  ++m_Size;
  insertFix(*current);
  return true;
}

void CTree::rotateLeft(CNode* node) {
  CNode* tmp = node->m_Right;

  node->m_Right = tmp->m_Left;
  if (node->m_Right)
    node->m_Right->m_Parent = node;

  tmp->m_Parent = node->m_Parent;
  if (!node->m_Parent)
    m_Root = tmp;  
  else if (node->m_Parent->m_Left == node)
    node->m_Parent->m_Left = tmp;
  else 
    node->m_Parent->m_Right = tmp;

  tmp->m_Left = node;
  node->m_Parent = tmp;
}

void CTree::rotateRight(CNode* node) {
  CNode* tmp = node->m_Left;

  node->m_Left = tmp->m_Right;
  if (node->m_Left)
    node->m_Left->m_Parent = node;

  tmp->m_Parent = node->m_Parent;
  if (!node->m_Parent)
    m_Root = tmp;  
  else if (node->m_Parent->m_Left == node)
    node->m_Parent->m_Left = tmp;
  else 
    node->m_Parent->m_Right = tmp;

  tmp->m_Right = node;
  node->m_Parent = tmp;
}

void CTree::insertFix(CNode* node) {
  // Iteratively propagates the changes from "problematic" node to the root
  // using Red-Black Tree rebalancing strategy
  while (node != m_Root && node->m_Parent->m_Color == color::RED) {
    if (node->m_Parent == node->m_Parent->m_Parent->m_Left) {
      CNode* uncle = node->m_Parent->m_Parent->m_Right;

      if (!uncle || uncle->m_Color == color::BLACK) {
        if (node == node->m_Parent->m_Right) {
          node = node->m_Parent;
          rotateLeft(node);
        }
        node->m_Parent->m_Color = color::BLACK;
        node->m_Parent->m_Parent->m_Color = color::RED;
        rotateRight(node->m_Parent->m_Parent);
      } else {
        node->m_Parent->m_Parent->m_Color = color::RED;
        uncle->m_Color = color::BLACK;
        node->m_Parent->m_Color = color::BLACK;
        node = node->m_Parent->m_Parent;
      }
    } else {
      CNode* uncle = node->m_Parent->m_Parent->m_Left;

      if (!uncle || uncle->m_Color == color::BLACK) {
        if (node == node->m_Parent->m_Left) {
          node = node->m_Parent;
          rotateRight(node);
        }
        node->m_Parent->m_Color = color::BLACK;
        node->m_Parent->m_Parent->m_Color = color::RED;
        rotateLeft(node->m_Parent->m_Parent);
      } else {
        node->m_Parent->m_Parent->m_Color = color::RED;
        uncle->m_Color = color::BLACK;
        node->m_Parent->m_Color = color::BLACK;
        node = node->m_Parent->m_Parent;
      }
    }
  }
  // Root is always BLACK
  m_Root->m_Color = color::BLACK;
}

void CTree::printTree(void) {
  printTreeRec(m_Root, "", true);
}

void CTree::printTreeRec(CNode * node, std::string pref, bool last) {
  if (node) {
    std::cout<<pref;
    if (last) {
      std::cout<<"R-->";
      pref += "    ";
    }
    else {
      std::cout<<"L-->";
      pref += "|   ";
    }
    for (const auto& it: node->m_Val)
      printf("%02x ", it);
    std::cout << " (" << (node->m_Color == color::RED ? "R" : "B") << ")"<<std::endl;

    printTreeRec(node->m_Left, pref, false);
    printTreeRec(node->m_Right, pref, true);
  }
}