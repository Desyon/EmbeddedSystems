/**
 * Created by Til on 06.05.2017.
 */

#include <iostream>
#include "../include/PoolAllocator.h"


int main() {
  CREATE(test10, 10, sizeof(int));
  CREATE(test1, 1, sizeof(int));

  // Testing
  /* Test 1 - Allocate() */
  std::cout << "Test 1.1:\t" << test10.Available() << std::endl;
  int *tp1 = static_cast<int *>(test10.Allocate(sizeof(int)));
  std::cout << "Test 1.2:\t" << test10.Available() << std::endl;
  int *tp2 = static_cast<int *>(test10.Allocate(sizeof(int)));
  std::cout << "Test 1.3:\t" << test10.Available() << std::endl;

  /* Test 2 - Deallocate() */
  std::cout << "Test 2.1:\t" << test10.Available() << std::endl;
  test10.Deallocate(tp1);
  std::cout << "Test 2.2:\t" << test10.Available() << std::endl;
  test10.Deallocate(tp2);
  std::cout << "Test 2.2:\t" << test10.Available() << std::endl;

  /* Test 3 - Allocate more than available */
  std::cout << "Test 3.2:\t" << test1.Available() << std::endl;
  tp1 = static_cast<int *>(test1.Allocate(sizeof(int)));
  std::cout << "Test 3.2:\t" << test1.Available() << std::endl;
  test1.Allocate(sizeof(int));
  std::cout << "Test 3.3:\t" << test1.Available() << std::endl;

  /* Test 4 - Deallocate unavailable address */
  std::cout << "Test 4.1\t" << test1.Available() << std::endl;
  test1.Deallocate(tp1);
  std::cout << "Test 4.2\t" << test1.Available() << std::endl;
  test1.Deallocate(tp1+1);
  std::cout << "Test 4.3\t" << test1.Available() << std::endl;
}