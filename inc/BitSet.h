/// \file VecCore/BitSet.h
/// \author Philippe Canal (pcanal@fnal.gov)
/// \author Exported from the original verison in ROOT (root.cern.ch).

#ifndef VECCORE_BITSET_H
#define VECCORE_BITSET_H

// This file will eventually move in VecCore.
#include "base/Global.h"

#include "VariableSizeObj.h"

// For memset and memcpy
#include <string.h>

// For operator new with placement
#include <new>

//
// Fast bitset operations.
//

class TRootIOCtor;

// gcc 4.8.2's -Wnon-virtual-dtor is broken and turned on by -Weffc++, we
// need to disable it for SOA3D

#if __GNUC__ < 3 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 8)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Weffc++"
#define GCC_DIAG_POP_NEEDED
#endif

namespace VecCore {

/**
 * @brief   Fast operation on a set of bit, all information stored contiguously in memory.
 * @details Resizing operations require explicit new creation and copy.
 *          
 */

   class BitSet : protected VariableSizeObjectInterface<BitSet, unsigned char> {
   public:
      using Base_t = VariableSizeObjectInterface<BitSet, unsigned char>;

   private:
      friend Base_t;
      using VariableData_t = VariableSizeObj<unsigned char>;

      unsigned int    fNbits; // Highest bit set + 1
      VariableData_t  fData;

      // Required by VariableSizeObjectInterface
      VariableData_t &GetVariableData() { return fData; }

      // Required by VariableSizeObjectInterface
      VariableData_t const &GetVariableData() const { return fData; }

      static inline size_t GetNbytes(size_t nbits) { return  (( (nbits ? nbits : 8) -1)/8) + 1; }

      BitSet(const BitSet &other) : fNbits(other.fNbits), fData(other.fData) {
         // We assume that the memory allocated and use is large enough to
         // hold the full values (i.e. Sizeof(other.fData.fN) )
      }

      BitSet(size_t new_size, const BitSet &other) : fNbits(other.fNbits), fData(new_size, other.fData) {
         // We assume that the memory allocated and use is large enough to
         // hold the full values (i.e. Sizeof(new_size) )
         // If new_size is smaller than the size of 'other' then the copy is trucated.
         // if new_size is greater than the size of 'other' the value are zero initialized.

         if (fNbits*8 > new_size) {
             fNbits = 8*new_size;
         }
         if (new_size > other.fData.fN) {
            memset(fData.GetValues()+other.fData.fN,0,new_size - other.fData.fN);
         }
      }

      BitSet(size_t nvalues, size_t nbits) : fNbits(nbits), fData( nvalues ) {
         memset(fData.GetValues(),0,GetNbytes());
      }

      void DoAndEqual(const BitSet& rhs)
      {
         // Execute (*this) &= rhs;
         // Extra bits in rhs are ignored
         // Missing bits in rhs are assumed to be zero.

         size_t min = (GetNbytes()<rhs.GetNbytes()) ? GetNbytes() : rhs.GetNbytes();
         for(size_t i=0; i<min; ++i) {
            fData[i] &= rhs.fData[i];
         }
         if (GetNbytes()>min) {
            memset(&(fData[min]),0,GetNbytes()-min);
         }
      }

      void DoOrEqual (const BitSet& rhs)
      {
         // Execute (*this) &= rhs;
         // Extra bits in rhs are ignored
         // Missing bits in rhs are assumed to be zero.

         size_t min = (GetNbytes()<rhs.GetNbytes()) ? GetNbytes() : rhs.GetNbytes();
         for(size_t i=0; i<min; ++i) {
            fData[i] |= rhs.fData[i];
         }
      }

      void DoXorEqual(const BitSet& rhs)
      {
         // Execute (*this) ^= rhs;
         // Extra bits in rhs are ignored
         // Missing bits in rhs are assumed to be zero.

         size_t min = (GetNbytes()<rhs.GetNbytes()) ? GetNbytes() : rhs.GetNbytes();
         for(size_t i=0; i<min; ++i) {
            fData[i] ^= rhs.fData[i];
         }
      }

