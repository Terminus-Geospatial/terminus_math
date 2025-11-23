/**
 * @file    Compound_Types.hpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#pragma once

// C++ Libraries
#include <type_traits>

// Terminus Libraries
#include <terminus/math/types/fundamental_types.hpp>

namespace tmns::math {

/**
 * Default definitions for the Compound-Type Traits.  The goal is to allow for
 * simple access of specific attributes from a PixelType.  That said, we do
 * support more broadly defined cases, such as a single-channel image or scalars.
 *
 * If you implement a Pixel-Type of your own,
 * make sure to add one of these, as it will allow the other compound types to reference.
 *
 * Types to build:
 *
 * 1. Compound_Channel_Type     - Registers the type
 * 2. Compound_Channel_Count    - Provides how many channels this type has
 * 3. Compound_Channel_Cast     - Provides bindings to other types.
 * 4. Is_Compound               - Tests if type is compound (Multiple channels)
 * 5. Is_Scalar_Or_Compound     - Tests if type is compound or scalar (basic arithmetic type)
 * 6. Compound_Is_Compatible    - Tests if Compound is compatible with another type.
 * 7. Compound_Accumulator_Type - Provides an accumulator type for doing math with.
 *
 * Note:  Is_Scalar requires no template instanciations, and is defined in @ref Fundamental_Types.hpp.
 *
 * See the glossary for common terms in this codebase.
 */

/**
 * Base type which you need to add for your own types
*/
template <typename PixelT>
struct Compound_Channel_Type
{
    typedef PixelT type;
}; // End of Compound_Channel_Type Struct

/**
 * Const version of Compound-Channel-Type
*/
template <class PixelT>
struct Compound_Channel_Type<const PixelT> : public Compound_Channel_Type<PixelT> {};

/**
 * Simple lookup for how many channels your type has
*/
template <typename PixelT>
struct Compound_Channel_Count
{
    static const size_t value = 1;
}; // End of Compound_Channel_Count Struct

/**
 * Const Compound-Channel-Count
*/
template <class PixelT>
struct Compound_Channel_Count<const PixelT> : public Compound_Channel_Count<PixelT> {};

/**
 * Support Channel Casting Bindings you support
*/
template <typename PixelT, typename ChannelT>
struct Compound_Channel_Cast
{
    typedef ChannelT type;
}; // End of Compound_Channel_Cast Struct

/**
 * Channel-Cast method for Const Pixel-Types
*/
template <class PixelT, class ChannelT>
struct Compound_Channel_Cast<const PixelT, ChannelT> : public Compound_Channel_Cast<PixelT, ChannelT>{};

/**
 * Test if type is a compound type.  Compound being a complex type unlike "float" for
 * example.  THis basically verifies that a type based on a "float" isn't in fact just a float.
*/
template <class PixelT>
struct Is_Compound : public std::negation< std::is_same< typename Compound_Channel_Type<PixelT>::type, PixelT > >
{
}; // End of Is_Compound Struct

/**
 * Const form of Is_Compound check
*/
template <class PixelT>
struct Is_Compound<const PixelT> : public Is_Compound<PixelT> {};

/**
 * Test if Pixel-Type is Scalar or a Compound.  Needed for more generic API calls
*/
template <typename PixelT>
struct Is_Scalar_Or_Compound : public std::disjunction< typename Is_Scalar<PixelT>::type,
                                                        typename Is_Compound<PixelT>::type >::type
{
};

/**
 * Check if a cast between two types is compatible
*/
template <class PixelT1, class PixelT2>
struct Compound_Is_Compatible
       : public std::is_same< typename Compound_Channel_Cast<PixelT1,
                                                             typename Compound_Channel_Type<PixelT2>::type>::type,
                                                                                            PixelT2 >::type
{
};

/**
 * Const form of Compound_Is_Compatible
*/
template <class PixelT1, class PixelT2>
struct Compound_Is_Compatible<PixelT1, const PixelT2>
    : public Compound_Is_Compatible<PixelT1,PixelT2> {};

/**
 * Return the accumulator type for the compound pixel
*/
template <class PixelT>
struct Compound_Accumulator_Type
{
    typedef typename Compound_Channel_Cast< PixelT,
                                            typename Accumulator_Type<typename Compound_Channel_Type<PixelT>::type>::type>::type type;
};

/**
 * Get the name of the datatype
*/
template <typename PixelT>
struct Compound_Name
{
    typedef typename std::conditional< Is_Compound<PixelT>::value,
                                            Data_Type_Name<typename Compound_Channel_Type<PixelT>::type>,
                                            Data_Type_Name<PixelT>>::type type;

    static std::string name() { return type::name(); }

}; // End of Compound_Name Class

} // end of tmns::math namespace