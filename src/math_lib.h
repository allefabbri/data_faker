#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

  // 2d vector algebra
  typedef struct VEC2D {
    double x, y, mod;
  } VEC2D;

  VEC2D set_vec2d(double, double);

  void setmod_vec2d(VEC2D *);

  void normalize_vec2d(VEC2D *);

  double prod_dot_2d(VEC2D, VEC2D);

  double prod_cross_2d(VEC2D, VEC2D);            // returns z-component on a right-handed frame

  void multiply_vec2d(double, VEC2D *);


  // 3d vector algebra
  typedef struct VEC3D {
    double x, y, z, mod;
  } VEC3D;

  VEC3D set_vec3d(double, double, double);

  void setmod_vec3d(VEC3D *);

  void normalize_vec3d(VEC3D *);

  VEC3D _normalize_vec3d(VEC3D v);

  double prod_dot(VEC3D, VEC3D);

  VEC3D prod_cross(VEC3D, VEC3D);

  void multiply_vec3d(double, VEC3D *);


  // 2d matrix algebra
  typedef struct MAT2D {
    double xx, xy, yx, yy;
  } MAT2D;

  MAT2D set_mat2d(double, double, double, double);

  MAT2D transpose_mat2d(MAT2D);

  MAT2D product_mat2d(MAT2D, MAT2D);

  void multiply_mat2d(double, MAT2D *);

  MAT2D make_rotation_2d_cs(double, double);

  MAT2D make_rotation_2d(double);

  VEC2D rotate_vec2d(MAT2D, VEC2D);

  MAT2D rotate_mat2d(MAT2D, MAT2D);


  // 3d matrix algebra
  typedef struct MAT3D {
    double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  } MAT3D;

  MAT3D set_mat3d(double, double, double, double, double, double, double, double, double);

  MAT3D transpose_mat3d(MAT3D);

  MAT3D product_mat3d(MAT3D, MAT3D);

  void multiply_mat3d(double, MAT3D *);

  MAT3D make_rotation_cs(VEC3D, double, double);

  MAT3D make_rotation(VEC3D, double);

  VEC3D rotate_vec3d(MAT3D, VEC3D);

  MAT3D rotate_mat3d(MAT3D, MAT3D);


  // 6d vector
  typedef struct VEC6D {
    VEC3D a, g;
  } VEC6D;

  VEC6D set_vec6d(double, double, double, double, double, double);

  VEC6D rotate_vec6d(MAT3D, VEC6D);


  // 6d matrix
  typedef struct MAT6D {
    MAT3D A[2][2];
  } MAT6D;

  MAT6D set_mat6d(MAT3D, MAT3D, MAT3D, MAT3D);

  MAT6D rotate_mat6d(MAT3D, MAT6D);


  // 2d eigenvalue problem
  typedef struct EigenSys {
    MAT2D A;
    double l1, l2;
    VEC2D v1, v2, u1, u2;
  } EigenSys;

  EigenSys eigs_2x2_sym_normalized(double, double);

  EigenSys eigs_2x2_sym(MAT2D);

  int check_eigs(EigenSys);


  // display function
  void print_vec2d(VEC2D, const char *);
  
  void print_vec3d(VEC3D, const char *);

  void print_mat2d(MAT2D, const char *);

  void print_mat3d(MAT3D, const char *);

  void print_vec6d(VEC6D, const char *);

  void print_mat6d(MAT6D, const char *);

  void print_eigs(EigenSys, const char *);

#ifdef __cplusplus
}
#endif
