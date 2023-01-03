#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../def.hpp"
#include "../Vec.hpp"
#include "../Ray.hpp"
#include "./BaseGeo.hpp"

class Sphere: public BaseGeo 
{
public:
    double radius;
    Vec3D center;
    Sphere():radius(0),center(Vec3D()),BaseGeo(Vec3D(),Vec3D(),UNDEF,"Unknown",0){};
    Sphere(double radius_,Vec3D center_,Vec3D emission_,Vec3D color_,REFLECT reflect_,double alpha_):
        radius(radius_),center(center_),BaseGeo(emission_,color_,reflect_,"Sphere",alpha_){};
    
    
    double intersect(const Ray &r) const{
        // printf("heihei sphere!");
        double t;
        Vec3D ids = center - r.o;
        double b = ids.dot(r.d);
        double det = pow2d(radius)+ pow2d(b) - ids.pow2();
        if (det >= 0)det = sqrt(det);
        else return 0;
        return (t = b - det) > epsilon ? t : ((t = b + det) > epsilon ? t : 0);
    }

    Vec3D get_nnorm(const Vec3D intersector, const Ray &r) const{
        return (intersector-center).normalize();
    }

    void print(string name){
        printf("Sphere-");
        printf(name.append(":\n").c_str());
        printf("radius:%f\n",radius);
        center.print("center");
        emission.print("emission");
        color.print("color");
    }
};

#endif