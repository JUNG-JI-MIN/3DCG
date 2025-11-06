#include "18.h"

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
	if (animate_z) {
		if (step == 0) {
			if (objects[0]->xyz.y < 1.0f) {
				objects[0]->xyz.y += 0.1f;
				objects[1]->xyz.y -= 0.1f;
			}
			else {
				step = 1;
			}
		}
		else if (step == 1) {
			if (objects[0]->vang.x > 0) {
				if (objects[0]->xyz.x < objects[0]->vang.x) {
					objects[0]->xyz.x += 0.1f;
					objects[1]->xyz.x -= 0.1f;
				}
				else {
					step = 2;
				}
			}
			else {
				if (objects[0]->xyz.x > objects[0]->vang.x) {
					objects[0]->xyz.x -= 0.1f;
					objects[1]->xyz.x += 0.1f;
				}
				else {
					step = 2;
				}
			}
		}
		else if (step == 2) {
			if (objects[0]->xyz.y > 0.0f) {
				objects[0]->xyz.y -= 0.1f;
				objects[1]->xyz.y += 0.1f;
			}
			else {
				animate_z = false;
			}
		}
	}
	if (animate_t) {
		for (Shape* s : objects) {
			s->add_XYZ(
				(s->vang.x - s->xyz.x) / 10,
				(s->vang.y - s->xyz.y) / 10,
				(s->vang.z - s->xyz.z) / 10);
		}
		if (glm::length(objects[0]->vang - objects[0]->xyz) < 0.1f &&
			glm::length(objects[1]->vang - objects[1]->xyz) < 0.1f) {
			animate_t = false;
		}
	}
	if (animate_u) {
		// u: 원형 경로로 회전하면서 이동
		for (Shape* s : objects) {
			s->angle += 5.0f;

			// 원형 경로 계산 (반지름 3)
			float rad = glm::radians(s->angle);
			float x = 3.0f * cos(rad);
			float y = 3.0f * sin(rad);
			s->set_state({ x, y, s->xyz.z }, s->multy);
		}
		ani_count++;
		if (ani_count >= 36) {
			animate_u = false;
		}
	}
	for (Shape* s : objects) {
		if (!s->able) continue;
		if (jaX) {
			s->ja_addRotation(s->d_ja_angleX, 0.0f, 0.0f);
		}
		if (jaY) {
			s->ja_addRotation(0.0f, s->d_ja_angleY, 0.0f);
		}
		if (gong) {
			s->add_gongRotation(s->d_gong_angle);
		}
		if (scales) {
			s->add_scale(s->d_scale);
		}
		if (onescale) {
			s->add_onemult(s->d_onemul);
		}
	}
	glutPostRedisplay();  // 화면 다시 그리기
	glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
	
}
void onKey(unsigned char key, int x, int y) {
	bool flagToggled = false;
	if (key == '1' || key == '2' || key == '3') {
		for (Shape* s : objects) {
			if (key == '1') {
				if (!flagToggled) {
					s->able = true;
					flagToggled = true;
				}
				else s->able = false;
			}
			else if (key == '2') {
				if (!flagToggled) {
					s->able = false;
					flagToggled = true;
				}
				else s->able = true;
			}
			else if (key == '3') {
				s->able = true;
			}
		}
	}
	flagToggled = false;
	for (Shape* s : objects) {
		if (!s->able) continue;


		switch (key)
		{
		case 'x': s->d_ja_angleX = 5.0f; jaX = true; break;
		case 'X': s->d_ja_angleX = -5.0f; jaX = true;  break;

		case 'y': s->d_ja_angleY = -5.0f; jaY = true;  break;
		case 'Y': s->d_ja_angleY = 5.0f; jaY = true;  break;

		case 'r': s->d_gong_angle = -5.0f; flagToggled = true; break;
		case 'R': s->d_gong_angle = 5.0f; flagToggled = true; break;

		case 'a': s->d_scale = 0.05f; flagToggled = true; break;
		case 'A': s->d_scale = -0.05f; flagToggled = true; break;

		case 'b': s->d_onemul = 0.05f; flagToggled = true; break;
		case 'B': s->d_onemul = -0.05f; flagToggled = true; break;

		case 'd': s->add_XYZ(-0.1f, 0.0f, 0.0f); break;
		case 'D': s->add_XYZ(0.1f, 0.0f, 0.0f); break;

		case 'e': s->add_XYZ(0.0f, -0.1f, 0.0f); break;
		case 'E': s->add_XYZ(0.0f, 0.1f, 0.0f); break;

		case 't':
			if (!animate_t && !animate_u && !animate_z) {
				animate_t = true;
				objects[0]->vang = objects[1]->xyz;
				objects[1]->vang = objects[0]->xyz;
			}
			break;
		case 'u':
			
			if (!animate_t && !animate_u && !animate_z) {
				animate_u = true;
				objects[0]->vang = objects[1]->xyz;
				objects[1]->vang = objects[0]->xyz;
				ani_count = 0;
			}
			break;
		case 'z':
			if (!animate_t && !animate_u &&!animate_z) {
				animate_z = true;
				objects[0]->vang = objects[1]->xyz;
				objects[1]->vang = objects[0]->xyz;
				step = 0;
			}
				break;
		case 'v': 
			if (!flagToggled) {
				s->d_scale = 0.05f;
				flagToggled = true;
			}
			else {
				s->d_scale = -0.05f;
			}
			jaX = true;
			jaY = true;
			gong = true;
			scales = true;
			break;
		case 'V':
			if (!flagToggled) {
				s->d_scale = -0.05f;
				flagToggled = true;
			}
			else {
				s->d_scale = 0.05f;
			}
			jaX = true;
			jaY = true;
			gong = true;
			scales = true;
			break;
		case 'c':
			flagToggled = true;

			if (change) {
				objects.clear();
				objects.push_back(&cube);
				objects.push_back(&pyramid);
			}
			else {
				objects.clear();
				objects.push_back(&prism);
				objects.push_back(&frustum);
			}
			break;
		case 's': 
			for (Shape* s : objects) {
				s->reset_state();
				jaY = false;
				jaX = false;
				gong = false;
				scales = false;
				onescale = false;
			}
			break;
		case 'q': break;
		}
		
	}
	if (flagToggled) {
		if (key == 'r' || key == 'R') gong = !gong;
		else if (key == 'a' || key == 'A') scales = !scales;
		else if (key == 'b' || key == 'B') onescale = !onescale;
		else if (key == 'c') change = !change;
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

	line.Init();

	// 객체 리스트에 객체 주소 추가
	objects.clear();
	objects.push_back(&cube);
	objects.push_back(&pyramid);

	// 객체 일괄 초기화
	for (Shape* s : objects) {
		if (s == &cube) s->set_state(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		else if (s == &pyramid) s->set_state(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		s->Init();
		s->able = true;
	}
	frustum.Init();
	prism.Init();
	frustum.set_state(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	prism.set_state(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	
	cube.angle = 180.0f;
	prism.angle = 180.0f;
	frustum.angle = 0.0f;
	pyramid.angle = 0.0f;

	shaderProgramID = make_shaderProgram();

	cout << "---------- 명령어 ----------" << endl;
	cout << "x/X: 객체의 x축에 대하여 각각 양/음 방향으로 회전하기 (자전)" << endl;
	cout << "y/Y: 객체의 y축에 대하여 각각 양/음 방향으로 회전하기 (자전)" << endl;
	cout << "r/R: 중앙의 y축에 대하여 양/음 방향으로 회전하기 (공전)" << endl;
	cout << "a/A: 도형이 제자리에서 확대/축소" << endl;
	cout << "b/B: 도형이 원점에 대해서 확대/축소 (위치도 바뀐다.)" << endl;
	cout << "d/D: 도형이 x축에서 좌/우로 이동" << endl;
	cout << "e/E: 도형이 y축에서 위/아래로 이동" << endl;
	cout << "t: 두 도형이 원점을 통과하며 상대방의 자리로 이동하는 애니메이션" << endl;
	cout << "u: 두 도형이 한 개는 위로, 다른 도형은 아래로 이동하면서 상대방의 자리로 이동하는 애니메이션" << endl;
	cout << "v: 키보드 5: 두 도형이 한 개는 확대, 다른 한 개는 축소되며 자전과 공전하기" << endl;
	cout << "c: 두 도형을 다른 도형으로 바꾼다." << endl;
	cout << "s: 초기화 하기" << endl;
	cout << "q: 프로그램 종료" << endl;
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

	result_line_matrix(camera, line);
	line.Update();
	line.Draw();

	for (Shape* s : objects) {
		result_matrix(camera, *s); // *s를 사용하여 Shape 객체 자체를 전달
		s->Update();
		s->Draw();
		
	}
	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}