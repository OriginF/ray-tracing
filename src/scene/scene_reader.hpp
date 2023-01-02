#ifndef __SCENE_READER_H__
#define __SCENE_READER_H__

#include "../../include/include.hpp"
#include "./scene.hpp"
#include "../def.hpp"

vector<string> divide(string line){
    vector<string> temps;
    if(line.size()==0)return temps;
    int last_position = 0;
    do{
        int position = line.find(" ");
        if(position==line.npos){
            temps.push_back(line);
            break;
        }
        temps.push_back(line.substr(last_position,position));
        line = line.substr(position+1);
    }while(true);
    return temps;
}

void scene_reader(string filename){
    ifstream file(filename);
    string line;
    // vector<BaseGeo*> scenes;
    getline(file,line);
    vector<string> argd = divide(line);
    if(argd[0]=="SIMPLE")mode = SIMPLE;
    else if(argd[0]=="BRDF")mode = BRDF;
    else if(argd[0]=="MIS")mode = MIS;
    samples = atoi(argd[1].c_str());
    while(getline(file,line)){
        if(line[0]=='#')continue;
        vector<string> temps = divide(line);
        if(temps.size()==0){printf("aha");continue;}
        for(int i=0;i<temps.size();i++){
            printf(temps[i].c_str());
            printf(" ");
        }
        printf("\n");
        if(temps[0]=="Mesh"){
            Vec3D v1(atof(temps[1].c_str()),atof(temps[2].c_str()),atof(temps[3].c_str()));
            Vec3D v2(atof(temps[4].c_str()),atof(temps[5].c_str()),atof(temps[6].c_str()));
            Vec3D v3(atof(temps[7].c_str()),atof(temps[8].c_str()),atof(temps[9].c_str()));
            Vec3D emission(atof(temps[10].c_str()),atof(temps[11].c_str()),atof(temps[12].c_str()));
            Vec3D color(atof(temps[13].c_str()),atof(temps[14].c_str()),atof(temps[15].c_str()));
            REFLECT reflect = temps[16]=="DIFF"?DIFF:temps[16]=="SPEC"?SPEC:REFR;
            double alpha = 0;
            if(temps.size()==18)alpha = atof(temps[17].c_str());
            Mesh* mesh = new Mesh(v1,v2,v3,emission,color,reflect,alpha);
            geos.push_back((BaseGeo*)mesh);
        }
        else if(temps[0]=="Sphere"){
            double radius = atof(temps[1].c_str());
            Vec3D center(atof(temps[2].c_str()),atof(temps[3].c_str()),atof(temps[4].c_str()));
            Vec3D emission(atof(temps[5].c_str()),atof(temps[6].c_str()),atof(temps[7].c_str()));
            Vec3D color(atof(temps[8].c_str()),atof(temps[9].c_str()),atof(temps[10].c_str()));
            REFLECT reflect = temps[11]=="DIFF"?DIFF:(temps[11]=="SPEC"?SPEC:REFR);
            double alpha = 0;
            if(temps.size()==13)alpha = atof(temps[12].c_str());
            Sphere* sphere = new Sphere(radius,center,emission,color,reflect,alpha);
            geos.push_back((BaseGeo*)sphere);
        }
        else{
            printf("wrong scene file");
            exit(-1);
        }
    }
}

#endif