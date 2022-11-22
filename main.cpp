
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>

#include "Cubo.h"

#define NUM_OBJ 5
#define NUM_NODES 12

//Variables dimensiones de la pantalla
int WIDTH = 500;
int HEIGTH = 500;
//Variables para establecer los valores de gluPerspective
float FOVY = 65.0;
float ZNEAR = 0.01;
float ZFAR = 900.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X = 100.0;
float EYE_Y = 860.0;
float EYE_Z = 0.0;
float CENTER_X = 0;
float CENTER_Y = 0;
float CENTER_Z = 0;
float UP_X = 0;
float UP_Y = 1;
float UP_Z = 0;
//Variables para dibujar los ejes del sistema
float X_MIN = -500;
float X_MAX = 500;
float Y_MIN = -500;
float Y_MAX = 500;
float Z_MIN = -500;
float Z_MAX = 500;

//Size del tablero
int DimBoard_X = 300;
int DimBoard_Z = 450;
int Lightcolor = 0;
int LightCTRL = 0;

// Localizacion de los nodos
float LocNodos[NUM_NODES][2];

vector<vector<int>> TransitionMatrix(NUM_NODES * 2, vector<int>(NUM_NODES * 2, 0));

vector<void *> objects(NUM_OBJ);

void TrafficLight(int lightcolor){

  glPushMatrix();
  glTranslatef(0,20,0);
  if(lightcolor==0){
    glColor3f(1.0,0.0,0.0);
  }
  if(lightcolor==1){
    glColor3f(0.0,1.0,0.0);
  }
  if(lightcolor==2){
    glColor3f(0.0,0.0,1.0);
  }
  glutSolidSphere(20,50,50);
  glPopMatrix();
}

int LightControl(int &lightCTRL){
  if(lightCTRL==20){
    lightCTRL=0;
    Lightcolor ++;

  }else{
    LightCTRL++;
  }
  if(Lightcolor==3){
    Lightcolor=0;
  }
  return Lightcolor;
}

void PopulateLocNodes(){

  float cx = -DimBoard_X + 200;
  float cy = -DimBoard_Z;

  for (int i = 0; i < NUM_NODES; i++){
  	LocNodos[i][0] = cx;
  	LocNodos[i][1] = cy;
  	cx += 250;
  	if(cx > DimBoard_X){
  		cx = -DimBoard_X + 200;
  		cy += 180;
  	}
  }
}



void PopulateTMatrix(){
	TransitionMatrix[0][1] = 1;
	TransitionMatrix[1][2] = 1;
	TransitionMatrix[1][12] = 1;
	TransitionMatrix[1][15] = 1;
	TransitionMatrix[2][3] = 1;
	TransitionMatrix[3][4] = 1;
	TransitionMatrix[3][17] = 1;
	TransitionMatrix[4][5] = 1;
	// 5 -> final
	TransitionMatrix[6][7] = 1;
	TransitionMatrix[7][8] = 1;
	TransitionMatrix[7][17] = 1;
	TransitionMatrix[8][9] = 1;
	TransitionMatrix[9][10] = 1;
	TransitionMatrix[9][12] = 1;
	TransitionMatrix[9][15] = 1;
	TransitionMatrix[10][11] = 1;
	// 11 - > final
	TransitionMatrix[12][22] = 1;
	TransitionMatrix[13][2] = 1;
	TransitionMatrix[13][10] = 1;
	TransitionMatrix[13][15] = 1;
	TransitionMatrix[14][2] = 1;
	TransitionMatrix[14][10] = 1;
	TransitionMatrix[14][12] = 1;
	TransitionMatrix[15][19] = 1;
	TransitionMatrix[16][4] = 1;
	TransitionMatrix[16][8] = 1;
	TransitionMatrix[17][21] = 1;
	TransitionMatrix[18][14] = 1;
	// 19 -> final
	TransitionMatrix[20][16] = 1;
	// 21 -> final
	// 22 -> final
	TransitionMatrix[23][13] = 1;
	
	cout << "Traffic Transition Matrix with 24 nodes" << endl;
	  for (int i = 0; i < 24; i++){
	    for (int j = 0; j < 24; j++){
	      cout << TransitionMatrix[i][j] << " ";
	    }
	    cout << "\n" << endl;
	  }
}

void drawString(int x, int y, int z, const char* text) {
  glColor3f(1.0f, 1.0f, 1.0f);
 
  glRasterPos3i(x, y, z);
  string s = text;
  void * font = GLUT_BITMAP_9_BY_15;
  for (string::iterator i = s.begin(); i != s.end(); ++i){
    char c = *i;
    glutBitmapCharacter(font, c);
  }
}

 void init()
{
	
	PopulateTMatrix();
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    srand(time(nullptr));

    for (int i = 0; i < 5; i++){
    	objects[i] = new Cubo(DimBoard_X, DimBoard_Z, 0.5);
    }

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.3, 0.3, 0.3);
    //El piso es dibujado
    glBegin(GL_QUADS);
        glVertex3d(-DimBoard_X - 50, 0.0, -DimBoard_Z - 50);
        glVertex3d(-DimBoard_X - 50, 0.0, DimBoard_Z + 50);
        glVertex3d(DimBoard_X + 50, 0.0, DimBoard_Z + 50);
        glVertex3d(DimBoard_X + 50, 0.0, -DimBoard_Z - 50);
    glEnd();

    for (int i = 0; i < NUM_NODES; i++){
    std::string s = std::to_string(i);
    char const *pchar = s.c_str();
    drawString(LocNodos[i][0],10,LocNodos[i][1], pchar);
    }

    TrafficLight(Lightcolor);
    Lightcolor= LightControl(LightCTRL);

    for (int i = 0; i < int(objects.size()); i++){
    	((Cubo *)objects[i])->draw();
      ((Cubo *)objects[i])->update();
    }
        
    glutSwapBuffers();
    Sleep(5);

}

void idle(){
     display();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {//SOLID
    case 's':
    case 'S':
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glShadeModel(GL_FLAT);
      break;
    //INTERPOL
    case 'i':
    case 'I':
            glShadeModel(GL_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;

    case 'w':
    case 'W':
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      break;

    case 27:   // escape
      exit(0);
      break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {

  PopulateLocNodes();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WIDTH, HEIGTH);
  glutCreateWindow("Traffic Simulation");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

}
