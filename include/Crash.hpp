#ifndef __CRASH_H__
#define __CRASH_H__

#include "./Vec.hpp"

class Crash{
public:
    Vec3D color;
    Vec3D position, normal, flux;
    double r2;
    unsigned int n;
    int pixel_index;
};

class CrashList{
public:
    Crash *id;
    CrashList *next;

    CrashList* add(Crash *point){
        CrashList* nlist = new CrashList;
        nlist->id = point;
        nlist->next = this;
        return nlist;
    }
};

#endif