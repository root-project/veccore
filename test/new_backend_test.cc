//
// New backend tests
//

#undef NDEBUG
#include <cassert>

#include "backend/scalar.h"
#include "backend/vector.h"

#include <cstdio>

template <class Backend> void test_backend()
{
    typedef typename Backend::Real_t Real_t;
    typedef typename Backend::Real_v Real_v;
    typedef typename Backend::Real_v::Mask Mask_t;

    Mask_t m_true(true), m_false(false);

    assert( m_true.isFull()  == true);
    assert(m_false.isEmpty() == true);

    printf("Real_v::Size       == %d\n\n", Real_v::Size);
    printf("Real_v::Mask::Size == %d\n\n", Real_v::Size);

    Real_v x(18.0);

    for (int i = 0; i < Real_v::Size; i++)
	    x[i] = i*i;

    x = cos(x);

    Mask_t mask = x >= Real_v(2.0);

    Real_t *xptr = (Real_t*)(&x);

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %f, x[i] >= 2.0 == %s\n",
	        i, xptr[i], mask[i] ? "true" : "false");

    for (int i = 0; i < Real_v::Size; i++) x[i] = i*i*i;

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %f, x[i] >= 2.0 == %s\n",
	        i, xptr[i], mask[i] ? "true" : "false");

    mask = x < Real_v(4.0);

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %f, x[i] < 4.0 == %s\n",
	        i, xptr[i], mask[i] ? "true" : "false");

    x(mask) = Real_v(17.0);

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %f, mask[i] == %s\n",
	        i, xptr[i], mask[i] ? "true" : "false");
}

int main(int argc, char *argv[])
{

    printf("Scalar Backend Test:\n\n");
    test_backend<VecCore::Backend::Scalar<double>>();

    printf("\n\n");

    printf("Vector Backend Test:\n\n");
    test_backend<VecCore::Backend::Vector<double>>();

    return 0;
}