      void DoLeftShift(size_t shift)
      {
         // Execute the left shift operation.

         if (shift==0) return;
         const size_t wordshift = shift / 8;
         const size_t offset = shift % 8;
         if (offset==0) {
            for(size_t n = GetNbytes() - 1; n >= wordshift; --n) {
               fData[n] = fData[ n - wordshift ];
            }
         } else {
            const size_t sub_offset = 8 - offset;
            for(size_t n = GetNbytes() - 1; n > wordshift; --n) {
               fData[n] = (fData[n - wordshift] << offset) |
            (fData[n - wordshift - 1] >> sub_offset);
            }
            fData[wordshift] = fData[0] << offset;
         }
         memset(fData.GetValues(),0,wordshift);
      }

      void DoRightShift(size_t shift)
      {
         // Execute the left shift operation.

         if (shift==0) return;
         const size_t wordshift = shift / 8;
         const size_t offset = shift % 8;
         const size_t limit = GetNbytes() - wordshift - 1;

         if (offset == 0)
            for (size_t n = 0; n <= limit; ++n)
               fData[n] = fData[n + wordshift];
         else
            {
               const size_t sub_offset = 8 - offset;
               for (size_t n = 0; n < limit; ++n)
                  fData[n] = (fData[n + wordshift] >> offset) |
                     (fData[n + wordshift + 1] << sub_offset);
               fData[limit] = fData[GetNbytes()-1] >> offset;
            }

         memset(&(fData[limit + 1]),0, GetNbytes() - limit - 1);
      }

      void DoFlip()
      {
         // Execute ~(*this)

         for(size_t i=0; i<GetNbytes(); ++i) {
            fData[i] = ~fData[i];
         }
         // NOTE: out-of-bounds bit were also flipped!
      }

   public:
      class reference {
         friend class BitSet;

         unsigned char &fBit; //! reference to the data storage
         unsigned char  fPos;  //! position (0 through 7)

         reference() : fBit(fPos),fPos(0) {
            // default constructor, default to all bits looking false.
            // assignment are ignored.
         }

      public:
         reference(unsigned char& bit, unsigned char pos) : fBit(bit),fPos(pos) { }
         ~reference() { }

         // For b[i] = val;
         reference& operator=(bool val) {
            if (val) {
               fBit |= val<<fPos;
            } else {
               fBit &= (0xFF ^ (1<<fPos));
            }
            return *this;
         }

         // For b[i] = b[__j];
         reference& operator=(const reference& rhs) {
            *this = rhs.operator bool();
            return *this;
         }

         // Flips the bit
         bool operator~() const { return (fBit & (1<<fPos)) == 0; };

         // For val = b[i];
         operator bool() const { return (fBit & (1<<fPos)) != 0; };
      };

      
      // Enumerate the part of the private interface, we want to expose.
      using Base_t::MakeCopy;
      using Base_t::MakeCopyAt;
      using Base_t::ReleaseInstance;
      using Base_t::SizeOf;

      // This replaces the dummy constructor to make sure that I/O can be
      // performed while the user is only allowed to use the static maker
      BitSet(TRootIOCtor *marker) : fNbits(0), fData(marker) {}

       // Assignment allowed
      BitSet& operator=(const BitSet&rhs) {
           // BitSet assignment operator

         if (this != &rhs) {
            fNbits   = rhs.fNbits;
            // Default operator= does memcpy.
            // Potential trucation if this is smaller than rhs.
            fData    = rhs.fData;
         }
         return *this;
      }

      static size_t SizeOfInstance(size_t nbits) {
         return SizeOf( GetNbytes(nbits) );
      }

      static BitSet *MakeInstance(size_t nbits) {
         size_t nvalues = GetNbytes(nbits);
         return Base_t::MakeInstance(nvalues, nbits);
      }

