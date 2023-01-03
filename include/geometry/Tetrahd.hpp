#ifndef __TETRAHD_H__
#define __TETRAHD_H__

#include "../def.hpp"
#include "../Vec.hpp"
#include "../Ray.hpp"
#include "./Mesh.hpp"
#include "./BaseGeo.hpp"

class Tetrahd: public BaseGeo 
{
public:
    Vec3D vertices[4];
    Mesh meshes[4];
    Tetrahd():BaseGeo(Vec3D(),Vec3D(),UNDEF,"Unknown",0){};
    Tetrahd(Vec3D v1,Vec3D v2,Vec3D v3,Vec3D v4,Vec3D emission_,Vec3D color_,REFLECT reflect_,double alpha_):
        BaseGeo(emission_,color_,reflect_,"Sphere",alpha_){
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
        vertices[3] = v4;
        // meshes[0].set(v2,v4,v3);
        // meshes[1].set(v2,v4,v3);
        // meshes[2].set(v2,v4,v3);
        // meshes[3].set(v2,v4,v3);
        meshes[0].set(v1,v2,v3);
        meshes[1].set(v1,v2,v4);
        meshes[2].set(v1,v3,v4);
        meshes[3].set(v2,v3,v4);
    };


    bool in_test(Mesh mesh, Vec3D point) const {
        Vec3D v1 = mesh.vertices[1]-mesh.vertices[0];
        Vec3D v2 = mesh.vertices[2]-mesh.vertices[0];
        Vec3D norm = v1.cross(v2);
        Vec3D x = point-mesh.vertices[0];
        if(abs(norm.dot(x))<epsilon){
            return true;
        }
        else{
            return false;
        }
    }

    
    double intersect(const Ray &r) const{
        double min_t = 1e20;
        for(int i=0;i<4;i++){
            double ti = meshes[i].intersect(r);
            if(ti!=0&&ti<min_t){
                min_t = ti;
            }
        }
        if(min_t==1e20){
            return 0;
        }
        return min_t;
    }

    Vec3D get_nnorm(const Vec3D intersector,const Ray &r) const{
        for(int i=0;i<4;i++){
            bool injg = in_test(meshes[i],intersector);
            if(injg){
                return meshes[i].get_nnorm(intersector,r);
            }
        }
        printf("error no intersector");
    }

    void print(string name){
        printf("Tetrahd-");
        printf(name.append(":\n").c_str());
        vertices[0].print("vertices0");
        vertices[1].print("vertices1");
        vertices[2].print("vertices2");
        vertices[3].print("vertices3");
        emission.print("emission");
        color.print("color");
    }
};

#endif