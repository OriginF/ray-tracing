#ifndef __BRDF_H__
#define __BRDF_H__

#include "../../include/include.hpp"

double P_h_theta_ggx(Vec3D sampler_dir, Vec3D nl, const BaseGeo* bg){
    double alpha = bg->alpha;
    double pow2alpha = pow2d(alpha);
    double cos_theta = sampler_dir.dot(nl)/(sampler_dir.mod()*nl.mod());
    double sin_theta = sqrt(1.0-cos_theta*cos_theta);
    double pow2cos = pow2d(cos_theta);
    double son = 2.0*pow2alpha*cos_theta*sin_theta;
    double mother = pow2d((pow2alpha-1)*pow2cos+1);
    double ans = son/mother;
    return ans;
}

double p_theta_ggx(Vec3D sampler_dir, Vec3D nl, Vec3D rd, const BaseGeo* bg){
    double p_h_theta = P_h_theta_ggx(sampler_dir,nl,bg);
    double cos_theta = sampler_dir.dot(rd)/(sampler_dir.mod()*rd.mod());
    return p_h_theta/(4.0*cos_theta);
}

double get_theta_ggx(double r1, const BaseGeo* bg){
    double pow2alpha = pow2d(bg->alpha);
    double temp_sqrt = sqrt((1.0-r1)/(r1*(pow2alpha-1)+1));
    double theta = acos(temp_sqrt);
    return theta;
}

double get_fai_ggx(double r2){
    return 2.0*M_PI*r2;
}

double P_h_theta_Beck(Vec3D sampler_dir, Vec3D nl, const BaseGeo* bg){
    double pow2alpha = pow2d(bg->alpha);
    double cos_theta = abs(sampler_dir.dot(nl)/(sampler_dir.mod()*nl.mod()));
    double sin_theta = sqrt(1-pow2d(cos_theta));
    double tan_theta = sin_theta/cos_theta;
    double part1 = 2.0*sin_theta/(pow2alpha*cos_theta*cos_theta*cos_theta);
    double part2 = exp(-pow2d(tan_theta)/pow2alpha);
    double ans = part1 * part2;
    return ans;
}

double P_theta_Beck(Vec3D sampler_dir, Vec3D nl, Vec3D rd, const BaseGeo* bg){
    double p_h_theta = P_h_theta_Beck(sampler_dir,nl,bg);
    double cos_theta = abs(sampler_dir.dot(rd)/(sampler_dir.mod()*rd.mod()));
    return p_h_theta/(4.0*cos_theta);
}

double get_theta_Beck(double r1, const BaseGeo* bg){
    double pow2alpha = pow2d(bg->alpha);
    double temp_sqrt = sqrt(1.0/(1.0-pow2alpha*log(1.0-r1)));
    double theta = acos(temp_sqrt);
    return theta;
}

double get_fai_Beck(double r2){
    return 2*M_PI*r2;
}

double get_arg_Beck(const BaseGeo* bg){
    double alpha = bg->alpha;
    double pow2alpha = pow2d(alpha);
    double max = 1-exp(-(1.0/pow2alpha));
    return 1.0/max;
}

double get_brdf_value(Vec3D &d, double &cos_theta, Vec3D nl, const Vec3D rd_,const BaseGeo* bg, unsigned short *Xi){
    double theta = get_theta_Beck(erand48(Xi),bg);
    double fai = get_fai_Beck(erand48(Xi));
    Vec3D w = nl, u = ((fabs(w.x)>.1 ? Vec3D(0, 1, 0) : Vec3D(1,0,0)).cross(w)).normalize(), v = w.cross(u);  //w，v，u为正交基
    Vec3D nl1 = (w*cos(theta)+u*sin(theta)*cos(fai)+v*sin(theta)*sin(fai)).normalize();
    //d 是目前新产生的微观法向量的方向
    d = rd_ - nl1*2.0*nl1.dot(rd_);
    double brdf_value = P_theta_Beck(nl1,nl,rd_,bg);
    cos_theta = d.dot(nl1)/(d.mod()*nl1.mod());
    if(d.dot(nl)<0){
        return 0;
    }
    double argx = cos_theta/brdf_value;
    if(argx > 10)argx = 10;
    return argx;
}

#endif