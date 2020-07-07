repetitions=5
all:
	/usr/bin/gcc -pipe -Wall -I/usr/include/apr-1.0 binary-trees.c -o btree-none -lapr-1 -lm
	/usr/bin/gcc -pipe -Wall -march=x86-64  -I/usr/include/apr-1.0 binary-trees.c -o btree-march -lapr-1 -lm
	/usr/bin/gcc -pipe -Wall -O2  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O2 -lapr-1 -lm
	/usr/bin/gcc -pipe -Wall -O1  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O1 -lapr-1 -lm
	/usr/bin/gcc -pipe -Wall -O3  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O3 -lapr-1 -lm
	/usr/bin/gcc -pipe -Wall -fomit-frame-pointer -fguess-branch-probability -fpartial-inlining -foptimize-sibling-calls -finline  -I/usr/include/apr-1.0 binary-trees.c -o btrees-paper -lapr-1 -lm

all-asm:
	# /usr/bin/gcc -pipe -Wall -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btree-none.s > btree-none.dump
	# /usr/bin/gcc -pipe -Wall -march=x86-64  -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btree-march.s > btree-march.dump
	# /usr/bin/gcc -pipe -Wall -O2  -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btrees-O2.s > btrees-O2.dump
	# /usr/bin/gcc -pipe -Wall -O1  -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btrees-O1.s > btrees-O2.dump
	/usr/bin/gcc -pipe -Wall -O3 -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btrees-O3.s > btrees-O3.dump
	# /usr/bin/gcc -pipe -Wall -fomit-frame-pointer -fguess-branch-probability -fpartial-inlining -foptimize-sibling-calls -finline  -I/usr/include/apr-1.0 -S binary-trees.c -lapr-1 -lm
	# objdump -S --disassemble btrees-paper.s > btrees-paper.dump

perf-none:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/none/$$n-saida.txt ./btree-none 20; done
	cat ./results/none/*-saida.txt >> ./results/none/final-data.txt
perf-march:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/march/$$n-saida.txt ./btree-march 20; done
	cat ./results/march/*-saida.txt >> ./results/march/final-data.txt
perf-O1:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O1/$$n-saida.txt ./btrees-O1 20; done
	cat ./results/O1/*-saida.txt >> ./results/O1/final-data.txt
perf-O2:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O2/$$n-saida.txt ./btrees-O2 20; done	
	cat ./results/O2/*-saida.txt >> ./results/O2/final-data.txt
perf-O3:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O3/$$n-saida.txt ./btrees-O3 20; done	
	cat ./results/O3/*-saida.txt >> ./results/O3/final-data.txt
perf-paper:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/paper/$$n-saida.txt ./btrees-paper 20; done		
	cat ./results/paper/*-saida.txt >> ./results/paper/final-data.txt
perf-all:
	make perf-none
	make perf-march
	make perf-O1
	make perf-O2
	make perf-O3
	make perf-paper

perf-none-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/none/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btree-none 20; done
	cat ./results/none/*-saida-cache.txt >> ./results/none/final-data-cache.txt
perf-march-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/march/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btree-march 20; done
	cat ./results/march/*-saida-cache.txt >> ./results/march/final-data-cache.txt
perf-O1-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O1/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btrees-O1 20; done
	cat ./results/O1/*-saida-cache.txt >> ./results/O1/final-data-cache.txt
perf-O2-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O2/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btrees-O2 20; done	
	cat ./results/O2/*-saida-cache.txt >> ./results/O2/final-data-cache.txt
perf-O3-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/O3/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btrees-O3 20; done	
	cat ./results/O3/*-saida-cache.txt >> ./results/O3/final-data-cache.txt
perf-paper-cache:
	for n in $$(seq 1 $(repetitions)); do sudo perf stat --output ./results/paper/$$n-saida-cache.txt -e L1-dcache-loads,L1-dcache-load-misses ./btrees-paper 20; done		
	cat ./results/paper/*-saida-cache.txt >> ./results/paper/final-data-cache.txt
perf-all-cache:
	make perf-none-cache
	make perf-march-cache
	make perf-O1-cache
	make perf-O2-cache
	make perf-O3-cache
	make perf-paper-cache
