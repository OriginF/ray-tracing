#include "./include.hpp"
#include <time.h>

int main(int argc, char* argv[]){
    srand((int(time(0))));
    printf("w:%d,h:%d\n",w,h);
    string file1 = argv[1];
    string file2 = argv[2];
    printf(file2.c_str());
    string base = "../scene/";
    scene_reader(base.append(file1));
    if(mode==PPM){
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
        printf("in 1");
        while(lst!=NULL){
            Crash *hp = lst->id;
            lst = lst->next;
            int i = hp->pixel_index;
            Vec3D ans = hp->flux * (1.0 / (M_PI * hp->r2 * num_photon * 1000.0));
            c[i] = c[i] + ans;
        }
    }
    else{
        #pragma omp parallel for schedule(dynamic, 1) private(r)
        for (int y = 0; y<h; y++) {                       // Loop over image rows
            fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples * 4, 100.*y / (h - 1));
            for (unsigned short x = 0; x<w; x++){ 
                sample_Render(x,y);
            }
        }
    }

    int i = rand()%100;
    string filepath = to_string(i).append(file2).append(to_string(samples)).append(".ppm");
    FILE *f = fopen(filepath.c_str(), "w");         // Write image to PPM file.
    
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i = 0; i<w*h; i++)
        fprintf(f, "%d %d %d ", toInt(c[i].x)>255?255:toInt(c[i].x), toInt(c[i].y)>255?255:toInt(c[i].y), toInt(c[i].z)>255?255:toInt(c[i].z));

    return 0;
}