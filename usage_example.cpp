#include "src/set.h"
#include <iostream>
#include <memory>

using namespace std;

int main() {
   llrb::Set<int> set;
   set.insert(5);
   set.insert(4);
   set.insert(3);
   set.insert(2);
   for (int i=1; i<=5; i++) {
      set.erase(i);
      std::cout << bool(set.find(5) == set.end()) << std::endl;
      std::cout << bool(set.find(4) == set.end()) << std::endl;
      std::cout << bool(set.find(3) == set.end()) << std::endl;
      std::cout << bool(set.find(2) == set.end()) << std::endl;
      std::cout << "====" << std::endl;
   }
   return 0;
}
