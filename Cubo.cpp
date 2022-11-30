#include "Cubo.h"

const int NUMNODES = 35;


using namespace std;


Cubo::Cubo(int dim_x,int dim_z, float speed, vector<vector<float>> locNodos, int idNodo) {

    // Set the dimensions of the map
    DimBoard_X = dim_x;
    DimBoard_Z= dim_z;
    idn = idNodo;

    // Randomly generate the location of the car in a node
    int num_ran = rand() % 33;
    nnode = num_ran;
    
    //Inits the position of the car in a valid node
    Position[0] = locNodos[num_ran][0];
    Position[2] = locNodos[num_ran][1];
    
    //Inits the direction of the car in a valid direction
    Direction[0] = rand();
    Direction[1] = rand();
    Direction[2] = rand();

    //Normalize the direction vector
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;
    //Cross the speed with the direction vector
    Direction[0] *= speed;
    Direction[1] *= speed;
    Direction[2] *= speed;

    cout << Direction[0] << endl;
    cout << Direction[2] << endl;

    trafficLight1Crosses = 0;
    trafficLight2Crosses = 0;

    radio = sqrt(3 * (10 * 10));

}

Cubo::~Cubo() {
    //Destructor
}

void Cubo::draw() {

    glPushMatrix();
    glTranslatef(Position[0], Position[1], Position[2]);
    glScaled(7,7,7);
    //The cube is drawn
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


// Function to normalize a direction vector
void L2Norm(float Direction[3]){
    
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    m += 0.00001;
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;

}

// Function to set the direction of the car
void NodeDirection(int targetNode, vector<vector<float>> locNodos, float Direction[3], float Position[3]){
    Direction[0] = locNodos[targetNode][0]-Position[0];
    Direction[2] = locNodos[targetNode][1]-Position[2];
    L2Norm(Direction);
}

// Function to set the next node valid to go
int retrieveNextnode(int cNode, vector<vector<int>> transitionMatrix) {
    vector<int> aux(NUMNODES);
    int k = 0;

    // Generate a list of node options among the valid ones in the transition matrix 
    for (int i = 0; i < NUMNODES; i++) {
        if(transitionMatrix[cNode][i] == 1) {
            aux[k] = i;
            k++;
        }
    }

    int sel = rand() % k;
    return aux[sel];
    
}

// Function to update the position of the car
int Cubo::update(vector<vector<float>> locNodos, vector<vector<int>> trafficLight1, vector<vector<int>> trafficLight2, vector<vector<int>> transitionMatrix, int nextNode, float speed) {

    NodeDirection(nextNode, locNodos, Direction, Position);
    float dist = dist2node(Position, nextNode, locNodos);

    // Conditional to check if a car should low the speed
    if(dist < 100 && trafficLight1[1][0] == 1 || trafficLight2[1][0] == 1) {

        if((find(trafficLight1[0].begin(), trafficLight1[0].end(), nextNode) != trafficLight1[0].end()) || (find(trafficLight2[0].begin(), trafficLight2[0].end(), nextNode) != trafficLight2[0].end())){
            Position[0] -= Direction[0] * (speed / 2);
            Position[2] -= Direction[2] * (speed / 2);

        }

    }

    // Condition to check if the car is in the intersection
    if(dist < 5){

        if((find(trafficLight1[0].begin(), trafficLight1[0].end(), nextNode) != trafficLight1[0].end() && (trafficLight1[1][0] == 0 ||trafficLight1[1][0] == 2))){

            if((trafficLight1[1][0] == 0)){
                Position[0] += Direction[0] * 0;
                Position[2] += Direction[2] * 0;    
            }

            if((trafficLight1[1][0] == 2)) {
                nextNode = retrieveNextnode(nextNode, transitionMatrix);
                
                if(nextNode == 16 || nextNode == 17){
                    trafficLight1Crosses++;
                }
                if(nextNode == 18 || nextNode == 19){
                    trafficLight2Crosses++;
                }
            }

    } 
        
        else if(find(trafficLight2[0].begin(), trafficLight2[0].end(), nextNode) != trafficLight2[0].end() && (trafficLight2[1][0] == 0 ||trafficLight2[1][0] == 2)) {
           
           if(trafficLight2[1][0] == 0){
                Position[0] += Direction[0] * 0;
                Position[2] += Direction[2] * 0;    
            }

            if((trafficLight2[1][0] == 2)) {
                nextNode = retrieveNextnode(nextNode, transitionMatrix);
                
                if(nextNode == 16 || nextNode == 17){
                    trafficLight1Crosses++;
                }
                if(nextNode == 18 || nextNode == 19){
                    trafficLight2Crosses++;
                }
            }
        }
        
        else {
            nextNode = retrieveNextnode(nextNode, transitionMatrix);
            
            if(nextNode == 16 || nextNode == 17){
                trafficLight1Crosses++;
            }
            if(nextNode == 18 || nextNode == 19){
                trafficLight2Crosses++;
            }
        }

    }

    // Make the car move on the road
    Position[0] += Direction[0] * speed;
    Position[2] += Direction[2] * speed;
    
    nnode = nextNode;
    return nextNode;

}


// Function to return the position of the car
int Cubo::getininopde() { return (nnode); }

// Function to return the radio of the car
float Cubo::getRadio() { return (radio); }

// Function to return the X coordinate of the car
float Cubo::getX() { return (Position[0]); }

// Function to return the Y coordinate of the car
float Cubo::getZ() { return (Position[2]); }

// Function to return the id of the car
int Cubo::getidn() { return (idn); }