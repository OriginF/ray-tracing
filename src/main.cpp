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
    #pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int y = 0; y<h; y++) {                       // Loop over image rows
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples * 4, 100.*y / (h - 1));
        for (unsigned short x = 0; x<w; x++){ 
            for (int sy = 0, i = (h - y - 1)*w + x; sy<2; sy++){
                for (int sx = 0; sx<2; sx++) {
                    Vec3D r = sample_Render(x, y, sx, sy);
                    c[i] = c[i] + Vec3D(clamp(r.x), clamp(r.y), clamp(r.z))*.25;
                }
            }
        }
    }
    // string base_str = "image";
    // base_str.append(to_string(samples)).append(".ppm");
    int i = rand()%100;
    string filepath = to_string(i).append(file2).append(to_string(samples)).append(".ppm");
    FILE *f = fopen(filepath.c_str(), "w");         // Write image to PPM file.
    // if(!f){printf("wrong\n");};
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i = 0; i<w*h; i++)
        fprintf(f, "%d %d %d ", toInt(c[i].x)>255?255:toInt(c[i].x), toInt(c[i].y)>255?255:toInt(c[i].y), toInt(c[i].z)>255?255:toInt(c[i].z));
}