#!/bin/sh

# This script installs libyaml and libcyaml libraries required for parser to work

cd ~ || exit

git clone https://github.com/yaml/libyaml
cd libyaml || exit
./bootstrap
./configure
make
make install
cd ..
rm -rf libyaml

git clone https://github.com/tlsa/libcyaml
cd libcyaml || exit
make VARIANT=release
make install VARIANT=release
cd ..
rm -rf libcyaml
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
