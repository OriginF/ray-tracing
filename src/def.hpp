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

#endif