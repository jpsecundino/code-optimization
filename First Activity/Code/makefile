repetitions=10
matrix_size=10

##to run: make run_function repetition=nmberofrepetitions matrix_size=size
compile:
	gcc matrixMult.c -o matrixMult

run_normal:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/normal/$(matrix_size)/$$n-cache.csv -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult 0 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/normal/$(matrix_size)/$$n-branch.csv -e branch-instructions,branch-misses ./matrixMult 0 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ./results/normal/$(matrix_size)/*-cache.csv >> ./results/normal/$(matrix_size)/cache-data.csv
	cat ./results/normal/$(matrix_size)/*-branch.csv >> ./results/normal/$(matrix_size)/branch-data.csv

run_interchange:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/interchange/$(matrix_size)/$$n-cache.csv -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult 1 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/interchange/$(matrix_size)/$$n-branch.csv -e branch-instructions,branch-misses ./matrixMult 1 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ./results/interchange/$(matrix_size)/*-cache.csv >> ./results/interchange/$(matrix_size)/cache-data.csv
	cat ./results/interchange/$(matrix_size)/*-branch.csv >> ./results/interchange/$(matrix_size)/branch-data.csv
run_unrolling:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/unrolling/$(matrix_size)/$$n-cache.csv -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult 2 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/unrolling/$(matrix_size)/$$n-branch.csv -e branch-instructions,branch-misses ./matrixMult 2 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ./results/unrolling/$(matrix_size)/*-cache.csv >> ./results/unrolling/$(matrix_size)/cache-data.csv
	cat ./results/unrolling/$(matrix_size)/*-branch.csv >> ./results/unrolling/$(matrix_size)/branch-data.csv

clean_dirs:
	sudo rm results/interchange/10/*
	sudo rm results/interchange/100/*
	sudo rm results/interchange/1000/*
	sudo rm results/unrolling/10/*
	sudo rm results/unrolling/100/*
	sudo rm results/unrolling/1000/*
	sudo rm results/normal/10/*
	sudo rm results/normal/100/*
	sudo rm results/normal/1000/*

clean_normal:
	sudo rm results/normal/10/*
	sudo rm results/normal/100/*
	sudo rm results/normal/1000/*

clean_unrolling:
	sudo rm results/unrolling/10/*
	sudo rm results/unrolling/100/*
	sudo rm results/unrolling/1000/*

clean_interchange:
	sudo rm results/interchange/10/*
	sudo rm results/interchange/100/*
	sudo rm results/interchange/1000/*
