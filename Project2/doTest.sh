N_threads=$1
InputFile=$2
Counter=1
Tudo=0

speedup_sufix=".speedups.csv"
file_sufix=".res"
input=$(basename $InputFile) 
nome_file="$input$speedup_sufix"

echo '#threads,exec_time,speedup' > results/$nome_file

$(./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2)
tempo_seq=$(cat "$InputFile$file_sufix" | grep 'time' | cut -d '=' -f 2 | cut -d ' ' -f 2)
$(cd ..)
echo "1S,$tempo_seq,1" >> results/$nome_file

while [[ $Counter -le $N_threads ]]
do
	$(./CircuitRouter-ParSolver/CircuitRouter-ParSolver $2 -t $Counter)
	tempo_par=$(cat "$InputFile$file_sufix" | grep 'time' | cut -d '=' -f 2 | cut -d ' ' -f 2)
	speedup=$(echo "scale=6; $tempo_seq/$tempo_par" | bc)
	$(cd ..)
	echo "$Counter,$tempo_par,$speedup" >> results/$nome_file
	((Counter++))
done

