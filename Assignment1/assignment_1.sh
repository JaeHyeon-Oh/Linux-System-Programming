#!/bin/bash

PrimeNumSum(){
let sum=0
if((a> b))
then 	
	let i=a
	let a=b
	let b=i
fi

for((;a<=b;a++))
do
	for((i=2;i<=a;i++))
	do
		if((a%i==0))
		then
			break
		fi
	done
	if((i==a))
	then
		sum=$((sum+a))
	fi
done
echo "$sum"
}

read a b
PrimeNumSum $a $b
