#include <GL/glut.h>
#include <cstdio>
#include <iostream>

using namespace std;

constexpr auto WindowsTitle = "����";
constexpr const GLuint WindowsWidth = 400;
constexpr const GLuint WindowsHeight = 400;
constexpr const GLuint BMP_Header_Length = 54;
GLuint texture;
GLint width, height, total_bytes;
GLubyte* pixels = nullptr;

void display() {
	//�����Ļ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//����������������
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex2f(0.0f, 0.5f);
		glTexCoord2f(2.0f, 0.0f); glVertex2f(0.5f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

int main(int argc,char* argv[]) {
	//GLUT��ʼ��
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowsWidth, WindowsHeight);
	glutCreateWindow(WindowsTitle);

	//���ļ������ʧ�ܣ�����
	FILE* pFile;
	fopen_s(&pFile, "wood.bmp", "rb");
	if (pFile == nullptr)
		return 3;

	//��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, sizeof(width), 1, pFile);
	fread(&height, sizeof(height), 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);


	//����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}


	//�����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == nullptr) {
		cout << 1 << endl;
		fclose(pFile);
		return 1;
	}

	//��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0) {
		cout << 2 << endl;
		free(pixels);
		fclose(pFile);
		return 2;
	}
	fclose(pFile);
	
	glutDisplayFunc(display);
	glutMainLoop();

	free(pixels);
	return 0;
}