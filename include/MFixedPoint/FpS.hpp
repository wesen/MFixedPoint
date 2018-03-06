///
/// @file 				FpS.hpp
/// @author 			Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
/// @edited 			n/a
/// @created			2013-07-22
/// @last-modified		2018-01-09
/// \brief 				A slower, more powerful fixed point library.
/// \details
///		See README.rst in root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
#error Please build with C++ compiler
#endif

#ifndef MN_MFIXEDPOINT_FPS_H
#define MN_MFIXEDPOINT_FPS_H

// System includes
#include <stdint.h>
#include <type_traits>

namespace mn {
namespace MFixedPoint {

/// \brief 		A class which represents a "slow" fixed-point number, where each instance supports an arbitrary number of fractional bits,
///				and arithmetic is supported between these instances.
/// \tparam		BaseType		The underlying data type which will be store the raw fixed point data. It is recommended that
///								this should be a signed integer type (e.g. int32_t).
/// \tparam		OverflowType	The type that the basetype will be cast to before doing fixed point operations
///								that have a possibility of intermediate overflowing (e.g. multiplication, division).
///								It is recommended that this should be twice the bit size of the BaseType 
///								(e.g. if BaseType = int32_t, OverflowType = int64_t).
template<class BaseType, class OverflowType, uint8_t numFracBits_>
class FpS {

public:

    //===============================================================================================//
    //================================== CONSTRUCTORS/DESTRUCTORS ===================================//
    //===============================================================================================//

    /// \brief		Create a fixed-point value from a integer and a num. of fractional bits.
    FpS(int32_t integer) {
        static_assert(std::is_integral<BaseType>::value, "Integral BaseType required for FpS class.");
        rawVal_ = integer << numFracBits_;
    }

    /// \brief		Create a fixed-point value from a double and a num. of fractional bits.
    FpS(double dbl) {
        static_assert(std::is_integral<BaseType>::value, "Integral BaseType required for FpS class.");
        rawVal_ = (BaseType)(dbl * ((BaseType)1 << numFracBits_));
    }

    //===============================================================================================//
    //========================================= GETTERS/SETTERS =====================================//
    //===============================================================================================//

    /// \brief		Get the raw value (memory representation) of this fixed-point number,
    BaseType GetRawVal() const {
        return rawVal_;
    }

    /// \brief		Returns the number of fractional bits used in this fixed-point number.
    uint8_t GetNumFracBits() const {
        return numFracBits_;
    }

    //===============================================================================================//
    //================================== COMPOUND ARITHMETIC OPERATORS ==============================//
    //===============================================================================================//

    /// \brief		Overload for '+=' operator.
    /// \details	Result has the same num. frac bits as the lowest num. frac bits of the two inputs.
    FpS &operator+=(FpS r) {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        rawVal_ = rawVal_ + r.rawVal_;
        // No need to change num. frac. bits, both are the same
        return *this;
    }

    FpS operator-() const {
        return FpS(-rawVal_);
    }

    /// \brief		Overload for '-=' operator.
    /// \details	Result has the same num. frac bits as the lowest num. frac bits of the two inputs.
    FpS &operator-=(FpS r) {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        // Q the same for both numbers
        rawVal_ = rawVal_ - r.rawVal_;
        // No need to change Q, both are the same
        return *this;
    }

    /// \brief		Overlaod for '*=' operator.
    /// \details	Uses intermediatary casting to OverflowType to prevent overflows.
    FpS &operator*=(FpS r) {

        // Optimised for when numFracBits_ is the same for both

        // operators (first if statement).
        // Q the same for both numbers, shift right by Q
        rawVal_ = (BaseType)(((OverflowType)rawVal_ * (OverflowType)r.rawVal_) >> numFracBits_);

        // No need to change Q, both are the same
        return *this;
    }

    /// \brief		Overlaod for '/=' operator.
    /// \details	Uses intermediatary casting to OverflowType to prevent overflows.
    FpS &operator/=(FpS r) {

        // Optimised for when numFracBits_ is the same for both

        // operators (first if statement).
        // Q the same for both numbers, shift right by Q

        rawVal_ = (BaseType)((((OverflowType)rawVal_ << numFracBits_) / (OverflowType)r.rawVal_));
        return *this;
    }

    /// \brief		Overload for '%=' operator.
    FpS &operator%=(FpS r) {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        // Q the same for both numbers
        rawVal_ = rawVal_ % r.rawVal_;
        // No need to change Q, both are the same
        return *this;
    }

    //===============================================================================================//
    //==================================== SIMPLE ARITHMETIC OPERATORS ==============================//
    //===============================================================================================//

