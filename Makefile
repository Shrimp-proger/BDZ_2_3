assemble: clean
	gcc -fPIC -c map_lib.c -o map_lib.o
	gcc -shared map_lib.o -o libmap_lib.so
	gcc main.c -L. -lmap_lib -o map_test
	rm -f ./*.o
clean:
	rm -f ./*.o
