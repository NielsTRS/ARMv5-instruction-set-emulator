#!/usr/bin/env bash

make_option=false
run_option=false
debug_option=false

while getopts ":mrd" opt
do
  case $opt in
    m)
      make_option=true
      ;;
    r)
      run_option=true
      ;;
    d)
      debug_option=true
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
  if $debug_option
  then
    ./arm_simulator --gdb-port 50000 --irq-port 50001 --trace-memory --trace-registers --trace-position --trace-state USR --trace-file trace.txt > run.log 2>&1 &
  else
    ./arm_simulator --gdb-port 50000 --irq-port 50001 > run.log 2>&1 &
  fi
  gdb-multiarch
fi