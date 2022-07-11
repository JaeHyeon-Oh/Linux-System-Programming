#!/bin/bash
whatDay(){
let sum=$day
let month=month-1
monthOfYear=("31","28","31","30","31","30","31","31","30","31","30","31")
for((i=0;i<month-1;i++))
do
	sum=$((sum+monthOfYear[i]))
done
let num=$sum%7
case $num in 
"0") echo "Thursday";;
"1") echo "Friday";;
"2") echo "Saturday";;
"3") echo "Sunday";;
"4") echo "Monday";;
"5") echo "Tuesday";;
"6") echo "wednesday";;
esac
}
echo "Month Day"
read month day
whatDay $month $day
