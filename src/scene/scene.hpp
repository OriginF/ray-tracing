#ifndef __SCENE_H__
#define __SCENE_H__

#include "../../include/include.hpp"
#include "./all_objs.hpp"

// int geosize = 9;

vector<BaseGeo*> geos;
// {
//     &ball1,
//     &ball2,
//     &sphere_back,
//     &sphere_bottom,
//     &sphere_front,
//     &sphere_left,
//     &sphere_right,
//     &sphere_up,
//     &light
// };

inline int sizeofGeos(){
    return geos.size();
}

#endif