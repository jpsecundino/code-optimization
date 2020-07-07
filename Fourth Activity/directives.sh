# None optimization
/usr/bin/gcc -pipe -Wall -I/usr/include/apr-1.0 binary-trees.c -o btree-none -lapr-1 -lm
# -march optimization
/usr/bin/gcc -pipe -Wall -march=x86-64  -I/usr/include/apr-1.0 binary-trees.c -o btree-march -lapr-1 -lm
# -O1 optimization
/usr/bin/gcc -pipe -Wall -O1  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O1 -lapr-1 -lm
# -O2 optimization
/usr/bin/gcc -pipe -Wall -O2  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O2 -lapr-1 -lm
# -O3 optimization
/usr/bin/gcc -pipe -Wall -O3  -I/usr/include/apr-1.0 binary-trees.c -o btrees-O3 -lapr-1 -lm
# Article flags optimization
/usr/bin/gcc -pipe -Wall -fomit-frame-pointer -fguess-branch-probability -fpartial-inlining -foptimize-sibling-calls -finline  -I/usr/include/apr-1.0 binary-trees.c -o btrees-paper -lapr-1 -lm