#include <fstream>
#include <cmath>
#include <iomanip>
#include <random>

#include "math_lib.h"

using namespace std;

#define SAMPLES           1000
#define dt                1./400.
#define T0                0

// Center of Mass polar coordinates
#define THETA_DEG         135                  // [0, 360] angle between v and x-axis
#define PHI_DEG           35                   // [0, 180] angle between v and z-axis
#define ALPHA_DEG         75                   // [0, 360] rigid body angle
#define RADIUS            1                    // [0, inf[

// Cloud parameters
#define A_X           0.12
#define A_Y           0.25
#define A_Z           0.50

// Conversions and constants
#define RAD_TO_DEG    57.2957795131             // 180/pi
#define M_PI          3.14159265358979323846    /* pi */

std::default_random_engine generator;
std::uniform_real_distribution<double> dist_u(0.0,1.0);
std::uniform_real_distribution<double> dist_theta(0.0,2.0*M_PI);
std::uniform_real_distribution<double> dist_phi(0.0,M_PI);

void make_ellipsoid_noise(double &nx, double &ny, double &nz){
	double u, theta, phi;
	u = dist_u(generator);
	theta = dist_theta(generator);
	phi = dist_phi(generator);
	nx = A_X * u * cos(theta) * sin(phi);
	ny = A_Y * u * sin(theta) * sin(phi);
	nz = A_Z * u * cos(phi);
}

void make_box_noise(double &nx, double &ny, double &nz){
	double u;
	u = dist_u(generator);
	nx = -A_X/2 + A_X * u;
	u = dist_u(generator);
	ny = -A_Y/2 + A_Y * u;
	u = dist_u(generator);
	nz = -A_Z/2 + A_Z * u;
}

void make_no_noise(double &nx, double &ny, double &nz){
	nx = 0.0;
	ny = 0.0;
	nz = 0.0;
}

void rotate(double &x, double &y, double &z){
	VEC3D vr;
	MAT3D rot_z1, rot_y, rot_z2;
	vr = set_vec3d(x, y, z);
	rot_z1 = make_rotation(set_vec3d(0.0, 0.0, 1.0), ALPHA_DEG/RAD_TO_DEG);
	rot_y = make_rotation(set_vec3d(0.0, 1.0, 0.0), PHI_DEG/RAD_TO_DEG);
	rot_z2 = make_rotation(set_vec3d(0.0, 0.0, 1.0), THETA_DEG/RAD_TO_DEG);
	vr = rotate_vec3d(vr, rot_z1);
	vr = rotate_vec3d(vr, rot_y);
	vr = rotate_vec3d(vr, rot_z2);
	x = vr.x;
	y = vr.y;
	z = vr.z;
}

void (*make_noise)(double &, double &, double &);

int main(){
	double x0,y0,z0;
	x0 = 0.0;
	y0 = 0.0;
	z0 = RADIUS;

//	make_noise = &make_no_noise;
//	make_noise = &make_box_noise;
	make_noise = &make_ellipsoid_noise;

	ofstream data("fake_data.txt");
	for(int i=0; i<SAMPLES; i++){
		double t,x,y,z;

		double noise_x, noise_y, noise_z;
		make_noise(noise_x, noise_y, noise_z);
		
		t = T0 + i*dt;
		x = x0 + noise_x;
		y = y0 + noise_y;
		z = z0 + noise_z;
		rotate(x, y, z);

		data << fixed << setprecision(3) << t << "\t" << x << "\t" << y << "\t" << z << endl;
	}
	data.close();

	return 0;
}
