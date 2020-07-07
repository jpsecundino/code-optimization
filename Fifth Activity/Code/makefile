repetitions=10
matrix_size=16
folder="flag"

##to run: make run_function repetition=nmberofrepetitions matrix_size=size
compile:
	gcc -msse3 -g matrixMult.c -o matrixMult.exe

compile_flag:
	gcc -msse3 -ftree-vectorize -g matrixMult.c -o matrixMult.exe

run_all:
	make compile
	make run_normal matrix_size=1024 folder="normal"
	make run_interchange matrix_size=1024 folder="normal"
	make run_unrolling matrix_size=1024 folder="normal"
	make run_intrinsics_v1 matrix_size=1024 folder="normal"
	make run_intrinsics_v2 matrix_size=1024 folder="normal"
	make run_optimized matrix_size=1024 folder="normal"
	make run_blocked matrix_size=1024 folder="normal"
	make run_blocked_normal matrix_size=1024 folder="normal"
	make run_blocked_vec matrix_size=1024 folder="normal"
	make compile_flag
	make run_normal matrix_size=1024 folder="flag"
	make run_interchange matrix_size=1024 folder="flag"
	make run_unrolling matrix_size=1024 folder="flag"
	make run_intrinsics_v1 matrix_size=1024 folder="flag"
	make run_intrinsics_v2 matrix_size=1024 folder="flag"
	make run_optimized matrix_size=1024 folder="flag"
	make run_blocked matrix_size=1024 folder="flag"
	make run_blocked_normal matrix_size=1024 folder="flag"
	make run_blocked_vec matrix_size=1024 folder="flag"
	

run_normal:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/normal/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 0 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/normal/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 0 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/normal/$(folder)/*-cache >> ../results/normal/$(folder)/cache-data
	cat ../results/normal/$(folder)/*-branch >> ../results/normal/$(folder)/branch-data

run_interchange:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/interchange/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 1 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/interchange/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 1 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/interchange/$(folder)/*-cache >> ../results/interchange/$(folder)/cache-data
	cat ../results/interchange/$(folder)/*-branch >> ../results/interchange/$(folder)/branch-data

run_unrolling:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/unrolling/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 2 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/unrolling/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 2 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/unrolling/$(folder)/*-cache >> ../results/unrolling/$(folder)/cache-data
	cat ../results/unrolling/$(folder)/*-branch >> ../results/unrolling/$(folder)/branch-data

run_intrinsics_v1:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/intrinsicsV1/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 3 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/intrinsicsV1/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 3 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/intrinsicsV1/$(folder)/*-cache >> ../results/intrinsicsV1/$(folder)/cache-data
	cat ../results/intrinsicsV1/$(folder)/*-branch >> ../results/intrinsicsV1/$(folder)/branch-data

run_intrinsics_v2:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/intrinsicsV2/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 4 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/intrinsicsV2/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 4 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/intrinsicsV2/$(folder)/*-cache >> ../results/intrinsicsV2/$(folder)/cache-data
	cat ../results/intrinsicsV2/$(folder)/*-branch >> ../results/intrinsicsV2/$(folder)/branch-data

run_optimized:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/optimized/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 5 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/optimized/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 5 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/optimized/$(folder)/*-cache >> ../results/optimized/$(folder)/cache-data
	cat ../results/optimized/$(folder)/*-branch >> ../results/optimized/$(folder)/branch-data

run_blocked:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blocked/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 6 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blocked/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 6 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/blocked/$(folder)/*-cache >> ../results/blocked/$(folder)/cache-data
	cat ../results/blocked/$(folder)/*-branch >> ../results/blocked/$(folder)/branch-data

run_blocked_vec:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blockedVec/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 7 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blockedVec/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 7 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/blockedVec/$(folder)/*-cache >> ../results/blockedVec/$(folder)/cache-data
	cat ../results/blockedVec/$(folder)/*-branch >> ../results/blockedVec/$(folder)/branch-data

run_blocked_normal:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blockedNormal/$(folder)/$$n-cache -e L1-dcache-loads,L1-dcache-load-misses ./matrixMult.exe 8 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ../results/blockedNormal/$(folder)/$$n-branch -e branch-instructions,branch-misses ./matrixMult.exe 8 $(matrix_size) $(matrix_size) $(matrix_size) $(matrix_size); done
	cat ../results/blockedNormal/$(folder)/*-cache >> ../results/blockedNormal/$(folder)/cache-data
	cat ../results/blockedNormal/$(folder)/*-branch >> ../results/blockedNormal/$(folder)/branch-data







clean_all:
	make clean_normal
	make clean_interchange
	make clean_intrinsics
	make clean_unrolling
	make clean_intrinsics_V1
	make clean_intrinsics_V2

clean_normal:
	sudo rm ../results/normal/16/*
	sudo rm ../results/normal/128/*
	sudo rm ../results/normal/1024/*

clean_interchange:
	sudo rm ../results/interchange/16/*
	sudo rm ../results/interchange/128/*
	sudo rm ../results/interchange/1024/*

clean_unrolling:
	sudo rm ../results/unrolling/16/*
	sudo rm ../results/unrolling/128/*
	sudo rm ../results/unrolling/1024/*

clean_intrinsics_V1:
	sudo rm ../results/intrinsicsV1/16/*
	sudo rm ../results/intrinsicsV1/128/*
	sudo rm ../results/intrinsicsV1/1024/*
clean_intrinsics_V2:
	sudo rm ../results/intrinsicsV2/16/*
	sudo rm ../results/intrinsicsV2/128/*
	sudo rm ../results/intrinsicsV2/1024/*