      static BitSet *MakeInstanceAt(size_t nbits, void *addr) {
         size_t nvalues = GetNbytes(nbits);
         return Base_t::MakeInstanceAt(nvalues, addr, nbits);
      }

      static BitSet *MakeCompactCopy(BitSet &other)
      {
         // Make a copy of a the variable size array and its container with
         // a new_size of the content.
         // If no compact was needed, return the address of the original

         if (other.GetNbytes() <= 1) return &other;

         size_t needed;
         for(needed=other.GetNbytes()-1;
             needed > 0 && other.fData[needed]==0; ) { needed--; };
         needed++;

         if (needed!=other.GetNbytes()) {
            return MakeCopy( needed, other );
         } else {
            return &other;
         }
      }

      size_t SizeOf() const
      { return SizeOf( fData.fN ); }

      //----- bit manipulation
      //----- (note the difference with TObject's bit manipulations)
      void   ResetAllBits(bool value=false)
      {
         // Reset all bits to 0 (false).
         // if value=1 set all bits to 1

         if (fData.GetValues()) memset(fData.GetValues(),value ? 1 : 0,GetNbytes());
      }
      void   ResetBitNumber(size_t bitnumber)
      {
         SetBitNumber(bitnumber,false);
      }
      bool   SetBitNumber(size_t bitnumber, bool value = true)
      {
         // Set bit number 'bitnumber' to be value

         if (bitnumber >= fNbits) {
            size_t new_size = (bitnumber/8) + 1;
            if (new_size > GetNbytes()) {
               // too far, can not set
               return false;
            }
            fNbits = bitnumber+1;
         }
         size_t  loc = bitnumber/8;
         unsigned char bit = bitnumber%8;
         if (value)
            fData[loc] |= (1<<bit);
         else
            fData[loc] &= (0xFF ^ (1<<bit));
         return true;
      }
      bool TestBitNumber(size_t bitnumber) const
      {
         // Return the current value of the bit

         if (bitnumber >= fNbits) return false;
         size_t  loc = bitnumber/8;
         unsigned char value = fData[loc];
         unsigned char bit = bitnumber%8;
         bool result = (value & (1<<bit)) != 0;
         return result;
         // short: return 0 != (fAllBits[bitnumber/8] & (1<< (bitnumber%8)));
      }

      //----- Accessors and operator
      BitSet::reference operator[](size_t bitnumber) {
         if (bitnumber >= fNbits) return reference();
         size_t  loc = bitnumber/8;
         unsigned char bit = bitnumber%8;
         return reference(fData[loc],bit);
      }
      bool operator[](size_t bitnumber) const
      {
         return TestBitNumber(bitnumber);
      }

      BitSet& operator&=(const BitSet& rhs) { DoAndEqual(rhs); return *this; }
      BitSet& operator|=(const BitSet& rhs) {  DoOrEqual(rhs); return *this; }
      BitSet& operator^=(const BitSet& rhs) { DoXorEqual(rhs); return *this; }
      BitSet& operator<<=(size_t rhs) { DoLeftShift(rhs); return *this; }
      BitSet& operator>>=(size_t rhs) { DoRightShift(rhs); return *this; }
//      BitSet  operator<<(size_t rhs) { return BitSet(*this)<<= rhs; }
//      BitSet  operator>>(size_t rhs) { return BitSet(*this)>>= rhs; }
//      BitSet  operator~() { BitSet res(*this); res.DoFlip(); return res; }

      //----- Optimized setters
      // Each of these will replace the contents of the receiver with the bitvector
      // in the parameter array.  The number of bits is changed to nbits.  If nbits
      // is smaller than fNbits, the receiver will NOT be compacted.
      bool   Set(size_t nbits, const char *array)
      {
         // Set all the bytes.

         size_t nbytes=(nbits+7)>>3;

         if (nbytes > GetNbytes()) return false;

         fNbits=nbits;
         memcpy(fData.GetValues(), array, nbytes);
         return true;
      }
      bool   Set(size_t nbits, const unsigned char *array) { return Set(nbits, (const char*)array); }
      bool   Set(size_t nbits, const unsigned short *array) { return Set(nbits, (const short*)array); }
      bool   Set(size_t nbits, const unsigned int *array) { return Set(nbits, (const int*)array); }
      bool   Set(size_t nbits, const unsigned long long *array) { return Set(nbits, (const long long*)array); }

#ifdef R__BYTESWAP  /* means we are on little endian */

