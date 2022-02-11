<div align="center">
  <h1> Left-leaning Red-black Tree 🌳 </h1>

This repository aims at making the best implementation of the [LLRB-tree data structure](https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf).
  
The data structure resembles a usual Red-Black tree, remade with simplicity in mind.
  
Unlike lots of another implementations ([here](https://www.geeksforgeeks.org/left-leaning-red-black-tree-insertion/) and [here, in russian](http://neerc.ifmo.ru/wiki/index.php?title=%D0%9B%D0%B5%D0%B2%D0%BE%D1%81%D1%82%D0%BE%D1%80%D0%BE%D0%BD%D0%BD%D0%B8%D0%B5_%D0%BA%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D1%91%D1%80%D0%BD%D1%8B%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D1%8C%D1%8F) ), `ll-rb-tree` 🌳 actually supports deletion.

The codebase is thoroughly reviewed and tested using the [gtest](https://github.com/google/googletest) library.
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

### Adding to another project

1. Clone file `src/set.h`
2. Copy it to your project
3. Include as any other header: `#include "set.h"`
4. After including `set.h` file, `Set` and `Set::iterator` classes can be used.

### Testing
1. Clone this repository -- `git clone https://github.com/chopikus/ll-rb-tree.git`.
2. Open the cloned folder -- `cd ll-rb-tree`.
3. Create new folder for building tests, and open it -- `mkdir build && cd build`.
4. Run the following command: `cmake .. && make tests`.

All tests are written in `tests/tests.cpp` file.

## Benchmarks
### Insertion

### Deletion

## How does it work?
TODO

## Contrubuting
