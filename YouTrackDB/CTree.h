#ifndef CTREE_H
#define CTREE_H

#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <mutex>
#include <shared_mutex>

using byteVector = std::vector<uint8_t>;

/**
 * @brief Thread-safe Red-Black Tree implementation
 *
 * This class provides a balanced binary search tree with insert and find
 * operations. The tree uses a Red-Black balancing strategy to guarantee
 * O(log n) lookup and insertion complexity
 *
 * - Keys and values are stored as byte vectors (std::vector<uint8_t>)
 * - Thread safety is ensured using std::shared_mutex:
 *    - Shared access for find
 *    - Exclusive access for insert
 */
class CTree {
    // Each node in Red-Black Tree can have either RED of BLACK "color"
    enum color{RED, BLACK};

    // Basic BST node that stores key, value and three pointers to left child, right child and parent nodes
    // Red-Black Tree implementation requires to store one extra variable containing "color" of the node
    struct CNode {
      CNode(const byteVector& key, const byteVector& val, CNode* parent, color color)
      : m_Key(key),
        m_Val(val), 
        m_Left(nullptr), 
        m_Right(nullptr), 
        m_Parent(parent), 
        m_Color(color) {}
      ~CNode() {
        delete m_Left;
        delete m_Right;
      }

      byteVector m_Key;
      byteVector m_Val;
      CNode* m_Left;
      CNode* m_Right;
      CNode* m_Parent;
      color m_Color;
    };

    std::shared_mutex m_Mutex;
    size_t m_Size = 0;
    CNode* m_Root = nullptr;
    // Simple method that implemets LEFT rotation for rebalancing
    void rotateLeft(CNode* node);
    // Simple Method that implemets RIGHT rotation for rebalancing
    void rotateRight(CNode* node);
    // Method that fixes possible inconsistencies in Reb-Black Tree structure after the insertion 
    void insertFix(CNode* node);
    // Helping method for plotting
    void printTreeRec(CNode * node, std::string pref, bool last);
  public:
    CTree() = default;
    ~CTree() {delete m_Root;}
    // Helping method that plots the entire tree
    void printTree(void);
    // Thread-safe searching method
    std::pair<bool, byteVector> find(const byteVector& key);
    // Thread-safe insertion method
    bool insert(const byteVector& key, const byteVector& value);
};

#endif // CTREE_H