#ifndef TESTS_H
#define TESTS_H

#include <cassert>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include "CTree.h"

using byteVector = std::vector<uint8_t>;

// Simple function that converts integer 'i' into four bytes of its binary representation 
byteVector toByteVector(int i) {
  byteVector res;
  int mask = 0xff000000;
  for (int j = 3; j >= 0; --j) {
      res.push_back((mask & i) >> (j * 8));
      mask >>= 8;
  }
  return res;
}

// Basic insertion and search testing
void testTreeBasic (void) {
  CTree t;
  
  assert(t.insert({0x0, 0x0}, {0xff, 0xff}));
  assert(t.insert({0x0, 0x1}, {0xff, 0xfe}));
  assert(t.insert({0x0, 0x2}, {0xff, 0xfd}));
  assert(t.insert({0x0, 0x3}, {0xff, 0xfc}));
  assert(t.insert({0x0, 0x4}, {0xff, 0xfb}));
  assert(t.insert({0x0, 0x5}, {0xff, 0xfa}));
  

  assert(t.find({0x0, 0x0}).first && t.find({0x0, 0x0}).second == byteVector({0xff, 0xff}));
  assert(t.find({0x0, 0x1}).first && t.find({0x0, 0x1}).second == byteVector({0xff, 0xfe}));
  assert(t.find({0x0, 0x2}).first && t.find({0x0, 0x2}).second == byteVector({0xff, 0xfd}));
  assert(t.find({0x0, 0x3}).first && t.find({0x0, 0x3}).second == byteVector({0xff, 0xfc}));
  assert(t.find({0x0, 0x4}).first && t.find({0x0, 0x4}).second == byteVector({0xff, 0xfb}));
  assert(t.find({0x0, 0x5}).first && t.find({0x0, 0x5}).second == byteVector({0xff, 0xfa}));

  assert(!t.insert({0x0, 0x0}, {0xff, 0xfa}));
  assert(!t.insert({0x0, 0x1}, {0xff, 0xfa}));
  assert(!t.insert({0x0, 0x2}, {0xff, 0xfa}));
  assert(!t.insert({0x0, 0x3}, {0xff, 0xfa}));
  assert(!t.insert({0x0, 0x4}, {0xff, 0xfa}));
  assert(!t.insert({0x0, 0x5}, {0xff, 0xfa}));
}

// Insertion and search testing for a big number of data
void testTreeBig (void) {
  CTree t;

  for (uint8_t i = 0; i < 255; ++i)
    for (uint8_t j = 0; j < 255; ++j)
      assert(t.insert({i, j}, {j, i}));

  for (uint8_t i = 0; i < 255; ++i)
    for (uint8_t j = 0; j < 255; ++j)
      assert(t.find({i, j}).first && t.find({i, j}).second == byteVector({j, i}));
}

// Randomized insertion and search testing
void testTreeRand (void) {
  int seed = 42;
  CTree t;

  std::srand(seed);
  for (int i = 0; i < 1000; ++i) {
    int k = std::rand();
    auto tmp = toByteVector(k);
    assert(t.insert(tmp, tmp));
  }

  std::srand(seed);
  for (int i = 0; i < 1000; ++i) {
    int k = std::rand();
    auto tmp = toByteVector(k);
    assert(t.find(tmp).first && t.find(tmp).second == byteVector(tmp));
  }
}

// Test class for concurrent insertion and search operations
// Two threads run simultaneously:
//   - The first thread inserts and looks up even numbers
//   - The second thread inserts and looks up odd numbers
class CTestTwoThreads{
    CTree m_Tree;
    void test1_inner(int mod) {
      for (int i = 0; i < 100000; ++i)
        if (i % 2 == mod) {
          byteVector key = toByteVector(i);
          assert(m_Tree.insert(key, key));
        }
    
      for (int i = 0; i < 100000; ++i)
        if (i % 2 == mod) {
          byteVector key = toByteVector(i);
          assert(m_Tree.find(key).first && (m_Tree.find(key).second == key));
        }
    }
  public:
    void run(void) {
      std::thread thread1 (&CTestTwoThreads::test1_inner, this, 0);
      std::thread thread2 (&CTestTwoThreads::test1_inner, this, 1);
    
      thread1.join();
      thread2.join();
    }
};

// Test class for concurrent insertion and search operations with one writer and many readers
// - The writer inserts elements from 0 to 100000
// - Each reader starts at position 'i' and looks up elements with a step size of 'n_readers'
class CTestManyReaders{
    CTree m_Tree;
    
    void reader(int first, int n_readers) {
      for (int i = first; i < 100000; i += n_readers) {
        byteVector key = toByteVector(i);
        if (!m_Tree.find(key).first) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          continue;
        }
        assert(m_Tree.find(key).first && (m_Tree.find(key).second == key));
      }
    }

    void writer(void) {
      for (int i = 0; i < 100000; ++i) {
        byteVector key = toByteVector(i);
        assert(m_Tree.insert(key, key));
      }
    }        
      
  public:
    void run(size_t readers = 100) {
      std::vector<std::thread> vec;

      for (size_t i = 0; i < readers; ++i)
        vec.emplace_back(&CTestManyReaders::reader, this, i, readers);
        
      std::thread t(&CTestManyReaders::writer, this);

      for (auto& it: vec)
        it.join();

      t.join();
    }
};

#endif // TESTS_H