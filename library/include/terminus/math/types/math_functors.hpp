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
 * @file    math_functors.hpp
 * @author  Marvin Smith
 * @date    10/13/2023
 */
#pragma once

// C++ Libraries
#include <queue>

// Terminus Libraries
#include <terminus/log/utility.hpp>

namespace tmns::math {

/**
 * Define the "base-type" as a double
 */
template <typename T>
struct Std_Math_Type
{
    typedef double type;
};

/**
 * Define the "base-type" as a float
 */
template <>
struct Std_Math_Type<float>
{
    typedef float type;
};

/**
 * Define the "base-type" as a long double
 */
template <>
struct Std_Math_Type<long double>
{
    typedef long double type;
};

template <typename T1,
          typename T2>
struct Std_Math_Type_2
{
    typedef typename Std_Math_Type<typename Promote_Type<T1,T2>::type>::type type;
};


template <typename FunctorT,
          typename ArgumentT,
          bool     ArgIsCompound>
struct Arg_Unary_Functor_Type_Helper : public Std_Math_Type<ArgumentT> {};

template <typename FunctorT,
          typename ArgumentT>
struct Arg_Unary_Functor_Type_Helper<FunctorT,ArgumentT,true>
{
    typedef typename Compound_Channel_Type<ArgumentT>::type channel_type;
    typedef typename Arg_Unary_Functor_Type_Helper<FunctorT,
                                                   channel_type,
                                                   Is_Compound<channel_type>::value>::type result_channel_type;
    typedef typename Compound_Channel_Cast<ArgumentT,result_channel_type>::type type;
};

template <typename F, typename T>
struct Arg_Unary_Functor_Type : public Arg_Unary_Functor_Type_Helper<F,T,Is_Compound<T>::value> {};


template <typename FunctorT,
          typename Arg1T,
          typename Arg2T,
          bool Arg1_Is_Compound,
          bool Arg2_Is_Compound>
struct Binary_Functor_Type_Helper : public Std_Math_Type_2<Arg1T,Arg2T> {};

template <typename FunctorT,
          typename Arg1T,
          typename Arg2T>
struct Binary_Functor_Type_Helper<FunctorT,Arg1T,Arg2T,true,false>
{
    typedef typename Compound_Channel_Type<Arg1T>::type channel_type;
    typedef typename Binary_Functor_Type_Helper<FunctorT,channel_type,Arg2T,Is_Compound<channel_type>::value,false>::type result_channel_type;
    typedef typename Compound_Channel_Cast<Arg1T,result_channel_type>::type type;
};

template <typename FunctorT,
          typename Arg1T,
          typename Arg2T>
struct Binary_Functor_Type_Helper<FunctorT,Arg1T,Arg2T,false,true>
{
    typedef typename Compound_Channel_Type<Arg2T>::type channel_type;
    typedef typename Binary_Functor_Type_Helper<FunctorT,Arg1T,channel_type,false,Is_Compound<channel_type>::value>::type result_channel_type;
    typedef typename Compound_Channel_Cast<Arg2T,result_channel_type>::type type;
};


template <typename FunctorT,
          typename Argument1T,
          typename Argument2T>
struct Binary_Functor_Type_Helper<FunctorT,Argument1T,Argument2T,true,true>
{
    typedef typename Compound_Channel_Type<Argument1T>::type channel1_type;
    typedef typename Compound_Channel_Type<Argument2T>::type channel2_type;
    typedef typename Binary_Functor_Type_Helper<FunctorT,
                                                channel1_type,
                                                channel2_type,
                                                Is_Compound<channel1_type>::value,
                                                Is_Compound<channel2_type>::value>::type result_channel_type;
    typedef typename std::disjunction<Compound_Is_Compatible<Argument1T,Argument2T>,
                                      typename Compound_Channel_Cast<Argument1T,
                                                                     result_channel_type>::type,
                                      Type_Deduction_Error<Binary_Functor_Type_Helper> >::type type;
};

template <typename FunctorT,
          typename Argument1T,
          typename Argument2T>
struct Binary_Functor_Type : public Binary_Functor_Type_Helper<FunctorT,
                                                               Argument1T,
                                                               Argument2T,
                                                               Is_Compound<Argument1T>::value,
                                                               Is_Compound<Argument2T>::value> {};

// ********************************************************************
// Standard Mathematical Functors
// ********************************************************************
/*

// Macros to easily create functors with a certain interface
#define __VW_UNARY_MATH_FUNCTOR(name,func)                              \
    struct Arg##name##Functor                                           \
      : UnaryReturnBinaryTemplateBind1st<ArgUnaryFunctorType,Arg##name##Functor> { \
        template <class ValueT>                                           \
          typename ArgUnaryFunctorType<Arg##name##Functor,ValueT>::type   \
          operator()( ValueT arg ) const {                                \
          return func(arg);                                             \
        }                                                               \
        float operator()( float arg ) const {                           \
          return func##f(arg);                                          \
        }                                                               \
        __VW_MATH_UNARY_LONG_DOUBLE_IMPL(func)                          \
          };                                                            \
    using ::func;
// END __VW_UNARY_MATH_FUNCTOR
#define __VW_BINARY_MATH_FUNCTOR(name,func)                             \
    struct ArgArg##name##Functor                                        \
      : BinaryReturnTernaryTemplateBind1st<BinaryFunctorType,ArgArg##name##Functor> { \
    float operator()( float arg1, float arg2 ) const {                  \
    return func##f(arg1,arg2);                                          \
  }                                                                     \
    __VW_MATH_BINARY_LONG_DOUBLE_IMPL(func)                             \
    template <class Arg1T, class Arg2T>                                 \
    typename BinaryFunctorType<ArgArg##name##Functor,Arg1T,Arg2T>::type \
    inline operator()( Arg1T const& arg1, Arg2T const& arg2 ) const {   \
    return func( arg1, arg2 );                                          \
  }                                                                     \
  };                                                                    \
    template <class ValueT>                                               \
    struct ArgVal##name##Functor                                        \
      : UnaryReturnTernaryTemplateBind1st3rd<BinaryFunctorType,ArgArg##name##Functor,ValueT> { \
    ValueT m_val;                                                         \
    ArgVal##name##Functor(ValueT val) : m_val(val) {}                     \
    template <class ArgT>                                               \
    typename BinaryFunctorType<ArgArg##name##Functor,ArgT,ValueT>::type   \
    inline operator()( ArgT const& arg ) const {                        \
    return ArgArg##name##Functor()( arg, m_val );                       \
  }                                                                     \
  };                                                                    \
    template <class ValueT>                                               \
    struct ValArg##name##Functor                                        \
      : UnaryReturnTernaryTemplateBind1st2nd<BinaryFunctorType,ArgArg##name##Functor,ValueT> { \
    ValueT m_val;                                                         \
    ValArg##name##Functor(ValueT val) : m_val(val) {}                     \
    template <class ArgT>                                               \
    typename BinaryFunctorType<ArgArg##name##Functor,ValueT,ArgT>::type   \
    inline operator()( ArgT const& arg ) const {                        \
    return ArgArg##name##Functor()( m_val, arg );                       \
  }                                                                     \
  };                                                                    \
    using ::func;
// END __VW_BINARY_MATH_FUNCTOR

    __VW_UNARY_MATH_FUNCTOR( Fabs, fabs )
    __VW_UNARY_MATH_FUNCTOR( Acos, acos )
    __VW_UNARY_MATH_FUNCTOR( Asin, asin )
    __VW_UNARY_MATH_FUNCTOR( Atan, atan )
    __VW_UNARY_MATH_FUNCTOR( Cos, cos )
    __VW_UNARY_MATH_FUNCTOR( Sin, sin )
    __VW_UNARY_MATH_FUNCTOR( Tan, tan )
    __VW_UNARY_MATH_FUNCTOR( Cosh, cosh )
    __VW_UNARY_MATH_FUNCTOR( Sinh, sinh )
    __VW_UNARY_MATH_FUNCTOR( Tanh, tanh )
    __VW_UNARY_MATH_FUNCTOR( Exp, exp )
    __VW_UNARY_MATH_FUNCTOR( Log, log )
    __VW_UNARY_MATH_FUNCTOR( Log10, log10 )
    __VW_UNARY_MATH_FUNCTOR( Sqrt, sqrt )
    __VW_UNARY_MATH_FUNCTOR( Ceil, ceil )
    __VW_UNARY_MATH_FUNCTOR( Floor, floor )

    __VW_BINARY_MATH_FUNCTOR( Atan2, atan2 )
    __VW_BINARY_MATH_FUNCTOR( Pow, pow )
    __VW_BINARY_MATH_FUNCTOR( Hypot, hypot )

#ifndef WIN32
    __VW_UNARY_MATH_FUNCTOR( Acosh, acosh )
    __VW_UNARY_MATH_FUNCTOR( Asinh, asinh )
    __VW_UNARY_MATH_FUNCTOR( Atanh, atanh )

#ifdef VW_HAVE_EXP2
    __VW_UNARY_MATH_FUNCTOR( Exp2, exp2 )
#endif
#ifdef VW_HAVE_LOG2
    __VW_UNARY_MATH_FUNCTOR( Log2, log2 )
#endif
#ifdef VW_HAVE_TGAMMA
    __VW_UNARY_MATH_FUNCTOR( Tgamma, tgamma )
#endif
    __VW_UNARY_MATH_FUNCTOR( Lgamma, lgamma )
    __VW_UNARY_MATH_FUNCTOR( Expm1, expm1 )
    __VW_UNARY_MATH_FUNCTOR( Log1p, log1p )
    __VW_UNARY_MATH_FUNCTOR( Cbrt, cbrt )
    __VW_UNARY_MATH_FUNCTOR( Erf, erf )
    __VW_UNARY_MATH_FUNCTOR( Erfc, erfc )
    __VW_UNARY_MATH_FUNCTOR( Round, round )
      __VW_UNARY_MATH_FUNCTOR( Trunc, trunc )

      __VW_BINARY_MATH_FUNCTOR( Copysign, copysign )
    __VW_BINARY_MATH_FUNCTOR( Fdim, fdim )
#endif

// Clean up the macros we are finished using
#undef __VW_UNARY_MATH_FUNCTOR
#undef __VW_BINARY_MATH_FUNCTOR

// Real part functor
struct Arg_Real_Functor : Unary_Return_Template_Type<Make_Real>
{
    template <typename ValueT>
    ValueT operator()( const ValueT& val ) const
    {
        return val;
    }

    template <typename ValueT>
    ValueT operator()( const std::complex<ValueT>& val ) const
    {
        return std::real(val);
    }
};

    // Imaginary part functor
    struct ArgImagFunctor : UnaryReturnTemplateType<MakeReal> {
      template <class ValueT>
      ValueT operator()( ValueT const& ) const {
        return ValueT();
      }

      template <class ValueT>
      ValueT operator()( std::complex<ValueT> const& val ) const {
        return std::imag(val);
      }
    };
*/

/**
 * Absolute value functors
 * This one's tricky because we have a bunch of distinct cases
 * for integer types, floating-point types, and complex types.
 *
 * @internal This is outside ArgAbsFunctor because explicit template
 *           specialization doesn't work at class scope.
 */
template <bool IntegralN>
struct Default_Abs_Behavior
{
    template <typename ValueT>
    static int apply( ValueT val )
    {
        return std::abs( val );
    }
};

template <>
struct Default_Abs_Behavior<false>
{
    template <typename ValueT>
    static double apply( ValueT val )
    {
        return std::fabs( val );
    }
};

/**
 * Standard ABS Functor
*/
struct Arg_Abs_Functor
{
    template <typename Args>
    struct result;

