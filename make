g++ -std=c++11 -c *.cpp;
g++ -std=c++11 draw.o genome.o gnn.o innovation.o random.o evaluate.o main.cpp -framework GLUT -framework OpenGL;
