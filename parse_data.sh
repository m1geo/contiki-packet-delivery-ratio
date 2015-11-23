echo "George Smart, M1GEO"
echo "Monday 24 March 2014"

echo "Working on file: $1"

mkdir -p "data/"

for A in `seq 11 26`
do
	echo -n "Channel ${A}:  "
	cat "$1".dat | grep "RESULT FOR CH ${A}" | awk '{print $7}' > "data/${A}.dat"

	count=0
	total=0
	for i in $( awk '{ print $1; }' "data/${A}.dat" )
	do
		total=$(echo $total+$i | bc )
		((count++))
	done
	echo "scale=2; $total / $count" | bc
done
