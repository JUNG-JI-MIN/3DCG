#include "28.h"

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
	// 사람눈
	if (snow_man) {
		for (auto& s : snows) {
			if (s->plus_xyz.y >= 20) s->plus_xyz.y = 19.9f;
		}
	}
	for (auto& s : snows) {
		if (s->plus_xyz.y <= 19.9f && s->plus_xyz.y > -50) s->plus_xyz.y -= s->d_angle;
		else s->plus_xyz.y = 20.0f;
	}


	// 조명 회전
	if (light_rotate) {
		if (light_angle >= 360) light_angle = 0;
		light_angle += d_light_angle;

		light.position.x = light_di * cos(glm::radians(light_angle));
		light.position.z = light_di * sin(glm::radians(light_angle));
	}

	// 행성, 위성 자전 및 공전
	for (Shape* s : shapes) {
		s->ja_addRotation(0, s->d_angle, 0);
	}
	for (Shape* s : shapes) {
		// if (s == &c4) continue;
		float x = ring * 1.5f * cos(s->real_gong_angle);
		float z = ring * 1.5f * sin(s->real_gong_angle);
		if (s == &b1 || s == &b2 || s == &b3) {
			float b_x = ring * 0.5f * cos(s->real_gong_angle);
			float b_z = ring * 0.5f * sin(s->real_gong_angle);
			s->B_xyz = { b_x,0,b_z };
		}
		s->real_gong_angle += s->d_real_gong_angle;
		if (s->real_gong_angle >= 360) s->real_gong_angle = 0;
		s->set_XYZ(x, 0, z);
	}
	if (zrotate[0]) {
		for (Shape* s : shapes) {
			s->gong_angle += 0.3f;
		}
		for (Line* l : lines) {
			l->rotate_angle += 0.3f;
		}
	}
	else if (zrotate[1]) {
		for (Shape* s : shapes) {
			s->gong_angle -= 0.3f;
		}
		for (Line* l : lines) {
			l->rotate_angle -= 0.3f;
		}
	}
	// shapes와 lines 동기화
	d1.xyz = a1.move_xyz;
	d2.xyz = a2.move_xyz;
	d3.xyz = a3.move_xyz;

	glutPostRedisplay();  // 화면 다시 그리기
	glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
}
void moving(float dx, float dy, float dz) {
	for (Shape* s : shapes) {
		s->add_XYZ(dx, dy, dz);
	}
	for (Line* l : lines) {
		l->add_XYZ(dx, dy, dz);
	}
}
void onKey(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'p': trand = false; break;
	case 'P': trand = true; break;
	case 'm': wi = true; break;
	case 'M': wi = false;  break;
	case 'o':
	case 'O':
		for (Shape* s : shapes) {
			s->reset_state();
			//if (s == &c4) c4.move_xyz.x = 0.0f;
		}
		ring = 1;
		trand = true;
		wi = false;
		zrotate[0] = false;
		zrotate[1] = false;
		light.light_color = { 1,1,1 };
		light.apply_color();
		break;
	case 'y': ring += 0.3f; break;
	case 'Y': ring -= 0.3f; break;
	case 'z':
		zrotate[0] = true;
		zrotate[1] = false;
		break;
	case 'Z':
		zrotate[1] = true;
		zrotate[0] = false;
		break;


	case 'w': moving(0.0f, 0.5f, 0.0f); break;
	case 'a': moving(-0.5f, 0.0f, 0.0f); break;
	case 's': moving(0.0f, -0.5f, 0.0f); break;
	case 'd': moving(0.5f, 0.0f, 0.0f); break;

	case '+': moving(0.0f, 0.0f, 0.5f); break;
	case '-': moving(0.0f, 0.0f, -0.5f); break;

		// 조명 관련

	case '1':
		light.light_color = { light_color[light_color_count][0],
							light_color[light_color_count][1],
							light_color[light_color_count][2] };
		light_color_count = (light_color_count + 1) % 5;
		light.apply_color();
		break;
	case '2':
		light_rotate = !light_rotate;
		d_light_angle = 3.0f;
		break;
	case '@':
		light_rotate = !light_rotate;
		d_light_angle = -3.0f;
		break;
	case '3':
		light_di += 0.2f;
		light.position.x = light_di * cos(glm::radians(light_angle));
		light.position.z = light_di * sin(glm::radians(light_angle));
		break;
	case '#':
		light_di -= 0.2f;
		light.position.x = light_di * cos(glm::radians(light_angle));
		light.position.z = light_di * sin(glm::radians(light_angle));
		break;
	case '4':
		if (light.light_color.x < 1 && light.light_color.y < 1 && light.light_color.z < 1)
			light.light_color += glm::vec3(0.1f, 0.1f, 0.1f);
		break;
	case '$':
		if (light.light_color.x > 0 && light.light_color.y > 0 && light.light_color.z > 0)
			light.light_color -= glm::vec3(0.1f, 0.1f, 0.1f);
		break;
	case '5': // 눈 내리는 기능
		snow_man = !snow_man;
		break;
	case 'q': exit(1);
	}
	glutPostRedisplay();
}
void onSpecialKey(int key, int x, int y) {
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
	
	
	snows_init();

	lines.push_back(&c1);
	lines.push_back(&c2);
	lines.push_back(&c3);

	lines.push_back(&d1);
	lines.push_back(&d2);
	lines.push_back(&d3);

	shapes.push_back(&pyramid);
	//shapes.push_back(&c4);

	shapes.push_back(&a1);
	shapes.push_back(&a2);
	shapes.push_back(&a3);

	shapes.push_back(&b1);
	shapes.push_back(&b2);
	shapes.push_back(&b3);

	for (Line* l : lines) {
		l->Init();
	}
	for (Shape* s : shapes) {
		s->Init();
		if (s == &pyramid) {
			continue;
		}
		s->d_angle = random(0.0f, 3.0f);
		if (s == &a1 || s == &a2 || s == &a3)
			s->d_real_gong_angle = random(0.0f, 0.03f);
	}
	b1.d_real_gong_angle = a1.d_real_gong_angle;
	b2.d_real_gong_angle = a2.d_real_gong_angle;
	b3.d_real_gong_angle = a3.d_real_gong_angle;
	cout << "'o가 초기화'" << endl;

	screen_quad.Init();

	light_cube.Init();

	cout << "\n========== 조명 관련 명령어 ==========" << endl;
	cout << "'1' : 조명 색상 변경 (5가지 프리셋)" << endl;
	cout << "'2' : 조명 시계방향 회전 ON/OFF" << endl;
	cout << "'@' : 조명 반시계방향 회전 ON/OFF" << endl;
	cout << "'3' : 조명 거리 증가" << endl;
	cout << "'#' : 조명 거리 감소" << endl;
	cout << "'4' : 조명 밝기 증가" << endl;
	cout << "'$' : 조명 밝기 감소" << endl;
	cout << "'5' : 눈 내리는 기능 " << endl;
	cout << "===================================\n" << endl;
	// 카메라 위치도 셰이더에 전달
	GLuint u = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3fv(u, 1, glm::value_ptr(camera.position));
	shaderProgramID = make_shaderProgram(); //--- 세이더 프로그램 만들기

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
	// 
	// 깊이 테스트와 뒷면 제거 활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 뒷면 제거 설정
	glCullFace(GL_BACK);        // 뒷면을 제거
	light.Init();

	for (Line* l : lines) {
		result_line_matrix(camera, *l);
		l->Update();
		l->Draw();
	}
	for (Shape* s : shapes) {
		if (s == &pyramid) {
			result_matrix2(camera, *s);
			s->Update();
			s->Draw();
			continue;
		}
		result_matrix(camera, *s);
		s->Update();
		s->Draw();
	}

	result_matrix2(camera, screen_quad);
	screen_quad.Draw();

	light_cube.plus_xyz = light.position;
	result_matrix2(camera, light_cube);
	light_cube.Draw();

	for (auto& s : snows) {
		result_matrix2(camera, *s);
		s->Draw();
	}

	

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}