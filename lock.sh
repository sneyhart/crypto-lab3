#!/bin/bash

dir=""
pub=""
priv=""
val=""

for var in "$@"
do
	if [ "$dir" = "-d" ]; then
		dir="$var"
		continue
  	fi
	if [ "$pub" = "-p" ]; then
		pub="$var"
		continue
  	fi
	if [ "$priv" = "-r" ]; then
		priv="$var"
		continue
  	fi
	if [ "$val" = "-vk" ]; then
		val="$var"
		continue
  	fi
	if [ "$var" = "-d" ]; then
		dir="-d"
		continue
  	fi
	if [ "$var" = "-p" ]; then
		pub="-p"
		continue
  	fi	
	if [ "$var" = "-r" ]; then
		priv="-r"
		continue
  	fi	
	if [ "$var" = "-vk" ]; then
		val="-vk"
		continue
  	fi
done

echo "$dir"
echo "$pub"
echo "$priv"
echo "$val"

pubc="$pub-casig"
test=$(./rsa-validate -k "$val" -m "$pub" -s "$pubc")
./rsa-validate -k "$val" -m "$pub" -s "$pubc"
#echo $test

head -c 16 /dev/random > key.txt
./rsa-enc "-k" "$priv" "-i" "key.txt" "-o" "skm.txt"
./rsa-sign "-k" "keys/p1-priv" "-m" "skm.txt" "-s" "signed.txt"

array=$(ls $dir| cat)
for entry in $array
do
  tmp=$dir
  tmp+="/"
  tmp+=$entry
  entry=$tmp
  
  encent="$entry+enc"
  tagent="$entry+tag"
  ./cbc-enc "-k" "key.txt" "-i" "$entry" "-o" "$encent" "-v" "key.txt"
  ./cbcmac-tag "-k" "key.txt" "-m" "$encent" "-t" "$tagent"
  #./cbcmac-validate "-k" "key.txt" "-m" "$encent" "-t" "$tagent"
  rm entry
  echo $entry
done

rm keys.txt