 /*
 If we are on a little endian machine, a bitvector represented using
 any integer type is identical to a bitvector represented using bytes.
 -- FP.
 */
      bool   Set(size_t nbits, const short *array) { return Set(nbits, (const char*)array); }
      bool   Set(size_t nbits, const int *array) { return Set(nbits, (const char*)array); }
      bool   Set(size_t nbits, const long long *array) { return Set(nbits, (const char*)array); }
#else
 /*
 If we are on a big endian machine, some swapping around is required.
 */

      bool Set(size_t nbits, const short *array)
      {
         // make nbytes even so that the loop below is neat.
         size_t nbytes = ((nbits+15)>>3)&~1;

         if (nbytes > GetNbytes()) return false;

         fNbits=nbits;

         const unsigned char *cArray = (const unsigned char*)array;
         for (size_t i=0; i<nbytes; i+=2) {
            fData[i] = cArray[i+1];
            fData[i+1] = cArray[i];
         }
         return true;
      }

      bool Set(size_t nbits, const int *array)
      {
         // make nbytes a multiple of 4 so that the loop below is neat.
         size_t nbytes = ((nbits+31)>>3)&~3;

         if (nbytes > GetNbytes()) return false;

         fNbits=nbits;

         const unsigned char *cArray = (const unsigned char*)array;
         for (size_t i=0; i<nbytes; i+=4) {
            fData[i] = cArray[i+3];
            fData[i+1] = cArray[i+2];
            fData[i+2] = cArray[i+1];
            fData[i+3] = cArray[i];
         }
         return true;
      }

      bool Set(size_t nbits, const long long *array)
      {
         // make nbytes a multiple of 8 so that the loop below is neat.
         size_t nbytes = ((nbits+63)>>3)&~7;

         if (nbytes > GetNbytes()) return false;

         fNbits=nbits;

         const unsigned char *cArray = (const unsigned char*)array;
         for (size_t i=0; i<nbytes; i+=8) {
            fData[i] = cArray[i+7];
            fData[i+1] = cArray[i+6];
            fData[i+2] = cArray[i+5];
            fData[i+3] = cArray[i+4];
            fData[i+4] = cArray[i+3];
            fData[i+5] = cArray[i+2];
            fData[i+6] = cArray[i+1];
            fData[i+7] = cArray[i];
         }
         return true;
      }

#endif

      //----- Optimized getters
      // Each of these will replace the contents of the parameter array with the
      // bits in the receiver.  The parameter array must be large enough to hold
      // all of the bits in the receiver.
      // Note on semantics: any bits in the parameter array that go beyond the
      // number of the bits in the receiver will have an unspecified value.  For
      // example, if you call Get(Int*) with an array of one integer and the BitSet
      // object has less than 32 bits, then the remaining bits in the integer will
      // have an unspecified value.
      void   Get(char *array) const
      {
         // Copy all the byes.

         memcpy(array, fData.GetValues(), (fNbits+7)>>3);
      }
      void   Get(unsigned char *array) const { Get((char*)array); }
      void   Get(unsigned short *array) const { Get((short*)array); }
      void   Get(unsigned int *array) const { Get((int*)array); }
      void   Get(unsigned long long *array) const { Get((long long*)array); }

#ifdef R__BYTESWAP  /* means we are on little endian */

 /*
 If we are on a little endian machine, a bitvector represented using
 any integer type is identical to a bitvector represented using bytes.
 -- FP.
 */
      void   Get(short *array) const { Get((char*)array); }
      void   Get(int *array) const { Get((char*)array); }
      void   Get(long long *array) const { Get((char*)array); }

#else

