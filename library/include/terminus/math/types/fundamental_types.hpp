/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2024 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    fundamental_types.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// C++ Libraries
#include <cinttypes>
#include <complex>
#include <deque>
#include <type_traits>
#include <vector>

namespace tmns::math {

/**
 * Simple structure to verify whether or not a given pixel or channel
 * type is a scalar type.  This "scalar" definition
 *
 * https://en.wikipedia.org/wiki/Scalar_(mathematics)
*/
template <typename ValueT> struct Is_Scalar : public std::is_arithmetic<ValueT> {};
template <typename ValueT> struct Is_Scalar<std::complex<ValueT> > : public std::true_type {};
template <typename ValueT> struct Is_Scalar<const ValueT> : public Is_Scalar<ValueT> {};

template <typename ValueT> using Is_Scalar_t = typename Is_Scalar<ValueT>::type;

/**
 * Returns a type for doing arithmetic on for an input type.  For example, if doing math on
 * an 8-bit unsigned type (uint8_t), it won't take long before it overflows the 8-bits.  Therefore,
 * we should use a 32-bit signed value.  This also allows for subtractions and other operations
 * which may result in negative values.
*/
template <class ValueT> struct Accumulator_Type {};
template <> struct Accumulator_Type<bool>     { typedef int      type; };
template <> struct Accumulator_Type<uint8_t>  { typedef int32_t  type; };
template <> struct Accumulator_Type<int8_t>   { typedef int32_t  type; };
template <> struct Accumulator_Type<uint16_t> { typedef int32_t  type; };
template <> struct Accumulator_Type<int16_t>  { typedef int32_t  type; };
template <> struct Accumulator_Type<uint32_t> { typedef int64_t  type; };
template <> struct Accumulator_Type<int32_t>  { typedef int64_t  type; };
template <> struct Accumulator_Type<uint64_t> { typedef int64_t  type; };
template <> struct Accumulator_Type<int64_t>  { typedef int64_t  type; };
template <> struct Accumulator_Type<float>    { typedef double   type; };
template <> struct Accumulator_Type<double>   { typedef double   type; };
template <class ValueT> struct Accumulator_Type<std::complex<ValueT> >
{
    typedef std::complex<typename Accumulator_Type<ValueT>::type> type;
};

/**
 * Simply class for getting the name of the datatype
*/
template <typename ValueT>
struct Data_Type_Name
{
    static std::string name() { return ""; }
};

template <> struct Data_Type_Name<uint8_t>{   static std::string name(){ return "uint8_t"; }  };
template <> struct Data_Type_Name<uint16_t>{  static std::string name(){ return "uint16_t"; } };
template <> struct Data_Type_Name<uint32_t>{  static std::string name(){ return "uint32_t"; } };
template <> struct Data_Type_Name<uint64_t>{  static std::string name(){ return "uint64_t"; } };

template <> struct Data_Type_Name<int8_t>{   static std::string name(){ return "int8_t"; }  };
template <> struct Data_Type_Name<int16_t>{  static std::string name(){ return "int16_t"; } };
template <> struct Data_Type_Name<int32_t>{  static std::string name(){ return "int32_t"; } };
template <> struct Data_Type_Name<int64_t>{  static std::string name(){ return "int64_t"; } };

template <> struct Data_Type_Name<float>{   static std::string name(){ return "float"; }  };
template <> struct Data_Type_Name<double>{  static std::string name(){ return "double"; } };

/**
 * Simple wrapper that converts char types for printing.
*/
template <typename TP>
TP numeric( TP v )
{
    return v;
}

/**
 * uint8 overload
*/
inline uint16_t numeric( uint8_t v )
{
    return v;
}

/**
 * int8 overload
*/
inline int16_t numeric( int8_t v )
{
    return v;
}


/**
 * Structure to determine if we are using an STL-compliant container.
*/
template <typename ValueT> struct Is_STL_Container : public std::false_type {};

// Deque
template <typename ValueT> struct Is_STL_Container<std::deque<ValueT>> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<const std::deque<ValueT>> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<std::deque<ValueT>&> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<const std::deque<ValueT>&> : public std::true_type {};

// Vector
template <typename ValueT> struct Is_STL_Container<std::vector<ValueT>> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<const std::vector<ValueT>> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<std::vector<ValueT>&> : public std::true_type {};
template <typename ValueT> struct Is_STL_Container<const std::vector<ValueT>&> : public std::true_type {};

//template <typename ValueT> using Is_STL_Container = typename Is_Scalar<ValueT>::va;

} // End of tmns::image namespace