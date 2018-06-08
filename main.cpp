
// includes
#include <GL/glut.h>


// misc
static constexpr GLsizei NUM_COORDINATES = 3;
static constexpr GLsizei NUM_COLOR_VALUES = 3;

// cube
static constexpr GLsizei CUBE_NUM_FACES = 6;
static constexpr GLsizei CUBE_NUM_VERTICES = 8;
static constexpr GLsizei CUBE_NUM_VERTICES_PER_FACE = 4;
static constexpr GLfloat CUBE_CENTER[] = {0.5, 0.5, 0.5};;
static constexpr GLuint CUBE_DISPLAY_LIST = 1;
static constexpr GLfloat CUBE_VERTEX_COORDINATES[CUBE_NUM_VERTICES][NUM_COORDINATES] = {
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{1.0, 0.0, 1.0},
		{0.0, 0.0, 1.0},
		{1.0, 1.0, 0.0},
		{1.0, 1.0, 1.0},
		{0.0, 1.0, 1.0},
		{0.0, 1.0, 0.0}
};
static constexpr GLubyte CUBE_FACE_INDICES[CUBE_NUM_FACES][CUBE_NUM_VERTICES_PER_FACE] = {
		{0, 1, 2, 3},
		{1, 4, 5, 2},
		{4, 7, 6, 5},
		{7, 0, 3, 6},
		{3, 2, 5, 6},
		{7, 4, 1, 0}
};
static constexpr GLfloat CUBE_FACE_COLORS[CUBE_NUM_FACES][NUM_COLOR_VALUES] = {
		{0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0},
		{0.0, 1.0, 1.0},
		{1.0, 0.0, 0.0},
		{1.0, 0.0, 1.0},
		{1.0, 1.0, 0.0}
};

// window
static constexpr GLchar WIN_NAME[] = "Polyhedron Animation";
static constexpr GLint WIN_WIDTH = 800;
static constexpr GLint WIN_HEIGHT = 800;

// perspective
static constexpr GLdouble FOV = 40.0;
static constexpr GLdouble ASPECT = 1.0;
static constexpr GLdouble NEAR = 1.0;
static constexpr GLdouble FAR = 10.0;

// orthographic
static constexpr GLdouble ORTHO_COORDS[] = {-1, 1, -1, 1, 1, 10};

// eye
static constexpr GLdouble EYE_POS[NUM_COORDINATES] = {0.5, 0.5, 5.0};
static constexpr GLdouble EYE_CENTER[NUM_COORDINATES] = {0.5, 0.5, 0.5};
static constexpr GLdouble EYE_UP[NUM_COORDINATES] = {0.0, 1.0, 0.0};

// animation
static constexpr GLfloat ROTATION_RATE = 0.5f;
static constexpr GLfloat ROTATION_AXIS[NUM_COORDINATES] = {0.0, 1.0, 0.0};


void setPerspectiveProjection(GLfloat aspect) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, aspect, NEAR, FAR);
	glMatrixMode(GL_MODELVIEW);
}


void setOrthographicProjection(GLfloat aspect) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble a = 1.0, b = 1.0;
	aspect < 1.0 ? a=aspect : b = 1.0/aspect;
	glOrtho(a*ORTHO_COORDS[0], a*ORTHO_COORDS[1], b*ORTHO_COORDS[2], b*ORTHO_COORDS[3], ORTHO_COORDS[4], ORTHO_COORDS[5]);
	glMatrixMode(GL_MODELVIEW);
}

void setCameraView() {
	glLoadIdentity();
	gluLookAt(EYE_POS[0], EYE_POS[1], EYE_POS[2], EYE_CENTER[0], EYE_CENTER[1], EYE_CENTER[2], EYE_UP[0], EYE_UP[1], EYE_UP[2]);
	glutPostRedisplay();
}

void displayFun() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(CUBE_CENTER[0], CUBE_CENTER[1], CUBE_CENTER[2]);
	glRotatef(ROTATION_RATE, ROTATION_AXIS[0], ROTATION_AXIS[1], ROTATION_AXIS[2]);
	glTranslatef(-CUBE_CENTER[0], -CUBE_CENTER[1], -CUBE_CENTER[2]);
	glCallList(CUBE_DISPLAY_LIST);
	glutSwapBuffers();
}


void reshapeFun(int w, int h) {
	glViewport(0, 0, w, h);
	setPerspectiveProjection((GLfloat) w / (GLfloat) h);
	setCameraView();
}


void keyboardFun(unsigned char key, int x, int y) {
	switch (key) {
		case 'o':
			setOrthographicProjection((GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case 'p':
			setPerspectiveProjection((GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT));
			break;
		default:
			return;
	}
	setCameraView();
}


void idleFun() {
	glutPostRedisplay();
}


void initCubeDisplayList() {
	glEnable(GL_DEPTH_TEST);
	glNewList(CUBE_DISPLAY_LIST, GL_COMPILE);
	glBegin(GL_QUADS);
	for (int i = 0; i < CUBE_NUM_FACES; ++i) {
		glColor3fv(CUBE_FACE_COLORS[i]);
		for (int j = 0; j < CUBE_NUM_VERTICES_PER_FACE; ++j) {
			glVertex3fv(CUBE_VERTEX_COORDINATES[CUBE_FACE_INDICES[i][j]]);
		}
	}
	glEnd();
	glEndList();
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2 - WIN_WIDTH / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2 - WIN_HEIGHT / 2);
	glutCreateWindow(WIN_NAME);
	glutDisplayFunc(displayFun);
	glutReshapeFunc(reshapeFun);
	glutKeyboardFunc(keyboardFun);
	glutIdleFunc(idleFun);
	initCubeDisplayList();
	glutMainLoop();
}