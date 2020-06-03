repetitions=10
sort_folder = "notSorted"
##to run: make run_function repetition=nmberofrepetitions 
compile:
	gcc matrixMult.c -o matrixMult
run_all:
	make run_opt_off
	make run_opt_on
	make run_noIfConv
	make run_noLoop
	make run_noIfConvLoop
	make run_opt_on_sorted
	make run_noIfConv_sorted
	make run_noLoop_sorted
	make run_noIfConvLoop_sorted

run_notSorted:
	make run_opt_on
	make run_noIfConv
	make run_noLoop
	make run_noIfConvLoop

run_sorted:
	make run_opt_on_sorted
	make run_noIfConv_sorted
	make run_noLoop_sorted
	make run_noIfConvLoop_sorted

run_opt_off:
	gcc kernel11-branch_prediction.c -o kernel11-branch_prediction
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOff/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOff/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction; done
	cat ./results/notSorted/optOff/*-cache.csv >> ./results/notSorted/optOff/cache-data.csv
	cat ./results/notSorted/optOff/*-branch.csv >> ./results/notSorted/optOff/branch-data.csv

run_opt_on:
	gcc -O3 kernel11-branch_prediction.c -o kernel11-branch_prediction-O3
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/O3/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/O3/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3; done
	cat ./results/notSorted/optOn/O3/*-cache.csv >> ./results/notSorted/optOn/O3/cache-data.csv
	cat ./results/notSorted/optOn/O3/*-branch.csv >> ./results/notSorted/optOn/O3/branch-data.csv

run_noIfConv:
	gcc -O3 kernel11-branch_prediction.c -fno-if-conversion -o kernel11-branch_prediction-O3-fif
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/FIF/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-fif; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/FIF/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-fif; done
	cat ./results/notSorted/optOn/FIF/*-cache.csv >> ./results/notSorted/optOn/FIF/cache-data.csv
	cat ./results/notSorted/optOn/FIF/*-branch.csv >> ./results/notSorted/optOn/FIF/branch-data.csv

run_noLoop:
	gcc -O3 kernel11-branch_prediction.c -fno-tree-loop-vectorize -o kernel11-branch_prediction-O3-loop
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/noLOOP/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-loop; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/noLOOP/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-loop; done
	cat ./results/notSorted/optOn/noLOOP/*-cache.csv >> ./results/notSorted/optOn/noLOOP/cache-data.csv
	cat ./results/notSorted/optOn/noLOOP/*-branch.csv >> ./results/notSorted/optOn/noLOOP/branch-data.csv

run_noIfConvLoop:
	gcc -O3 kernel11-branch_prediction.c -fno-tree-loop-vectorize -fno-if-conversion -o kernel11-branch_prediction-O3-fif-loop
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/noFifLoop/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-fif-loop; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/notSorted/optOn/noFifLoop/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-fif-loop; done
	cat ./results/notSorted/optOn/noFifLoop/*-cache.csv >> ./results/notSorted/optOn/noFifLoop/cache-data.csv
	cat ./results/notSorted/optOn/noFifLoop/*-branch.csv >> ./results/notSorted/optOn/noFifLoop/branch-data.csv

##SORTED

run_opt_off_sorted:
	gcc -DSORT kernel11-branch_prediction.c -o kernel11-branch_prediction-sorted
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOff/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-sorted; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOff/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-sorted; done
	cat ./results/Sorted/optOff/*-cache.csv >> ./results/Sorted/optOff/cache-data.csv
	cat ./results/Sorted/optOff/*-branch.csv >> ./results/Sorted/optOff/branch-data.csv

run_opt_on_sorted:

	gcc -O3 -DSORT kernel11-branch_prediction.c -o kernel11-branch_prediction-O3-sorted
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/O3/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-sorted; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/O3/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-sorted; done
	cat ./results/Sorted/optOn/O3/*-cache.csv >> ./results/Sorted/optOn/O3/cache-data.csv
	cat ./results/Sorted/optOn/O3/*-branch.csv >> ./results/Sorted/optOn/O3/branch-data.csv

run_noIfConv_sorted:
	gcc -O3 -DSORT kernel11-branch_prediction.c -fno-if-conversion -o kernel11-branch_prediction-O3-fif-sorted
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/FIF/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-fif-sorted; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/FIF/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-fif-sorted; done
	cat ./results/Sorted/optOn/FIF/*-cache.csv >> ./results/Sorted/optOn/FIF/cache-data.csv
	cat ./results/Sorted/optOn/FIF/*-branch.csv >> ./results/Sorted/optOn/FIF/branch-data.csv

run_noLoop_sorted:
	gcc -O3 -DSORT kernel11-branch_prediction.c -fno-tree-loop-vectorize -o kernel11-branch_prediction-O3-loop-sorted
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/noLOOP/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-loop-sorted; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/noLOOP/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-loop-sorted; done
	cat ./results/Sorted/optOn/noLOOP/*-cache.csv >> ./results/Sorted/optOn/noLOOP/cache-data.csv
	cat ./results/Sorted/optOn/noLOOP/*-branch.csv >> ./results/Sorted/optOn/noLOOP/branch-data.csv

run_noIfConvLoop_sorted:
	gcc -O3 -DSORT kernel11-branch_prediction.c -fno-tree-loop-vectorize -fno-if-conversion -o kernel11-branch_prediction-O3-fif-loop-sorted
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/noFifLoop/$$n-cache.csv -e cache-references,cache-misses ./kernel11-branch_prediction-O3-fif-loop-sorted; done
	for n in $$(seq 1 $(repetitions)); do sudo perf stat -x , --output ./results/Sorted/optOn/noFifLoop/$$n-branch.csv -e branches,branch-misses ./kernel11-branch_prediction-O3-fif-loop-sorted; done
	cat ./results/Sorted/optOn/noFifLoop/*-cache.csv >> ./results/Sorted/optOn/noFifLoop/cache-data.csv
	cat ./results/Sorted/optOn/noFifLoop/*-branch.csv >> ./results/Sorted/optOn/noFifLoop/branch-data.csv