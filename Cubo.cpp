#include "Cubo.h"
#include "bits/stdc++.h"

const int NUMNODES = 35;

using namespace std;


Cubo::Cubo(int dim_x,int dim_z, float vel, vector<vector<float>> locNodos)
{
    DimBoard_X = dim_x;
    DimBoard_Z= dim_z;
    //srand(time(nullptr));
    //int c;

    int num_ran = rand() % 33;
    nnode = num_ran;
    //Se inicializa una posicion aleatoria dentro del tablero
    Position[0] = locNodos[num_ran][0];
    Position[2] = locNodos[num_ran][1];
    //Se inicializa el vector de direccion con un valor aleatorio
    Direction[0] = rand();
    Direction[1] = rand();
    Direction[2] = rand();
    //Se normaliza el vector de direccion
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;
    //se multiplica el vector de direccion por la magnitud de la velocidad
    Direction[0] *= vel;
    Direction[1] *= vel;
    Direction[2] *= vel;

    cout << Direction[0] << endl;
    cout << Direction[2] << endl;
}

Cubo::~Cubo()
{
    //dtor
}

void Cubo::draw()
{
    glPushMatrix();
    glTranslatef(Position[0], Position[1], Position[2]);
    glScaled(10,10,10);
    //Se dibuja el cubo
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexCoords);
    glColorPointer(3, GL_FLOAT, 0, vertexColors);
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,elementArray);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

//Calculate the distance to a node
float dist2node(float Position[3], int targetNode, vector<vector<float>> locNodos){

    float dx = locNodos[targetNode][0] - Position[0];
    float dy = locNodos[targetNode][1] - Position[2];

    return sqrt(dx * dx + dy * dy);

}

//Function to find the next node
int findIdxNode(int targetNode, vector<int> nodeSeq) {

    for (int i = 0; i < 100; i++){
        if(targetNode == nodeSeq[i]){ 
            return i;
        }
    }
    return 0;
}

void L2Norm(float Direction[3]){
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    m += 0.00001;
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;
}

void NodeDirection(int targetNode, vector<vector<float>> locNodos, float Direction[3], float Position[3]){
    Direction[0] = locNodos[targetNode][0]-Position[0];
    Direction[2] = locNodos[targetNode][1]-Position[2];
    L2Norm(Direction);
}

int retrieveNextnode(int cNode, vector<vector<int>> transitionMatrix) {
    vector<int> aux(NUMNODES);
    int k = 0;

    for (int i = 0; i < NUMNODES; i++) {
        if(transitionMatrix[cNode][i] == 1) {
            aux[k] = i;
            k++;
        }
    }

    int sel = rand() % k;
    return aux[sel];
    
}

int Cubo::update(vector<vector<float>> locNodos, vector<vector<int>> trafficLight1, vector<vector<int>> trafficLight2, vector<vector<int>> transitionMatrix, int nextNode, float speed) {


    NodeDirection(nextNode, locNodos, Direction, Position);
    float dist = dist2node(Position, nextNode, locNodos);

    cout << "Semaforo 1: " << trafficLight1[1][0] << endl;

    if(dist < 5){

        if(find(trafficLight1[0].begin(), trafficLight1[0].end(), nextNode) != trafficLight1[0].end()){

            if(trafficLight1[1][0] == 0){

                Position[0] += Direction[0] * 0;
                Position[2] += Direction[2] * 0;
            
            }

            else if (trafficLight1[1][0] == 1){
                nextNode = retrieveNextnode(nextNode, transitionMatrix);
            }

            else if(trafficLight1[1][0] == 2) {
                nextNode = retrieveNextnode(nextNode, transitionMatrix);
            }

        } else {
            nextNode = retrieveNextnode(nextNode, transitionMatrix);
        }

    }

    if(dist < 100 && trafficLight1[1][0] == 2) {

        if(find(trafficLight1[0].begin(), trafficLight1[0].end(), nextNode) != trafficLight1[0].end()){
            Position[0] += Direction[0] * (speed / 2);
            Position[2] += Direction[2] * (speed / 2);
        }

    }

    Position[0] += Direction[0] * speed;
    Position[2] += Direction[2] * speed;

    return nextNode;

}


int Cubo::getininopde() { return (nnode); }

float Cubo::getRadio() { return (radio); }
float Cubo::getX() { return (Position[0]); }
float Cubo::getZ() { return (Position[2]); }

int Cubo::getidn() { return (idn); }