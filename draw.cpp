#include "draw.hpp"

#include <GLUT/glut.h>
#include <cmath>

void drawCircle(float cx, float cy, float r, int num_segments) {
  glBegin(GL_LINE_LOOP);
  for(int ii = 0; ii < num_segments; ii++) {
    float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
    float x = r * cosf(theta);//calculate the x component 
    float y = r * sinf(theta);//calculate the y component
    glVertex2f(x + cx, y + cy);//output vertex
  }
  glEnd();
}
void drawLine(float x1,float y1, float x2, float y2) {
  glBegin(GL_LINES);
  glVertex2d(x1, y1);
  glVertex2d(x2, y2);
  glEnd();
}
void drawPoint(float x1,float y1) {
  glBegin(GL_POINTS);
  glVertex2f(x1,y1);
  glEnd();
}
