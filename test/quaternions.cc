#include <cstdio>
#include <cstdlib>
#include <vector>

#include <VecCore>
#include <Timer.h>

using namespace vecCore;

const int N = 1024*1024;

// performance benchmark: matrix vs quaternion transformations

template <class Backend, template <typename> class Rotation>
void transformation_benchmark() {
	typedef typename Backend::Real_t Real_t;

	const Real_t L = 1.0;

	// generate random points

	std::vector<Point3D<Real_t> > points;

	srand48(1); // use the same seed

	for (int i = 0; i < N; i++) {
		Real_t x, y, z;

		x = (Real_t) (L * (drand48() - 0.5));
		y = (Real_t) (L * (drand48() - 0.5));
		z = (Real_t) (L * (drand48() - 0.5));

		points.push_back(Point3D<Real_t>(x, y, z));
	}

	// transform all points with the transformation

	Point3D<Real_t> origin;
	Vector3D<Real_t> axis;
	Real_t angle;

	origin[0] = (Real_t) (drand48() - 0.5);
	origin[1] = (Real_t) (drand48() - 0.5);
	origin[2] = (Real_t) (drand48() - 0.5);

	axis[0] = (Real_t) (drand48() - 0.5);
	axis[1] = (Real_t) (drand48() - 0.5);
	axis[2] = (Real_t) (drand48() - 0.5);

	angle = (Real_t) M_PI * drand48();

	axis.Normalize();

	Transform3D<Real_t, Rotation> T(origin, Quaternion<Real_t>(axis, angle));

	/* warm up cache */
	Point3D<Real_t> p(0,0,0);

	for (int j = 0; j < 1024; j++) {
		for (int i = 0; i < N; i++) {
			p = p + T(points[i]);
		}
	}

	Timer<microseconds> timer;

	timer.Start();

	for (int i = 0; i < N; i++) {
		origin = origin + T(points[i]);
	}

	Real_t t = timer.Elapsed();
	fprintf(stdout, "t = %.3f ms, result: (%f, %f, %f)\n", t/1000.0, origin[0], origin[1], origin[2]);
	return;
}

int main(int argc, char *argv[])
{
	printf("Single precision:\n\n");

	printf("Matrix3x3:  ");
	transformation_benchmark<backend::Scalar<float>, Matrix3x3>();

	printf("Quaternion: ");
	transformation_benchmark<backend::Scalar<float>, Quaternion>();

	printf("Double precision:\n\n");

	printf("Matrix3x3:  ");
	transformation_benchmark<backend::Scalar<double>, Matrix3x3>();

	printf("Quaternion: ");
	transformation_benchmark<backend::Scalar<double>, Quaternion>();
	return 0;
}
