///**************************************************************************
/// HEADER: Contains All Structures
///**************************************************************************

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "Main.h"


// Kernel Types
enum Kernel {Gaussian, Bell_shaped, Cubic_spline, Quadratic, Quintic, Quintic_spline};

// model of viscosity formulation
enum ViscosityModel {violeauArtificial};

// integrationMethod = euler ou RK2
enum IntegrationMethod {euler, RK2};

// integrationMethod = euler ou RK2
enum AdaptativeTimeStep {no, yes};

// densityInitMethod = hydrosatic, etc.
enum DensityInitMethod {hydrostatic, homogeneous};

// stateEquationMethod = quasiIncompressible, perfectGas, etc.
enum StateEquationMethod {quasiIncompressible, perfectGas};

// massInitMethod = violeau2012 (all particles have same volumes), etc.
enum MassInitMethod {violeau2012};

// speedLaw = Will dictate the behaviour of moving boundaries: constant, sine, exponential
enum SpeedLaw {constant, sine, exponential};

// Write Format output
enum Format {ParaView, Matlab, Both};

// charactTime = characteristic time of movement or period of oscillations

// movingDirection = direction de la paroi mouvante

/* Parameter Structure
 * kh = smothing length
 * k = time step
 * T = simulation time
 * densityRef = density of the fluid at atmospheric pressure
 * l & u = lower and upper limit of the domain
 * B & gamma = fluid constants
 * g = gravity
 * writeInteval = time interval between two outputs file are generated
 * c, alpha, beta, epsilon = parameter relativ to artificial viscosity
*/


struct Parameter {
    double kh, k, T, densityRef, B, gamma, g, writeInterval, charactTime, c, alpha, beta, epsilon;
    double movingDirection[3];
    Kernel kernel;
    ViscosityModel viscosityModel;
    IntegrationMethod integrationMethod;
    AdaptativeTimeStep adaptativeTimeStep;
    DensityInitMethod densityInitMethod;
    StateEquationMethod stateEquationMethod;
    MassInitMethod massInitMethod;
    SpeedLaw speedLaw;
    Format format;
};


struct Field {
    int nFree, nFixed, nMoving, nTotal;

    double l[3];
    double u[3];

    double nextK=0.0;

    std::vector<double> pos;

    std::vector<double> speed;

    std::vector<double> density;

    std::vector<double> pressure;

    std::vector<double> mass;

};



#endif
