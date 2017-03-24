##
# Runs all queries in the folder passed as argument
##
# arg1 - path to queries files
# arg2 - path to run_queries binary
##

echo "Baseline"
for f in `ls -v $1/*.queries.*`
do
    echo $f
    $2/run_queries 0 ../data/time_series.txt $f
done

echo "-----"

echo "Index1"
for f in `ls -v $1/*.queries.*`
do
    echo $f
    $2/run_queries 1 ../data/time_series.txt $f
done
