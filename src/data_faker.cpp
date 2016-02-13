#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <cmath>
#include <iomanip>

#include "jsoncons/json.hpp"

#include "noise_lib.hpp"

using namespace std;
using namespace jsoncons;

#define MAJOR             1
#define MINOR             1

int main(int argc, char ** argv) {
  cout << "DataFaker v" << MAJOR << "." << MINOR << endl;

  // Parse command line
  string config_name;
  if (argc == 2) {
    config_name = argv[1];
  }
  else {
    cout << "Usage: " << argv[0] << " path/to/config.json" << endl;
    cout << "      path/to/config.json must be a valid config JSON file" << endl;
    cout << endl;
    cout << "Usage: " << argv[0] << " -conf_t" << endl;
    cout << "      generate a dummy config JSON file" << endl;
    exit(2);
  }

  // Generate a dummy JSON config
  if (config_name == "-conf_t") {
    cout << "Creating dummy config JSON \"conf.json\". Quitting..." << endl;
    ofstream dummy("conf.json");
    dummy << R"({
    "samples":1000,         // number of samples
    "dt":0.0025,            // time interval between samples
    "initial_time":0.0,     // starting time
    "out_basename":"fake",  // name of output file
    "shape":"ellipsoid",    // shape selector : "box" , "ellipsoid" , "none"
    "L_X":0.1,              // x-axis shape length parameter
    "L_Y":0.25,             // y-axis shape length parameter
    "L_Z":0.50,             // z-axis shape length parameter
    "radius":1,             // radius rotation parameter
    "alpha_deg":40,         // [0,360] rotation parameter
    "theta_deg":135,        // [0,360] rotation parameter
    "phi_deg":65            // [0,180] rotation parameter
})";
    dummy.close();
    exit(2);
  }

  std::ifstream config(config_name);
  if (!config) {
    cout << "Configuration file " << config_name << " not found. Quitting..." << endl;
    exit(4);
  }
  config.close();

  // Parse config
  int samples;                                    // data parameters
  double t0, dt;
  string out_basename;
  string shape;                                   // shape parameters
  double Lx, Ly, Lz;
  void(*make_noise)(double, double, double, double &, double &, double &);
  double radius, alpha, theta, phi, x0, y0, z0;   // rotation parameters

  json jconf = jsoncons::json::parse_file(config_name);

  ////// data parameters
  samples = (jconf.has_member("samples")) ? jconf["samples"].as<int>() : SAMPLES;
  dt = (jconf.has_member("dt")) ? jconf["dt"].as<double>() : DT;
  t0 = (jconf.has_member("initial_time")) ? jconf["initial_time"].as<double>() : T0;
  out_basename = (jconf.has_member("out_basename")) ? jconf["out_basename"].as<string>() : "fake_data";

  ////// shape parameters
  Lx = (jconf.has_member("L_X")) ? jconf["L_X"].as<double>() : L_X;
  Ly = (jconf.has_member("L_Y")) ? jconf["L_Y"].as<double>() : L_Y;
  Lz = (jconf.has_member("L_Z")) ? jconf["L_Z"].as<double>() : L_Z;
  shape = (jconf.has_member("shape")) ? jconf["shape"].as<string>() : "none";
  if (shape == "ellipsoid") {
    make_noise = &make_ellipsoid_noise;
  }
  else if (shape == "box") {
    make_noise = &make_box_noise;
  }
  else {
    make_noise = &make_no_noise;
  }

  ////// rotation parameters
  radius = (jconf.has_member("radius")) ? jconf["radius"].as<double>() : RADIUS;
  alpha = (jconf.has_member("alpha_deg")) ? jconf["alpha_deg"].as<double>() / RAD_TO_DEG : ALPHA_DEG / RAD_TO_DEG;
  theta = (jconf.has_member("theta_deg")) ? jconf["theta_deg"].as<double>() / RAD_TO_DEG : THETA_DEG / RAD_TO_DEG;
  phi = (jconf.has_member("phi_deg")) ? jconf["phi_deg"].as<double>() / RAD_TO_DEG : PHI_DEG / RAD_TO_DEG;
  x0 = 0.0;
  y0 = 0.0;
  z0 = radius;

  // Data generation
  int header_counter = 1;
  ofstream data(out_basename+".txt");
  data << "# Rotations parameters" << endl; header_counter++;
  data << "# ALPHA = " << fixed << setprecision(1) << setw(5) << alpha*RAD_TO_DEG
    << " (deg) = " << fixed << setprecision(3) << setw(5) << alpha << " (rad)" << endl; header_counter++;
  data << "# THETA = " << fixed << setprecision(1) << setw(5) << theta*RAD_TO_DEG
    << " (deg) = " << fixed << setprecision(3) << setw(5) << theta << " (rad)" << endl; header_counter++;
  data << "# PHI   = " << fixed << setprecision(1) << setw(5) << phi*RAD_TO_DEG
    << " (deg) = " << fixed << setprecision(3) << setw(5) << phi   << " (rad)" << endl; header_counter++;
  data << "#" << endl; header_counter++;
  data << "#    t       x      y       z" << endl; header_counter++;
  for (int i = 0; i < samples; i++) {
    double t, x, y, z;
    double noise_x, noise_y, noise_z;

    make_noise(Lx, Ly, Lz, noise_x, noise_y, noise_z);
    t = t0 + i*dt;
    x = x0 + noise_x;
    y = y0 + noise_y;
    z = z0 + noise_z;
    rotate(alpha, theta, phi, x, y, z);

    data << fixed << setprecision(4) << t << "\t" 
      << fixed << setprecision(3) << x << "\t" << y << "\t" << z << endl;
  }
  data.close();

  // Generate GNUPLOT script
  ofstream gnuplot(out_basename+".plt");
  gnuplot << R"(#!/usr/bin/gnuplot
reset
set terminal pngcairo dashed size 900, 700 enhanced font 'Verdana,10'
set output ')" << out_basename << R"(.png'
set size ratio 1
set multiplot layout 2, 2 title "Data 3d dispersion" font ",14"
# Styles
linew = 1.2
points = 0.3
set style line  21 lc rgb '#0072bd' pointtype 7 pointsize points  # blue
set style line  22 lc rgb '#d95319' pointtype 7 pointsize points  # orange
set style line  23 lc rgb '#77ac30' pointtype 7 pointsize points  # green
set style line 101 lc rgb '#000000' lt 1 lw 1                     # black
set style line 102 lc rgb '#d6d7d9' lt 1 lw 1                     # gray
# Axes label
range = 1.2
set xrange[-range:range]
set yrange[-range:range]
# Border xy
set border 3 front ls 101
set tics nomirror out scale 0.75
set format '%g'
set border linewidth 1.5
# Grid
set x2tics - 2, 0.25, 2 scale 0 format " "
set y2tics - 2, 0.25, 2 scale 0 format " "
set grid x2tics y2tics back ls 102
# Legend
set key top
# Plot
input = ")" << out_basename << R"(.txt"
set size 0.5, 0.5
set xlabel 'x'
set ylabel 'y'
plot input every ::)" << header_counter << R"( using 2:3 notitle with points ls 21
set xlabel 'z'
set ylabel 'y'
plot input every ::)" << header_counter << R"( using 4:3 notitle with points ls 22
set xlabel 'x'
set ylabel 'z'
plot input every ::)" << header_counter << R"( using 2:4 notitle with points ls 23
unset multiplot
)";


  return 0;
}
