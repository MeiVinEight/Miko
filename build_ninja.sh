#!/bin/bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja
cmake --build . --config Release -j 12
