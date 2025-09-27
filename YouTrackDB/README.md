# Thread-safe Red-Black Tree

A simple thread-safe implementation of a Red-Black Tree that supports insertion and search operations.

Keys and values are stored as `std::vector<uint8_t>`.

## Files
- `main.cpp`              – main.  
- `CTree.h` + `CTree.cpp` – implementation of the custom thread-safe `CTree` class.  
- `tests.h`               – basic and extended test scenarios.

## Thread Safety
- Read operations (`find`) use shared locks for concurrent access.  
- Write operations (`insert`) use exclusive locks to prevent race conditions.

## Compilation

g++ main.cpp CTree.cpp -std=c++20
