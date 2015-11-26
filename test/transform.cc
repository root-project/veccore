//
// Transform3D test
//

#undef NDEBUG
#define DEBUG

#include <cassert>

#include <cstdio>

#include <VecCore>

using namespace vecCore;

template <class Backend> void test() {
  typedef typename Backend::Real_t Real_t;
  typedef typename Backend::Real_v Real_v;

  Point3D<Real_t> px(Real_t(1.0), Real_t(0.0), Real_t(0.0));
  Point3D<Real_t> py(Real_t(0.0), Real_t(1.0), Real_t(0.0));
  Point3D<Real_t> pz(Real_t(0.0), Real_t(0.0), Real_t(1.0));

  Vector3D<Real_t> vx(Real_t(1.0), Real_t(0.0), Real_t(0.0));
  Vector3D<Real_t> vy(Real_t(0.0), Real_t(1.0), Real_t(0.0));
  Vector3D<Real_t> vz(Real_t(0.0), Real_t(0.0), Real_t(1.0));

  Point3D<Real_v> px_v(Real_v(1.0), Real_v(0.0), Real_v(0.0));
  Point3D<Real_v> py_v(Real_v(0.0), Real_v(1.0), Real_v(0.0));
  Point3D<Real_v> pz_v(Real_v(0.0), Real_v(0.0), Real_v(1.0));

  Vector3D<Real_v> vx_v(Real_v(1.0), Real_v(0.0), Real_v(0.0));
  Vector3D<Real_v> vy_v(Real_v(0.0), Real_v(1.0), Real_v(0.0));
  Vector3D<Real_v> vz_v(Real_v(0.0), Real_v(0.0), Real_v(1.0));

  Quaternion<Real_t> qi(1.0, 0.0, 0.0, 0.0);
  Quaternion<Real_t> qj(0.0, 1.0, 0.0, 0.0);
  Quaternion<Real_t> qk(0.0, 0.0, 1.0, 0.0);
  Quaternion<Real_t> q1(0.0, 0.0, 0.0, 1.0); // identity

  // define an angle to rotate

  Real_t angle = 30.0 * 3.1415926535 / 180.0;

  // define an axis of rotation

  Vector3D<Real_t> axis = vz;

  // create the corresponding quaternion

  Quaternion<Real_t> orientation(axis, angle);

  // define the origin of the affine transform

  Point3D<Real_t> origin = Real_t(5.0) * pz;

  // define a scaling factor
  
  Real_t scaling = 0.5;

  // create a coordinate transformation using a matrix

  Transform3D<Real_t, Matrix3x3> Tm(origin, orientation, scaling);

  // create a coordinate transformation using a quaternion

  Transform3D<Real_t, Quaternion> Tq(origin, orientation, scaling);

  Transform3D<Real_t, Matrix3x3> invTm = Inverse(Tm);
  Transform3D<Real_t, Quaternion> invTq = Inverse(Tq);

  Point3D<Real_t>  test_p = px + py + pz;
  Vector3D<Real_t> test_v = vx + vy + vz;

  // create vector versions of the transformations

  Point3D<Real_v> origin_v = Real_v(5.0) * pz_v;
  Quaternion<Real_v> orientation_v(vz_v, Real_v(angle));
  Real_v scaling_v = Real_v(0.5);

  Transform3D<Real_v, Matrix3x3> Tm_v(origin_v, orientation_v, scaling_v);
  Transform3D<Real_v, Quaternion> Tq_v(origin_v, orientation_v, scaling_v); 

  Transform3D<Real_v, Matrix3x3> invTm_v = Inverse(Tm_v);
  Transform3D<Real_v, Quaternion> invTq_v = Inverse(Tq_v);

  // test that forward and inverse transformations work

  Point3D<Real_v>  test_v_p = px_v + py_v + pz_v;
  Vector3D<Real_v> test_v_v = vx_v + vy_v + vz_v;

  assert(IsFull(Norm(invTm(Tm(test_p)) - test_p) < Real_t(1.0e-3)));
  assert(IsFull(Norm(invTm(Tm(test_v)) - test_v) < Real_t(1.0e-3)));
  assert(IsFull(Norm(invTq(Tq(test_p)) - test_p) < Real_t(1.0e-3)));
  assert(IsFull(Norm(invTq(Tq(test_v)) - test_v) < Real_t(1.0e-3)));
  assert(IsFull(Norm(invTm_v(Tm_v(test_v_p)) - test_v_p) < Real_v(1.0e-3)));
  assert(IsFull(Norm(invTm_v(Tm_v(test_v_v)) - test_v_v) < Real_v(1.0e-3)));
  assert(IsFull(Norm(invTq_v(Tq_v(test_v_p)) - test_v_p) < Real_v(1.0e-3)));
  assert(IsFull(Norm(invTq_v(Tq_v(test_v_v)) - test_v_v) < Real_v(1.0e-3)));
}

void test_all() {
  printf("test (scalar, float):\n");
  test<vecCore::backend::Scalar<float>>();
  printf("\n");
  printf("test (scalar, double):\n");
  test<vecCore::backend::Scalar<double>>();
  printf("\n");
  printf("test (vector, float):\n");
  test<vecCore::backend::Vector<float>>();
  printf("\n");
  printf("test (vector, double):\n");
  test<vecCore::backend::Vector<double>>();
}

int main(int argc, char *argv[]) {
  test_all();
  return 0;
}
