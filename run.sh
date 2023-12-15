#!/usr/bin/env bash
make distclean
./configure CFLAGS='-Werror -Wall -g'
make
./arm_simulator --gdb-port 50000 --irq-port 50001 &
gdb-multiarch
