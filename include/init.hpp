#ifndef __INIT_H__
#define __INIT_H__

#include "./def.hpp"
#include "./Vec.hpp"
#include "./Ray.hpp"

extern int w, h ,samples;
//camera EXtrinsics
extern Campos campos;
//camera Coordinate
extern Vec3D cx;
extern Vec3D r;//color of samples
extern Vec3D *c;//Image
extern MODE mode;

#endif