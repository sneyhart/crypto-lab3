#!/bin/bash

array=$(ls | cat)
for entry in $array
do
  cat $entry
done
