#!/usr/bin/env bash

make_option=false

while getopts ":m" opt; do
  case $opt in
    m)
      make_option=true
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if $make_option
then
  make distclean
  ./configure CFLAGS='-Werror -Wall -g'
  make
fi

./arm_simulator --gdb-port 50000 --irq-port 50001 &
gdb-multiarch
