#include <cstdio>

#include <VecCore>

template <class Backend>
void backend_syntax_example()
{
    typedef typename Backend::Real_t Real_t;
    typedef typename Backend::Real_v Real_v;
    typedef typename Backend::Real_v::Mask Mask_t;

    Mask_t m_true(true), m_false(false);

    assert( m_true.isFull()  == true);
    assert(m_false.isEmpty() == true);

	// Note: Masks and associated types have same size

    printf("Real_v::Size == %d\n\n", Real_v::Size);

    Real_v x(0.0);

    for (int i = 0; i < Real_v::Size; i++)
	    x[i] = i;

    Mask_t mask = x >= Real_v(Real_v::Size/2.0);

	// need pointer because cannot print x[i] with Vc...

    Real_t *xptr = (Real_t*)(&x);

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %.1f, mask[%d] = x[%i] >= %.2f == %s\n",
	        i, xptr[i], i, i, Real_v::Size/2.0, mask[i] ? "true" : "false");

    printf("\nafter x(mask) = -1;\n\n");

	x(mask) = Real_v(-1.0);

    for (int i = 0; i < Real_v::Size; i++)
	    printf("x[%d] = %.1f, mask[%d] = x[%i] >= %.2f == %s\n",
	        i, xptr[i], i, i, Real_v::Size/2.0, mask[i] ? "true" : "false");
}

int main(int argc, char *argv[])
{
    printf("Scalar Backend (float):\n\n");
    backend_syntax_example<VecCore::Backend::Scalar<float>>();
    printf("\n\n");

    printf("Scalar Backend (double):\n\n");
    backend_syntax_example<VecCore::Backend::Scalar<double>>();
    printf("\n\n");

    printf("Vector Backend (float):\n\n");
    backend_syntax_example<VecCore::Backend::Vector<float>>();
    printf("\n\n");

    printf("Vector Backend (double):\n\n");
    backend_syntax_example<VecCore::Backend::Vector<double>>();

    return 0;
}