    template <typename FunctorT,
              typename ValueT>
    struct result<FunctorT(ValueT)>
    {
        using type = typename std::conditional_t<std::numeric_limits<ValueT>::is_integer,
                                                int,
                                                double>;
    };

    template <typename FunctorT>
    struct result<FunctorT(float)>
    {
        typedef float type;
    };

    template <typename FunctorT>
    struct result<FunctorT(long double)>
    {
        typedef long double type;
    };

    template <typename FunctorT>
    struct result<FunctorT(int32_t)>
    {
        typedef int32_t type;
    };

    template <typename FunctorT>
    struct result<FunctorT(int64_t)>
    {
        typedef int64_t type;
    };

    template <typename FunctorT, class ValueT>
    struct result<FunctorT(std::complex<ValueT>)>
    {
        typedef ValueT type;
    };

    template <class ValueT>
    typename result<Arg_Abs_Functor(ValueT)>::type
       operator()( ValueT val ) const
    {
        return Default_Abs_Behavior<std::numeric_limits<ValueT>::is_integer>::apply(val);
    }

    float operator()( float val ) const
    {
        return ::fabsf(val);
    }

    long operator()( long val ) const
    {
        return std::labs(val);
    }
#ifdef VW_HAVE_FABSL
      inline long double operator()( long double val ) const { return ::fabsl(val); }
#endif
#ifdef VW_HAVE_LLABS
      inline long long operator()( long long val ) const { return ::llabs(val); }
#endif

