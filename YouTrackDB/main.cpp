#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <cassert>
#include <thread>
#include <optional>
#include <climits>
#include "CMapWrapper.h"

using byteVector = std::vector<uint8_t>;

/** A simple function to convert an integer into 4 bytes of its binray representation
*/
byteVector toByteVector(int i) {
  byteVector res;
  int mask = 0xff000000;
  for (int j = 3; j >= 0; --j) {
      res.push_back((mask & i) >> (j * 8));
      mask >>= 8;
  }
  return res;
}

CMapWrapper testMap;

void test1(int mod) { 
  for (int i = 0; i < 100000; ++i)
    if (i % 2 == mod) {
      byteVector key = toByteVector(i);
      testMap.insert(key, key);
    }

  for (int i = 0; i < 100000; ++i)
    if (i % 2 == mod) {
      byteVector key = toByteVector(i);
      assert(testMap.find(key) != std::nullopt);
    }
}

int main(void) {
  std::thread t1 (test1, 0);
  std::thread t2 (test1, 1);

  t1.join();
  t2.join();

  return 0;
}
