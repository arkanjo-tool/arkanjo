# This script was used in the first try to evaluate the tool using the big clone bench dataset
# as I remember, this does not work, but I will keep this as I am not sure anymore
#
# !/bin/bash
# This script automatize the evaluation of BigCloneEval on this tool
# Set the BigCloneEval repository correct on this folder before executing this script
# Set the duplicate-code_detection-tool correct on this folder before executing this script

DIR="$(pwd)"
RELATIVE="/BigCloneEval/ijadataset/bcb_reduced/"
DIR+=$RELATIVE

FOLDER_LIST=("10" "13" "17" "2" "22" "25" "28" "30" "33" "36" "39" "41" "44" "6" "9")
FOLDER_LIST+=("11" "14" "18" "20" "23" "26" "29" "31" "34" "37" "4" "42" "45" "7" "12")
FOLDER_LIST+=("15" "19" "21" "24" "27" "3" "32" "35" "38" "40" "43" "5" "8")

FOLDER_LIST=("5" "8")

#Compile function_breaker commands
make breaker
g++ parser.cpp -o parser

echo "Enter minimum similarity desired on using the tool:"
read similarity

echo "Executing and Creating the CSV files"

for FOLDER in ${FOLDER_LIST[@]}; 
do
	#Build the correct folder path
	path=$DIR
	path+=$FOLDER

	echo Processing "${FOLDER}"

	#execute the breaker
	sudo rm -r tmp
	./breaker $path

	sudo python3 -W ignore duplicate-code-detection-tool/duplicate_code_detection.py -d tmp/source > output_tool.txt
	./parser $similarity < output_tool.txt > output_parsed.txt

	#Save the formated output
	sudo ./exec bi > "csvs/${FOLDER}.csv"

	echo Ended "${FOLDER}"
done

#echo "Passing CSV files to the Evaluator"
#
#for FOLDER in ${FOLDER_LIST[@]};
#do
#	echo Passing "${FOLDER}"
#	path="$(pwd)"
#	path+="/csvs/${FOLDER}.csv"
#
#	cur="$(pwd)"
#
#	cd BigCloneEval/commands
#	./importClones -t 1 -c $path
#	cd $cur
#done

