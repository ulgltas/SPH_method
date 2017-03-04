#ifndef SPH_H
#define SPH_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <map>
#include <cmath>
#include <cassert>
#include <ctime>
#include <fstream>
#include <random>
#include <sstream>
#include <float.h>
#include <iomanip>


// Memory and CPU consumption
size_t GetMemory(bool screen, bool print);
size_t GetMemoryProcess(bool screen, bool print);
size_t GetMemoryProcessPeak(bool screen, bool print);

// Geometry.cpp
void meshcube(double o[3], double L[3], double s, std::vector<double> &pos, double perturbation = 0.0, bool stack = false);
void meshcylinder(double o[3], double L[3], double s, std::vector<double> &pos, double perturbation = 0.0, bool stack = false);
void meshsphere(double o[3], double L[3], double s, std::vector<double> &pos, double perturbation = 0.0, bool stack = false);

// Playground.cpp

//********* void readParameter(char* parameterFilename, Parameter* parameter);
//********* void readGeometry(char* geometryFilename, Field* field);


// Neighborhood.cpp
void neighborAllPair (std::vector<double> &pos,
                         double kh,
                         std::vector<double> &values,
                         std::vector<int> &row,
                         std::vector<int> &column);
void neighborLinkedList (std::vector<double> &pos,
                         double l[3],
                         double u[3],
                         double kh,
                         std::vector<double> &values,
                         std::vector<int> &row,
                         std::vector<int> &column);
void surroundingBoxes(int box, int nBoxesX, int nBoxesY, int nBoxesZ, std::vector<int> &surrBoxes);
double distance(std::vector<double> pos, int partA, int partB);
void findNeighbors(int particleID, std::vector<double> &pos, double kh2,
                   std::vector<std::vector<int> > &boxes,
                   std::vector<int> &surrBoxes,
                   std::vector<int> &neighbors,
                   std::vector<double> &kernelGradients);
void sortParticles(std::vector<double> &pos, double l[3], double u[3], double kh,
                   std::vector<std::vector<int> > &boxes);
void boxMesh(double l[3], double u[3], double kh,
             std::vector<std::vector<int> > &boxes,
             std::vector<std::vector<int> > &surrBoxesAll);
void boxClear(std::vector<std::vector<int> > &boxes);

// TimeIntegration.cpp
int timeIntegration(std::vector<double> &pos, double l[3], double u[3], double kh);
//********** void updateMovingSpeed(Field* field, Parameter* parameter, double t);


// Kernel.cpp
double Wab(std::vector<double> pos, int partA, int partB, double h, size_t choice);
double grad_Wab(std::vector<double> pos, int partA, int partB, double h, size_t choice);

// ParaView.cpp
void paraview(std::string const &filename,
              int step,
              std::vector<double> const &pos,
              std::map<std::string, std::vector<double> *> const &scalars,
              std::map<std::string, std::vector<double> *> const &vectors);

// writeField.cpp
//********** void writeField(Field* field,double t);


// Init.cpp
//********** void densityInit(Field* field,Parameter* parameter);
//********** void pressureComputation(Field* field,Parameter* parameter);
//********** void massInit(Field* field,Parameter* parameter);
#endif
