	all:
	gcc main.c sorting.c -o prog.exe -pg -g
	./prog.exe
	gprof prog.exe gmon.out -Pcache_clean -Pfill -Qcache_clean -Qfill > chart.prev
	gprof2dot --node-thres=0.0 --edge-thres=0.0 chart.prev > chart.dot
	dot -Tpng -o chart.png chart.dot


