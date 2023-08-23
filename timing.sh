# number of tests to run for each combination
ITERATIONS=3

# directories
INPUT_DIRECTORY="./test/"
OUTPUT_DIRECTORY="./out/"

# thread counts to run
THREAD_COUNTS=(25 50 75 100)

# list of input files
DATA_FILES=("data.512m" "data.1g" "data.2g" "data.4g")

# path to output file
OUTPUT_PATH="./results.txt"

# remove existing file
rm "${OUTPUT_PATH}"

# loop through test files (for each iteration)
for current_test in "${DATA_FILES[@]}"
do
    for ((i=1; i<=$ITERATIONS; i++))
    do
        echo "copier | (1 thread) | $current_test | $i" >> ${OUTPUT_PATH}
        
        ./copier "${INPUT_DIRECTORY}/${current_test}" "/tmp/$student_number.out" "-t" >> ${OUTPUT_PATH}
        
        echo -en "\n" >> $OUTPUT_PATH
        echo "Completed copier: $current_test ($i/$ITERATIONS)"
    done
done

# loop through test files and thread counts (for each iteration)
for current_test in "${DATA_FILES[@]}"
do
    for thread_count in "${THREAD_COUNTS[@]}"
    do
        for ((i=1; i<=$ITERATIONS; i++))
        do
            echo "mtcopier | $thread_count | $current_test | $i" >> ${OUTPUT_PATH}
            
            ./mtcopier "$thread_count" "${INPUT_DIRECTORY}/${current_test}" "/tmp/$student_number.out" "-t" >> ${OUTPUT_PATH}
            
            echo -en "\n" >> $OUTPUT_PATH
            echo "Completed mtcopier: $current_test using $thread_count thread(s) ($i/$ITERATIONS)"
        done
    done
done

echo "All tasks completed."