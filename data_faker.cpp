#include <fstream>
#include <cmath>
#include <iomanip>
#include <random>

using namespace std;

#define SAMPLES           400
#define dt                1./400.
#define T0                0
#define THETA_DEG         43              // [0, 360] angle between v and x-axis
#define PHI_DEG           12              // [0, 180] angle between v and z-axis
#define RADIUS            1/0.48          // [0, inf[

#define NOISE_FRAC        0.1

#define RAD_TO_DEG        57.2957795131   // 180/pi

int main(){

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0,NOISE_FRAC*RADIUS);

	ofstream data("fake_data.txt");
	for(int i=0; i<SAMPLES; i++){
		double t,x,y,z;
		double noise;
		t = T0 + i*dt;
		noise = distribution(generator);
		x = RADIUS * cos(THETA_DEG/RAD_TO_DEG) * sin(PHI_DEG/RAD_TO_DEG) + noise;
		noise = distribution(generator);
		y = RADIUS * sin(THETA_DEG/RAD_TO_DEG) * sin(PHI_DEG/RAD_TO_DEG) + noise;
		noise = distribution(generator);
		z = RADIUS * cos(PHI_DEG/RAD_TO_DEG) + noise;

		data << fixed << setprecision(3) << t << "\t" << x << "\t" << y << "\t" << z << endl;
	}
	data.close();

	return 0;
}