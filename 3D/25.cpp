#include "25.h"

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
	if (light_rotate) {
		if (light_angle >= 360) light_angle = 0;
		light_angle += d_light_angle / ring;

		light.position.x = ring * 2.0f * cos(glm::radians(light_angle));
		light.position.z = ring * 2.0f * sin(glm::radians(light_angle));
		camera_cube.move_xyz = light.position;
		light.Init();
	}

	cube.rotatey += cube.rotate__;
	pyramid.rotatey += pyramid.rotate__;
	if (!change) {
		if (movement[0]) {
			if (-180.0f <= cube.Trotate + cube.d_Trotate
				&& 0.0f >= cube.Trotate + cube.d_Trotate) cube.Trotate += cube.d_Trotate;
			cout << cube.Trotate << endl;
		}
		if (movement[1]) {
			cube.Frotate += cube.d_Frotate;
			if (cube.Frotate == 360) cube.Frotate = 0;
		}
		if (movement[2]) {
			cube.Srotate += cube.d_Srotate;
			if (cube.Srotate == 360) cube.Srotate = 0;
		}
		if (movement[3]) {
			if (cube.Bmul.x + cube.d_Bmul.x <= 0.0f ||
				cube.Bmul.x + cube.d_Bmul.x >= 1.5f) {
				cube.d_Bmul *= -1;
			}
			cube.Bmul += cube.d_Bmul;
		}
	}
	else {
		if (movement[4]) {
			if (pyramid.Orotate + pyramid.d_Orotate >= 235 or pyramid.Orotate + pyramid.d_Orotate <= 0) {
				pyramid.d_Orotate *= -1;
			}
			pyramid.Orotate += pyramid.d_Orotate;
			cout << pyramid.Orotate << endl;
		}
		else if (movement[5]) {
			if (pyramid.Rrotate[pyramid.pyramid_face] + pyramid.d_Rrotate >= 120 || pyramid.Rrotate[pyramid.pyramid_face] + pyramid.d_Rrotate <= 0) {
				if ((pyramid.pyramid_face == 3 && !qkseofh) || (qkseofh && pyramid.pyramid_face == 0)) {
					pyramid.d_Rrotate *= -1;
					qkseofh = !qkseofh;
				}
				else {

					if (!qkseofh) {
						pyramid.pyramid_face++;
					}
					else pyramid.pyramid_face--;
				}
			}
			else pyramid.Rrotate[pyramid.pyramid_face] += pyramid.d_Rrotate;
		}
	}
	glutPostRedisplay();  // 화면 다시 그리기
	if (timer) {
		glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
	}
}
void onKey(unsigned char key, int x, int y) {
	if (key == 'p') change = !change;

	// 조명 켜기/끄기
	else if (key == '1') {
		light_off = !light_off;
		// 조명 활성화 여부를 셰이더에 전달
		GLuint lightEnabledLoc = glGetUniformLocation(shaderProgramID, "turn_off");
		glUniform1i(lightEnabledLoc, light_off ? 0 : 1);
	}
	else if (key == '2') {
		light_rotate = !light_rotate;
		d_light_angle = 3.0f;
	}
	else if (key == '@') {
		light_rotate = !light_rotate;
		d_light_angle = -3.0f;
	}
	else if (key == '3') {
		cube.ja_addRotation(0.0f, 5.0f, 0.0f);
		pyramid.ja_addRotation(0.0f, 5.0f, 0.0f);
	}
	else if (key == '#') {
		cube.ja_addRotation(0.0f, -5.0f, 0.0f);
		pyramid.ja_addRotation(0.0f, -5.0f, 0.0f);
	}
	else if (key == '4') {
		ring += 0.3f;
	}
	else if (key == '$') {
		ring -= 0.3f;
	}
	// 회전
	else if (key == 'h') {
		depth_test_enabled = !depth_test_enabled;
		if (depth_test_enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
	else if (key == 'u') {
		if (!glIsEnabled(GL_CULL_FACE)) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);        // 뒷면을 제거
			glFrontFace(GL_CCW);        // 반시계방향을 앞면으로 설정
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}
	else if (key == 'w') style = GL_LINES;
	else if (key == 'W') style = GL_TRIANGLES;
	else if (key == 'y' || key == 'Y') {
		if (!timer) {
			glutTimerFunc(16, TimerFunction, 1);
		}
		timer = true;
		radius_change = 1;
		cube.rotate__ = (key == 'y') ? 0.3f : -0.3f;
		pyramid.rotate__ = (key == 'y') ? 0.3f : -0.3f;
	}
	else if (key == 't' || key == 'f' || key == 's' || key == 'b' || key == 'o' || key == 'v')
	{
		if (!timer) {
			glutTimerFunc(16, TimerFunction, 1);
			timer = true;
		}
		if (key == 't') {
			if (movement[0]) {
				cube.d_Trotate *= -1;
				movement[0] = false;
			}
			else {
				movement[0] = true;
			}
		}
		else if (key == 'f') {
			if (movement[1]) {
				cube.d_Frotate *= -1;
				movement[1] = false;
			}
			else {
				movement[1] = true;
			}
		}
		else if (key == 's') {
			if (movement[2]) {
				cube.d_Srotate *= -1;
				movement[2] = false;
			}
			else {
				movement[2] = true;
			}
		}
		else if (key == 'b') {
			if (movement[3]) {
				movement[3] = false;
			}
			else movement[3] = true;
		}
		else if (key == 'o') {
			movement[5] = false;
			movement[4] = true;
			pyramid.Orotate = 0;
		}
		else if (key == 'v') {
			movement[4] = false;
			movement[5] = true;
			for (int i = 0; i < 4; pyramid.Rrotate[i++] = 0);
		}
	}

	else if (key == 'c') {
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
	//line.Init();
	camera_line.Init();
	camera_cube.Init();
	camera_cube.move_xyz = light.position;

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
	vertexSource = filetobuf("vertex25.glsl");
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
	fragmentSource = filetobuf("fragment25.glsl"); // 프래그세이더 읽어오기
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
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	// 각 객체의 회전 행렬을 셰이더에 전달
	// 깊이 테스트와 뒷면 제거 활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 뒷면 제거 설정
	glCullFace(GL_BACK);        // 뒷면을 제거
	glFrontFace(GL_CCW);        // 반시계방향을 앞면으로 설정
	
	light.Init();
	result_line_matrix(camera, camera_line);
	camera_line.Update();
	camera_line.r_Draw();

	if (!change) {
		result_matrix(camera, cube);
		cube.face_draw();
	}
	else {
		result_matrix(camera, pyramid);
		pyramid.face_draw();
	}

	result_matrix(camera, camera_cube);
	camera_cube.Draw(GL_LINES);

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}