    /// \brief		Overload for '+' operator.
    /// \details	Uses '+=' operator.
    FpS operator+(FpS r) const {
        FpS x = *this;
        x += r;
        return x;
    }

    /// \brief		Overload for '-' operator.
    /// \details	Uses '-=' operator.
    FpS operator-(FpS r) const {
        FpS x = *this;
        x -= r;
        return x;
    }

    /// \brief		Overload for '*' operator.
    /// \details	Uses '*=' operator.
    FpS operator*(FpS r) const {
        FpS x = *this;
        x *= r;
        return x;
    }

    /// \brief		Overload for '/' operator.
    /// \details	Uses '/=' operator.
    FpS operator/(FpS r) const {
        FpS x = *this;
        x /= r;
        return x;
    }

    /// \brief		Overload for '%' operator.
    /// \details	Uses '%=' operator.
    FpS operator%(FpS r) const {
        FpS x = *this;
        x %= r;
        return x;
    }

    //===============================================================================================//
    //====================================== COMPARISON OVERLOADS ===================================//
    //===============================================================================================//

    /// \brief		Overload for the '==' operator.
    bool operator==(FpS r) const {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        // Q the same for both numbers
        return rawVal_ == r.rawVal_;
    }

    /// \brief		Overload for the '!=' operator.
    bool operator!=(FpS r) const {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        // Q the same for both numbers
        return rawVal_ != r.rawVal_;
    }

    /// \brief		Overload for the '<' operator.
    bool operator<(FpS r) const {
        // Q the same for both numbers
        return rawVal_ < r.rawVal_;
    }

    /// \brief		Overload for the '>' operator.
    bool operator>(FpS r) const {
        // Q the same for both numbers
        return rawVal_ > r.rawVal_;
    }

    /// \brief		Overload for the '<=' operator.
    bool operator<=(FpS r) const {
        // Optimised for when numFracBits_ is the same for both
        // operators (first if statement).
        // Q the same for both numbers
        return rawVal_ <= r.rawVal_;
    }

    /// \brief		Overload for the '>=' operator.
    bool operator>=(FpS r) const {
        // Q the same for both numbers
        return rawVal_ >= r.rawVal_;
    }

    //===============================================================================================//
    //======================================= CONVERSION METHODS ====================================//
    //===============================================================================================//

    /// \brief		Converts the fixed-point number into an integer.
    /// \details	Always rounds to negative infinity (66.3 becomes 66, -66.3 becomes -67).
    /// \tparam		IntType		The return integer type.
    template<class IntType>
    IntType ToInt() const {
        // Right-shift to get rid of all the decimal bits
        // This rounds towards negative infinity
        return (IntType)(rawVal_ >> numFracBits_);
    }

    /// \brief		Converts the fixed-point number to a float.
    float ToFloat() const {
        return (float)rawVal_ / (float)((BaseType)1 << numFracBits_);
    }

    /// \brief		Converts the fixed-point number to a double.
    double ToDouble() const {
        return (double)rawVal_ / (double)((BaseType)1 << numFracBits_);
    }


    //===============================================================================================//
    //====================================== STRING/STREAM RELATED ==================================//
    //===============================================================================================//

    /// \brief		Converts the fixed-point number into a string representation, using a fixed-point->double->string
    /// 				conversion process.
    std::string ToString() const {
        return std::to_string(ToDouble());
    }

    /// \brief      Overload so we can print to a ostream (e.g. std::cout).
    friend std::ostream &operator<<(std::ostream &stream, FpS obj) {
        stream << obj.ToDouble();
        return stream;
    }

private:

    /// \brief		The fixed-point number is stored in this basic data type.
    BaseType rawVal_;

}; // class FpS

//===============================================================================================//
//========================================= SPECIALIZATIONS =====================================//
//===============================================================================================//

typedef ::mn::MFixedPoint::FpS<int16_t, int32_t, 8> fp16;
typedef ::mn::MFixedPoint::FpS<int32_t, int64_t, 16> fp32;
} // namespace MFixedPoint
} // namespace mn


inline ::mn::MFixedPoint::fp32 operator "" __fp32(unsigned long long int l) noexcept {
    return ::mn::MFixedPoint::fp32((int32_t)l);
}

inline ::mn::MFixedPoint::fp32 operator "" __fp32(long double l) noexcept {
    return ::mn::MFixedPoint::fp32((double)l);
}

inline ::mn::MFixedPoint::fp16 operator "" __fp16(unsigned long long int l) noexcept {
    return ::mn::MFixedPoint::fp16((int32_t)l);
}

inline ::mn::MFixedPoint::fp16 operator "" __fp16(long double l) noexcept {
    return ::mn::MFixedPoint::fp16((double)l);
}


#endif // #ifndef MN_MFIXEDPOINT_FPS_H

// EOF
