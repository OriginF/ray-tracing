#ifndef __MAIN_DEF_H__
#define __MAIN_DEF_H__

#include "../include/include.hpp"

int w=1024, h=768, samples = 11;
//camera EXtrinsics
Campos campos(Vec3D(50,52,295.6), Vec3D(0,-0.042612,-1).normalize());
//camera Coordinate
Vec3D cx = Vec3D(w*.5135 / h,0,0), cy = (cx.cross(campos.d)).normalize()*.5135;
Vec3D r;//color of samples
Vec3D *c = new Vec3D[w*h];//Image
MODE mode;

//ppm var
unsigned int pixel_index = 0;
unsigned int num_hash = 0;
unsigned int num_photon = 0;
double hash_s;
CrashList **hash_grid=NULL;
CrashList *hitpoints = NULL;
BDBox hpbbox;
double alpha_ppm = 0.7;
Vec3D vw;
int primes[61] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
        83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
        191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283
};

//fisheye camera var
double fish_f = 92;

//bdpt var
vector<BaseGeo*> lights;

#endif