## Build

```shell
# Clone repo
git clone https://github.com/E1pp/cmp_lattice.git
cd cmp_lattice
mkdir build && cd build
# Generate build files
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15 -DCMAKE_C_COMPILER=/usr/bin/clang-15 ..
# Build main
make main
# Run main
./src/main
```