#!/bin/bash
# Script to kind of speed up compiling os

make localmodconfig
make menuconfig

make -j2 CC="ccache gcc"
make -j2 modules_install
make -j2 install
