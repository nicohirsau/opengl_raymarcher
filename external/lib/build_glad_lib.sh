g++ -c -static -static-libgcc -static-libstdc++ -std=c++11 ../src/glad.c -I../include
ar rvs glad.a glad.o
rm glad.o