    template <class ValueT>
    ValueT operator()( std::complex<ValueT> const& val ) const
    {
        return std::abs(val);
    }
};

/*
    // Complex conjugation functor
    struct ArgConjFunctor : UnaryReturnSameType {
      template <class ValueT>
      ValueT operator()( ValueT const& val ) const {
        return val;
      }

      template <class ValueT>
      std::complex<ValueT> operator()( std::complex<ValueT> const& val ) const {
        return std::conj(val);
      }
    };
*/

/**
 * Square Functor (so we don't have to always invoke POW)
 */
struct Arg_Square_Functor : Unary_Return_Same_Type
{
    template <typename ValueT>
    ValueT operator()( const ValueT& val ) const
    {
        return val*val;
    }
};

/**
 * General-purpose accumulation functor
 */
template <typename AccumulatorT,
          typename FunctorT = Arg_Arg_In_Place_Sum_Functor>
struct Accumulator : Return_Fixed_Type<AccumulatorT const&>
{
    public:

        typedef AccumulatorT value_type;

        Accumulator() = default;

        Accumulator( const FunctorT& func ) : m_func(func) {}

        Accumulator( const AccumulatorT& accum ) : m_accum(accum), m_func() {}

        Accumulator( const AccumulatorT& accum,
                     const FunctorT&     func ) : m_accum(accum), m_func(func) {}

