#pragma once
#include "AntennaService.h"
#include "Dependencies.h"

const double M_PI = 22 / 7;
class Mpc
{

    Horn* horn;
    float fgz, horn_eff;
    float xc[4], yc[4], zc[4];
    float v[4];

public:
    float eta; // efficiency of antenna

    Mpc(Horn* hp, float fg);

    float Magnitude(const cvec a);

    cvec antenna_voltages_static(const float Ptdbm, const float GtdB, const float R, const float theta, const float phi, const vec& Pr);
    //----------------------------------------------------------------------------------//
};
