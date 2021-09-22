#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>

#include "population.hpp"
#include "draw.hpp"

#define POP_SIZE 150
#define INPUT_SIZE 2
#define OUTPUT_SIZE 1

/*NEAT::Population pop(POP_SIZE,INPUT_SIZE,OUTPUT_SIZE);
bool play = false;

void reshape(int,int);
void keyboard(unsigned char key, int x, int y);
void draw_genome();
void display();
void update();
void step_forward();

void step_forward() {
  pop.evaluate();
  std::cout << "generation: " << pop.get_generation() << ", total fitness: " << pop.get_total_fitness() << ", max fitness: " << pop.get_max_fitness() << std::endl;
  pop.next_generation();
}

void init() {
  reshape(1000,1000);
  glLoadIdentity();
  step_forward();
  draw_genome();
}

void display() {

}

void update() {
  if (play) {
    step_forward();
    draw_genome();
  }
}

void reshape(int width, int height) {
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,100,0,100);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 27) { //esc
    exit(0);
  }
  if (key == 'n') {
    step_forward();
    draw_genome();
  }
  if (key == 'p') {
    play = !play;
  }
}

void draw_genome() {
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  pop.get_best().draw();

  glFlush();
}*/

int main(int argc, char **argv) {
  
  /*glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowPosition(200,100);
  glutInitWindowSize(1000, 1000);
 
  glutCreateWindow("NEAT XOR");
 
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  init();

  glutMainLoop();*/

  int total_generations = 0;
  int num_trials = 1000;

  for (int i = 0; i < num_trials; i++) {
    NEAT::Population pop(POP_SIZE,INPUT_SIZE,OUTPUT_SIZE);
    while (1) {
      pop.evaluate();
      if(pop.get_max_fitness() == 16) {
        break;
      }
      pop.next_generation();
    }
    total_generations += pop.get_generation();
    std::cout << "Trial " << i+1 << " Complete" << std::endl;
  }

  std::cout << "Average Generations to Train: " << total_generations / num_trials << std::endl;

  return 0;
}
