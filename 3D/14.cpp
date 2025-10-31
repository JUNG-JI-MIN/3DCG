#include "14.h"

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void vector_reset() {
	Left.vertices.clear();
	Left.indices.clear();
	Right.vertices.clear();
	Right.indices.clear();
}
// 타이머 함수 구현
void TimerFunction(int value) {

	for (int i = 0;i < 3;i++) {
		mumu[i] -= asd;
	}
	if (mumu[0] <= 0.1f or mumu[0] >= 1.5f) asd *= -1;
	if (radius_change == 1) {
		rotationAngle += 2.0f;    // 매 프레임마다 2도씩 회전
	}
	else {
		rotationAngle -= 2.0f;    // 매 프레임마다 2도씩 회전
	}
	if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
	if (rotationAngle <= -360.0f) rotationAngle += 360.0f;

	glutPostRedisplay();  // 화면 다시 그리기
	if (running) glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
}
void onKey(unsigned char key, int x, int y) {
	if (key == 'c') {
		radius_change *= -1;
	}
	if (key == 's') {
		running = !running;
		if (running) {
			glutTimerFunc(16, TimerFunction, 1);
		}
	}
	if (key == 't') {
		tspin = !tspin;
		if (tspin) {
			center_x = 0.25f;
			center_y = 0.0f;
		}
	}
}
void onMouse(int button, int state, int x, int y) {
	float mx = (x / (float)width) * 2.0f - 1.0f;
	float my = 1.0f - (y / (float)height) * 2.0f;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		spin = true;
		center_x = mx;
		center_y = my;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		spin = false;
	}
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(NULL));
	width = 500;
	height = 500;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	
	Left.Init();
	Right.Init();

	shaderProgramID = make_shaderProgram();
	//--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutTimerFunc(16, TimerFunction, 1);  // 60 FPS
	glutKeyboardFunc(onKey); // 키보드
	glutMouseFunc(onMouse); // 마우스 콜백 등록
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(shaderID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return shaderID;
}

GLvoid drawScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	// 각 객체의 회전 행렬을 셰이더에 전달
	cout << spin << endl;
	if (spin) {
		Left.set_state(rotationAngle, center_x, center_y, mumu);
		Left.matrix_updata_vo2();
		Left.Update();
		Left.Draw();
		Right.set_state(-rotationAngle, center_x, center_y, mumu);
		Right.matrix_updata_vo2();
		Right.Update();
		Right.Draw();
	}
	else if (tspin) {
		Left.set_state(rotationAngle, 0.25f, 0.0f, mumu);
		Left.matrix_updata3();
		Left.Update();
		Left.Draw();
		Right.set_state(-rotationAngle, -0.25f, 0.0f, mumu);
		Right.matrix_updata3();
		Right.Update();
		Right.Draw();
	}
	else {
		Left.set_state(rotationAngle, 0.25, 0, mumu);
		Left.matrix_updata();
		Left.Update();
		Left.Draw();
		Right.set_state(-rotationAngle, -0.25, 0, mumu);
		Right.matrix_updata();
		Right.Update();
		Right.Draw();
	}

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}