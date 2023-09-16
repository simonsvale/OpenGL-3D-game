all:
	g++ -I src/include -I glad/include -L src/lib -o main main.cpp glad\src\glad.c -lmingw32 -lSDL2main -lSDL2