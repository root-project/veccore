//
// Geometry Test (Vector, Point, Quaternion, Matrix)
//

#undef NDEBUG
#include <cassert>
#include <cstdio>

#include <VecCore>

using namespace VecCore::Math;

template <class Backend> void test() {
  typedef typename Backend::Real_t Real_t;
  typedef typename Backend::Real_v Real_v;
  typedef typename Backend::Real_v::Mask Mask_t;

  // test points and vectors (scalar test)

  // constructors

  Point3D<Real_t> px(Real_t(1.0), Real_t(0.0), Real_t(0.0));
  Point3D<Real_t> py(Real_t(0.0), Real_t(1.0), Real_t(0.0));
  Point3D<Real_t> pz(Real_t(0.0), Real_t(0.0), Real_t(1.0));

  Vector3D<Real_t> vx(Real_t(1.0), Real_t(0.0), Real_t(0.0));
  Vector3D<Real_t> vy(Real_t(0.0), Real_t(1.0), Real_t(0.0));
  Vector3D<Real_t> vz(Real_t(0.0), Real_t(0.0), Real_t(1.0));

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

  // test points and vectors (vector test)

  // constructors

  Point3D<Real_v> px_v(Real_v(1.0), Real_v(0.0), Real_v(0.0));
  Point3D<Real_v> py_v(Real_v(0.0), Real_v(1.0), Real_v(0.0));
  Point3D<Real_v> pz_v(Real_v(0.0), Real_v(0.0), Real_v(1.0));

  Vector3D<Real_v> vx_v(Real_v(1.0), Real_v(0.0), Real_v(0.0));
  Vector3D<Real_v> vy_v(Real_v(0.0), Real_v(1.0), Real_v(0.0));
  Vector3D<Real_v> vz_v(Real_v(0.0), Real_v(0.0), Real_v(1.0));

  Mask_t m(false);

  m = abs(Dot(px_v, px_v) - Real_v(1.0)) < Real_v(1e-6);

  assert(m.isFull());

  // test quaternion

  Quaternion<Real_t> qi(1.0, 0.0, 0.0, 0.0);
  Quaternion<Real_t> qj(0.0, 1.0, 0.0, 0.0);
  Quaternion<Real_t> qk(0.0, 0.0, 1.0, 0.0);

  Quaternion<Real_t> q1, q2, q3, q4, q5;

#if 1
#define PRINT(name, q) printf("%s [% .2f, % .2f, % .2f, % .2g]\n", #name, q[0], q[1], q[2], q[3])

    q1 = Cross(qi, qj); PRINT(ij, q1);
    q5 = Cross(qj, qi); PRINT(ji, q5);
    q2 = Cross(qj, qk); PRINT(jk, q2);
    q3 = Cross(qk, qi); PRINT(ki, q3);
    q4 = Cross(qk, qk); PRINT(kk, q4);

#undef PRINT
#endif

  Real_t angle = 30.0 * 3.1415926535 / 180.0;

  Vector3D<Real_t> axis(Real_t(0.0), Real_t(0.0), Real_t(1.0));

  Quaternion<Real_t> orientation(axis, angle);

  Point3D<Real_t> origin(Real_t(0.0), Real_t(0.0), Real_t(5.0));

  Transform<Real_t, Quaternion> T(origin, orientation, 0.5);
  Transform<Real_t, Quaternion> invT = Inverse(T);

  Point3D<Real_t>  test_p = px + py + pz;
  Vector3D<Real_t> test_v = vx + vy + vz;

#if 1
#define PRINT(name, q) printf("%s [% .4f, % .4f, % .4f]\n", #name, q[0], q[1], q[2])

    PRINT(test_p, test_p);
    PRINT(test_v, test_v);

    test_p = orientation * test_p; //Point3D<Real_t>(q[0], q[1], q[2]);
    PRINT(test_p, test_p);
    test_v = T(test_v); PRINT(test_v, test_v);

    test_p = T(test_p); PRINT(test_p, test_p);
    test_v = T(test_v); PRINT(test_v, test_v);

    test_p = invT(T(px+py+pz)); PRINT(test_p, test_p);
    test_v = invT(T(vx+vy+vz)); PRINT(test_v, test_v);

#undef PRINT
#endif


}

void test_all() {
  printf("test (scalar, float):\n");
  test<VecCore::Backend::Scalar<float>>();
  printf("\n");
  printf("test (scalar, double):\n");
  test<VecCore::Backend::Scalar<double>>();
  printf("\n");
  printf("test (vector, float):\n");
  test<VecCore::Backend::Vector<float>>();
  printf("\n");
  printf("test (vector, double):\n");
  test<VecCore::Backend::Vector<double>>();
}

int main(int argc, char *argv[]) {
  test_all();
  return 0;
}
