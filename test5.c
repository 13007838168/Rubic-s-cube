#include <GL/glut.h>
#include <string.h>

// 文本框属性
int textPosX = 100;
int textPosY = 100;
int textWidth = 200;
int textHeight = 50;

// 文本框内容
char textBoxContent[1024] = "";

void renderText(const char* text, float x, float y)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void renderTextBox()
{
    // 绘制文本框的边框
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(textPosX, textPosY);
    glVertex2i(textPosX + textWidth, textPosY);
    glVertex2i(textPosX + textWidth, textPosY + textHeight);
    glVertex2i(textPosX, textPosY + textHeight);
    glEnd();

    // 绘制文本框内容
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(textBoxContent, textPosX + 5, textPosY + textHeight - 15);
}

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制文本框
    renderTextBox();

    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 13) // 回车键
    {
        // 处理回车事件
        // 可以在这里保存文本框内容或执行其他操作
        memset(textBoxContent, 0, sizeof(textBoxContent));
    }
    else if (key == 8) // 退格键
    {
        int len = strlen(textBoxContent);
        if (len > 0)
        {
            textBoxContent[len - 1] = '\0';
        }
    }
    else
    {
        int len = strlen(textBoxContent);
        if (len < sizeof(textBoxContent) - 1)
        {
            textBoxContent[len] = key;
            textBoxContent[len + 1] = '\0';
        }
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Text Box");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 400, 0, 300, -1, 1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
