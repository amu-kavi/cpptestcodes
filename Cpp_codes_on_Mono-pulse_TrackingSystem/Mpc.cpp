#include "Mpc.h"


    
    Mpc::Mpc(Horn* hp, float fg)
    {
        horn_eff = 0.75;
        horn = hp;
        Horn h = *hp;
        fgz = fg;
        std::cout << "# Mono-Pulse-Comparator " << std::endl;
        std::cout << "# Co-ordinates of Horn Antennas:" << std::endl;
        xc[0] = h.a1mm / 2; yc[0] = h.b1mm / 2; zc[0] = 0.0;
        xc[1] = -h.a1mm / 2; yc[1] = h.b1mm / 2; zc[1] = 0.0;
        xc[2] = -h.a1mm / 2; yc[2] = -h.b1mm / 2; zc[2] = 0.0;
        xc[3] = h.a1mm / 2; yc[3] = -h.b1mm / 2; zc[3] = 0.0;
        std::cout << std::endl;
    }
    float Mpc::Magnitude(const cvec a)
    {
        return sqrt(abs(a[0]) * abs(a[0]) + abs(a[1]) * abs(a[1]) + abs(a[2]) * abs(a[2]));
    }
    //-----------------------------------------------------------------------------------//
    cvec Mpc::antenna_voltages_static(const float Ptdbm, const float GtdB, const float R, const float theta, const float phi, const vec& Pr)
    {
        //
        float Pt = pow(10., Ptdbm / 10) * 1e-3; // in watts
        float lambda = (300.0 / fgz) * 1e-3; // in meter
        Pr.set_size(4);
        //
        // position of UAV in cartecian cordinates	
        float X = R * sin(theta) * cos(phi);
        float Y = R * sin(theta) * sin(phi);
        float Z = R * cos(theta);
        cvec v(4), ri(3);
        float magv, angv, Gr, Gt = pow(10, GtdB / 10);
        complex<double> jj(0., 1.0);

        for (int i = 0; i < 4; i++)
        {
            // vec_ri=-vec_R+ vec_ci (1e-3 conversion to meter from mm)
            float xci = xc[i] * 1e-3;
            float yci = yc[i] * 1e-3;
            float zci = zc[i] * 1e-3;

            // position of UAV in local cordinates 
            float xi = X - xci;
            float yi = Y - yci;
            float zi = Z;
            float ri = sqrt(pow(xi, 2) + pow(yi, 2) + pow(zi, 2));
            float thetai = acos(zi / ri);
            float phii = atan(yi / xi);
            cvec E = horn->E_field(ri * 1000, thetai, phii);
            Gr = sqrt(pow(abs(E[0]), 2) + pow(abs(E[1]), 2) + pow(abs(E[2]), 2));
            Pr[i] = Pt * Gt * Gr / pow(4. * M_PI * ri / lambda, 2);
            magv = sqrt(2. * Pr[i]);
            angv = (2. * M_PI / lambda) * ri;
            v[i] = magv * exp(jj * angv);
        }
        return v;
    }
  