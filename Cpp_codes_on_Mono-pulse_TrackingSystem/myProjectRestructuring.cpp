#include "Mpc.h"

//============================================================
//
itpp::cvec mpc_ideal_voltages(cvec sig)
{

	//output of monopulse comparator
	itpp::cvec mpc_op(4);
	itpp::complex<std::double> jj(0., 1.0);
	mpc_op[0] = (1. / 2.) * (sig[0] * 1.) + (sig[1] * (1.)) + (sig[2] * (1.)) + (sig[3] * (1.)); // sum
	mpc_op[1] = (1. / 2.) * (sig[0] + sig[1] - sig[2] - sig[3]) * (jj);// delta_el
	mpc_op[2] = (1. / 2.) * (-sig[0] + sig[1] + sig[2] - sig[3]) * (jj); // delta_az
	mpc_op[3] = (1. / 2.) * (-sig[0] + sig[1] - sig[2] + sig[3]); // unwanted
	return mpc_op;
}

void rician_channel_estimate(itpp::vec angle, float Pt_dbm, float Rmeter, float K_ricean, int N_multipath, int nexp)
{

	std::ofstream fout;
	fout.open("res_mpc");
	itpp::cvec vant_ricean(4);
	for (int o = 0; o < std::size(angle); o++)
	{
		float theta, phi; // global with origin middle of all 4 horn antennas
		theta = abs(angle[o]);
		phi = M_PI / 2;
		if (angle[o] < 0)phi = -M_PI / 2;
		// recieved voltages in each antenna 
		itpp::cvec vant = mpc.antenna_voltages_static(Pt_dbm, GtdB, Rmeter, theta, phi, Pr);
		// MPC outputs 
		itpp::itpp::cvec vmpc = mpc_ideal_voltages(vant); // no ricean & noise
		float sumrr = 0.0, azerr = 0.0, elerr = 0.0;

		for (int ex = 0; ex < nexp; ex++)
		{
			// ........................................................................
			// Ricean channel (from paper: write details)

			double sigma = 1. / sqrt(2. * (K_ricean + 1));
			double s = sqrt((K_ricean / (K_ricean + 1.)));

			for (int k = 0; k < 4; k++)
			{
				complex<std::double> h_ric = sigma * g_mean0_var1() + s + jj * sigma * g_mean0_var1();
				vant_ricean[k] = vant[k] * h_ric;
			}
			itpp::cvec vmpc_ricean = mpc_ideal_voltages(vant_ricean);
			sumrr = sumrr + abs(vmpc_ricean[0]);
			elerr = elerr + abs(vmpc_ricean[1]);
			azerr = azerr + abs(vmpc_ricean[2]);
		}
		sumrr = sumrr / nexp;
		elerr = elerr / nexp;
		azerr = azerr / nexp;
		fout << angle[o] * 180. / M_PI << "  " << abs(vant[0]) << "  " << abs(vant[1]) << "  " << abs(vant[2]) << "  " << abs(vant[3]);
		fout << "  " << abs(vmpc[0]) << "  " << abs(vmpc[1]) << "   " << abs(vmpc[2]);
		fout << "   " << sumrr << "   " << elerr << "   " << azerr << endl;

	}
	fout.close();
}


//===========================================================================
int main()
{
	float NF = 2.0;
	float f = 2.45;
	float lamm = 300 / f;
	float Tsymb = 10;
	// sampling freqency
	float Fsamp = itpp::double(3) / Tsymb;
	float Ac = 1.0;
	Horn hrn(88.9, 44.45, 2.5 * lamm, 1.25 * lamm, 3. * lamm / 2, 3 * lamm);
	hrn.set_freq(f);
	//hrn.data();
	float direc = hrn.Directivity();
	float gain = hrn.Gain(0.75);
	std::cout << "# Horn:" << std::endl;
	std::cout << "# direc = " << direc << std::endl;
	std::cout << "# gain  = " << 10. * log10(gain) << "dB" << std::endl;
	//return 0;
	//---------------------------------------------------//
	// UAV 
	int nexp = 100;
	float Pt_dbm = 30, GtdB = 0.0; // by UAV
	float Rmeter = 600; // distance from center of horn antennas
	float maxvel = 20. / (60 * 60); // (in meters /sec) ... 20 meter/hr typical, fastest is 160 meter/hr
	int N_multipath = 4; // for ricean
	float K_ricean = 6; // Direct path is K_ricean X indirect path power
	itpp::vec Pr, angle = itpp::linspace(-M_PI / 2, M_PI / 2, 201); // tragectory of UAV

	// Reciever Noise
	double kB = 1.38e-23;
	double N0 = kB * 300.0 * (pow(10, NF / 10) - 1.0); // power spectral density of noise

	// Define MPC 
	Mpc mpc(&hrn, f);// MPC

	// UAV is moving along the defined tragectory

	itpp::Uniform_RNG U(-M_PI, M_PI);
	itpp::Normal_RNG g_mean0_var1(0, 1);
	itpp::complex<double> jj(0., 1.);

	rician_channel_estimate(angle, Pt_dbm, Rmeter, K_ricean, N_multipath, nexp);

}

