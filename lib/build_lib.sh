g++ -c -static -static-libgcc -static-libstdc++ -std=c++11 ../src/Mantaray/Shader.cpp ../src/Mantaray/FileSystem.cpp ../src/Mantaray/Logger.cpp -I../include -I../external/include -L../external/lib
ar rvs Mantaray.a FileSystem.o Logger.o Shader.o
rm *.o