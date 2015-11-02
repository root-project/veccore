#undef NDEBUG
#include <cassert>
#include <cstdio>

#include <VecCore>

using namespace vecCore;

template <class Backend> void test() {
  typedef typename Backend::Real_t Real_t;
  typedef typename Backend::Real_t Real_t;

  // test points and vectors (scalar test)

  // constructors

  Point3D<Real_t> px(1.0, 0.0, 0.0);
  Point3D<Real_t> py(0.0, 1.0, 0.0);
  Point3D<Real_t> pz(0.0, 0.0, 1.0);

  Vector3D<Real_t> vx(1.0, 0.0, 0.0);
  Vector3D<Real_t> vy(0.0, 1.0, 0.0);
  Vector3D<Real_t> vz(0.0, 0.0, 1.0);

  // dot and cross products

  assert(abs(Dot(px, px) - 1.0) < 1e-6);
  assert(abs(Dot(py, py) - 1.0) < 1e-6);
  assert(abs(Dot(pz, pz) - 1.0) < 1e-6);

  assert(abs(Dot(px, py) - 0.0) < 1e-6);
  assert(abs(Dot(py, pz) - 0.0) < 1e-6);
  assert(abs(Dot(pz, px) - 0.0) < 1e-6);

  assert(abs(Dot(vx, vx) - 1.0) < 1e-6);
  assert(abs(Dot(vy, vy) - 1.0) < 1e-6);
  assert(abs(Dot(vz, vz) - 1.0) < 1e-6);

  assert(abs(Dot(vx, vy) - 0.0) < 1e-6);
  assert(abs(Dot(vy, vz) - 0.0) < 1e-6);
  assert(abs(Dot(vz, vx) - 0.0) < 1e-6);

  assert(abs(Norm(Cross(vx, vy) - vz)) < 1e-6);
  assert(abs(Norm(Cross(vz, vx) - vy)) < 1e-6);
  assert(abs(Norm(Cross(vy, vz) - vx)) < 1e-6);

  assert(abs(Norm<Real_t, 2>(Point3D<Real_t>(1.0, 2.0, 3.0)) - 14.0) < 1.0e-6);
  assert(abs(Norm<Real_t, 3>(Point3D<Real_t>(1.0, 2.0, 3.0)) - pow(36.0, 1.0 / 3.0)) < 1.0e-6);

  // test quaternion

  Quaternion<Real_t> qi(1.0, 0.0, 0.0, 0.0);
  Quaternion<Real_t> qj(0.0, 1.0, 0.0, 0.0);
  Quaternion<Real_t> qk(0.0, 0.0, 1.0, 0.0);

  Quaternion<Real_t> q1, q2, q3, q4, q5;

  Real_t angle = 45.0;

  Vector3D<Real_t> axis(0.0, 0.0, 1.0);

  Quaternion<Real_t> orientation(axis, angle * Real_t(3.1415926535 / 180.0));

  Point3D<Real_t> origin(0.0, 2.0, 5.0);

  Real_t scaling = 0.25;

  Transform3D<Real_t, Quaternion> T(origin, orientation, scaling);
  Transform3D<Real_t, Quaternion> invT = Inverse(T);

  Point3D<Real_t>  test_p = px + py + pz;
  Vector3D<Real_t> test_v = vx + vy + vz;

  printf("\nTransform3D: origin = (%f, %f, %f),\n"
         "            axis  = (%f, %f, %f),\n"
		 "            angle = %f, scaling = %f\n\n",
	origin[0], origin[1], origin[2], axis[0], axis[1], axis[2], angle, scaling);

  printf("original point:    (% 6.3f, % 6.3f, % 6.3f)\n", test_p[0], test_p[1], test_p[2]);

  test_p = T(test_p);

  printf("transformed point: (% 6.3f, % 6.3f, % 6.3f)\n", test_p[0], test_p[1], test_p[2]);

  test_p = invT(test_p);

  printf("transformed back:  (% 6.3f, % 6.3f, % 6.3f)\n", test_p[0], test_p[1], test_p[2]);

  printf("\n\n");

  printf("original vector:    (% 6.3f, % 6.3f, % 6.3f)\n", test_v[0], test_v[1], test_v[2]);

  test_v = T(test_v);

  printf("transformed vector: (% 6.3f, % 6.3f, % 6.3f)\n", test_v[0], test_v[1], test_v[2]);

  test_v = invT(test_v);

  printf("transformed back:   (% 6.3f, % 6.3f, % 6.3f)\n", test_v[0], test_v[1], test_v[2]);
}

int main(int argc, char *argv[]) {
  printf("\ntest (float):\n\n");
  test<vecCore::backend::Scalar<float> >();

  printf("\ntest (double):\n\n");
  test<vecCore::backend::Scalar<double> >();
  return 0;
}
