/// \file VecCore/VariableSizeObj.h
/// \author Philippe Canal (pcanal@fnal.gov)

#ifndef VECCORE_VARIABLESIZEOBJ_H
#define VECCORE_VARIABLESIZEOBJ_H

// This file will eventually move in VecCore.
#include "base/Global.h"

// For memset and memcpy
#include <string.h>

class TRootIOCtor;

namespace VecCore {

/**
 * @brief   Array/vector of items with all information stored contiguously in memory.
 * @details Resizing operations require explicit new creation and copy.
 *    This is decomposed in the content part (VariableSizeObj) and the interface
 *    part (VariableSizeObjectInterface) so that the variable size part can be placed
 *    at the end of the object.
 */

   template <typename V> class VariableSizeObj {
   public:
      unsigned int  fN; // Number of elements.
      V            *fValues; //[fN] array of values
      V             fRealArray[1]; //! Beginning address of the array values.

      VariableSizeObj(TRootIOCtor *) : fN(0), fValues(0) {}

      VariableSizeObj(unsigned int nvalues) : fN(nvalues), fValues(&fRealArray[1]) {}

      VariableSizeObj(const VariableSizeObj &other) : fN(other.fN), fValues(&fRealArray[1]) {
         if (other.fN) memcpy(fValues, other.fValues, (other.fN)*sizeof(V));
      }

      VariableSizeObj(size_t new_size, const VariableSizeObj &other) : fN(new_size), fValues(&fRealArray[1]) {
         if (other.fN) memcpy(fValues, other.fValues, (other.fN)*sizeof(V));
      }
   };

   template <typename Cont, typename V> class VariableSizeObjectInterface {
   private:
      bool          fSelfAlloc : 1; // Record whether the memory is externally managed.

   public:
      VariableSizeObjectInterface() : fSelfAlloc(false) {}

      // The static maker to be used to create an instance of the variable size object.

      template <typename... T>
      static  Cont *MakeInstance(size_t nvalues, const T&... params) {
         // Make an instance of the class which allocates the node array. To be
         // released using ReleaseInstance. If addr is non-zero, the user promised that
         // addr contains at least that many bytes:  size_t needed = SizeOf(nvalues);

         size_t needed = SizeOf(nvalues);
         char *ptr = new char[ needed ];
         if (!ptr) return 0;
         Cont *obj = new (ptr) Cont(nvalues, params...);
         obj->fSelfAlloc = true;
         return obj;
      }

      template <typename... T>
      static  Cont *MakeInstanceAt(size_t nvalues, void *addr, const T&... params) {
         // Make an instance of the class which allocates the node array. To be
         // released using ReleaseInstance. If addr is non-zero, the user promised that
         // addr contains at least that many bytes:  size_t needed = SizeOf(nvalues);
         if (!addr) {
            return MakeInstance(nvalues, params...);
         } else {
            Cont *obj = new (addr) Cont(nvalues, params...);
            obj->fSelfAlloc = false;
            return obj;
         }
      }

      // The equivalent of the copy constructor
      static Cont *MakeCopy(const Cont &other)
      {
         // Make a copy of a the variable size array and its container.

         size_t needed = SizeOf(other.fData.fN);
         char *ptr = new char[ needed ];
         if (!ptr) return 0;
         Cont *copy = new (ptr) Cont(other);
         copy->fSelfAlloc = true;
         return copy;
      }

      // The equivalent of the copy constructor
      static Cont *MakeCopyAt(const Cont &other, void *addr)
      {
         // Make a copy of a the variable size array and its container at the location (if indicated)
         if (addr) {
            Cont *copy = new (addr) Cont(other);
            copy->fSelfAlloc = false;
            return copy;
         } else {
            return MakeCopy(other);
         }
      }

      static Cont *MakeCopy(size_t new_size, const Cont &other)
      {
         // Make a copy of a the variable size array and its container with
         // a new_size of the content.

         size_t needed = SizeOf(new_size);
         char *ptr = new char[ needed ];
         if (!ptr) return 0;
         Cont *copy = new (ptr) Cont(new_size,other);
         copy->fSelfAlloc = true;
         return copy;
      }

      // The equivalent of the destructor
      static void    ReleaseInstance(Cont *obj)
      {
         // Releases the space allocated for the object
         obj->~Cont();
         if (obj->fSelfAlloc) delete [] (char*)obj;
      }

      // Equivalent of sizeof function
      static size_t SizeOf(size_t nvalues)
      { return (sizeof(Cont)+sizeof(V)*(nvalues-1)); }

   };

}

#endif
