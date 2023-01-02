#ifndef __VEC_H__
#define __VEC_H__

#include "./def.hpp"

class Vec3D
{
public:
    double x,y,z;

    Vec3D():x(0),y(0),z(0){};
    Vec3D(double x_, double y_, double z_):x(x_),y(y_),z(z_){};

    double pow2(){
        return x*x+y*y+z*z;
    }

    double mod(){
        return sqrt(x*x+y*y+z*z);
    }

    double dot(const Vec3D &b) const{
        return x*b.x+y*b.y+z*b.z; 
    }

    Vec3D cross(const Vec3D &b){
        return Vec3D(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
    }

    Vec3D operator*(double b) const{
        return Vec3D(x*b,y*b,z*b);
    }

    Vec3D operator*(const Vec3D &b) const{
        return Vec3D(x*b.x,y*b.y,z*b.z);
    }

    Vec3D operator+(const Vec3D &b) const{
        return Vec3D(x+b.x,y+b.y,z+b.z);
    }

    Vec3D operator+(double b) const{
        return Vec3D(x+b,y+b,z+b);
    }

    Vec3D operator-(const Vec3D &b) const{
        return Vec3D(x-b.x,y-b.y,z-b.z);
    }

    Vec3D operator-(double b) const{
        return Vec3D(x-b,y-b,z-b);
    }

    Vec3D operator-() const{
        return Vec3D(-x,-y,-z);
    }

    void print(string name) const{
        printf(name.append("(%f,%f,%f)\n").c_str(),x,y,z);
    }

    Vec3D& normalize(){
        return *this = (*this)*(1.0/(this->mod()));
    }

};

#endif