#ifndef __PPM_H__
#define __PPM_H__

#include "./helper_base.hpp"

void Trace(const Ray &r, int dpt, bool m, const Vec3D &fl, const Vec3D &adj, int i) {
    double t;
    int id = 0;

    dpt++;
    if (!intersect(r, t, id) || (dpt >= 20))return;

    int d3 = dpt * 3;

    const BaseGeo* obj = geos[id];
    Vec3D x = r.o + r.d * t;
    Vec3D n = obj->get_nnorm(x,r);
    Vec3D f = obj->color;
    Vec3D nl = n.dot(r.d) < 0 ? n : n * -1;
    double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

    if (obj->reflect == DIFF) {
        // Lambertian
        // use QMC to sample the next direction
        double r1 = 2. * M_PI * hal(d3 - 1, i), r2 = hal(d3 + 0, i);
        double r2s = sqrt(r2);
        Vec3D w = nl, u = ((fabs(w.x) > .1 ? Vec3D(0, 1, 0) : Vec3D(1, 0, 0)).cross(w)).normalize();
        Vec3D v = w.cross(u), d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
        if(m){
            Crash *hp = new Crash;
            hp->color = f*adj;
            hp->position = x;
            hp->normal = n;
            hp->pixel_index = pixel_index;
            hitpoints = hitpoints->add(hp);
        }
        else{
            // photon ray
            // find neighboring measurement points and accumulate flux via progressive density estimation
            Vec3D hh = (x - hpbbox.min) * hash_s;
            int ix = abs(int(hh.x)), iy = abs(int(hh.y)), iz = abs(int(hh.z));
            // strictly speaking, we should use #pragma omp critical here.
            // it usually works without an artifact due to the fact that photons are
            // rarely accumulated to the same measurement points at the same time (especially with QMC).
            // it is also significantly faster.
            {
                CrashList *hp = hash_grid[BDBhash(ix, iy, iz)];
                while (hp != NULL) {
                    Crash *hitpoint = hp->id;
                    hp = hp->next;
                    Vec3D v = hitpoint->position - x;
                    // check normllizeals to be closer than 90 degree (avoids some edge brightning)
                    if ((hitpoint->normal.dot(n) > 1e-3) && (v.dot(v) <= hitpoint->r2)) {
                        // unlike N in the paper, hitpoint->n stores "N / ALPHA" to make it an integer value
                        double g = (hitpoint->n * alpha_ppm + alpha_ppm) / (hitpoint->n * alpha_ppm + 1.0);
                        hitpoint->r2 = hitpoint->r2 * g;
                        hitpoint->n++;
                        hitpoint->flux = (hitpoint->flux + (hitpoint->color*fl) * (1. / M_PI)) * g;
                    }
                }
            }
            if (hal(d3 + 1, i) < p) Trace(Ray(x, d), dpt, m, (f*fl) * (1. / p), adj, i);
        }  
    }
     else if (obj->reflect == SPEC) {
        // mirror
        Trace(Ray(x, r.d - n * 2.0 * n.dot(r.d)), dpt, m, f*fl, f*adj, i);

    } else {
        // glass
        Ray lr(x, r.d - n * 2.0 * n.dot(r.d));
        bool into = (n.dot(nl) > 0.0);
        double nc = 1.0, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;

        // total internal reflection
        if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) return Trace(lr, dpt, m, fl, adj, i);

        Vec3D td = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).normalize();
        double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : td.dot(n));
        double Re = R0 + (1 - R0) * c * c * c * c * c, P = Re;
        Ray rr(x, td);
        Vec3D fa = f*adj;
        if (m) {
            // eye ray (trace both rays)
            Trace(lr, dpt, m, fl, fa * Re, i);
            Trace(rr, dpt, m, fl, fa * (1.0 - Re), i);
        } else {
            // photon ray (pick one via Russian roulette)
            (hal(d3 - 1, i) < P) ? Trace(lr, dpt, m, fl, fa, i) : Trace(rr, dpt, m, fl, fa, i);
        }
    }
    return;
}



#endif