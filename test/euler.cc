
// example of problem with Euler angles for composition of rotations

#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <vector>

#include <VecCore>

using namespace vecCore;

/* Geant4 code for calculating Euler angles from rotation matrix */

#ifdef GEANT4_CODE

G4ThreeVector G4GDMLWriteDefine::GetAngles(const G4RotationMatrix& mat)
{
   G4double x,y,z;

   const G4double cosb = std::sqrt(mat.xx()*mat.xx()+mat.yx()*mat.yx());

   if (cosb > kRelativePrecision)
   {
      x = std::atan2(mat.zy(),mat.zz());
      y = std::atan2(-mat.zx(),cosb);
      z = std::atan2(mat.yx(),mat.xx());
   }
   else
   {
      x = std::atan2(-mat.yz(),mat.yy());
      y = std::atan2(-mat.zx(),cosb);
      z = 0.0;
   }

   return G4ThreeVector(x,y,z);
}

#endif

/* Copy of Geant4 code using current classes */

template <typename T> Vector3D<T> GetEulerAngles(const Matrix3x3<T>& M)
{
	T alpha, beta, gamma; /* Euler angles */

	const T cosb = math::Sqrt(M[0][0]*M[0][0] + M[1][0]*M[1][0]);

	if (cosb > 1e-8) {
		alpha = math::Atan2( M[2][1], M[2][2]);
		beta  = math::Atan2(-M[2][0], cosb);
		gamma = math::Atan2( M[1][0], M[0][0]);
	} else {
		alpha = math::Atan2(-M[1][2], M[1][1]);
		beta  = math::Atan2(-M[2][0], cosb);
		gamma = 0.0;
	}

	return Vector3D<T>(alpha, beta, gamma);
}

template <typename T> Matrix3x3<T> GetRotationMatrix(const Vector3D<T>& EulerAngles)
{
	Vector3D<T> xaxis(1,0,0), yaxis(0,1,0), zaxis(0,0,1);

	Matrix3x3<T> Rx(xaxis, EulerAngles[0]);
	Matrix3x3<T> Ry(yaxis, EulerAngles[1]);
	Matrix3x3<T> Rz(zaxis, EulerAngles[2]);
	Matrix3x3<T> R = Rz * Ry * Rx;
	return R;
}

template <typename T> T Norm(const Matrix3x3<T>& M)
{
	return math::Sqrt(M[0][0]*M[0][0] + M[0][1]*M[0][1] + M[0][2]*M[0][2] +
	                  M[1][0]*M[1][0] + M[1][1]*M[1][1] + M[1][2]*M[1][2] +
	                  M[2][0]*M[2][0] + M[2][1]*M[2][1] + M[2][2]*M[2][2]);
}

template <class Backend>
void test_euler_angles(float a, float b, float c, bool check = false) {
	typedef typename Backend::Real_t Real_t;

	Real_t alpha = a, beta = b, gamma = c;

	Vector3D<Real_t> euler_angles(alpha, beta, gamma);
	Matrix3x3<Real_t> M = GetRotationMatrix(euler_angles);

	Vector3D<Real_t> recovered_angles = GetEulerAngles(M);
	Matrix3x3<Real_t> R = GetRotationMatrix(recovered_angles);

	Matrix3x3<Real_t> D = M - R;

	if ((!check) || Norm(D) > 1) {
	printf("    Euler angles: %f, %f, %f\n\n", alpha, beta, gamma);

	printf("Recovered angles: %f, %f, %f\n\n",
			recovered_angles[0], recovered_angles[1], recovered_angles[2]);

	printf("M = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			M[0][0], M[0][1], M[0][2],
			M[1][0], M[1][1], M[1][2],
			M[2][0], M[2][1], M[2][2]);

	M = GetRotationMatrix(recovered_angles);


	printf("R = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			M[0][0], M[0][1], M[0][2],
			M[1][0], M[1][1], M[1][2],
			M[2][0], M[2][1], M[2][2]);

	printf("D = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			D[0][0], D[0][1], D[0][2],
			D[1][0], D[1][1], D[1][2],
			D[2][0], D[2][1], D[2][2]);
	}

	return;
}

template <class Backend>
void test_matrix_to_euler(Matrix3x3<typename Backend::Real_t> M) {
	typedef typename Backend::Real_t Real_t;

	Vector3D<Real_t> recovered_angles = GetEulerAngles(M);
	Matrix3x3<Real_t> R = GetRotationMatrix(recovered_angles);

	Matrix3x3<Real_t> D = M - R;

	printf("Recovered Euler angles: %f, %f, %f\n\n",
			recovered_angles[0], recovered_angles[1], recovered_angles[2]);

	printf("M = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			M[0][0], M[0][1], M[0][2],
			M[1][0], M[1][1], M[1][2],
			M[2][0], M[2][1], M[2][2]);

	printf("R = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			R[0][0], R[0][1], R[0][2],
			R[1][0], R[1][1], R[1][2],
			R[2][0], R[2][1], R[2][2]);

	printf("D = [ % f, % f, % f ],\n    [ % f, % f, % f ],\n    [ % f, % f, % f ]\n\n",
			D[0][0], D[0][1], D[0][2],
			D[1][0], D[1][1], D[1][2],
			D[2][0], D[2][1], D[2][2]);
	return;
}

int main(int argc, char *argv[])
{
	printf("Euler Angles Test\n");

    printf("Convert angle to matrices to angles, no problem is seen\n\n");

	float a = M_PI/180.0 * atof(argv[1]);
	float b = M_PI/180.0 * atof(argv[2]);
	float c = M_PI/180.0 * atof(argv[3]);

	test_euler_angles<backend::Scalar<float> >(a,b,c);

    printf("Convert matrix (with small 10^-6 perturbations) to angles to matrix -> problems\n\n");

	double m10 = sin(b)*cos(c) - cos(b)*sin(c);
	double m11 = sin(b)*sin(c) + cos(b)*cos(c);
	double m20 = cos(b)*cos(c) + sin(b)*sin(c);
	double m21 = cos(b)*sin(c) - sin(b)*cos(c);

	Matrix3x3<double> M(0, 0, 1, m10, m11, 0, m20, m21, 0);

	Matrix3x3<double> RND(drand48(), drand48(), drand48(),
                          drand48(), drand48(), drand48(),
                          drand48(), drand48(), drand48());

	/* add small perturbation (could come from rounding errors) */

	M = M + 1e-6*RND;

	test_matrix_to_euler<backend::Scalar<double> >(M);

	return 0;
}
