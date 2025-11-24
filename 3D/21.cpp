#include "21.h"

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
    for (auto& ball : Balls) {
        ball.add_XYZ(ball.velocity.x/10, ball.velocity.y / 10, ball.velocity.z / 10);
		// 벽과 충돌 검사
        if (ball.move_xyz.y > -3) {
            if (abs(ball.move_xyz.x) + ball.radius > 3.0f) {
                ball.velocity.x = -ball.velocity.x;
                ball.add_XYZ(ball.velocity.x / 10, ball.velocity.y / 10, ball.velocity.z / 10);
            }
            if (abs(ball.move_xyz.z) + ball.radius > 3.0f) {
                ball.velocity.z = -ball.velocity.z;
                ball.add_XYZ(ball.velocity.x / 10, ball.velocity.y / 10, ball.velocity.z / 10);
            }
        }
        if (!opened) {
            if (abs(ball.move_xyz.y) + ball.radius > 3.0f) {
                ball.velocity.y = -ball.velocity.y;
                ball.add_XYZ(ball.velocity.x / 10, ball.velocity.y / 10, ball.velocity.z / 10);
            }
        }
        else {
            if (ball.move_xyz.y + ball.radius > 3.0f) {
                ball.velocity.y = -ball.velocity.y;
                ball.add_XYZ(ball.velocity.x / 10, ball.velocity.y / 10, ball.velocity.z / 10);
            }
        }
        
    }
    if (yrotate) {
		camera.position = glm::rotateY(camera.position, glm::radians(dy_rotate));
	}
    if (opened) {
        if (space.open_angle > -80.f) {
            space.open_Draw();
			cout << space.open_angle << endl;
        }
    }
    glm::quat space_rotation = space.ro; // space의 현재 회전 쿼터니언을 가져옵니다.

    for (auto& cube : cubes) {
        // 큐브의 로컬 크기(절반)를 충돌 검사에 전달합니다.
        // (예: cube.multy.y의 절반 크기)
        float half_cube_size = cube.multy.y / 2.0f;

        // 1. 💡 수정된 충돌 검사: space_rotation을 전달
        cube.checkCollision(space_rotation, 3.0f, half_cube_size);

        // 2. 월드 회전이 적용된 낙하
        if (cube.is_falling) {
            // 💡 world_angle을 전달 (이전 단계에서 구현한 낙하 로직)
            cube.applyGravity_WorldSpace(world_angle);
        }
    }
    glutPostRedisplay();  // 화면 다시 그리기
    if (atime) {
        glutTimerFunc(16, TimerFunction, 1);  // 다음 타이머 설정
    }
}
void onKey(unsigned char key, int x, int y) {
    switch (key) {
    case 'z': camera.position.z += 1.0f; break;
    case 'Z': camera.position.z -= 1.0f; break;

    case 'y': yrotate = !yrotate; dy_rotate = 2.0f; break;
    case 'Y': yrotate = !yrotate; dy_rotate = -2.0f; break;

    case 'b':
    case 'B': initialize_spheres(1); break;

    case 'a': opened = !opened; break;
    }
    glutPostRedisplay();
}

void onSpecialKey(int key, int x, int y) {
    
    glutPostRedisplay();
}
void onSpecialKeyUp(int key, int x, int y) {
    
    glutPostRedisplay();
}
void onMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        // 마우스 버튼을 눌렀을 때
        mouse_rotate = true;
        startx = x;
        if (x > 250) {
            d_angle = -3.0f;
        }
        else {
            d_angle = 3.0f;
        }
        
    }
    else if (state == GLUT_UP) {
        // 마우스 버튼을 땔 때
        mouse_rotate = false;
    }
}
void onMouseMotion(int x, int y) {
    if (mouse_rotate) {
        world_angle = (x - startx) * d_angle;
    }
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

	space.Init();
    initialize_cubes();

    glutDisplayFunc(drawScene); //--- 출력 콜백 함수
    glutTimerFunc(16, TimerFunction, 1);  // 60 FPS
    glutKeyboardFunc(onKey); // 키보드
    glutSpecialFunc(onSpecialKey); // 특수키
    glutSpecialUpFunc(onSpecialKeyUp); // 떼기
    glutMouseFunc(onMouse); // 마우스 콜백 등록
    glutMotionFunc(onMouseMotion);
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
    // 깊이 테스트와 뒷면 제거 활성화
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // 뒷면 제거 설정
    glCullFace(GL_BACK);        // 뒷면을 제거
	glFrontFace(GL_CW);        // 시계방향 면을 앞면으로 설정
    
    result_matrix(camera, space);
	space.Update();
	space.Draw();

    glFrontFace(GL_CCW);        // 반시계방향 면을 앞면으로 설정
    for (auto& ball : Balls) {
        result_matrix(camera, ball);
        ball.Update();
        ball.Draw();
	}
    for (auto& cube : cubes) {
        result_matrix(camera,cube);
        cube.Update();
        cube.Draw();
    }

    glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}