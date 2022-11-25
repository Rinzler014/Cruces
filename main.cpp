
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>
#include <string>

#include "Cubo.h"

#include "RgbImage.h"

#define NUM_OBJ 2
#define NUM_NODES 51

#define NTextures 1
GLuint	texture[NTextures];

//Variables dimensiones de la pantalla
int WIDTH = 500;
int HEIGTH = 500;
//Variables para establecer los valores de gluPerspective
float FOVY = 65.0;
float ZNEAR = 0.01;
float ZFAR = 900.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X = 0.0;
float EYE_Y = 810.0;
float EYE_Z = 100.0;
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
int DimBoard_X = 450;
int DimBoard_Z = 300;

//Ligther control
int Lightcolor = 0;
int LightCTRL = 0;

// Control variables of each agent
vector<int> AiNextNode(NUM_OBJ);
float speed = 0.5;

// Localizacion de los nodos
vector<vector<float>> locNodos(NUM_NODES, vector<float>(2, 0));

//vector<vector<int>> TransitionMatrix(NUM_NODES * 2, vector<int>(NUM_NODES * 2, 0));

vector<void *> objects(NUM_OBJ);

char* filename0 = "map.bmp";

void loadTextureFromFile(char *filename, int index) {

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT); 
	//glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap(filename);

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texture[index]);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texture[index]);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
    theTexMap.Reset();
}

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
	
locNodos[0][0] = -419;locNodos[0][1] = -266;
locNodos[1][0] = -382;locNodos[1][1] = -234;
locNodos[2][0] = -142;locNodos[2][1] = -267;
locNodos[3][0] = -143;locNodos[3][1] = -236;
locNodos[4][0] = -92;locNodos[4][1] = -266;
locNodos[5][0] = -92;locNodos[5][1] = -234;
locNodos[6][0] = 98;locNodos[6][1] = -267;
locNodos[7][0] = 98;locNodos[7][1] = -234;
locNodos[8][0] = 148;locNodos[8][1] = -266;
locNodos[9][0] = 151;locNodos[9][1] = -234;
locNodos[10][0] = 374;locNodos[10][1] = -234;
locNodos[11][0] = 418;locNodos[11][1] = -267;
locNodos[12][0] = -419;locNodos[12][1] = -19;
locNodos[13][0] = -383;locNodos[13][1] = -19;
locNodos[14][0] = -419;locNodos[14][1] = 27;
locNodos[15][0] = -383;locNodos[15][1] = 27;
locNodos[16][0] = -142;locNodos[16][1] = -19;
locNodos[17][0] = -143;locNodos[17][1] = 27;
locNodos[18][0] = -92;locNodos[18][1] = -19;
locNodos[19][0] = -94;locNodos[19][1] = 25;
locNodos[20][0] = 98;locNodos[20][1] = -19;
locNodos[21][0] = 140;locNodos[21][1] = -18;
locNodos[22][0] = 374;locNodos[22][1] = -21;
locNodos[23][0] = 422;locNodos[23][1] = -21;
locNodos[24][0] = 372;locNodos[24][1] = 27;
locNodos[25][0] = 418;locNodos[25][1] = 27;
locNodos[26][0] = -418;locNodos[26][1] = 271;
locNodos[27][0] = -383;locNodos[27][1] = 244;
locNodos[28][0] = -140;locNodos[28][1] = 245;
locNodos[29][0] = -142;locNodos[29][1] = 272;
locNodos[30][0] = -91;locNodos[30][1] = 244;
locNodos[31][0] = -94;locNodos[31][1] = 272;
locNodos[32][0] = 372;locNodos[32][1] = 244;
locNodos[33][0] = 418;locNodos[33][1] = 272;

}



/*
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

*/

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

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  loadTextureFromFile(filename0, 0);

  /*

  for (int i = 0; i < NUM_OBJ; i++){
    objects[i] = new Cubo(DimBoard_X, DimBoard_Z, speed);
  }

  */

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
        glVertex3d(-DimBoard_X, 0.0, -DimBoard_Z);
		glTexCoord2f(0.0, 0.0);
        glVertex3d(-DimBoard_X, 0.0, DimBoard_Z);
		glTexCoord2f(1.0, 0.0);
        glVertex3d(DimBoard_X, 0.0, DimBoard_Z);
		glTexCoord2f(1.0, 1.0);
        glVertex3d(DimBoard_X, 0.0, -DimBoard_Z);
    glEnd();
	glDisable(GL_TEXTURE_2D);

    TrafficLight(Lightcolor);
    Lightcolor = LightControl(LightCTRL);

	
	for (int i = 0; i < NUM_NODES - 1; i++){

		drawString(locNodos[i][0], 10, locNodos[i][1], to_string(i).c_str());

	}


  	
   // corregir funcion populatelocnodes
   // a�adir funcion de ir a nodos
   // current node -> choose a random node from the ones it can go
   
        
    glutSwapBuffers();
    Sleep(5);

}

void idle(){
     display();
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
  glutMainLoop();

}
