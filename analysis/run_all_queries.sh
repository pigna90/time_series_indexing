##
# Runs all queries in the folder passed as argument
##

echo "Baseline"
for f in `ls -v $1/*.queries.*`
do
    echo $f
    ../_build/bin/run_queries 0 ../data/time_series.txt $f
done

echo "-----"

echo "Index1"
for f in `ls -v $1/*.queries.*`
do
    echo $f
    ../_build/bin/run_queries 1 ../data/time_series.txt $f
done
