#include <GLUT/glut.h>

#include "genome.hpp"
#include "draw.hpp"

void reshape(int,int);

void init() {
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  reshape(1000,1000);
  glLoadIdentity();
  
  NEAT::InnovationTable it;
  NEAT::Genome g(2,1,it);
  g.draw();
  
  glFlush();
}

void display() {

}

void reshape(int width, int height) {
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,100,0,100);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowPosition(200,100);
  glutInitWindowSize(1000, 1000);
 
  glutCreateWindow("GLUT program");
 
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  init();

  glutMainLoop(); 
  return 0;
}
