#include <GL/glut.h>
#include <stdio.h>

// 矩形左下角和右上角的坐标
float rectLeft = -0.5f;
float rectBottom = -0.5f;
float rectRight = 0.5f;
float rectTop = 0.5f;

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制矩形
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(rectLeft, rectBottom);
    glVertex2f(rectRight, rectBottom);
    glVertex2f(rectRight, rectTop);
    glVertex2f(rectLeft, rectTop);
    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // 转换为NDC坐标
        float ndcX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
        float ndcY = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f - 1.0f;

        // 判断是否在矩形内
        if (ndcX >= rectLeft && ndcX <= rectRight && ndcY >= rectBottom && ndcY <= rectTop)
        {
            printf("Clicked inside the rectangle at (%d, %d), NDC: (%f, %f)\n", x, y, ndcX, ndcY);
        }
        else
        {
            printf("Clicked outside the rectangle at (%d, %d), NDC: (%f, %f)\n", x, y, ndcX, ndcY);
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Mouse Events");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
