#include "src/set.h"
#include <iostream>

using namespace std;

int main() {
   Node<int> node{12};
   Node<int> node2{node};
   Node<int> node3{24};
   node3 = node;
   std::cout << node2.value << std::endl;
   std::cout << node3.value << std::endl;
   return 0;
}
