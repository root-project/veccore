#include <cstdio>
#include <cstdlib>

#include <VecCore>
#include <Timer.h>

using namespace veccore;

// performance benchmark: matrix vs quaternion transformations

template <class Backend, template <typename> class Rotation>
void transformation_benchmark() {
	typedef typename Backend::Real_t Real_t;

	const Real_t L = 1.0;

	// generate random transformations

	std::vector<Rotation<Real_t> > R;

	srand48(1); // use the same seed

	for (int i = 0; i < 1024*1024; i++) {
		Real_t x, y, z, w, s;

		x = (Real_t) (L * (drand48() - 0.5));
		y = (Real_t) (L * (drand48() - 0.5));
		z = (Real_t) (L * (drand48() - 0.5));
		w = (Real_t) (L * (drand48() - 0.5));
		s = math::Sqrt(x*x + y*y + z*z + w*w);

		x /= s; y /= s; z /= s; w /= s;

		Rotation<Real_t> rot(Quaternion<Real_t>(x, y, z, w));

		R.push_back(rot);
	}

	// compose all rotations by multiplying them

	Rotation<Real_t> R0(Quaternion<Real_t>(0,0,0,1)); // init with identity

	Timer<microseconds> timer;

	timer.Start();

	for (int i = 0; i < 1024*1024; i++) {
		R0.Compose(R[i]);
	}

	Point3D<Real_t> origin(0.0, 1.0, 0.0);

	origin = R0 * origin;

	Real_t t = timer.Elapsed();
	fprintf(stdout, "t = %.3f ms, result: (%f, %f, %f)\n", t/1000.0, origin[0], origin[1], origin[2]);
	return;
}

int main(int argc, char *argv[])
{
	printf("Matrix3x3:  ");
	transformation_benchmark<backend::Scalar<double>, Matrix3x3>();

	printf("Quaternion: ");
	transformation_benchmark<backend::Scalar<double>, Quaternion>();

	return 0;
}
