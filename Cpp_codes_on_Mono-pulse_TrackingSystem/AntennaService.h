#include<complex.h>

class Antenna
{

public:
	virtual void set_freq(float fgz) {};
	virtual float Gain(float eta) {};
	virtual float Directivity() {};
	virtual cvec E_field(float rmm, float theta, float phi) {};


};

class Horn : public Antenna
{
private:
	float amm, bmm, a1mm, b1mm, R1mm, R2mm, Pemm, Phmm, Remm, Rhmm;
	float a, b, a1, b1, R1, R2, fgz, lamm;
	int setf;

public:
	Horn(float amm, float bmm, float a1mm, float b1mm, float R1mm, float R2mm);
	void set_freq(float fgz);
	float Gain(float eta);
	float Directivity();
	cvec E_field(float rmm, float theta, float phi);
	mat RadPat2D(std::string pl, float rmm, float dtheta);
	void data();
};

class Dish : public Antenna
{
private:
	float amm, bmm, a1mm, b1mm, R1mm, R2mm, Pemm, Phmm, Remm, Rhmm;
	float a, b, a1, b1, R1, R2, fgz, lamm;
	int setf;

public:
	Dish(float amm, float bmm, float a1mm, float b1mm, float R1mm, float R2mm);
	void set_freq(float fgz);
	float Gain(float eta);
	float Directivity();
	cvec E_field(float rmm, float theta, float phi);
	mat RadPat2D(std::string pl, float rmm, float dtheta);
	void data();
};