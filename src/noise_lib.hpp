#include <random>
#include "math_lib.h"

// Data parameters
#define SAMPLES           1000
#define DT                1./400.
#define T0                0

// Center of Mass polar coordinates
#define THETA_DEG         135                  // [0, 360] angle between v and x-axis
#define PHI_DEG           35                   // [0, 180] angle between v and z-axis
#define ALPHA_DEG         75                   // [0, 360] rigid body angle
#define RADIUS            1                    // [0, inf[

// Cloud parameters
#define L_X           0.12
#define L_Y           0.25
#define L_Z           0.50

// Conversions and constants
#define RAD_TO_DEG    57.2957795131             // 180/pi
#define M_PI          3.14159265358979323846    /* pi */

//#define ENABLE_NORMAL

std::default_random_engine generator;
#ifdef ENABLE_NORMAL
std::normal_distribution<double> dist_u(0.0, 0.05);
#else
std::uniform_real_distribution<double> dist_u(0.0, 1.0);
#endif
std::uniform_real_distribution<double> dist_theta(0.0, 2.0*M_PI);
std::uniform_real_distribution<double> dist_phi(0.0, M_PI);

void make_ellipsoid_noise(double Lx, double Ly, double Lz, double &nx, double &ny, double &nz) {
  double u, theta, phi;
  u = fabs(dist_u(generator));                  // fabs in case of NORMAL distribution
  theta = dist_theta(generator);
  phi = dist_phi(generator);
  nx = Lx * u * cos(theta) * sin(phi);
  ny = Ly * u * sin(theta) * sin(phi);
  nz = Lz * u * cos(phi);
}

void make_box_noise(double Lx, double Ly, double Lz, double &nx, double &ny, double &nz) {
  double u;
  u = dist_u(generator);
  nx = -Lx / 2 + Lx * u;
  u = dist_u(generator);
  ny = -Ly / 2 + Ly * u;
  u = dist_u(generator);
  nz = -Lz / 2 + Lz * u;
}

void make_no_noise(double Lx, double Ly, double Lz, double &nx, double &ny, double &nz) {
  nx = 0.0;
  ny = 0.0;
  nz = 0.0;
}

void rotate(double alpha, double theta, double phi, double &x, double &y, double &z) {
  VEC3D vr;
  MAT3D rot_z1, rot_y, rot_z2;
  vr = set_vec3d(x, y, z);
  rot_z1 = make_rotation(set_vec3d(0.0, 0.0, 1.0), alpha);
  rot_y = make_rotation(set_vec3d(0.0, 1.0, 0.0), phi);
  rot_z2 = make_rotation(set_vec3d(0.0, 0.0, 1.0), theta);
  vr = rotate_vec3d(vr, rot_z1);
  vr = rotate_vec3d(vr, rot_y);
  vr = rotate_vec3d(vr, rot_z2);
  x = vr.x;
  y = vr.y;
  z = vr.z;
}
