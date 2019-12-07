g++ -c -static -static-libgcc -static-libstdc++ -std=c++11 src/Mantaray/Shader.cpp src/Mantaray/FileSystem.cpp src/Mantaray/Logger.cpp src/Mantaray/Mesh.cpp src/Mantaray/InputManager.cpp -Iinclude -Iexternal/include -Lexternal/lib
ar rvs lib/Mantaray.a FileSystem.o Logger.o Shader.o Mesh.o InputManager.o
rm *.o