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
ver1=$(./rsa-validate -k "$val" -m "$pub" -s "$pubc")
ver2=$(./rsa-validate -k "$pub" -m "skm.txt" -s "signed.txt")
echo "Verified locking party public key: $ver1"
echo "Verified symetric key manifest: $ver2"

./rsa-dec "-k" "$priv" "-i" "skm.txt" "-o" "keys.txt"

array=$(ls $dir| cat)
base=""

for entry in $array
do
  tmp=$dir
  tmp+="/"
  tmp+=$entry
  entry=$tmp
  base=${entry%+*}
  encent="$base+enc"
  tagent="$base+tag"
  echo $base
  if [ "$entry" != "$encent"  ] ; then
   	continue
  fi 
  ver3=$(./cbcmac-validate "-k" "key.txt" "-m" "$encent" "-t" "$tagent")
  ./cbc-dec -k key.txt -i "$encent" -o "$base" -v key.txt
  echo "$var3"
  #echo $entry
done


