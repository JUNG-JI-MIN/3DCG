#include "20.h"
vector<Vertex> body = {
    // 1. --- : 왼쪽-아래-뒤
    { glm::vec3(-5.0f, -2.0f, -5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 2. +-- : 오른쪽-아래-뒤
    { glm::vec3(5.0f, -2.0f, -5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 3. -+- : 왼쪽-위-뒤
    { glm::vec3(-5.0f,  2.0f, -5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 4. ++- : 오른쪽-위-뒤
    { glm::vec3(5.0f,  2.0f, -5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 5. --+ : 왼쪽-아래-앞
    { glm::vec3(-5.0f, -2.0f,  5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 6. +-+ : 오른쪽-아래-앞
    { glm::vec3(5.0f, -2.0f,  5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 7. -++ : 왼쪽-위-앞
    { glm::vec3(-5.0f,  2.0f,  5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) },

    // 8. +++ : 오른쪽-위-앞
    { glm::vec3(5.0f,  2.0f,  5.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) }
};
vector<unsigned int> body_index = {
    // 1. 앞면 (Front Face, +Z)
    // 꼭짓점 4, 5, 7, 6
    4, 5, 7,
    4, 7, 6,

    // 2. 뒷면 (Back Face, -Z)
    // 꼭짓점 1, 0, 2, 3
    1, 0, 2,
    1, 2, 3,

    // 3. 윗면 (Top Face, +Y)
    // 꼭짓점 2, 6, 7, 3
    2, 6, 7,
    2, 7, 3,

    // 4. 아랫면 (Bottom Face, -Y)
    // 꼭짓점 0, 1, 5, 4
    0, 1, 5,
    0, 5, 4,

    // 5. 왼쪽 면 (Left Face, -X)
    // 꼭짓점 0, 4, 6, 2
    0, 4, 6,
    0, 6, 2,

    // 6. 오른쪽 면 (Right Face, +X)
    // 꼭짓점 5, 1, 3, 7
    5, 1, 3,
    5, 3, 7
};

vector<Vertex> center = {
    // 1. --- : 왼쪽-아래-뒤
    { glm::vec3(-3.5f, -1.0f, -3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 2. +-- : 오른쪽-아래-뒤
    { glm::vec3(3.5f, -1.0f, -3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 3. -+- : 왼쪽-위-뒤
    { glm::vec3(-3.5f,  1.0f, -3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 4. ++- : 오른쪽-위-뒤
    { glm::vec3(3.5f,  1.0f, -3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 25. --+ : 왼쪽-아래-앞
    { glm::vec3(-3.5f, -1.0f,  3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 6. +-+ : 오른쪽-아래-앞
    { glm::vec3(3.5f, -1.0f,  3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 7. -++ : 왼쪽-위-앞
    { glm::vec3(-3.5f,  1.0f,  3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

    // 8. +++ : 오른쪽-위-앞
    { glm::vec3(3.5f,  1.0f,  3.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) } };
vector<unsigned int> center_index = {
    // 1. 앞면 (Front Face, +Z)
        // 꼭짓점 4, 5, 7, 6
        4, 5, 7,
        4, 7, 6,

        // 2. 뒷면 (Back Face, -Z)
        // 꼭짓점 1, 0, 2, 3
        1, 0, 2,
        1, 2, 3,

        // 3. 윗면 (Top Face, +Y)
        // 꼭짓점 2, 6, 7, 3
        2, 6, 7,
        2, 7, 3,

        // 4. 아랫면 (Bottom Face, -Y)
        // 꼭짓점 0, 1, 5, 4
        0, 1, 5,
        0, 5, 4,

        // 5. 왼쪽 면 (Left Face, -X)
        // 꼭짓점 0, 4, 6, 2
        0, 4, 6,
        0, 6, 2,

        // 6. 오른쪽 면 (Right Face, +X)
        // 꼭짓점 5, 1, 3, 7
        5, 1, 3,
        5, 3, 7
};

vector<Vertex> flag = {
    // 1. --- : 왼쪽-아래-뒤
    { glm::vec3(-0.5f, -4.0f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 2. +-- : 오른쪽-아래-뒤
    { glm::vec3(0.5f, -4.0f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 3. -+- : 왼쪽-위-뒤
    { glm::vec3(-0.5f,  4.0f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 4. ++- : 오른쪽-위-뒤
    { glm::vec3(0.5f,  4.0f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 5. --+ : 왼쪽-아래-앞
    { glm::vec3(-0.5f, -4.0f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 6. +-+ : 오른쪽-아래-앞
    { glm::vec3(0.5f, -4.0f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 7. -++ : 왼쪽-위-앞
    { glm::vec3(-0.5f,  4.0f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },

    // 8. +++ : 오른쪽-위-앞
    { glm::vec3(0.5f,  4.0f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }
};
vector<unsigned int> flag_index = {
    // 1. 앞면 (Front Face, +Z)
        // 꼭짓점 4, 5, 7, 6
        4, 5, 7,
        4, 7, 6,

        // 2. 뒷면 (Back Face, -Z)
        // 꼭짓점 1, 0, 2, 3
        1, 0, 2,
        1, 2, 3,

        // 3. 윗면 (Top Face, +Y)
        // 꼭짓점 2, 6, 7, 3
        2, 6, 7,
        2, 7, 3,

        // 4. 아랫면 (Bottom Face, -Y)
        // 꼭짓점 0, 1, 5, 4
        0, 1, 5,
        0, 5, 4,

        // 5. 왼쪽 면 (Left Face, -X)
        // 꼭짓점 0, 4, 6, 2
        0, 4, 6,
        0, 6, 2,

        // 6. 오른쪽 면 (Right Face, +X)
        // 꼭짓점 5, 1, 3, 7
        5, 1, 3,
        5, 3, 7
};

vector<Vertex> top = {
    // 1. --- : 왼쪽-아래-뒤
    { glm::vec3(-2.0f, -1.5f, -2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 2. +-- : 오른쪽-아래-뒤
    { glm::vec3(2.0f, -1.5f, -2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 3. -+- : 왼쪽-위-뒤
    { glm::vec3(-2.0f,  1.5f, -2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 4. ++- : 오른쪽-위-뒤
    { glm::vec3(2.0f,  1.5f, -2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 5. --+ : 왼쪽-아래-앞
    { glm::vec3(-2.0f, -1.5f,  2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 6. +-+ : 오른쪽-아래-앞
    { glm::vec3(2.0f, -1.5f,  2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 7. -++ : 왼쪽-위-앞
    { glm::vec3(-2.0f,  1.5f,  2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

    // 8. +++ : 오른쪽-위-앞
    { glm::vec3(2.0f,  1.5f,  2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }
};
vector<unsigned int> top_index = {
    // 1. 앞면 (Front Face, +Z)
        // 꼭짓점 4, 5, 7, 6
        4, 5, 7,
        4, 7, 6,

        // 2. 뒷면 (Back Face, -Z)
        // 꼭짓점 1, 0, 2, 3
        1, 0, 2,
        1, 2, 3,

        // 3. 윗면 (Top Face, +Y)
        // 꼭짓점 2, 6, 7, 3
        2, 6, 7,
        2, 7, 3,

        // 4. 아랫면 (Bottom Face, -Y)
        // 꼭짓점 0, 1, 5, 4
        0, 1, 5,
        0, 5, 4,

        // 5. 왼쪽 면 (Left Face, -X)
        // 꼭짓점 0, 4, 6, 2
        0, 4, 6,
        0, 6, 2,

        // 6. 오른쪽 면 (Right Face, +X)
        // 꼭짓점 5, 1, 3, 7
        5, 1, 3,
        5, 3, 7
};

vector<Vertex> posin = {
    // 1. --- : 왼쪽-아래-뒤
    { glm::vec3(-0.5f, -0.5f, -4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 2. +-- : 오른쪽-아래-뒤
    { glm::vec3(0.5f, -0.5f, -4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 3. -+- : 왼쪽-위-뒤
    { glm::vec3(-0.5f,  0.5f, -4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 4. ++- : 오른쪽-위-뒤
    { glm::vec3(0.5f,  0.5f, -4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 5. --+ : 왼쪽-아래-앞
    { glm::vec3(-0.5f, -0.5f,  4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 6. +-+ : 오른쪽-아래-앞
    { glm::vec3(0.5f, -0.5f,  4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 7. -++ : 왼쪽-위-앞
    { glm::vec3(-0.5f,  0.5f,  4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },

    // 8. +++ : 오른쪽-위-앞
    { glm::vec3(0.5f,  0.5f,  4.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }
};
vector<unsigned int> posin_index = {
    // 1. 앞면 (Front Face, +Z)
        // 꼭짓점 4, 5, 7, 6
        4, 5, 7,
        4, 7, 6,

        // 2. 뒷면 (Back Face, -Z)
        // 꼭짓점 1, 0, 2, 3
        1, 0, 2,
        1, 2, 3,

        // 3. 윗면 (Top Face, +Y)
        // 꼭짓점 2, 6, 7, 3
        2, 6, 7,
        2, 7, 3,

        // 4. 아랫면 (Bottom Face, -Y)
        // 꼭짓점 0, 1, 5, 4
        0, 1, 5,
        0, 5, 4,

        // 5. 왼쪽 면 (Left Face, -X)
        // 꼭짓점 0, 4, 6, 2
        0, 4, 6,
        0, 6, 2,

        // 6. 오른쪽 면 (Right Face, +X)
        // 꼭짓점 5, 1, 3, 7
        5, 1, 3,
        5, 3, 7
};
Tank tank(glm::vec3{ 0.0f,0.0f ,0.0f });

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
    if (tank_move[0]) {
        tank.moveing_man(-1.0f, 0.0f, 0.0f);
    }
    else if (tank_move[1]) {
        tank.moveing_man(1.0f, 0.0f, 0.0f);
    }
    if (tank_move[2]) {
        tank.moveing_man(0.0f, 0.0f, -1.0f);
    }
    else if (tank_move[3]) {
        tank.moveing_man(0.0f, 0.0f, 1.0f);
    }

    if (tro) tank.y_rotate();
    if (lro) {
        tank.Top_trace();
    }
    if (gro) tank.posin_Y();
    if (pro) tank.flag_X();
    if (yro) camera.camera_PY();
    else if (rro) camera.camera_TY();

	glutPostRedisplay();  // 화면 다시 그리기
    if (atime) {
        glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
    }
}
void all_reset() {
    tank.reset();
    camera.reset();
    rotate_tank_angle = 0.0f; rotate_posin_angle = 0.0f; rotate_flag_angle = 0.0f;
    d_posin_angle = 0.03f; d_flag_angle = 0.03f;
    RotateCamera = 0.0f; d_camera = 3.0f;
    atime = true;
	tro = false; lro = false; gro = false; pro = false; yro = false;
}
void onKey(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'z': camera.position.z -= 1.0f; break;
    case 'Z': camera.position.z += 1.0f; break;

    case 'x': camera.position.x -= 1.0f; break;
    case 'X': camera.position.x += 1.0f; break;

    case 'y': yro = !yro; d_camera = 3.0f; rro = false; break;
    case 'Y': yro = !yro; d_camera = -3.0f; rro = false; break;

    case 'r': rro = !rro; d_camera = 3.0f; yro = false; break;
    case 'R': rro = !rro; d_camera = -3.0f; yro = false; break;

    case 't': tro = !tro; break;
    case 'l': 
        tank.ttop.part_xyz.x *= -1; 
        tank.tflag.part_xyz.x *= -1;
        tank.tposin.part_xyz.x *= -1;
        lro = true; 
        break;
    case 'g': gro = !gro; break;
    case 'p': pro = !pro; break;

    case 'o':
        if (!atime)  glutTimerFunc(16, TimerFunction, 1);
        atime = !atime; break;
    case 'c': all_reset();  break;
    }
	glutPostRedisplay();
}

void onSpecialKey(int key, int x, int y) {
        switch (key)
        {
        case GLUT_KEY_LEFT: tank_move[0] = true; tank_move[1] = false; break;
        case GLUT_KEY_RIGHT:tank_move[1] = true; tank_move[0] = false; break;
        case GLUT_KEY_UP:tank_move[2] = true; tank_move[3] = false; break;
        case GLUT_KEY_DOWN: tank_move[3] = true; tank_move[2] = false; break;
        }
	glutPostRedisplay();
}
void onSpecialKeyUp(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_LEFT: tank_move[0] = false; break;
    case GLUT_KEY_RIGHT:tank_move[1] = false; break;
    case GLUT_KEY_UP:tank_move[2] = false; break;
    case GLUT_KEY_DOWN:  tank_move[3] = false; break;
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
	shaderProgramID = make_shaderProgram(); //--- 세이더 프로그램 만들기

    tank.Init();

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutTimerFunc(16, TimerFunction, 1);  // 60 FPS
	glutKeyboardFunc(onKey); // 키보드
	glutSpecialFunc(onSpecialKey); // 특수키
    glutSpecialUpFunc(onSpecialKeyUp); // 떼기
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	// 각 객체의 회전 행렬을 셰이더에 전달
	// 
	// 깊이 테스트와 뒷면 제거 활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 뒷면 제거 설정
	glCullFace(GL_BACK);        // 뒷면을 제거
    tank.Update();
    tank.Draw();

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}