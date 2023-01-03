#ifndef __BDPT_HELPER__
#define __BDPT_HELPER__

#include "../../include/include.hpp"
#include "./helper_base.hpp"

inline void init_all_light(){
    int sizeG = sizeofGeos();
    for(int i=0;i<sizeG;i++){
        if(geos[i]->emission.mod()!=0){
            lights.push_back(geos[i]);
        }
    }
    for(int i=0;i<lights.size();i++){
        
    }
}

inline Vec3D Render_bdpt(const Ray &r, int depth){

}

#endif