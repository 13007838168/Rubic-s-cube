#include <stdio.h>
#include <stdlib.h>
int main() {
    FILE* file = fopen("D:\\codes\\ccodes\\MY_PROJECT\\Rubic's_Cube\\a.txt", "r"); // 打开文件，以只读模式

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // 计算文件大小
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file); // 将文件指针重新定位到文件开头

    char* buffer = (char*) malloc(size + 1); // 为文件内容分配内存

    fread(buffer, size, 1, file); // 读取文件内容到缓冲区
    fclose(file);

    buffer[size] = '\0'; // 将缓冲区最后一个字符设置为 '\0'，以便将其作为 C 字符串使用

    printf("File contents:\n%s\n", buffer);

    free(buffer); // 释放内存

    return 0;
}