      void Get(short *array) const
      {
         // Get all the bytes.

         size_t nBytes = (fNbits+7)>>3;
         size_t nSafeBytes = nBytes&~1;

         unsigned char *cArray=(unsigned char*)array;
         for (size_t i=0; i<nSafeBytes; i+=2) {
            cArray[i] = fData[i+1];
            cArray[i+1] = fData[i];
         }

         if (nBytes>nSafeBytes) {
            cArray[nSafeBytes+1] = fData[nSafeBytes];
         }
      }

      void Get(int *array) const
      {
         // Get all the bytes.

         size_t nBytes = (fNbits+7)>>3;
         size_t nSafeBytes = nBytes&~3;

         unsigned char *cArray=(unsigned char*)array;
         size_t i;
         for (i=0; i<nSafeBytes; i+=4) {
            cArray[i] = fData[i+3];
            cArray[i+1] = fData[i+2];
            cArray[i+2] = fData[i+1];
            cArray[i+3] = fData[i];
         }

         for (i=0; i<nBytes-nSafeBytes; ++i) {
            cArray[nSafeBytes + (3 - i)] = fData[nSafeBytes + i];
         }
      }

      void Get(long long *array) const
      {
         // Get all the bytes.

         size_t nBytes = (fNbits+7)>>3;
         size_t nSafeBytes = nBytes&~7;

         unsigned char *cArray=(unsigned char*)array;
         size_t i;
         for (i=0; i<nSafeBytes; i+=8) {
            cArray[i] = fData[i+7];
            cArray[i+1] = fData[i+6];
            cArray[i+2] = fData[i+5];
            cArray[i+3] = fData[i+4];
            cArray[i+4] = fData[i+3];
            cArray[i+5] = fData[i+2];
            cArray[i+6] = fData[i+1];
            cArray[i+7] = fData[i];
         }

         for (i=0; i<nBytes-nSafeBytes; ++i) {
            cArray[nSafeBytes + (7 - i)] = fData[nSafeBytes + i];
         }
      }

#endif

