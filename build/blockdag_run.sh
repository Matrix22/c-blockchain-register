#!/bin/bash
function print_header {
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

ZERO=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=log_valgrind.txt"
TESTS=("test0.in" "test0.in" "test1.in" "test1.in" "test2.in" "test2.in" "test3.in" "test3.in" "test4.in" "test4.in")
NODES=("B" "H" "C" "D" "I" "E" "V3" "V5" "L" "C")
KVALUES=(3 4 4 3 2 4 5 3 2 5)

print_header "BLOCKCHAIN REGISTER"

make build > /dev/null 2>&1

echo "Check Valid DAG"
for i in {0..9}
do
	fileIn="tests/test"$i".in"
	fileRef="tests/test"$i"_1.ref"
	fileOut="blockdag.out"

	cp "$fileIn" "blockdag.in"
    rm $fileOut > /dev/null 2>&1

	timeout 20 ./blockdag -c1 > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?

	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi

	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "Test $idx .......................................................... PASS"
	else
		echo "Test $idx .......................................................... FAIL"
	fi
done

echo "Graph Sets"
for i in {0..9}
do
	fileIn="tests/"${TESTS[$i]}
	fileOut="blockdag.out"
	fileRef="tests/test"$i"_2.ref"

	cp "$fileIn" "blockdag.in"

	timeout 20 ./blockdag -c2 ${NODES[$i]} > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?

	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi

	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "Test $idx .......................................................... PASS"
	else
		echo "Test $idx .......................................................... FAIL"
	fi
done

echo "Valgrind Tests"
fileIn="tests/test9.in"
fileOut="blockdag.out"

cp "$fileIn" "blockdag.in"

$VALGRIND ./blockdag -c1 > /dev/null 2>&1

if [[ -z $(cat log_valgrind.txt) ]]; then
	printf "VALGRIND Test 1 .................................................. PASS\n"
else
	printf -- "VALGRIND Test 1 .................................................. FAIL\n"
fi

fileIn="tests/test3.in"
fileOut="blockdag.out"

cp "$fileIn" "blockdag.in"

$VALGRIND ./blockdag -c2 V3 > /dev/null 2>&1

if [[ -z $(cat log_valgrind.txt) ]]; then
	printf "VALGRIND Test 2 .................................................. PASS\n"
else
	printf -- "VALGRIND Test 2 .................................................. FAIL\n"
fi

make clean > /dev/null 2>&1