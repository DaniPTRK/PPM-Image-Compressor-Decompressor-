build: tema2.c
	gcc tema2.c -o quadtree

run: tema2
	./quadtree

clean:
	rm quadtree