#ifndef __BDBOX_HELPER__
#define __BDBOX_HELPER__

#include "./helper_base.hpp"

void build_hash_grid(const int w, const int h) {  //hash 网格 
    // find the bounding box of all the measurement points
    hpbbox.reset();
    CrashList *lst = hitpoints;
    while (lst != NULL) {
        Crash *hp = lst->id; 
        lst = lst->next; 
        hpbbox.fit(hp->position); 
    }

    // heuristic for initial radius  启发式 
    Vec3D ssize = hpbbox.max - hpbbox.min;
    double irad = ((ssize.x + ssize.y + ssize.z) / 3.0) / ((w + h) / 2.0) * 2.0;

    // determine hash table size
    // we now find the bounding box of all the measurement points inflated by the initial radius
    hpbbox.reset();
    lst = hitpoints;
    int vphoton = 0;
    while (lst != NULL) {
        Crash *hp = lst->id;
        lst = lst->next;
        hp->r2 = irad * irad;
        hp->n = 0;
        hp->flux = Vec3D();
        vphoton++;
        hpbbox.fit(hp->position - irad);
        hpbbox.fit(hp->position + irad);
    }

    // make each grid cell two times larger than the initial radius
    hash_s = 1.0 / (irad * 2.0);
    printf("vphoton:%d\n",vphoton);
    num_hash = vphoton;

    // build the hash table
    hash_grid = new CrashList *[num_hash];
    for (unsigned int i = 0; i < num_hash; i++) hash_grid[i] = NULL;
    lst = hitpoints;
    while (lst != NULL) {
        Crash *hp = lst->id;
        lst = lst->next;
        Vec3D BMin = ((hp->position - irad) - hpbbox.min) * hash_s;
        Vec3D BMax = ((hp->position + irad) - hpbbox.min) * hash_s;
        for (int iz = abs(int(BMin.z)); iz <= abs(int(BMax.z)); iz++) {
            for (int iy = abs(int(BMin.y)); iy <= abs(int(BMax.y)); iy++) {
                for (int ix = abs(int(BMin.x)); ix <= abs(int(BMax.x)); ix++) {
                    int hv = BDBhash(ix, iy, iz);
                    // hash_grid[hv] = ListAdd(hp, hash_grid[hv]);
                    hash_grid[hv] = hash_grid[hv]->add(hp);
                }
            }
        }
    }
}

#endif