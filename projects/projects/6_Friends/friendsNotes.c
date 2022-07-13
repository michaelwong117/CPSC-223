edited gmap_unit.c to include gmap.o
edited gmap.c to be struct gmap; from previously struct _gmap;
typedef struct gmap gmap; from typedef struct _gmap gmap;

valgrind --leak-check=full --show-leak-kinds=all ./GraphUnit 1
./Friends tests/graph_alpha_subgraphs_2.in distance
valgrind --leak-check=full --show-leak-kinds=all ./Friends tests/graph_alpha_subgraphs_2.in distance
time ./GraphUnit 8 10 1

valgrind --leak-check=full --show-leak-kinds=all ./Friends tests/graph_alpha_subgraphs_2.in distance < tests/query_alpha_4.in
./Friends tests/graph_alpha_subgraphs_2.in distance < tests/query_alpha_4.in
//GraphUnit 10 <10 1 will fail because word goes from 1 to 10 automatically if
// the on flag is 1

/c/cs223/bin/submit 6 friends.c graph.c gmap.c makefile log gmap.h gqueue.c
/c/cs223/bin/testit 6 Friends

valgrind --leak-check=full --show-leak-kinds=all ./Friends input1.in both < input2.in