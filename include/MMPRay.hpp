#ifndef __MMPRAY_H__
#define __MMPRAY_H__

#include "./def.hpp"
#include "./Ray.hpp"
#include "./Vec.hpp"
#include "./init.hpp"

inline int rev(const int i, const int p) {
    if (i == 0) return i; 
	else return p - i;
}

double hal(const int b, int j) {
    const int p = primes[b];
    double h = 0.0, f = 1.0 / (double) p, fct = f;
    while (j > 0) {
        h += rev(j % p, p) * fct;
        j /= p;
        fct *= f;
    }
    return h;
}

void MMPRay(Ray *pr, Vec3D *f, int i) {
    *f = Vec3D(2500, 2500, 2500) * (M_PI * 4.0); // flux
    double p = 2. * M_PI * hal(0, i), t = 2. * acos(sqrt(1. - hal(1, i)));
    double st = sin(t);
    pr->d = Vec3D(cos(p) * st, cos(t), sin(p) * st);
    pr->o = Vec3D(50, 60, 85);
}

#endif