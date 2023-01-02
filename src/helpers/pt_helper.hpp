#ifndef __PT_H__
#define __PT_H__

#include "./helper_base.hpp"
#include "./brdf_helper.hpp"
#include "./cosw_helper.hpp"

inline Vec3D Render(const Ray &r, int depth){
    double t;
    int id=0;
    if(!intersect(r,t,id))return Vec3D();
    const BaseGeo* obj = geos[id];
    Vec3D x = r.o + r.d*t;
    Vec3D n = obj->get_nnorm(x, r.o);
    Vec3D nl = n.dot(r.d) < 0 ? n : n*-1;
    Vec3D f = obj->color;
    double p = f.x>f.y && f.x>f.z ? f.x : f.y>f.z ? f.y : f.z; // max refl 
    if (++depth>5||!p) //if greater than 5 depth or p = 0,return 
        if (erand48(xseed)<p) //get darker or get emission 
            f = f*(1 / p); 
        else  
            return obj->emission; 
    if (obj->reflect == DIFF) {// Ideal DIFFUSE reflection 
        if(mode == SIMPLE || mode == FISH){
            double r1 = 2 * M_PI*erand48(xseed), r2 = erand48(xseed), r2s = sqrt(r2);
            Vec3D w = nl, u = ((fabs(w.x)>.1 ? Vec3D(0, 1, 0) : Vec3D(1,0,0)).cross(w)).normalize(), v = w.cross(u);  //w，v，u为正交基
            Vec3D d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1 - r2)).normalize();//随机产生一个半球的方向，r2表示的是theta，r1表示的是fai。
            return obj->emission + f*Render(Ray(x,d),depth);
        }
        else if(mode == BRDF){
            double theta = get_theta_Beck(erand48(xseed),obj);
            double fai = get_fai_Beck(erand48(xseed));
            Vec3D w = nl, u = ((fabs(w.x)>.1 ? Vec3D(0, 1, 0) : Vec3D(1,0,0)).cross(w)).normalize(), v = w.cross(u);  //w，v，u为正交基
            Vec3D nl1 = (w*cos(theta)+u*sin(theta)*cos(fai)+v*sin(theta)*sin(fai)).normalize();
            //d 是目前新产生的微观法向量的方向
            Vec3D d = r.d - nl1*2.0*nl1.dot(r.d);
            double brdf_value = P_theta_Beck(nl1,nl,r.d, obj);
            double cos_theta = d.dot(nl1)/(d.mod()*nl1.mod());
            if(d.dot(nl)<0){
                return obj->emission;
            }
            double argx = cos_theta/brdf_value;
            if(argx>10)argx = 10.0;
            return obj->emission + f*Render(Ray(x, d), depth)*cos_theta*(1.0/brdf_value);
        }
        else if(mode == MIS){
            Vec3D d_brdf,d_cosw;
            double cos_brdf,cos_cosw;
            double brdf_value = get_brdf_value(d_brdf,cos_brdf,nl,r.d,obj,xseed);//获得brdf cos*p
            double cosw_value = get_cosw_value(d_cosw,cos_cosw,nl,r.d,obj,xseed);//获得cosw cos*p
            double weight = (cos_brdf/brdf_value)/(cos_brdf/brdf_value+cos_cosw/cosw_value);
            if(brdf_value==0||erand48(xseed)<=weight){ //俄罗斯轮盘赌，原始公式weight为比重，现在改为发生该事件的概率。
                return obj->emission+f*Render(Ray(x,d_brdf),depth)*brdf_value;
            }
            else{
                return obj->emission+f*Render(Ray(x,d_cosw),depth)*cosw_value;
            }
        }
    }
    else if (obj->reflect == SPEC)            // Ideal SPECULAR reflection 
        return obj->emission + f*(Render(Ray(x, r.d - n * 2 * n.dot(r.d)), depth)); //直接找到反射光线
    Ray reflRay(x, r.d - n * 2 * n.dot(r.d));     // Ideal dielectric REFRACTION 由平行四边形的方法求得反射光的direction
    bool into = n.dot(nl)>0;                // 光线从外面射入
    double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = (r.d).dot(nl), cos2t;
    if ((cos2t = 1 - nnt*nnt*(1 - ddn*ddn))<0)    // Total internal reflection 完全是内部的反射
        return obj->emission + f*(Render(reflRay, depth));
    Vec3D tdir = (r.d*nnt - n*((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))).normalize(); //新的反射方向。
    double a = nt - nc, b = nt + nc, R0 = a*a / (b*b), c = 1 - (into ? -ddn : tdir.dot(n));
    double Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re, P = .25 + .5*Re, RP = Re / P, TP = Tr / (1 - P);
    return obj->emission + f*(depth>2 ? (erand48(xseed)<P ?   // Russian roulette 俄罗斯轮盘产生外部反射光线的值。
        Render(reflRay, depth)*RP : Render(Ray(x, tdir), depth)*TP) :
        Render(reflRay, depth)*Re + Render(Ray(x, tdir), depth)*Tr);
}


#endif