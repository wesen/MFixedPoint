//!
//! @file 		FixedPoint-Fp32s.hpp
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 	n/a
//! @date 		2013/07/22
//! @brief 		A slower, more powerful 32-bit fixed point library.
//! @details
//!				See README.rst in root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#ifndef FP32S_H
#define FP32S_H

#include <stdint.h>

// Fixed-point configuration file
#include "FixedPoint-Config.hpp"

// Port-specific code
#include "FixedPoint-Port.hpp"

namespace Fp
{

	//! The template argument p in all of the following functions refers to the 
	//! fixed point precision (e.g. p = 8 gives 24.8 fixed point functions).
	//! Contains mathematical operator overloading. Doesn't have modulus (%) overloading
	class Fp32s {
		
		public:
		
		//! @brief		The fixed-point number is stored in this basic data type.
		int32_t rawVal;			
		
		//! @brief		This stores the number of fractional bits.
		uint8_t q;
		
		Fp32s()
		{
			#if(fpConfig_PRINT_DEBUG_GENERAL == 1)
				//Port::DebugPrint("FP: New fixed-point object created.");
			#endif
		}
		
		Fp32s(int32_t i, uint8_t qin)
		{
			rawVal = i << qin;
			q = qin;
		}
		
		Fp32s(double dbl, uint8_t qin)
		{
			rawVal = (int32_t)(dbl * (1 << qin));
			q = qin;
		}
		
		Fp32s& operator += (Fp32s r)
		{ 
			if(q > r.q)
			{
				// First number smaller
				rawVal = (rawVal > (q - r.q)) + r.rawVal; 
				q = q - r.q;
			}
			return *this;
		}
		
		//! @brief		Conversion operator from fixed-point to int32_t.
		operator int32_t()
		{
			// Right-shift to get rid of all the decimal bits
			return (rawVal >> q);
		}
		
		//! @brief		Conversion operator from fixed-point to int64_t.
		operator int64_t()
		{
			// Right-shift to get rid of all the decimal bits
			return (int64_t)(rawVal >> q);
		}
		
		//! @brief		Conversion operator from fixed-point to float.
		//! @note		Similar to double conversion.
		operator float()
		{ 
			return (float)rawVal / (float)(1 << q);
		}
		
		//! @brief		Conversion operator from fixed-point to double.
		//! @note		Similar to float conversion.
		operator double()
		{ 
			return (double)rawVal / (double)(1 << q);
		}
		
	
	};

} // namespace Fp

#endif // #ifndef FP32S_H

// EOF