        template <typename ArgumentT>
        AccumulatorT const& operator()( const ArgumentT& arg )
        {
            m_func(m_accum, arg);
            return m_accum;
        }

        AccumulatorT const& value() const
        {
            return m_accum;
        }

        void reset( const Accumulator& accum = AccumulatorT() )
        {
            m_accum = accum;
        }

    private:

        /// Underlying accumulator
        AccumulatorT m_accum;

        /// @brief Functor method
        FunctorT m_func;
}; //


/// Computes minimum and maximum values
template <typename ValueT>
class Min_Max_Accumulator : public Return_Fixed_Type<void>
{
    public:

        typedef std::pair<ValueT,ValueT> value_type;

        Min_Max_Accumulator() = default;

        void operator()( const ValueT& arg )
        {
            if( !m_valid )
            {
                m_minval = m_maxval = arg;
                m_valid = true;
            }
            else
            {
                if( arg < m_minval ) m_minval = arg;
                if( m_maxval < arg ) m_maxval = arg;
            }
        }

        ValueT minimum() const
        {
            if( !m_valid )
            {
                std::stringstream sout;
                sout << "Min_Max_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
            return m_minval;
        }

        ValueT maximum() const
        {
            if( !m_valid )
            {
                std::stringstream sout;
                sout << "Min_Max_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
            return m_maxval;
        }

        std::pair<ValueT,ValueT> value() const
        {
            if( !m_valid )
            {
                std::stringstream sout;
                sout << "Min_Max_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
            return std::make_pair( m_minval, m_maxval );
        }

    private:

        ValueT m_minval { 0 };
        ValueT m_maxval { 0 };
        bool m_valid { false };
};

// Note: This function modifies the input!
template <typename T>
T destructive_median( std::vector<T>& vec )
{
    int len = vec.size();

    if( len <= 0 )
    {
        std::stringstream sout;
        sout << "destructive_median: no valid samples.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }
    std::sort(vec.begin(), vec.end());
    return len%2 ? vec[len/2] : (vec[len/2 - 1] + vec[len/2]) / 2;
}

/**
 * Computes the median of the values to which it is applied.
 */
template <typename ValueT>
class Median_Accumulator : public Return_Fixed_Type<void>
{
    public:

        typedef ValueT value_type;

        void operator()( const ValueT& value )
        {
            m_values.push_back( value );
        }

        // This is to check if there are any values
        size_t size()
        {
            return m_values.size();
        }

        ValueT value()
        {
            return destructive_median(m_values);
        }

    private:

        std::vector<ValueT> m_values;
};

/**
 * Compute the normalized median absolute deviation:
 * nmad = 1.4826 * median(abs(X - median(X)))
 * Note: This function modifies the input!
 */
template <typename T>
T destructive_nmad( std::vector<T>& vec )
{
    if( vec.empty() )
    {
        std::stringstream sout;
        sout << "nmad: no valid samples.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    // Find the median. This sorts the vector, but that is not a problem.
    T median = destructive_median(vec);

    for( size_t it = 0; it < vec.size(); it++ )
    {
        vec[it] = std::abs(vec[it] - median);
    }
    median = destructive_median(vec);

    median *= 1.4826;

    return median;
}

/**
 * Compute the percentile using
 * https://en.wikipedia.org/wiki/Percentile#The_nearest-rank_method
 * Note: This function modifies the input!
 */
template <typename T>
T destructive_percentile( std::vector<T>& vec,
                          double          percentile )
{
    int len = vec.size();

    if( vec.empty() )
    {
        std::stringstream sout;
        sout << "percentile: no valid samples.";
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    if( percentile < 0 || percentile > 100 )
    {
        std::stringstream sout;
        sout << "percentile must be between 0 and 100.  Actual: " << percentile;
        tmns::log::error( sout.str() );
        throw std::runtime_error( sout.str() );
    }

    // Sorting is vital
    std::sort( vec.begin(), vec.end() );

    int index = static_cast<int>(ceil((percentile/100.0) * double(len)));

    // Account for the fact that in C++ indices start from 0
    index--;

    if( index < 0 ) index = 0;
    if( index >= len ) index = len-1;

    return vec[index];
}

/**
 * Computes the mean of the values to which it is applied.
 */
template <typename ValueT>
class Mean_Accumulator : public Return_Fixed_Type<void>
{
    public:

        typedef typename Compound_Channel_Cast<ValueT,double>::type accum_type;

        typedef accum_type value_type;

        Mean_Accumulator() = default;

        void operator()( const ValueT& value )
        {
            m_accum += value;
            m_count += 1.0;
        }

        value_type value() const
        {
            if( m_count <= 0 )
            {
                std::stringstream sout;
                sout << "Mean_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }

            return m_accum / m_count;
        }

    private:

        accum_type m_accum;
        double m_count;
};


/**
 * Computes the standard deviation of the values to which it is applied.
 *  - This implementation normalizes by num_samples, not num_samples - 1.
 */
template <typename ValueT>
class Std_Dev_Accumulator : public Return_Fixed_Type<void>
{
    public:

        typedef typename Compound_Channel_Cast<ValueT,double>::type accum_type;

        typedef accum_type value_type;

        Std_Dev_Accumulator() : mom1_accum(), mom2_accum(), num_samples() {}

        void operator()( const ValueT& arg )
        {
            mom1_accum += arg;
            mom2_accum += (accum_type) arg * arg;
            num_samples += 1.0;
        }

        /// Return the standard deviation
        value_type value() const
        {
            if( num_samples <= 0 )
            {
                std::stringstream sout;
                sout << "Std_Dev_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
            return sqrt(mom2_accum/num_samples - (mom1_accum/num_samples)*(mom1_accum/num_samples));
        }

        /// Return the mean
        value_type mean() const
        {
            if( num_samples <= 0 )
            {
                std::stringstream sout;
                sout << "Std_Dev_Accumulator: no valid samples.";
                tmns::log::error( sout.str() );
                throw std::runtime_error( sout.str() );
            }
            return mom1_accum / num_samples;
        }

    private:

        accum_type mom1_accum, mom2_accum;
        double num_samples;
};


/**
 * Compute the standard deviation of values in a fixed size list.
 *  - When a new value is added, the oldest value is removed from the statistics.
 *  - This implementation normalizes by num_samples, not num_samples - 1.
 */
class Std_Dev_Sliding_Functor
{
    public:

        /**
         * Constructor set with the sliding window size.
         */
        Std_Dev_Sliding_Functor( const size_t max_size )
            : m_max_size(max_size),
              m_mean(0),
              m_squared(0) {}

        /**
         * Implement push with the standard functor interface
         */
        void operator()( double new_val )
        {
            push(new_val);
        }

        /**
         * Add a new value and eject the oldest value.
         */
        void push( double new_val )
        {

            // If we grew larger than the size limit remove the oldest value
            if( m_values.size() >= m_max_size )
            {
                pop();
            }

            // Now incorporate the newest value
            m_values.push(new_val); // Record the new value
            double count = static_cast<double>(m_values.size());

            // Update the statistics to add in the new value
            double delta = new_val - m_mean;
            m_mean += delta/count;

            double delta2 = new_val - m_mean;
            m_squared += delta*delta2;
        }

        /**
         * Remove the oldest value
         */
        void pop()
        {
            if( m_values.empty() )
            {
                return;
            }

            double old_val = m_values.front();
            double count   = static_cast<double>(m_values.size());
            m_values.pop();

            // Update the statistics to account for removing the old value
            double shrunk_count = count - 1.0;
            double new_mean = (count*m_mean - old_val)/shrunk_count;

            m_squared -= (old_val - m_mean) * (old_val - new_mean);
            m_mean = new_mean;
        };

        /**
         * Compute the standard deviation of all current values.
         */
        double get_std_dev()
        {
            double count = static_cast<double>( m_values.size() );
            if( count < 2.0 )
            {
                return 0;
            }
            return std::sqrt( m_squared /count );
        }

    private:

        /// @brief Max Number of Values to Store
        size_t m_max_size;

        /// @brief Store Current Values
        std::queue<double> m_values;

        /// @brief Store the Mean
        double m_mean { 0 };

        /// @brief Store the Sum of Differences Squared
        double m_squared { 0 };

}; // End class Std_Dev_Sliding_Functor

} // End of tmns::math namespace