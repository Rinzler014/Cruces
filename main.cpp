
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>
#include <iostream>
#include <string>

#include <chrono>
#include <ctime>

#include "Cubo.h"

#include "RgbImage.h"

#define NUM_OBJ 20
#define NUM_NODES 35

#define NTextures 1
GLuint	texture[NTextures];

using namespace std;

int time_ = 0;

// Global variables
// Screen dimension constants
int WIDTH = 500;
int HEIGTH = 500;
// gluPerspective values
float FOVY = 65.0;
float ZNEAR = 0.01;
float ZFAR = 900.0;
// Eye position
float EYE_X = 0.0;
float EYE_Y = 810.0;
float EYE_Z = 100.0;
float CENTER_X = 0;
float CENTER_Y = 0;
float CENTER_Z = 0;
float UP_X = 0;
float UP_Y = 1;
float UP_Z = 0;
// Axis variables
float X_MIN = -500;
float X_MAX = 500;
float Y_MIN = -500;
float Y_MAX = 500;
float Z_MIN = -500;
float Z_MAX = 500;

// Board Size
int DimBoard_X = 450;
int DimBoard_Z = 300;

// Ligther control
vector<int> lightColors = {0, 0};
int LightCTRL = 0;

// Control variables of each agent
vector<int> AiNextNode(NUM_OBJ);
float speed = 10;

// Traffic light positions
vector<vector<float>> trafficLightsPos = {{-72, -68}, 
                                          {-199, 59}};

//Vector of booleans to store if there is a collision
vector<bool> collisions(NUM_OBJ, false);

// Nodes locations
vector<vector<float>> locNodos(NUM_NODES, vector<float>(2, 0));

// Transition matrix
vector<vector<int>> TransitionMatrix(NUM_NODES * 2, vector<int>(NUM_NODES * 2, 0));

// Nodes connections with traffic lights
vector<vector<int>> trafficLight1 = {{18, 19}, {0}};
vector<vector<int>> trafficLight2 = {{16, 17}, {0}};

int crosses1 = 0;
int crosses2 = 0;


// Cars vector
vector<void *> objects(NUM_OBJ);

// Car comparator array
int CarComp[NUM_OBJ][NUM_OBJ];

// Variable to check the collision between car i and j
int compCarICarK = 1;

// File name of the map texture
const char* filename0 = "map.bmp";

// Function to calculate the distance bewtween two points
float distance(float x1, float z1, float x2, float z2){
    return sqrt(pow(x2 - x1, 2) + pow(z2 - z1, 2));
}

// Function to calculate the sum of radio bethween two points
float sumRadio(float r1, float r2){
    return r1 + r2;
}

