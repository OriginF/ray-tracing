#ifndef __MESH_H__
#define __MESH_H__

#include "./BaseGeo.hpp"
#include "../Vec.hpp"

class Mesh: BaseGeo
{
public:
    Vec3D vertices[3];
    Mesh():BaseGeo(Vec3D(),Vec3D(),UNDEF,"Unknown",0){
        for(int i=0;i<3;i++){
            vertices[i] = Vec3D();
        }
    }

    //if the radius hit this sphere, return the hit distance,
    //else return 0
    Mesh(Vec3D v1, Vec3D v2, Vec3D v3, Vec3D emission_, Vec3D color_, REFLECT reflect_, double alpha_):BaseGeo(emission,color_,reflect_,"Mesh",alpha_){
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
    }

    Vec3D toleft_test(Vec3D v1, Vec3D v2, Vec3D point) const {
        return (v1-v2).cross(v1-point);
    }

    bool in_test(Vec3D v1, Vec3D v2, Vec3D v3, Vec3D point) const {
        Vec3D norm1 = toleft_test(v1,v2,point);
        Vec3D norm2 = toleft_test(v2,v3,point);
        Vec3D norm3 = toleft_test(v3,v1,point);
        if(norm1.dot(norm2)<0||norm2.dot(norm3)<0||norm3.dot(norm1)<0){
            return false;
        }
        else return true;
    }

    double intersect(const Ray &r) const {
        Vec3D x1 = vertices[1]-vertices[0];
        Vec3D x2 = vertices[2]-vertices[0];
        Vec3D vp = x1.cross(x2).normalize();
        double vpt = r.d.dot(vp);
        if(vpt==0)return 0;
        else{
            double t = ((vertices[0].x-r.o.x)*vp.x+(vertices[0].y-r.o.y)*vp.y+(vertices[0].z-r.o.z)*vp.z)/vpt;
            Vec3D intersector = r.o+r.d*t;
            if(t<epsilon)t=0;
            if(!in_test(vertices[0],vertices[1],vertices[2],intersector)){
                t=0;
            }
            return t;
        }
    }

    Vec3D get_nnorm(const Vec3D intersector, const Vec3D ro) const{
        Vec3D norm = (vertices[1]-vertices[0]).cross(vertices[2]-vertices[0]);
        return norm.normalize();
    }

    void print(string name){
        printf("Mesh-");
        printf(name.append(":\n").c_str());
        vertices[0].print("v1");
        vertices[1].print("v2");
        vertices[2].print("v3");
        emission.print("emission");
        color.print("color");
    }
};

#endif