#include <cstdio>

#include <VecCore>

template <class Backend>
void backend_syntax_example() {
  typedef typename Backend::Real_t Real_t;
  typedef typename Backend::Real_v Real_v;
  typedef typename Backend::Real_v::Mask Mask_t;

  Mask_t m_true(true), m_false(false);

  assert(m_true.isFull() == true);
  assert(m_false.isEmpty() == true);

  // Note: Masks and associated types have same size

  printf("Real_v::Size == %d\n\n", (int)Real_v::Size);

  Real_v x(0.0);

  for (size_t i = 0; i < Real_v::Size; i++)
    x[i] = i;

  Mask_t mask = x >= Real_v(Real_v::Size / 2.0);

  // need pointer because cannot print x[i] with Vc...

  Real_t *xptr = (Real_t *)(&x);

  for (size_t i = 0; i < Real_v::Size; i++)
    printf("x[%lu] = %.1f, mask[%lu] = x[%lu] >= %.2f == %s\n",
            i, xptr[i], i, i, (int)Real_v::Size / 2.0, mask[i] ? "true" : "false");

  printf("\nafter x(mask) = -1;\n\n");

  x(mask) = Real_v(-1.0);

  for (size_t i = 0; i < Real_v::Size; i++)
    printf("x[%lu] = %.1f, mask[%lu] = x[%lu] >= %.2f == %s\n",
            i, xptr[i], i, i, (int)Real_v::Size / 2.0, mask[i] ? "true" : "false");
}

int main(int argc, char *argv[]) {
  printf("Scalar Backend (float):\n\n");
  backend_syntax_example<vecCore::backend::Scalar<float>>();
  printf("\n\n");

  printf("Scalar Backend (double):\n\n");
  backend_syntax_example<vecCore::backend::Scalar<double>>();
  printf("\n\n");

  printf("Vector Backend (float):\n\n");
  backend_syntax_example<vecCore::backend::Vector<float>>();
  printf("\n\n");

  printf("Vector Backend (double):\n\n");
  backend_syntax_example<vecCore::backend::Vector<double>>();

  return 0;
}
