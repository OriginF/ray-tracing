#ifndef __COSW_H__
#define __COSW_H__

#include "../../include/include.hpp"

double p_theta_cosw(Vec3D nl, Vec3D d){
    double cos_theta = abs(nl.dot(d)/(nl.mod()*d.mod()));
    double sin_theta = sqrt(1-pow2d(cos_theta));
    double ans = 2.0*sin_theta*cos_theta;
    return ans;
}

double get_theta_cosw(double r1){
    double temp_sqrt = sqrt(1-r1);
    double theta = acos(temp_sqrt);
    return theta;
}

double get_fai_cosw(double r2){
    return 2.0*M_PI*r2;
}

double get_cosw_value(Vec3D &d, double &cos_theta, Vec3D nl, const Vec3D rd_,const BaseGeo* bg, unsigned short *Xi){
    double theta = get_theta_cosw(erand48(Xi));
    double fai = get_theta_cosw(erand48(Xi));
    Vec3D w = nl, u = ((fabs(w.x)>.1 ? Vec3D(0, 1, 0) : Vec3D(1,0,0)).cross(w)).normalize(), v = w.cross(u);  //w，v，u为正交基
    d = (w*cos(theta)+u*sin(theta)*cos(fai)+v*sin(theta)*sin(fai)).normalize();
    Vec3D rd = rd_;
    Vec3D nl1 = (d-rd.normalize())*0.5;
    double cosw_value = p_theta_cosw(nl,d);
    cos_theta = d.dot(nl1)/(nl1.mod()*d.mod());
    return cos_theta/cosw_value;
}

#endif