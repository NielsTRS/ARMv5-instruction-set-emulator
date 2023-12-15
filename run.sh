#!/usr/bin/env bash

make_option=false
run_option=false

while getopts ":mr" opt
do
  case $opt in
    m)
      make_option=true
      ;;
    r)
      run_option=true
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

if $run_option
then
  ./arm_simulator --gdb-port 50000 --irq-port 50001 &
  gdb-multiarch
fi

if ! $make_option && ! $run_option
then
  make distclean
  ./configure CFLAGS='-Werror -Wall -g'
  make
  ./arm_simulator --gdb-port 50000 --irq-port 50001 &
  gdb-multiarch
fi