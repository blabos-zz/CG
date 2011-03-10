/*
 * main.c
 *
 *  Created on: Mar 10, 2011
 *      Author: blabos
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#define MAX_POINTS 50

#define VERT2D(p) p.x, p.y
#define COLOR_RGB(c) c.r, c.g, c.b
#define COLOR_RGBA(c) c.r, c.g, c.b, c.a

typedef enum {
    sh_tri, sh_ln, sh_pt, sh_tri_strip
} shape_t;

typedef enum {
    cap_clear, cap_on
} capture_mode_t;

typedef struct {
    double x;
    double y;
} point_t;

typedef struct {
    double r;
    double g;
    double b;
    double a;
} color_t;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} window_geometry_t;

typedef struct {
    shape_t shape;
    capture_mode_t capture_mode;
    point_t points[MAX_POINTS];
    int curr_points;
    window_geometry_t geometry;
    color_t color;
    color_t bg_color;
} state_t;

static state_t app_state;

const static color_t red = {1.0, 0.0, 0.0, 0.0};
const static color_t green = {0.0, 1.0,0.0, 0.0};
const static color_t blue = {0.0, 0.0, 1.0, 0.0};
const static color_t black = {0.0, 0.0, 0.0, 0.0};
const static color_t white = {1.0, 1.0, 1.0, 0.0};

void display();
void keyboard(unsigned char key, int x, int y);
void special_funcs(int key, int x, int y);
void mouse(int button, int state, int x, int y);

void _debug_state();
point_t _norm_coords_xy(int x, int y);
point_t _norm_coords_pt(point_t p);

void display() {
    int i;
    
    _debug_state();
    
    glClearColor(COLOR_RGBA(app_state.bg_color));
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(COLOR_RGB(app_state.color));

    switch (app_state.shape) {
    case sh_pt:
        glBegin(GL_POINTS);
        for (i = 0; i < app_state.curr_points; i++) {
            glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i])));
        }
        glEnd();
        break;
    case sh_ln:
        if (app_state.curr_points > 1) {
            glBegin(GL_LINE);
            for (i = 1; i < app_state.curr_points; i++) {
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i - 1])));
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i])));
            }
            glEnd();
        }
        break;
    case sh_tri:
        if (app_state.curr_points > 2) {
            glBegin(GL_TRIANGLES);
            for (i = 0; i < app_state.curr_points; i++) {
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i])));
            }
            glEnd();
        }
        break;
    case sh_tri_strip:
        if (app_state.curr_points > 2) {
            glBegin(GL_TRIANGLE_STRIP);
            for (i = 2; i < app_state.curr_points; i++) {
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i - 2])));
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i - 1])));
                glVertex2d(VERT2D(_norm_coords_pt(app_state.points[i])));
            }
            glEnd();
        }
        break;
    default:
        break;
    }

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            app_state.color = red;
            break;
        case 'R':
            app_state.bg_color = red;
            break;
        case 'g':
            app_state.color = green;
            break;
        case 'G':
            app_state.bg_color = green;
            break;
        case 'u':
            app_state.color = blue;
            break;
        case 'U':
            app_state.bg_color = blue;
            break;
        case 'b':
            app_state.color = black;
            break;
        case 'B':
            app_state.bg_color = black;
            break;
        case 'w':
            app_state.color = white;
            break;
        case 'W':
            app_state.bg_color = white;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void special_funcs(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        app_state.capture_mode = cap_on;
        app_state.shape = sh_pt;
        app_state.curr_points = 0;
        break;
    case GLUT_KEY_F2:
        app_state.capture_mode = cap_on;
        app_state.shape = sh_ln;
        app_state.curr_points = 0;
        break;
    case GLUT_KEY_F3:
        app_state.capture_mode = cap_on;
        app_state.shape = sh_tri;
        app_state.curr_points = 0;
        break;
    case GLUT_KEY_F4:
        app_state.capture_mode = cap_on;
        app_state.shape = sh_tri_strip;
        app_state.curr_points = 0;
        break;
    case GLUT_KEY_F5:
        app_state.shape = sh_pt;
        break;
    case GLUT_KEY_F6:
        app_state.shape = sh_ln;
        break;
    case GLUT_KEY_F7:
        app_state.shape = sh_tri;
        break;
    case GLUT_KEY_F8:
        app_state.shape = sh_tri_strip;
        break;
    default:
        app_state.capture_mode = cap_clear;
        break;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (app_state.capture_mode == cap_on
            && app_state.curr_points < MAX_POINTS) {
        if (state == GLUT_UP) {
            app_state.points[app_state.curr_points].x = x;
            app_state.points[app_state.curr_points].y = y;
            app_state.curr_points++;
            glutPostRedisplay();
        }
    }
}

int main(int argc, char** argv) {
    app_state.capture_mode = cap_clear;
    app_state.shape = sh_pt;
    app_state.curr_points = 0;
    
    app_state.geometry.x = 400;
    app_state.geometry.y = 0;
    app_state.geometry.w = 800;
    app_state.geometry.h = 600;
    
    app_state.color = white;
    app_state.bg_color = black;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(400, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab01");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_funcs);
    glutMouseFunc(mouse);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void _debug_state() {
    int i;

    printf("Shape:      %d\n", (int) (app_state.shape));
    printf("Capture:    %d\n", (int) (app_state.capture_mode));
    printf("Num Points: %d\n", app_state.curr_points);
    for (i = 0; i < app_state.curr_points; i++) {
        printf("(%3.2f, %3.2f), ", app_state.points[i].x, app_state.points[i].y);
    }
    printf("\n");
}

point_t _norm_coords_xy(int x, int y) {
    point_t p;

    if (app_state.geometry.w < 1)
        app_state.geometry.w = 1;
    if (app_state.geometry.h < 1)
        app_state.geometry.h = 1;

    p.x = 2.0 * x / app_state.geometry.w - 1.0;
    p.y = 1.0 - 2.0 * y / app_state.geometry.h;

    return p;
}

point_t _norm_coords_pt(point_t p) {
    point_t pr;

    if (app_state.geometry.w < 1)
        app_state.geometry.w = 1;
    if (app_state.geometry.h < 1)
        app_state.geometry.h = 1;

    pr.x = 2.0 * p.x / app_state.geometry.w - 1.0;
    pr.y = 1.0 - 2.0 * p.y / app_state.geometry.h;

    return pr;
}

