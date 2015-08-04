//
// Geometry Test (Vector, Point, Quaternion, Matrix)
//

#undef NDEBUG
#define DEBUG

#include <cassert>

#include <cstdio>

#include "math/Point3D.h"
#include "math/Vector3D.h"
#include "math/Matrix3x3.h"
#include "math/Quaternion.h"
#include "math/Transform.h"

#include "backend/scalar.h"
#include "backend/vector.h"

using namespace VecCore::Math;

template <class Backend> void test()
{
    typedef typename Backend::Real_t Real_t;
    typedef typename Backend::Real_v Real_v;
    typedef typename Backend::Real_v::Mask Mask_t;

    // test points and vectors (scalar test)
    
    // constructors

    Point3D<Real_t>  px(1.0, 0.0, 0.0);
    Point3D<Real_t>  py(0.0, 1.0, 0.0);
    Point3D<Real_t>  pz(0.0, 0.0, 1.0);

    Vector3D<Real_t> vx(1.0, 0.0, 0.0);
    Vector3D<Real_t> vy(0.0, 1.0, 0.0);
    Vector3D<Real_t> vz(0.0, 0.0, 1.0);

    Point3D<Real_t>  tmp1 = Real_t(3.0) * px + (py / Real_t(2.0)) - vz;
    Vector3D<Real_t> tmp2 = Real_t(3.0) * px + (py / Real_t(2.0)) - pz;

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
    assert(abs(Norm<Real_t, 3>(Point3D<Real_t>(1.0, 2.0, 3.0)) - pow(36.0,1.0/3.0)) < 1.0e-6);

    // test points and vectors (vector test)

    // constructors

    Point3D<Real_v>  px_v(1.0, 0.0, 0.0);
    Point3D<Real_v>  py_v(0.0, 1.0, 0.0);
    Point3D<Real_v>  pz_v(0.0, 0.0, 1.0);

    Vector3D<Real_v> vx_v(1.0, 0.0, 0.0);
    Vector3D<Real_v> vy_v(0.0, 1.0, 0.0);
    Vector3D<Real_v> vz_v(0.0, 0.0, 1.0);

    Mask_t m(false);
    
    m = abs(Dot(px, px) - 1.0) < 1e-6;
    
    assert(m.isFull());
}

void test_all()
{
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

int main(int argc, char *argv[])
{
    test_all();
    return 0;
}
