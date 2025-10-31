#include "16.h"

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
// 타이머 함수 구현
void TimerFunction(int value) {
	if (radius_change == 0)
	{
		cube.addRotation(cube.rotate__, 0.0f); // X축 회전 변화량 누적
		pyramid.addRotation(pyramid.rotate__, 0.0f);
	}
	else
	{
		cube.addRotation(0.0f, cube.rotate__); // Y축 회전 변화량 누적
		pyramid.addRotation(0.0f, pyramid.rotate__);
	}
	glutPostRedisplay();  // 화면 다시 그리기
	if (timer) {
		glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
	}
}
void onKey(unsigned char key, int x, int y) {
	if (key == 'c') change = false;
	else if (key == 'p') change = true;
	if (key == 'h') {
		depth_test_enabled = !depth_test_enabled;
		if (depth_test_enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
	if (key == 'u') {
		back = !back;
		if (back) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);        // 뒷면을 제거
			glFrontFace(GL_CCW);        // 반시계방향을 앞면으로 설정
		}
		else {
			glFrontFace(GL_CW);
		}
	}
	if (key == 'w') style = GL_LINES;
	else if (key == 'W') style = GL_TRIANGLES;
	if (key == 'x' || key == 'X') {
		if (!timer) {
			glutTimerFunc(16, TimerFunction, 1);
		}
		timer = true;
		radius_change = 0;
		cube.rotate__ = (key == 'x') ? 0.3f : -0.3f;
		pyramid.rotate__ = (key == 'x') ? 0.3f : -0.3f;
	}
	else if (key == 'y' || key == 'Y') {
		if (!timer) {
			glutTimerFunc(16, TimerFunction, 1);
		}
		timer = true;
		radius_change = 1;
		cube.rotate__ = (key == 'y') ? 0.3f : -0.3f;
		pyramid.rotate__ = (key == 'y') ? 0.3f : -0.3f;
	}
	if (key == 's') {
		cube.reset_state();
		pyramid.reset_state();
		timer = false;
	}
		glutPostRedisplay();
}
void onSpecialKey(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		cube.move_xyz.x -= 0.1f;
		pyramid.move_xyz.x -= 0.1f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		cube.move_xyz.x += 0.1f;
		pyramid.move_xyz.x += 0.1f;
	}
	else if (key == GLUT_KEY_UP) {
		cube.move_xyz.y += 0.1f;
		pyramid.move_xyz.y += 0.1f;
	}
	else if (key == GLUT_KEY_DOWN) {
		cube.move_xyz.y -= 0.1f;
		pyramid.move_xyz.y -= 0.1f;
	}
	glutPostRedisplay();
}
void onMouse(int button, int state, int x, int y) {
	float mx = (x / (float)width) * 2.0f - 1.0f;
	float my = 1.0f - (y / (float)height) * 2.0f;

}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(NULL));
	width = 500;
	height = 500;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	
	cube.Init();
	pyramid.Init();
	line.Init();

	shaderProgramID = make_shaderProgram();
	//--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutTimerFunc(16, TimerFunction, 1);  // 60 FPS
	glutKeyboardFunc(onKey); // 키보드
	glutSpecialFunc(onSpecialKey); // 특수키
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
	glClearColor(0.0,0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	// 각 객체의 회전 행렬을 셰이더에 전달
	// 깊이 테스트와 뒷면 제거 활성화
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	// 뒷면 제거 설정
	if (back) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}
	else {
		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}
	line.matrix_updata(); // uModel 
	camera.View_matrix_updata(); // uView
	camera.Projection_matrix_updata(); // uProj
	
	line.Update();
	line.Draw();

	cube.matrix_updata(); // uModel
	if (!change) {
		cube.Update();
		cube.Draw(style);
	}
	else {
		pyramid.Update();
		pyramid.Draw(style);
	}

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}