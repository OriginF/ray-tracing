#ifndef __INIT_H__
#define __INIT_H__

#include "./def.hpp"
#include "./Vec.hpp"
#include "./Ray.hpp"
#include "./BDBox.hpp"

extern int w, h ,samples;
//camera EXtrinsics
extern Campos campos;
//camera Coordinate
extern Vec3D cx;
extern Vec3D r;//color of samples
extern Vec3D *c;//Image
extern MODE mode;

extern unsigned int pixel_index;
extern unsigned int num_hash;
extern unsigned int num_photon;
extern double hash_s;
extern CrashList **hash_grid;
extern CrashList *hitpoints;
// extern BDBox hpbbox;
extern Vec3D vw;
extern int primes[61];


inline unsigned int BDBhash(const int ix, const int iy, const int iz) {
    return (unsigned int) ((ix * 73856093) ^ (iy * 19349663) ^ (iz * 83492791)) % num_hash;
}

#endif