#!/bin/bash
rm -f ./files/*
echo "$1"
LD_PRELOAD=./logger.so ./test_aclog -c $1
counter=0
FILES=./files/file*
filetype=".encrypt"
#echo hello allo |cut -f1
#find . -name -'file*'
for file in $FILES
do
	encryptedfile="$file$filetype"
	echo "file is :$encryptedfile"
	openssl enc -aes-256-ecb -in $file -out encryptedData.txt -k 1234
	output=$(cat encryptedData.txt)
	LD_PRELOAD=./logger.so ./test_aclog -e $output -s $encryptedfile

	counter=`expr $counter + 1`
	rm -f $file
done
#echo "counter is :$counter"


rm -rf encryptedData.txt