// Function to load the map texture
void loadTextureFromFile(const char *filename, int index) {

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

// Function to draw the traffic light
void TrafficLight(int lightcolor, vector<vector<float>> trafficLightsPos, int trafficLight){

  glPushMatrix();
  glTranslatef(trafficLightsPos[trafficLight][0],20,trafficLightsPos[trafficLight][1]);

  // Traffic light base
  /*
  
    0 = Rojo
    1 = Amarillo
    2 = Verde
  
  */

  if(lightcolor==0){
    glColor3f(1.0,0.0,0.0);
  }
  if(lightcolor==1){
    glColor3f(0.8,0.8,0.3);
  }
  if(lightcolor==2){
    glColor3f(0.0, 1.0, 0.0);
  }
  glutSolidSphere(10,50,50);
  glPopMatrix();
}

// Function to control the traffic light color
vector<int> LightControl(int &lightCTRL){
  
  if(lightCTRL==500){
    lightCTRL=0;
    lightColors[0]++;
    lightColors[1]++;

  }else{
    LightCTRL++;
  }

  if(lightColors[0] == 3){
    lightColors[0] = 0;
  }

  if(lightColors[1] == 3){
    lightColors[1] = 0;
  }
  
  return lightColors;

}

// Function to populate the locations of the nodes
void PopulateLocNodes(){
	
  locNodos[0][0] = -419;locNodos[0][1] = -266;
  locNodos[1][0] = -382;locNodos[1][1] = -234;
  locNodos[2][0] = -144;locNodos[2][1] = -267;
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

// Function to populate the available conections between nodes
void PopulateTMatrix(){

  TransitionMatrix[0][12] = 1;
  TransitionMatrix[1][3] = 1;
  TransitionMatrix[2][0] = 1;
  TransitionMatrix[3][5] = 1;
  TransitionMatrix[3][16] = 1;
  TransitionMatrix[4][3] = 1;
  TransitionMatrix[5][2] = 1;
  TransitionMatrix[5][7] = 1;
  TransitionMatrix[6][4] = 1;
  TransitionMatrix[7][9] = 1;
  TransitionMatrix[7][20] = 1;
  TransitionMatrix[8][6] = 1;
  TransitionMatrix[8][7] = 1;
  TransitionMatrix[9][6] = 1;
  TransitionMatrix[9][10] = 1;
  TransitionMatrix[10][22] = 1;
  TransitionMatrix[11][8] = 1;
  TransitionMatrix[12][14] = 1;
  TransitionMatrix[12][15] = 1;
  TransitionMatrix[13][1] = 1;
  TransitionMatrix[13][14] = 1;
  TransitionMatrix[14][26] = 1;
  TransitionMatrix[15][13] = 1;
  TransitionMatrix[15][17] = 1;
  TransitionMatrix[16][13] = 1;
  TransitionMatrix[16][17] = 1;
  TransitionMatrix[17][19] = 1;
  TransitionMatrix[17][28] = 1;
  TransitionMatrix[18][5] = 1;
  TransitionMatrix[18][16] = 1;
  TransitionMatrix[19][18] = 1;
  TransitionMatrix[19][24] = 1;
  TransitionMatrix[20][18] = 1;
  TransitionMatrix[21][9] = 1;
  TransitionMatrix[21][20] = 1;
  TransitionMatrix[22][21] = 1;
  TransitionMatrix[22][24] = 1;
  TransitionMatrix[23][11] = 1;
  TransitionMatrix[24][23] = 1;
  TransitionMatrix[24][32] = 1;
  TransitionMatrix[25][22] = 1;
  TransitionMatrix[25][23] = 1;
  TransitionMatrix[26][29] = 1;
  TransitionMatrix[27][15] = 1;
  TransitionMatrix[28][27] = 1;
  TransitionMatrix[28][31] = 1;
  TransitionMatrix[29][30] = 1;
  TransitionMatrix[29][31] = 1;
  TransitionMatrix[30][19] = 1;
  TransitionMatrix[30][28] = 1;
  TransitionMatrix[31][33] = 1;
  TransitionMatrix[32][30] = 1;
  TransitionMatrix[33][25] = 1; 
	
}

// Function to draw a character in the screen
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

// Function to initialize the OpenGL window
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

  for (int i = 0; i < NUM_OBJ; i++){
    objects[i] = new Cubo(DimBoard_X, DimBoard_Z, speed, locNodos, i);
    AiNextNode[i] = ((Cubo*)objects[i])->getininopde();
  }

}

// Function to cycle the control variables
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

  // Draw the traffic lights
  TrafficLight(lightColors[0], trafficLightsPos, 0);
  TrafficLight(lightColors[1], trafficLightsPos, 1);
  
  // Assign the traffic light color to the traffic light
  trafficLight1[1][0] = LightControl(LightCTRL)[0];
  trafficLight2[1][0] = LightControl(LightCTRL)[1];

  // Draw the nodes number in the screen
	for (int i = 0; i < NUM_NODES - 1; i++){
		drawString(locNodos[i][0], 10, locNodos[i][1], to_string(i).c_str());
	}

  // Draw and update the objects 
  for (int i = 0; i < NUM_OBJ; i++){

    for(int j = 0; j < NUM_OBJ; j++){//Actualiza si hay colision
      
      if(i != j){

          if ( ((Cubo*)objects[i])->getininopde() == ((Cubo*)objects[j])->getininopde() ) {

              float carDist = distance(((Cubo*)objects[i])->getX(), ((Cubo*)objects[i])->getZ(), ((Cubo*)objects[j])->getX(), ((Cubo*)objects[j])->getZ());
              float radios = sumRadio(((Cubo*)objects[i])->getRadio(), ((Cubo*)objects[j])->getRadio()) + 2;

              if (carDist < radios) {

                  float dist2nodeCar = distance(((Cubo*)objects[i])->getX(), ((Cubo*)objects[i])->getZ(), locNodos[AiNextNode[i]][0], locNodos[AiNextNode[i]][0]);
                  float dist2nodeCar2 = distance(((Cubo*)objects[j])->getX(), ((Cubo*)objects[j])->getZ(), locNodos[AiNextNode[j]][0], locNodos[AiNextNode[j]][0]);

                  if (dist2nodeCar <= dist2nodeCar2) {
                      CarComp[i][j] = 1;
                  }
                  else {
                      CarComp[i][j] = 0;
                  }

              }
              else {
                  CarComp[i][j] = 1;
              }
          }
          else
          {
              CarComp[i][j] = 1;
          }

        

      }
      else
      {
          CarComp[i][j] = 1;
      }

    }
    
  }

  for (int i = 0; i < NUM_OBJ; i++)
  {
      compCarICarK = 1;
      for (int k = 0; k < NUM_OBJ; k++)
      {
          if (i==k)
          {
              continue;
          }
          if (CarComp[i][k]==0)
          {
              compCarICarK = 0;
          }
      }

      ((Cubo*)objects[i])->draw();
      if (compCarICarK == 0)
      {
          //cout << "colision" << i << endl;
          AiNextNode[i] = ((Cubo*)objects[i])->update(locNodos, trafficLight1, trafficLight2, TransitionMatrix, AiNextNode[i], 0);
      }
      else
      {
          AiNextNode[i] = ((Cubo*)objects[i])->update(locNodos, trafficLight1, trafficLight2, TransitionMatrix, AiNextNode[i], speed);
      }

  }

  if(time_ == 10000) {
    
    for (int i = 0; i < NUM_OBJ; i++){
      crosses1 += ((Cubo *)objects[i])->trafficLight1Crosses;
      crosses2 += ((Cubo *)objects[i])->trafficLight2Crosses;
    }

    cout << "Traffic Light 1 crosses: " << crosses1 << endl;
    cout << "Traffic Light 2 crosses: " << crosses2 << endl;

    exit(0);

  }



  // Release the buffer to screen
  glutSwapBuffers();
  Sleep(5);

  time_++;

}

// Function to cycle the display when the keyboard is IDLE
void idle(){
     display();
}

// Main function
int main(int argc, char **argv) {

  PopulateLocNodes();
  PopulateTMatrix();

  // Assign Speed to the objects
  speed = 1;

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
