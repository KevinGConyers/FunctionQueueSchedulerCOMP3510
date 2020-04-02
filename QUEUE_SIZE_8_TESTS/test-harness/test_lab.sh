#!/bin/bash

declare -a DEVS=(2 4 8)
declare -a MUs=(30 60 90)
mkdir /home/u3/kgc0019/COMP3510/projects/lab3/QUEUE_SIZE_8_TESTS/TEST_RESULTS
RESULT_PATH=/home/u3/kgc0019/COMP3510/projects/lab3/QUEUE_SIZE_8_TESTS/TEST_RESULTS
cd /home/u3/kgc0019/COMP3510/projects/lab3/code/
cc -o lab3 lab3.c devices3.o -lm -DEIGHT_QUEUE
for DEVICE in "${DEVS[@]}"
do
	for MU in "${MUs[@]}"
	do
		if [[ $DEVICE -lt 8 ]]
		then
			echo "Starting test with $DEVICE devices, Lambda of 2, MU of $MU and show = 1" 
			./lab3 $DEVICE 2 $MU 1 >> "$RESULT_PATH/DEVICES_$DEVICE LAMBA 2 MU_$MU"
			echo "Test Done"
		fi
		if [[ $DEVICE -eq 8 ]]
		then
			if [[ $MU -lt 60 ]]
			then
				echo "Starting test with $DEVICE devices, Lambda of 4, MU of $MU and show = 1" 
				./lab3 $DEVICE 4 $MU 1 >> "$RESULT_PATH/DEVICES_$DEVICE LAMBA 4 MU_$MU"
				echo "Test Done"
			else
				echo "Starting test with $DEVICE devices, Lambda of 6, MU of $MU and show = 1" 
				./lab3 $DEVICE 6 $MU 1 >> "$RESULT_PATH/DEVICES_$DEVICE LAMBA 6 MU_$MU"
				echo "Test Done"

			fi
		fi

	done
done

