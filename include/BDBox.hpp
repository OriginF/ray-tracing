#ifndef __BDBOX_H__
#define __BDBOX_H__

#include "./def.hpp"
#include "./Vec.hpp"
#include "./Crash.hpp"
#include "./init.hpp"

class BDBox{
public:
	Vec3D min, max;  //OBB求交

public:
    BDBox():min(Vec3D(1e20,1e20,1e20)),max(Vec3D(-1e20,-1e20,-1e20)){};
	void fit(const Vec3D &pos){
		if(pos.x < min.x) min.x = pos.x;
		if(pos.y < min.y) min.y = pos.y;
		if(pos.z < min.z) min.z = pos.z;
		max.x = MAX(pos.x, max.x);
		max.y = MAX(pos.y, max.y);
		max.z = MAX(pos.z, max.z);
	}
	void reset(){
		min = Vec3D(1e20, 1e20, 1e20);
		max = Vec3D(-1e20, -1e20, -1e20);
	} 
};

#endif