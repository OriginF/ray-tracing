#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../../include/include.hpp"
#include "../def.hpp"

Vec3D get_fish_ray(Vec2D point2d){
    double x = point2d.x;
    double y = point2d.y;
    double rd = sqrt(pow2d(x)+pow2d(y));
    double fai = 0;
    if(rd==0){
        fai = 0;
    }
    else{
        if(x<0){
            fai = M_PI - asin(y/rd);
        }
        else{
            fai = asin(y/rd);
        }
    }

    double theta = 2.0*asin(rd/(2.0*fish_f));
    // double theta = 2.0*atan(rd/(2.0*fish_f));

    Vec3D ans_d;

    ans_d.x = (double)(sin(theta)*cos(fai));
    ans_d.y = (double)(sin(theta)*sin(fai));
    ans_d.z = (double)(cos(theta));

    Vec3D ans = cx*ans_d.x+cy*ans_d.y+campos.d.normalize()*ans_d.z;

    return ans;
}

#endif