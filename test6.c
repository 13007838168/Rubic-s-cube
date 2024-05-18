#include <GL/glut.h>

void renderBitmapString(float x, float y, void *font, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(100, 100, GLUT_BITMAP_HELVETICA_18, "Hello, OpenGL!");
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutCreateWindow("OpenGL Bitmap Font Example");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 300.0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