      //----- Utilities
      void    Clear() {
         fNbits = 0;
         memset(&(fData[0]),0,GetNbytes());
      }
      size_t  CountBits(size_t startBit=0)     const
      {
         // Return number of bits set to 1 starting at bit startBit

         static const unsigned char nbits[256] = {
            0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
            1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
            1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
            1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
            2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
            3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
            3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
            4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

         size_t i,count = 0;
         if (startBit == 0) {
            for(i=0; i<GetNbytes(); i++) {
               count += nbits[fData[i]];
            }
            return count;
         }
         if (startBit >= fNbits) return count;
         size_t startByte = startBit/8;
         size_t ibit = startBit%8;
         if (ibit) {
            for (i=ibit;i<8;i++) {
               if (fData[startByte] & (1<<ibit)) count++;
            }
            startByte++;
         }
         for(i=startByte; i<GetNbytes(); i++) {
            count += nbits[fData[i]];
         }
         return count;
      }

      size_t  FirstNullBit(size_t startBit=0)  const
      {
         // Return position of first null bit (starting from position 0 and up)

         static const unsigned char fbits[256] = {
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,7,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
             0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,8};

         size_t i;
         if (startBit == 0) {
            for(i=0; i<GetNbytes(); i++) {
               if (fData[i] != 255) return 8*i + fbits[fData[i]];
            }
            return fNbits;
         }
         if (startBit >= fNbits) return fNbits;
         size_t startByte = startBit/8;
         size_t ibit = startBit%8;
         if (ibit) {
            for (i=ibit;i<8;i++) {
               if ((fData[startByte] & (1<<i)) == 0) return 8*startByte+i;
            }
            startByte++;
         }
         for(i=startByte; i<GetNbytes(); i++) {
            if (fData[i] != 255) return 8*i + fbits[fData[i]];
         }
         return fNbits;
      }
      size_t  FirstSetBit(size_t startBit=0)   const
      {
         // Return position of first non null bit (starting from position 0 and up)

         static const char fbits[256] = {
             8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
             4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0};

         size_t i;
         if (startBit == 0) {
            for(i=0; i<GetNbytes(); i++) {
               if (fData[i] != 0) return 8*i + fbits[fData[i]];
            }
            return fNbits;
         }
         if (startBit >= fNbits) return fNbits;
         size_t startByte = startBit/8;
         size_t ibit = startBit%8;
         if (ibit) {
            for (i=ibit;i<8;i++) {
               if ((fData[startByte] & (1<<i)) != 0) return 8*startByte+i;
            }
            startByte++;
         }
         for(i=startByte; i<GetNbytes(); i++) {
            if (fData[i] != 0) return 8*i + fbits[fData[i]];
         }
         return fNbits;
      }
      size_t  LastNullBit() const { return LastNullBit(fNbits-1); }
      size_t  LastNullBit(size_t startBit) const
      {
         // Return position of first null bit (starting from position startBit and down)

         static const unsigned char fbits[256] = {
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
            3,3,3,3,3,3,3,3,2,2,2,2,1,1,0,8};

         size_t i;
         if (startBit>=fNbits) startBit = fNbits-1;
         size_t startByte = startBit/8;
         size_t ibit = startBit%8;
         if (ibit<7) {
            for (i=ibit+1;i>0;i--) {
               if ((fData[startByte] & (1<<(i-1))) == 0) return 8*startByte+i-1;
            }
            startByte--;
         }
         for(i=startByte+1; i>0; i--) {
            if (fData[i-1] != 255) return 8*(i-1) + fbits[fData[i-1]];
         }
         return fNbits;
      }
      size_t  LastSetBit()  const { return LastSetBit(fNbits-1); }
      size_t  LastSetBit(size_t startBit)  const
      {
         // Return position of first non null bit (starting from position fNbits and down)

         static const char fbits[256] = {
             8,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
             4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

         size_t i;
         if (startBit>=fNbits) startBit = fNbits-1;
         size_t startByte = startBit/8;
         size_t ibit = startBit%8;
         if (ibit<7) {
            for (i=ibit+1;i>0;i--) {
               if ((fData[startByte] & (1<<(i-1))) != 0) return 8*startByte+i-1;
            }
            startByte--;
         }
         for(i=startByte+1; i>0; i--) {
            if (fData[i-1] != 0) return 8*(i-1) + fbits[fData[i-1]];
         }
         return fNbits;
      }
      size_t  GetNbits()      const { return fNbits; }
      size_t  GetNbytes()     const { return fData.fN; }

      bool  operator==(const BitSet &other) const
      {
         // Compare object.

         if (fNbits == other.fNbits) {
            return !memcmp(fData.GetValues(), other.fData.GetValues(), (fNbits+7)>>3);
         } else if (fNbits <  other.fNbits) {
            return !memcmp(fData.GetValues(), other.fData.GetValues(), (fNbits+7)>>3) && other.FirstSetBit(fNbits) == other.fNbits;
         } else {
            return !memcmp(fData.GetValues(), other.fData.GetValues(), (other.fNbits+7)>>3) && FirstSetBit(other.fNbits) == fNbits;
         }
      }
      bool  operator!=(const BitSet &other) const { return !(*this==other); }
   };

   inline bool operator&(const BitSet::reference& lhs, const BitSet::reference& rhs)
   {
      return (bool)lhs & rhs;
   }

   inline bool operator|(const BitSet::reference& lhs, const BitSet::reference& rhs)
   {
      return (bool)lhs | rhs;
   }

   inline bool operator^(const BitSet::reference& lhs, const BitSet::reference& rhs)
   {
      return (bool)lhs ^ rhs;
   }

}

#if defined(GCC_DIAG_POP_NEEDED)

#pragma GCC diagnostic pop
#undef GCC_DIAG_POP_NEEDED

#endif

#endif
