#include "tests.h"

int main(void) {
  testTreeBasic();  
  testTreeBig();
  testTreeRand();

  CTestTwoThreads t1;
  t1.run();

  CTestManyReaders t2;
  t2.run();

  return 0;
}
