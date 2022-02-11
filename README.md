<div align="center">
  <h1> Left-leaning Red-black Tree 🌳 </h1>

This repository aims at making the best implementation of the [LLRB-tree data structure](https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf).
  
The data structure resembles a usual Red-Black tree, remade with simplicity in mind.
  
Unlike lots of another implementations ([here](https://www.geeksforgeeks.org/left-leaning-red-black-tree-insertion/) and [here, in russian](http://neerc.ifmo.ru/wiki/index.php?title=%D0%9B%D0%B5%D0%B2%D0%BE%D1%81%D1%82%D0%BE%D1%80%D0%BE%D0%BD%D0%BD%D0%B8%D0%B5_%D0%BA%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D1%91%D1%80%D0%BD%D1%8B%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D1%8C%D1%8F) ), `ll-rb-tree` 🌳 actually supports deletion.

The codebase was thoroughly reviewed and tested using the [gtest](https://github.com/google/googletest) library.
</div>

## Features
* `insert`;
* `erase` (__and__ `eraseMin` __and__ `eraseMax`);
* `lower_bound`;
* `find`;
* iterators, `begin()`, `end()`;
* initalizing from `std::initializer_list`;
* copy constructor;
* and many more!

## Usage

All needed implementation is written in the `src/set.h` file.

The usage is very similar to `std::set`.

### Avaliable classes

* `Set<ValueType>` - set that stores `ValueType` elements;
* `Set<ValueType>::iterator` - set iterator, can be got using `begin()`, `end()`, `find(x)`, `lower_bound(x)`.

### Avaliable functions and constructors

* `Set()` - empty constructor

   Example: `Set<int> my_set;`
* `Set(Set& another)` - constructor that copies all elements
 
   Example: `Set<int> my_set(another_set);`
* `Set(const std::initializer_list<ValueType> &list)` - creating `Set<ValueType>`, copying elements out of `list`.

   Example: `Set<int> my_set{1, 2, 3}`.
*  `void insert(const ValueType& value)` - adds element to `Set<ValueType>`;
*  `void erase(const ValueType& value)` - removes element `value` if present in `Set<ValueType>`;
*  `void erase(const Set<ValueType>::iterator& iterator)` - removes element by iterator;
*  `bool empty()` - returns `true` if `Set<ValueType>` is empty;
*  `size_t size()` - returns amount of elements in `Set<ValueType>`;
*  `Set<ValueType>::iterator begin()` - returns iterator to the first element of `Set`
*  `Set<ValueType>::iterator end()` - returns iterator to past-the-end element of `Set`

Avaliable `Set<ValueType>::iterator` methods:
* `++`, `--` - changing iterator's element to next/previous;
* `*` - returns the element correspondant to the iterator;
* `->` - returns a reference to the element correspondant to the iterator.
* `==`, `!=` - comparing iterators.

### Code example (C++)
This program reads characters and numbers line by line and modifies data structure accoring to the input.
* `+ x` means adding number `x` to the Set;
* `- x` - removing number `x` from the Set; 
* `> x` prints first stored number, that's greater or equal to `x`;
* `?` - prints all numbers stored in Set.
```
#include <algorithm>
#include <iostream>
#include <vector>
#include "src/set.h"

using namespace std;

int main() {
    int q=0;
    cin >> q;
    int cnt=0;
    Set<int> my_set;
    for (int i=0; i<q; i++) {
        char x='1';
        cin >> x;
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            my_set.insert(value);
        } else if (x=='?') {
            for (auto s : my_set) {
                cout << s << ' ';
            }
            cout << endl;
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            my_set.erase(value);
        } else if (x == '>') {
            int y=0;
            cin >> y;
            if (my_set.lower_bound(y) != my_set.end()) {
                cout << *(my_set.lower_bound(y)) << endl;
            }
        }
    }
    return 0;
}
```

As you can see, the usage is very similar to the `std::set`.

### Testing
1. Clone this repository -- `git clone https://github.com/chopikus/ll-rb-tree.git`.
2. Open the cloned folder -- `cd ll-rb-tree`.
3. Create new folder for building tests, and open it -- `mkdir build && cd build`.
4. Run the following command: `cmake .. && make tests`.

All tests are written in `tests/tests.cpp` file.

## File structure
TODO 
## Benchmarks
### Insertion

### Deletion

## How does it work?
TODO

## Contrubuting
