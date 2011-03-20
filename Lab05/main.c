/*
 * main.c
 *
 *  Created on: Mar 11, 2011
 *      Author: blabos
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

GLfloat points[4][4][3] = {
    {{0.0, 0.0, 0.0}, {0.3, 0.5, 0.0}, {0.7, 0.5, 0.0}, {1.0, 0.0, 0.0}},
    {{0.0, 0.0, 0.3}, {0.3, 0.5, 0.3}, {0.7, 0.5, 0.3}, {1.0, 0.0, 0.3}},
    {{0.0, 0.0, 0.7}, {0.3, 0.5, 0.7}, {0.7, 0.5, 0.7}, {1.0, 0.0, 0.7}},
    {{0.0, 0.0, 1.0}, {0.3, 0.5, 1.0}, {0.7, 0.5, 1.0}, {1.0, 0.0, 1.0}}
};

#define SENS_ROT     5.0
#define SENS_OBS    15.0
#define SENS_TRANSL 30.0

typedef struct {
    double x;
    double y;
    double z;
} point_t;

typedef struct {
    double fovy;
    point_t rot;
    point_t obs;
    double aspect;
} app_state_t;
static app_state_t app_state;

void init();
void scene_configure();
void set_observer();
void display();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(400, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab05");
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(resize);
    
    init();
    
    glutMainLoop();
    return EXIT_SUCCESS;
}

void display(){
    float i, j;
    int a, b;
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3d(1.0, 1.0, 1.0);
    float delta = 0.1;
    
    // Superfície
    for(i = 0; i <= 1.01; i += delta) {
        glBegin(GL_LINE_STRIP);
            for(j = 0; j <= 1.01; j += delta) {
                glEvalCoord2f(i,j);
            }
        glEnd();
        
        glBegin(GL_LINE_STRIP);
            for(j = 0; j <= 1.01; j += delta) {
                glEvalCoord2f(j,i);
            }
        glEnd();
    }
    
    // Pontos
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    
    glBegin(GL_POINTS);
        for(a = 0; a < 4; ++a) {
            for(b = 0; b < 4; ++b) {
                glVertex3fv(points[a][b]);
            }
        }
    glEnd();
    
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    glutPostRedisplay();
}

void resize(int w, int h) {
    if ( h == 0 ) h = 1;
    glViewport(0, 0, w, h);
    printf("Foo\n");
    
    app_state.aspect = (double)(w) / (double) (h);
    scene_configure();
    
    glutPostRedisplay();
}

void scene_configure() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(app_state.fovy, app_state.aspect, 0.1, 1200);
    
    set_observer();
}

void set_observer() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(-app_state.obs.x,-app_state.obs.y,-app_state.obs.z);
    glRotatef(app_state.rot.x, 1, 0, 0);
    glRotatef(app_state.rot.y, 0, 1, 0);
}


void init() {
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, points);
    
    glEnable(GL_MAP2_VERTEX_3);
    
    app_state.fovy = 30;
    app_state.rot.x = 45;
    app_state.rot.y = 45;
    app_state.rot.z = 1;
    app_state.obs.x = app_state.obs.y = 0.0;
    app_state.obs.z = 5.0;
}

