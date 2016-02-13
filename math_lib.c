#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "math_lib.h"

#define EPSILON        1e-8

// vector algebra
VEC3D set_vec3d(double x, double y, double z){
	VEC3D v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.mod = sqrt(x*x+y*y+z*z);
	if( v.mod < EPSILON ) v.mod = 0.0;
	return v;
}

void setmod_vec3d(VEC3D * v) {
  v->mod = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

void normalize_vec3d(VEC3D * v){
	v->x /= v->mod;
	v->y /= v->mod;
	v->z /= v->mod;
	v->mod = 1.0;
}

double prod_dot(VEC3D a, VEC3D b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

VEC3D prod_cross(VEC3D a, VEC3D b){
	return set_vec3d( a.y * b.z - a.z * b.y ,
					  a.z * b.x - a.x * b.z ,
					  a.x * b.y - a.y * b.x );
}

void multiply_vec3d(double alpha, VEC3D * v) {
  v->x *= alpha;
  v->y *= alpha;
  v->z *= alpha;
  v->mod *= alpha;
}


// matrix algebra
MAT3D set_mat3d(double xx, double xy, double xz, double yx, double yy, double yz, double zx, double zy, double zz ){
	MAT3D m;
	m.xx = xx;
	m.xy = xy;
	m.xz = xz;
	m.yx = yx;
	m.yy = yy;
	m.yz = yz;
	m.zx = zx;
	m.zy = zy;
	m.zz = zz;
	return m;
}

MAT3D transpose_mat3d(MAT3D A){
	MAT3D C;
	C.xx = A.xx;
	C.xy = A.yx;
	C.xz = A.zx;
	C.yx = A.xy;
	C.yy = A.yy;
	C.yz = A.zy;
	C.zx = A.xz;
	C.zy = A.yz;
	C.zz = A.zz;
	return C;
}

MAT3D product_mat3d(MAT3D A, MAT3D B){
	MAT3D C;
	C.xx = A.xx * B.xx + A.xy * B.yx + A.xz * B.zx;
  C.xy = A.xx * B.xy + A.xy * B.yy + A.xz * B.zy;
  C.xz = A.xx * B.xz + A.xy * B.yz + A.xz * B.zz;
	C.yx = A.yx * B.xx + A.yy * B.yx + A.yz * B.zx;
  C.yy = A.yx * B.xy + A.yy * B.yy + A.yz * B.zy;
  C.yz = A.yx * B.xz + A.yy * B.yz + A.yz * B.zz;
	C.zx = A.zx * B.xx + A.zy * B.yx + A.zz * B.zx;
  C.zy = A.zx * B.xy + A.zy * B.yy + A.zz * B.zy;
  C.zz = A.zx * B.xz + A.zy * B.yz + A.zz * B.zz;
	return C;
}

void multiply_mat3d(double alpha, MAT3D * m) {
  m->xx *= alpha;
  m->xy *= alpha;
  m->xz *= alpha;
  m->yx *= alpha;
  m->yy *= alpha;
  m->yz *= alpha;
  m->zx *= alpha;
  m->zy *= alpha;
  m->zz *= alpha;
}

MAT3D make_rotation_cs(VEC3D axis, double c, double s){
	if ( axis.mod < EPSILON) { printf("Null vector as axis of rotation\n"); exit(5); }
	axis.x /= axis.mod;
	axis.y /= axis.mod;
	axis.z /= axis.mod;

	MAT3D rot;
	rot.xx = axis.x*axis.x*(1 - c) + c;   	    rot.xy = axis.x*axis.y*(1 - c) - axis.z*s;  rot.xz = axis.x*axis.z*(1 - c) + axis.y*s;
	rot.yx = axis.y*axis.x*(1 - c) + axis.z*s;	rot.yy = axis.y*axis.y*(1 - c) + c;	        rot.yz = axis.y*axis.z*(1 - c) - axis.x*s;
	rot.zx = axis.z*axis.x*(1 - c) - axis.y*s;	rot.zy = axis.z*axis.y*(1 - c) + axis.x*s;  rot.zz = axis.z*axis.z*(1 - c) + c;
	return rot;	
}

MAT3D make_rotation(VEC3D axis, double angle){
	double c,s;
	c = cos(angle);
	s = sin(angle);

	MAT3D rot = make_rotation_cs(axis, c, s);
	return rot;
}

VEC3D rotate_vec3d(VEC3D v, MAT3D rot){
	VEC3D vr;
	vr.x = rot.xx * v.x + rot.xy * v.y + rot.xz * v.z;
	vr.y = rot.yx * v.x + rot.yy * v.y + rot.yz * v.z;
	vr.z = rot.zx * v.x + rot.zy * v.y + rot.zz * v.z;
	return vr;
}

MAT3D rotate_mat3d(MAT3D m, MAT3D rot){
	MAT3D mr;
	mr.xx = m.xx * rot.xx * rot.xx + m.xy * rot.xx * rot.xy + m.yx * rot.xx * rot.xy + m.yy * rot.xy * rot.xy + m.xz * rot.xx * rot.xz + m.zx * rot.xx * rot.xz + m.yz * rot.xy * rot.xz + m.zy * rot.xy * rot.xz + m.zz * rot.xz * rot.xz; 
	mr.xy = m.xx * rot.xx * rot.yx + m.yx * rot.xy * rot.yx + m.zx * rot.xz * rot.yx + m.xy * rot.xx * rot.yy + m.yy * rot.xy * rot.yy + m.zy * rot.xz * rot.yy + m.xz * rot.xx * rot.yz + m.yz * rot.xy * rot.yz + m.zz * rot.xz * rot.yz; 
	mr.xz = m.xx * rot.xx * rot.zx + m.yx * rot.xy * rot.zx + m.zx * rot.xz * rot.zx + m.xy * rot.xx * rot.zy + m.yy * rot.xy * rot.zy + m.zy * rot.xz * rot.zy + m.xz * rot.xx * rot.zz + m.yz * rot.xy * rot.zz + m.zz * rot.xz * rot.zz;
	mr.yx = m.xx * rot.xx * rot.yx + m.xy * rot.xy * rot.yx + m.xz * rot.xz * rot.yx + m.yx * rot.xx * rot.yy + m.yy * rot.xy * rot.yy + m.yz * rot.xz * rot.yy + m.zx * rot.xx * rot.yz + m.zy * rot.xy * rot.yz + m.zz * rot.xz * rot.yz; 
	mr.yy = m.xx * rot.yx * rot.yx + m.xy * rot.yx * rot.yy + m.yx * rot.yx * rot.yy + m.yy * rot.yy * rot.yy + m.xz * rot.yx * rot.yz + m.zx * rot.yx * rot.yz + m.yz * rot.yy * rot.yz + m.zy * rot.yy * rot.yz + m.zz * rot.yz * rot.yz;
	mr.yz = m.xx * rot.yx * rot.zx + m.yx * rot.yy * rot.zx + m.zx * rot.yz * rot.zx + m.xy * rot.yx * rot.zy + m.yy * rot.yy * rot.zy + m.zy * rot.yz * rot.zy + m.xz * rot.yx * rot.zz + m.yz * rot.yy * rot.zz + m.zz * rot.yz * rot.zz;
	mr.zx = m.xx * rot.xx * rot.zx + m.xy * rot.xy * rot.zx + m.xz * rot.xz * rot.zx + m.yx * rot.xx * rot.zy + m.yy * rot.xy * rot.zy + m.yz * rot.xz * rot.zy + m.zx * rot.xx * rot.zz + m.zy * rot.xy * rot.zz + m.zz * rot.xz * rot.zz; 
	mr.zy = m.xx * rot.yx * rot.zx + m.xy * rot.yy * rot.zx + m.xz * rot.yz * rot.zx + m.yx * rot.yx * rot.zy + m.yy * rot.yy * rot.zy + m.yz * rot.yz * rot.zy + m.zx * rot.yx * rot.zz + m.zy * rot.yy * rot.zz + m.zz * rot.yz * rot.zz;
	mr.zz = m.xx * rot.zx * rot.zx + m.xy * rot.zx * rot.zy + m.yx * rot.zx * rot.zy + m.yy * rot.zy * rot.zy + m.xz * rot.zx * rot.zz + m.zx * rot.zx * rot.zz + m.yz * rot.zy * rot.zz + m.zy * rot.zy * rot.zz + m.zz * rot.zz * rot.zz;
	return mr;
}


// 6d vector
VEC6D set_vec6d(double ax, double ay, double az, double gx, double gy, double gz) {
  VEC6D v;
  v.a = set_vec3d(ax, ay, az);
  v.g = set_vec3d(gx, gy, gz);
  return v;
}

VEC6D rotate_vec6d(VEC6D v, MAT3D rot) {
  VEC6D vr;
  vr.a = rotate_vec3d(v.a, rot);
  vr.g = rotate_vec3d(v.g, rot);
  return vr;
}


// 6d matrix
MAT6D set_mat6d(MAT3D A00, MAT3D A01, MAT3D A10, MAT3D A11) {
  MAT6D m;
  m.A[0][0] = A00;
  m.A[0][1] = A01;
  m.A[1][0] = A10;
  m.A[1][1] = A11;
  return m;
}

MAT6D rotate_mat6d(MAT6D m, MAT3D rot) {
  MAT6D mr;
  mr.A[0][0] = rotate_mat3d(m.A[0][0], rot);
  mr.A[0][1] = rotate_mat3d(m.A[0][1], rot);
  mr.A[1][0] = rotate_mat3d(m.A[1][0], rot);
  mr.A[1][1] = rotate_mat3d(m.A[1][1], rot);
  return mr;
}


// 2d eigenvalue problem

// Returns the eigensystem of a 2x2 matrix of the form
// | 1 a |
// | a b |
// for real a,b
EigenSys eigs_2x2_sym_normalized(double a, double b){
	EigenSys es;
	es.a[0][0] = 1.0;
	es.a[0][1] = a;
	es.a[1][0] = a;
	es.a[1][1] = b;
	es.l1 = (1+b)*0.5 + 0.5*sqrt((b-1)*(b-1) + 4*a*a);
	es.l2 = (1+b)*0.5 - 0.5*sqrt((b-1)*(b-1) + 4*a*a);
	es.v1[0] = 1.0;
	es.v1[1] = (es.l1-1)/a;
	es.v2[0] = 1.0;
	//es.v2[1] = -1.0/es.v1[0]; // for theoretical reasons
	es.v2[1] = (es.l2-1)/a;
	if( es.v1[0]*es.v2[1]-es.v1[1]*es.v2[0] < 0 ){ // "right-handed", in the sense that z-axis is pointing "outward" the screen
		es.v2[0] = -es.v2[0];
		es.v2[1] = -es.v2[1];
	}
	es.u1[0] = es.v1[0]/sqrt(es.v1[0]*es.v1[0]+es.v1[1]*es.v1[1]);
	es.u1[1] = es.v1[1]/sqrt(es.v1[0]*es.v1[0]+es.v1[1]*es.v1[1]);
	es.u2[0] = es.v2[0]/sqrt(es.v2[0]*es.v2[0]+es.v2[1]*es.v2[1]);
	es.u2[1] = es.v2[1]/sqrt(es.v2[0]*es.v2[0]+es.v2[1]*es.v2[1]);
	return es;
}

// Returns the eigensystem of a 2x2 matrix of the form
// | a b |
// | b c |
// for real a,b,c
EigenSys eigs_2x2_sym(double a[2][2]){
	double trA, detA;
	EigenSys es;
	es.a[0][0] = a[0][0];
	es.a[0][1] = a[0][1];
	es.a[1][0] = a[1][0];
	es.a[1][1] = a[1][1];
	trA = a[0][0] + a[1][1];
	detA = a[0][0]*a[1][1]-a[0][1]*a[1][0];
	printf("detA = %f\n",trA*trA-4*detA);
	es.l1 = (trA + sqrt(trA*trA-4*detA))*0.5;
	es.l2 = (trA - sqrt(trA*trA-4*detA))*0.5;
	es.v1[0] = 1.0;
	es.v1[1] = (es.l1-a[0][0])/a[0][1];
	es.v2[0] = 1.0;
	es.v2[1] = -1.0/es.v1[1];
	if( es.v1[0]*es.v2[1]-es.v1[1]*es.v2[0] < 0 ){ // "right-handed", in the sense that z-axis is pointing "outward" the screen
		es.v2[0] = -es.v2[0];
		es.v2[1] = -es.v2[1];
	}
	es.u1[0] = es.v1[0]/sqrt(es.v1[0]*es.v1[0]+es.v1[1]*es.v1[1]);
	es.u1[1] = es.v1[1]/sqrt(es.v1[0]*es.v1[0]+es.v1[1]*es.v1[1]);
	es.u2[0] = es.v2[0]/sqrt(es.v2[0]*es.v2[0]+es.v2[1]*es.v2[1]);
	es.u2[1] = es.v2[1]/sqrt(es.v2[0]*es.v2[0]+es.v2[1]*es.v2[1]);
	return es;
}

double check_eigs(EigenSys es){
	double check = 0;
	check += fabs(es.a[0][0]*es.v1[0]+es.a[0][1]*es.v1[1] - es.l1*es.v1[0]);
	check += fabs(es.a[1][0]*es.v1[0]+es.a[1][1]*es.v1[1] - es.l1*es.v1[1]);
	check += fabs(es.a[0][0]*es.v2[0]+es.a[0][1]*es.v2[1] - es.l2*es.v2[0]);
	check += fabs(es.a[1][0]*es.v2[0]+es.a[1][1]*es.v2[1] - es.l2*es.v2[1]);
	return check;	
}


// display function
void print_vec3d(VEC3D v, const char * name){
	printf("%s = \n| %6.3f  %6.3f  %6.3f |\n", name, v.x, v.y, v.z);
}

void print_mat3d(MAT3D m, const char * name){
	printf("%s = \n| %6.3f %6.3f %6.3f |\n| %6.3f %6.3f %6.3f |\n| %6.3f %6.3f %6.3f |\n", name, m.xx, m.xy, m.xz, m.yx, m.yy, m.yz, m.zx, m.zy, m.zz);
}

void print_vec6d(VEC6D v, const char * name) {
  printf("%s = \n| %6.3f %6.3f %6.3f  ,  %6.3f %6.3f %6.3f |\n", name, v.a.x, v.a.y, v.a.z, v.g.x, v.g.y, v.g.z);
}

void print_mat6d(MAT6D m, const char * name) {
  printf("%s = \n", name);
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[0][0].xx, m.A[0][0].xy, m.A[0][0].xz, m.A[0][1].xx, m.A[0][1].xy, m.A[0][1].xz);
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[0][0].yx, m.A[0][0].yy, m.A[0][0].yz, m.A[0][1].yx, m.A[0][1].yy, m.A[0][1].yz);
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[0][0].zx, m.A[0][0].zy, m.A[0][0].zz, m.A[0][1].zx, m.A[0][1].zy, m.A[0][1].zz);
  printf("| %8s                                    |\n", " ");
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[1][0].xx, m.A[1][0].xy, m.A[1][0].xz, m.A[1][1].xx, m.A[1][1].xy, m.A[1][1].xz);
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[1][0].yx, m.A[1][0].yy, m.A[1][0].yz, m.A[1][1].yx, m.A[1][1].yy, m.A[1][1].yz);
  printf("| %6.3f %6.3f %6.3f \t %6.3f %6.3f %6.3f |\n", m.A[1][0].zx, m.A[1][0].zy, m.A[1][0].zz, m.A[1][1].zx, m.A[1][1].zy, m.A[1][1].zz);
}

void print_eigs(EigenSys es, const char * tag){
	printf("%s = \n\t| %6.3f  %6.3f |\n\t| %6.3f  %6.3f |\n", tag, es.a[0][0],es.a[0][1],es.a[1][0],es.a[1][1]);
	printf("l1 = %6.3f\nv1 = | %6.3f  %6.3f |\nu1 = | %6.3f  %6.3f |\n",es.l1,es.v1[0],es.v1[1],es.u1[0],es.u1[1]);
	printf("l2 = %6.3f\nv2 = | %6.3f  %6.3f |\nu2 = | %6.3f  %6.3f |\n",es.l2,es.v2[0],es.v2[1],es.u2[0],es.u2[1]);
	printf("check = %f\n",check_eigs(es));
}
