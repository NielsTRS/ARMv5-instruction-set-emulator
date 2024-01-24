#!/usr/bin/env bash
build_option=false

while getopts ":b" opt
do
  case $opt in
    b)
      build_option=true
      ;;
    \?)
      echo "Option invalide: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

if $build_option
then
  docker build -t projet_prog5 .
fi

docker run -it -v .:/app projet_prog5