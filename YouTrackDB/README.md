# Thread-safe version of std::map

Supported operations are insert and find.

Keys/values are represented by std::vector<uint8_t>.

## Files
`CMapWrapper.cpp` - impelemts the std::map wrapper
`main.cpp`        - provides basic multithreaded testing

## Compilation
g++ main.cpp -std=c++20