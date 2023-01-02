#ifndef __RAY_H__
#define __RAY_H__

#include "./def.hpp"
#include "./Vec.hpp"

class Ray{
public:
    Vec3D o,d;
    Ray():o(),d(){};
    Ray(Vec3D o_,Vec3D d_):o(o_),d(d_){};
    void print(string name){
        printf(name.append(" origin:(%f,%f,%f);direction:(%f,%f,%f)\n").c_str(),
            o.x,o.y,o.z,d.x,d.y,d.z);
    }
};

#endif