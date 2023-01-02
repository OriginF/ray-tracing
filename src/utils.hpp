#ifndef __UTILS_H__
#define __UTILS_H__

#include "../include/include.hpp"
#include "./scene/scene.hpp"
#include "./def.hpp"
#include "./helpers/helpers_include.hpp"

inline void writeBMP(int w, int h, Vec3D* img, const char* filename){
    char bmp[w*h*3];
    for(int i=0;i<w*h;i++){
        bmp[i*3] = img[i].x;
        bmp[i*3+1] = img[i].y;
        bmp[i*3+2] = img[i].z;
    }
    int l = (w*3+3)/4*4;
    int bmi[] = {l*h+54,0,54,40,w,h,1|3*8<<16,0,l*h,0,0,100,0};
    FILE *fp=fopen(filename,"w");
    fprintf(fp,"BM");
    fwrite(&bmi,52,1,fp);
    fwrite(img,1,l*h,fp);
    fclose(fp);
}

inline void sample_Render(int x, int y){
    for (int sy = 0, i = (h - y - 1)*w + x; sy<2; sy++){
        for (int sx = 0; sx<2; sx++) {
            Vec3D r;
            for (int s = 0; s<samples; s++) {
                //get dx dy randomly -1 ~ 0
                double r1 = 2 * erand48(xseed);
                double r2 = 2 * erand48(xseed);
                double dx = r1<1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                double dy = r2<1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
                // subpixels sx
                Vec3D d;
                if(mode == FISH){
                    double dx = (((sx + .5 + dx) / 2 + x)/w - .5)*(fish_f);
                    double dy = (((sy + .5 + dy) / 2 + y)/h - .5)*(fish_f);
                    d = get_fish_ray(Vec2D(dx,dy));
                    double nu = abs(fish_f/d.z);
                    Vec3D into = campos.o+d*nu;
                    r = r + Render(Ray(campos.o + d * nu, d.normalize()), 0)*(1. / samples);
                }
                else{
                    d = cx*(((sx + .5 + dx) / 2 + x) / w - .5) +
                        cy*(((sy + .5 + dy) / 2 + y) / h - .5) + campos.d; //焦距140
                    r = r + Render(Ray(campos.o + d * 140, d.normalize()), 0)*(1. / samples);
                }
            }
            c[i] = c[i] + Vec3D(clamp(r.x), clamp(r.y), clamp(r.z))*.25;
        }
    }
}

inline void sample_Tracer(){
    for (int y = 0; y < h; y++) {
        fprintf(stderr, "\rHitPointPass %5.2f%%", 100.0 * y / (h - 1));
        for (int x = 0; x < w; x++) {
            pixel_index = x + y * w;
            Vec3D d = cx * ((x + 0.5) / w - 0.5) + cy * (-(y + 0.5) / h + 0.5) + campos.d;
            Trace(Ray(campos.o + d * 140, d.normalize()), 0, true, Vec3D(), Vec3D(1, 1, 1), 0);
        }
    }
    build_hash_grid(w,h);
    num_photon = samples;
    vw = Vec3D(1,1,1);
    #pragma omp parallel for schedule(dynamic, 1) private(r)
    for(int i=0;i<num_photon;i++){
        double p = 100. * (i+1)/num_photon;
        fprintf(stderr, "\rPhotonPass %5.2f%%", p);
        int m = 1000*i;
        Ray r;
        Vec3D f;
        for(int j=0;j<1000;j++){
            MMPRay(&r,&f,m+j);
            Trace(r, 0 , false, f, vw, m + j);
        }
    }
    CrashList *lst = hitpoints;
    while(lst!=NULL){
        Crash *hp = lst->id;
        lst = lst->next;
        int i = hp->pixel_index;
        Vec3D ans = hp->flux * (1.0 / (M_PI * hp->r2 * num_photon * 1000.0));
        c[i] = c[i] + ans;
    }
}

#endif