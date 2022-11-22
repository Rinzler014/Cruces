
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>

#include "Cubo.h"

#define NUM_OBJ 1
#define NUM_NODES 24

//Variables dimensiones de la pantalla
int WIDTH = 500;
int HEIGTH = 500;
//Variables para establecer los valores de gluPerspective
float FOVY = 60.0;
float ZNEAR = 0.01;
float ZFAR = 900.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X = 100.0;
float EYE_Y = 850.0;
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

//Ligther control
int Lightcolor = 0;
int LightCTRL = 0;

// Control variables of each agent
int A1nextNode = 0;
int A2nextNode = 2;
float speed = 1;

// Localizacion de los nodos
vector<vector<float>> locNodos(NUM_NODES, vector<float>(2, 0));

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

/*

void PopulateLocNodes(){

  float cx = -DimBoard_X + 200;
  float cy = -DimBoard_Z;

  for (int i = 0; i < NUM_NODES; i++){
  	locNodos[i][0] = cx;
  	locNodos[i][1] = cy;
  	cx += 250;
  	if(cx > DimBoard_X){
  		cx = -DimBoard_X + 200;
  		cy += 180;
  	}
  }
}

*/

void PopulateLocNodes(){
	
	int x = 0, z = 1;
	
	// avenida principal
	
	locNodos[11][x] = -40;
	locNodos[11][z] = 420;

	locNodos[0][x] = 40;
	locNodos[0][z] = 420;
	
	locNodos[10][x] = -40;
	locNodos[10][z] = 220;
	
	locNodos[1][x] = 40;
	locNodos[1][z] = 220;
  	
	locNodos[9][x] = -40;
	locNodos[9][z] = 90;
	
	locNodos[2][x] = 40;
	locNodos[2][z] = 90;
  	
  	locNodos[8][x] = -40;
	locNodos[8][z] = -160;
	
	locNodos[3][x] = 40;
	locNodos[3][z] = -160;
  	
  	locNodos[7][x] = -40;
	locNodos[7][z] = -280;
	
	locNodos[4][x] = 40;
	locNodos[4][z] = -280;
  	
  	locNodos[6][x] = -40;
	locNodos[6][z] = -390;
	
	locNodos[5][x] = 40;
	locNodos[5][z] = -390;
  	
  	// calle 1
  	
  	locNodos[18][x] = -250;
	locNodos[18][z] = 190;
	
	locNodos[19][x] = -250;
	locNodos[19][z] = 135;
  	
  	locNodos[14][x] = -80;
	locNodos[14][z] = 190;
	
	locNodos[15][x] = -80;
	locNodos[15][z] = 135;
  	
  	locNodos[12][x] = 80;
	locNodos[12][z] = 190;
	
	locNodos[13][x] = 80;
	locNodos[13][z] = 135;
  	
  	locNodos[22][x] = 250;
	locNodos[22][z] = 190;
	
	locNodos[23][x] = 250;
	locNodos[23][z] = 135;
  	
  	// calle 2
  	
  	locNodos[16][x] = -80;
	locNodos[16][z] = -185;
	
	locNodos[17][x] = -80;
	locNodos[17][z] = -240;
  	
  	locNodos[20][x] = -250;
	locNodos[20][z] = -185;
	
	locNodos[21][x] = -250;
	locNodos[21][z] = -240;

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

 void init() {
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
  glClearColor(0,0,0,0);
  glEnable(GL_DEPTH_TEST);
  srand(time(nullptr));

  for (int i = 0; i < NUM_OBJ; i++){
    objects[i] = new Cubo(DimBoard_X, DimBoard_Z, speed);
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

    

    TrafficLight(Lightcolor);
    Lightcolor= LightControl(LightCTRL);

    ((Cubo *)objects[0])->draw();
    A1nextNode = ((Cubo *)objects[0])->update(locNodos, TransitionMatrix, A1nextNode, A2nextNode, speed);


    // avenida principal
  
  	drawString(-40,10,420, "11");
  	drawString(40,10,420, "0");
  	
  	drawString(-40,10,220, "10");
  	drawString(40,10,220, "1");
  	
  	drawString(-40,10,90, "9");
  	drawString(40,10,90, "2");
  	
  	drawString(-40,10,-160, "8");
  	drawString(40,10,-160, "3");
  	
  	drawString(-40,10,-280, "7");
  	drawString(40,10,-280, "4");
  	
  	drawString(-40,10,-390, "6");
  	drawString(40,10,-390, "5");
  	
  	// calle 1
  	
  	drawString(-250,10,190, "18");
  	drawString(-250,10,135, "19");
  	
  	drawString(-80,10,190, "14");
  	drawString(-80,10,135, "15");
  	
  	drawString(80,10,190, "12");
  	drawString(80,10,135, "13");
  	
  	drawString(250,10,190, "22");
  	drawString(250,10,135, "23");
  	
  	// calle 2
  	
  	drawString(-80,10,-185, "16");
  	drawString(-80,10,-240, "17");
  	
  	drawString(-250,10,-185, "20");
  	drawString(-250,10,-240, "21");
  	
   // corregir funcion populatelocnodes
   // a�adir funcion de ir a nodos
   // current node -> choose a random node from the ones it can go
   
        
    glutSwapBuffers();
    Sleep(5);

}

void idle(){
     display();
}

void keyboard(unsigned char key, int x, int y) {
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
  PopulateTMatrix();

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
