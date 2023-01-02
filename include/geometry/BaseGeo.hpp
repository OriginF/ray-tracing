#ifndef __BASEGEO_H__
#define __BASEGEO_H__

#include "../Ray.hpp"

class BaseGeo{
public:
    string TYPE;
    Vec3D emission,color;
    REFLECT reflect;
    double alpha;
    BaseGeo(Vec3D emission_,Vec3D color_,REFLECT reflect_, string TYPE_,double alpha_):emission(emission_),color(color_),reflect(reflect_),TYPE(TYPE_),alpha(alpha_){};
    virtual double intersect(const Ray &r) const{printf("i'm out");return -1;};
    virtual Vec3D get_nnorm(const Vec3D intersector, const Vec3D ro) const{printf("no! out!");return Vec3D();};
    virtual void print(string name){printf("this is base geo");};
};

#endif