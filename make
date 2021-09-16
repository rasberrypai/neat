g++ -std=c++11 -c -g *.cpp;
g++ -std=c++11 -g draw.o genome.o gnn.o innovation.o random.o evaluate.o population.o main.cpp -framework GLUT -framework OpenGL;
