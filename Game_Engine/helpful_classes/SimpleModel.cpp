#include "SimpleModel.h"

SimpleModel::SimpleModel(){

}

SimpleModel::SimpleModel(GLuint id, int numVert){
    vaoID = id;
    numVertices = numVert;
}

int SimpleModel::GetvaoID(){
    return vaoID;
}

void SimpleModel::SetvaoID(int val){
    vaoID = val;
}

int SimpleModel::GetnumVertices(){
    return numVertices;
}

void SimpleModel::SetnumVertices(int val){
     numVertices = val;
 }

SimpleModel::~SimpleModel(){

}
