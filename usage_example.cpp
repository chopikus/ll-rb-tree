#include "src/set.h"
#include <iostream>
#include <memory>

using namespace std;

int main() {
   std::unique_ptr<Node<int> > node = std::make_unique<Node<int> >(12);
   std::unique_ptr<Node<int> > node2 = std::make_unique<Node<int> >(11);
   std::unique_ptr<Node<int> > node3 = std::make_unique<Node<int> >(36);
   Set<int> set;
   set.insert(5);
   set.insert(4);
   set.insert(3);
   set.insert(2);
   Node<int>* node4 = set.find_node(10);
   std::cout << bool(node4) << std::endl;
   std::cout << bool(set.find_node(5)) << std::endl;
   std::cout << bool(set.find_node(4)) << std::endl;
   std::cout << bool(set.find_node(3)) << std::endl;
   std::cout << bool(set.find_node(2)) << std::endl;
   std::cout << bool(set.find_node(1)) << std::endl;
   return 0;
}
