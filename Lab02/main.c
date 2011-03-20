/*
 * main.c
 *
 *  Created on: Mar 10, 2011
 *      Author: blabos
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#define MAX_POINTS 50

#define VERT2D(p) p.x, p.y
#define COLOR_RGB(c) c.r, c.g, c.b
#define COLOR_RGBA(c) c.r, c.g, c.b, c.a

typedef enum {
    sh_tri = 0, sh_quad = 1
} shape_t;

typedef struct {
    double r;
    double g;
    double b;
    double a;
} color_t;

typedef struct {
    double x;
    double y;
} point_t;

typedef enum {
    red = 0, green = 1, blue = 2, black = 3, white = 4
} colors_t;

typedef enum {
    scale, rotation, translation
} transformation_t;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} window_geometry_t;

typedef struct {
    double l;
    double r;
    double t;
    double b;
} viewport_geometry_t;

typedef struct {
    shape_t shape;
    int curr_color;
    point_t points[4];
    window_geometry_t window;
    viewport_geometry_t viewport;
    int mouse_state;
    int mouse_button;
    point_t from;
    point_t to;
    transformation_t transformation;
} app_state_t;

static app_state_t app_state;

const static color_t colors[5] = { { 1.0, 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0,
        0.0 }, { 0.0, 0.0, 1.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }, { 1.0, 1.0,
        1.0, 0.0 }, };

void display();
void keyboard(const char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse_move(int x, int y);
void make_menu();

void cb_menu_shape(int option);
void cb_menu_color(int option);
void cb_main_menu(int option);

void draw_triangle();
void draw_quad();
typedef void (*shape_func_t)(void);
static shape_func_t shapes[2] = { draw_triangle, draw_quad };

void _debug_state();

void display() {
    _debug_state();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    printf("(%.2f, %.2f, %.2f, %.2f)\n", app_state.viewport.l,
            app_state.viewport.r, app_state.viewport.b, app_state.viewport.t);
    
    glPushMatrix();
    
    if (app_state.mouse_state == GLUT_DOWN) {
        if (app_state.transformation == translation) {
            double dx = 2 * app_state.viewport.r * (app_state.to.x
                    - app_state.from.x) / app_state.window.w;
            double dy = -2 * app_state.viewport.r * (app_state.to.y
                    - app_state.from.y) / app_state.window.h;
            glTranslated(dx, dy, 0);
        }
        
        if (app_state.transformation == rotation) {
            double dxt = 2 * app_state.viewport.r * (app_state.to.x
                    - app_state.window.w / 2) / app_state.window.w;
    
            double dyt = -2 * app_state.viewport.t * (app_state.to.y
                    - app_state.window.h / 2) / app_state.window.h;
    
            double dxf = 2 * app_state.viewport.r * (app_state.from.x
                    - app_state.window.w / 2) / app_state.window.w;
    
            double dyf = -2 * app_state.viewport.t * (app_state.from.y
                    - app_state.window.h / 2) / app_state.window.h;
    
            double angle = (atan2(dyt, dxt) - atan2(dyf, dxf)) * 180.0 / M_PI;
            
            glRotated(angle, 0, 0, 1);
        }
    }

    (*(shapes[(int) (app_state.shape)]))();

    glPopMatrix();
    glFlush();
}

void keyboard(const char key, int x, int y) {
    if (key == '\e')
        exit(0);
}

void mouse(int button, int state, int x, int y) {
    app_state.mouse_state = state;
    app_state.mouse_button = button;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        app_state.transformation = translation;
        app_state.from.x = x;
        app_state.from.y = y;
    }

    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        app_state.transformation = rotation;
        app_state.from.x = x;
        app_state.from.y = y;
    }

    glutPostRedisplay();
}

void mouse_move(int x, int y) {
    app_state.to.x = x;
    app_state.to.y = y;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (w == 0)
        w = 1;
    if (h == 0)
        h = 1;

    if (w > h) {
        app_state.viewport.r = (double) (w) / (double) (h);
        app_state.viewport.l = -app_state.viewport.r;
        app_state.viewport.t = 1;
        app_state.viewport.b = -1;
    } else {
        app_state.viewport.t = (double) (h) / (double) (w);
        app_state.viewport.b = -app_state.viewport.r;
        app_state.viewport.r = 1;
        app_state.viewport.l = -1;
    }

    printf("(%.2f, %.2f, %.2f, %.2f)[%d, %d]\n", app_state.viewport.l,
            app_state.viewport.r, app_state.viewport.b, app_state.viewport.t,
            w, h);

    app_state.window.w = w;
    app_state.window.h = h;

    glViewport(0, 0, app_state.window.w, app_state.window.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(app_state.viewport.l, app_state.viewport.r,
            app_state.viewport.b, app_state.viewport.t);
}

void make_menu() {
    int menu, subshape, subcolor;

    subshape = glutCreateMenu(cb_menu_shape);
    glutAddMenuEntry("Triangle", 0);
    glutAddMenuEntry("Quad", 1);
    glutAddMenuEntry("Teapot", 2);

    subcolor = glutCreateMenu(cb_menu_color);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);

    menu = glutCreateMenu(cb_main_menu);
    glutAddSubMenu("Shape", subshape);
    glutAddSubMenu("Color", subcolor);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    app_state.window.x = 400;
    app_state.window.y = 0;
    app_state.window.w = 800;
    app_state.window.h = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(app_state.window.x, app_state.window.y);
    glutInitWindowSize(app_state.window.w, app_state.window.h);
    glutCreateWindow("Lab02");

    make_menu();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return EXIT_SUCCESS;
}

void cb_menu_shape(int option) {
    app_state.shape = (shape_t) option;
    glutPostRedisplay();
}

void cb_menu_color(int option) {
    app_state.curr_color = option;
    glutPostRedisplay();
}

void cb_main_menu(int option) {
}

void draw_triangle() {
    int color = app_state.curr_color;

    app_state.points[0].x = -0.5;
    app_state.points[0].y = -0.5;

    app_state.points[1].x = 0.5;
    app_state.points[1].y = -0.5;

    app_state.points[2].x = 0.0;
    app_state.points[2].y = 0.5;

    glColor3f(COLOR_RGB(colors[color]));
    glBegin(GL_TRIANGLES);
    glVertex2d(VERT2D(app_state.points[0]));
    glVertex2d(VERT2D(app_state.points[1]));
    glVertex2d(VERT2D(app_state.points[2]));
    glEnd();
    glFlush();
}

void draw_quad() {
    int color = app_state.curr_color;

    app_state.points[0].x = -0.5;
    app_state.points[0].y = -0.5;

    app_state.points[1].x = -0.5;
    app_state.points[1].y = 0.5;

    app_state.points[2].x = 0.5;
    app_state.points[2].y = 0.5;

    app_state.points[3].x = 0.5;
    app_state.points[3].y = -0.5;

    glColor3f(COLOR_RGB(colors[color]));
    glBegin(GL_QUADS);
    glVertex2d(VERT2D(app_state.points[0]));
    glVertex2d(VERT2D(app_state.points[1]));
    glVertex2d(VERT2D(app_state.points[2]));
    glVertex2d(VERT2D(app_state.points[3]));
    glEnd();
    glFlush();
}

void _debug_state() {
    int i;

    printf("Shape:      %d\n", (int) (app_state.shape));
    printf("Color:      %d\n", (int) (app_state.curr_color));
}
