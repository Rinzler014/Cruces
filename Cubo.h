#ifndef CUBO_H
#define CUBO_H

#include <math.h>
#include <random>
#include <iomanip>
#include <GL/glut.h>
#include <iostream>


using namespace std;

class Cubo {
    public:
        Cubo(int,int, float);
        ~Cubo();
        void draw();
        void update();

        void getX();
        void getY();
        void getZ();

        void getRadio();

    protected:

    private:
        float vertexCoords[24] = {  // Coordinates for the vertices of a cube.
                   1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
                  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1  };

        float vertexColors[24] = {  // An RGB color value for each vertex
                   1,1,1,   1,0,0,   1,1,0,   0,1,0,
                   0,0,1,   1,0,1,   0,0,0,   0,1,1  };

        int elementArray[24] = {  // Vertex number for the six faces.
                  0,1,2,3, 0,3,7,4, 0,4,5,1,
                  6,2,1,5, 6,5,4,7, 6,7,3,2  };

        //Vector de direccion
        float Position[3] = {0.0, 5.0, 0.0};
        float Direction[3] = {0.0, 5.0, 0.0};
        int DimBoard_X;
        int DimBoard_Z;
        int radio;

};

#endif // CUBO_H
