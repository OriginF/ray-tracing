#ifndef __HELPER_BASE__
#define __HELPER_BASE__

#include "../../include/include.hpp"
#include ".././scene/scene.hpp"
#include ".././def.hpp"

inline bool intersect(const Ray &r, double &t, int &id) {
    int size = sizeofGeos();
    double distance;
    double inf = 1e20;
    t = 1e20;
    for (int i = size; i--;) {
        Vec3D a = geos[i]->color;
        if ((distance=geos[i]->intersect(r)) && distance < t) 
        {
            t = distance;
            id = i;
        }
    }
    return t<inf;
}


#endif