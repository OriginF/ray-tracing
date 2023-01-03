#ifndef __CIRC_H__
#define __CIRC_H__

#include "../def.hpp"
#include "../Vec.hpp"
#include "../Ray.hpp"
#include "./BaseGeo.hpp"

class Circ: public BaseGeo 
{
public:
    double radius;
    Vec3D cir_start;
    Vec3D cir_end;
    Circ():radius(0),cir_start(Vec3D()),cir_end(Vec3D()),BaseGeo(Vec3D(),Vec3D(),UNDEF,"Unknown",0){};
    Circ(double radius_,Vec3D cir_start_,Vec3D cir_end_,Vec3D emission_,Vec3D color_,REFLECT reflect_,double alpha_):
        radius(radius_),cir_start(cir_start_),cir_end(cir_end_),BaseGeo(emission_,color_,reflect_,"Sphere",alpha_){};
    
    bool in_line_test(Vec3D intersector,const Ray &r) const{
        Vec3D v1 = (intersector - r.o);
        double ans = v1.cross(r.d).mod();
        if(abs(ans)<epsilon)return true;
        else return false;
    }

    bool in_face_test(Vec3D norm, Vec3D point, const Vec3D intersector) const{
        if(abs(norm.dot(point-intersector))<=epsilon){
            return true;
        }
        else return false;
    }

    bool in_test(Vec3D v1, Vec3D center) const {
        if((v1-center).mod()<=radius)return true;
        else return false;
    }
    
    double intersect_circle(Vec3D vp, Vec3D m0, const Ray &r) const{
        //和圆圈的交点
        double vpt = r.d.dot(vp);
        if(vpt==0)return 0;
        else{
            double t = ((m0.x-r.o.x)*vp.x+(m0.y-r.o.y)*vp.y+(m0.z-r.o.z)*vp.z)/vpt;
            Vec3D intersector = r.o+r.d*t;
            if(t<epsilon)t=0;
            if(!in_test(intersector,m0)){
                t=0;
            }
            return t;
        }
    }

    double intersect_column(const Ray &r) const{

        //异面直线距离
        Vec3D vp = (cir_end-cir_start).normalize();
        Vec3D start = cir_start;
        Vec3D rd = r.d;
        Vec3D cross_e1e2 = vp.cross(rd.normalize());
        if(cross_e1e2.mod()<epsilon)return 0; //两直线平行
        Vec3D M1M2 = start-r.o;
        double d0 = abs(cross_e1e2.dot(M1M2)/(cross_e1e2.mod()));
        if(d0>=radius)return 0;

        //计算垂足
        double a = vp.dot(rd);
        double b = vp.dot(vp);
        double c = rd.dot(rd);
        double d = (r.o-start).dot(vp);
        double e = (r.o-start).dot(rd);
        double t1=0,t2=0;//分别表示两条直线的垂足
        if(a==0){
            t1 = d/b;
            t2 = -e/c;
        }
        else{
            t1 = (a*e-c*d)/(a*a-b*c);
            t2 = b/a*t1-d/a;
        }

        double wide_bias = sqrt(pow2d(radius)-pow2d(d0));
        double cos_theta = rd.dot(vp)/(rd.mod()*vp.mod());
        double sin_theta = sqrt(1-pow2d(cos_theta));
        double length = wide_bias/sin_theta;
        double deltat = abs(length/(rd.mod()));
        Vec3D temprd = r.d;
        double t = (t2 - deltat)/temprd.mod();
        Vec3D intersecor = r.o+r.d*t;
        Vec3D tempinter1 = intersecor-cir_start;
        Vec3D tempinter2 = intersecor-cir_end;
        if(tempinter1.dot(vp)<=0||(tempinter2.dot(vp)>=0))return 0;
        if(t<epsilon)return 0;
        // Vec3D pre_norm = intersecor-cir_start;
        // printf("thedot:%f\n",norm0.dot(pre_norm));
        // Vec3D projector = vp*(vp.dot(pre_norm));
        // if(abs((pre_norm-projector).mod()-radius)>=epsilon)printf("the value:%f\n",(pre_norm-projector).mod());
        return t;
    }


    double intersect(const Ray &r) const{
        Vec3D vp = (cir_end-cir_start).normalize();
        double intersect_circle_st = intersect_circle(vp,cir_start,r);
        double intersect_circle_ed = intersect_circle(vp,cir_end,r);
        double intersect_column_s = intersect_column(r);
        double the_min_t = 1e20;
        if(intersect_circle_st!=0&&intersect_circle_st<the_min_t){
            the_min_t = intersect_circle_st;
        }
        if(intersect_circle_ed!=0&&intersect_circle_ed<the_min_t){
            the_min_t = intersect_circle_ed;
        }
        if(intersect_column_s!=0&&intersect_column_s<the_min_t){
            the_min_t = intersect_column_s;
        }
        if(the_min_t == 1e20){
            the_min_t = 0;
        }
        return the_min_t;
    }

    Vec3D get_nnorm(const Vec3D intersector, const Ray &r) const{
        if(!in_line_test(intersector,r))printf("haha");//在对应的直线上
        Vec3D norm0 = (cir_end-cir_start).normalize();
        // printf("1\n");
        if(in_face_test(norm0,cir_start,intersector)){return -norm0;}
        // printf("2\n");
        if(in_face_test(norm0,cir_end,intersector)){return norm0;}
        Vec3D pre_norm = intersector-cir_start;
        // printf("thedot:%f\n",norm0.dot(pre_norm));
        Vec3D projector = norm0*(norm0.dot(pre_norm));
        // if(abs((pre_norm-projector).mod()-radius)>=epsilon)printf("the value:%f\n",(pre_norm-projector).mod());
        Vec3D norm = (pre_norm-projector).normalize();

        // printf("the dot:%f",norm.dot(norm0));
        return norm;
    }

    void print(string name){
        printf("Circ-");
        printf(name.append(":\n").c_str());
        printf("radius:%f\n",radius);
        cir_start.print("center_start");
        cir_end.print("center_end");
        emission.print("emission");
        color.print("color");
    }
};

#endif