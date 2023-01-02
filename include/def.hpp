
#ifndef __DEF_H__
#define __DEF_H__

#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#define pow2d(x) (x)*(x)

#define MAX(x,y) (((x)>(y))?(x):(y))

#define Campos Ray

unsigned short xseed[3] = {1,2,3}; //erand48 seed
enum REFLECT{DIFF,SPEC,REFR,UNDEF};
enum MODE{SIMPLE,BRDF,MIS,PPM,FISH};
double epsilon = 1e-4;

inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); } 

#endif