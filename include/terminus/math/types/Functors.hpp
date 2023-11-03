/**
 * @file    Functors.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// Terminus Math Libraries
#include "Type_Deduction.hpp"

namespace tmns::math {

/**
 * Forces the functor to return the same type as the argument
*/
struct Unary_Return_Same_Type
{
    template <typename Arguments>
    struct result;

    template <typename ChannelT, 
              typename FunctorT>
    struct result<FunctorT(ChannelT)>
    {
        typedef ChannelT type;
    };
}; // End of Unary_Return_Same_Type struct

/**
 * A mix-in specifying that a functor is a unary functor
 * whose return type is determined by the given traits
 *  template class.
 */
template <template<typename> class ResultT>
struct Unary_Return_Template_Type
{
    template <typename Arguments>
    struct result {};

    template <typename FunctorT,
              typename ArgumentT>
    struct result<FunctorT(ArgumentT)>
    {
        typedef typename ResultT<ArgumentT>::type type;
    };
}; // End of Unary_Return_Template_Type

/**
 * A mix-in specifying that a functor is a binary functor whose
 * return type is determined by the given traits template class.
 */
template <template<typename,typename> class ResultT>
struct Binary_Return_Template_Type
{
    template <typename Arguments> 
    struct result {};

    template <typename FunctorT,
              typename Argument1T,
              typename Argument2T>
    struct result<FunctorT(Argument1T,Argument2T)>
    {
        typedef typename ResultT<Argument1T,Argument2T>::type type;
    };
}; // End of Binary_Return_Template_Type struct

/**
 * Forces functors to use a particular type
*/
template <typename T>
struct Return_Fixed_Type
{
    template <typename Args>
    struct result
    {
        typedef T type;
    }; // End of result struct
}; // End of Return_Fixed_Type class

/**
 * A mix-in specifying that a binary functor always returns
 * the same type as its first argument.
 */
struct Binary_Return_1st_Type
{
    template <typename Arguments>
    struct result;
    
    template <typename FunctorT,
              typename Argument1T,
              typename Argument2T>
    struct result<FunctorT(Argument1T,Argument2T)> {
        typedef Argument1T type;
    };
}; // End of Binary_Return_1st_Type struct

/**
 * A mix-in specifying that a binary functor always returns
 * the same type as its second argument. 
 */
struct Binary_Return_2nd_Type
{
    template <typename Arguments> struct result;
    template<typename FunctorT,
             typename Argument1T,
             typename Argument2T>
    struct result<FunctorT(Argument1T,Argument2T)>
    {
        typedef Argument2T type;
    };
}; // End of Binary_Return_2nd_Type struct


/**
 * A mix-in specifying that a functor is an unary functor whose return type
 * is determined by the given binary type function with the given first argument.
 */
template <template<typename,typename> 
            typename ResultT, 
            typename ValueT>
struct Unary_Return_Binary_Template_Bind_1st
{
    template <typename Arguments>
    struct result { typedef void type; };
    
    template <typename FunctorT,
              typename ArgumentT>
    struct result<FunctorT(ArgumentT)>
    {
        typedef typename ResultT<ValueT,ArgumentT>::type type;
    };
}; // End of Unary_Return_Binary_Template_Bind_1st

/**
 * A mix-in specifying that a functor is an unary functor whose return 
 * type is determined by the given binary type function with the given 
 * second argument.
 */
template <template<typename,typename>
            typename ResultT,
            typename ValueT>
struct Unary_Return_Binary_Template_Bind_2nd
{
    template <typename Arguments>
    struct result
    {
        typedef void type;
    };
    
    template <typename FunctorT,
              typename ArgumentT>
    struct result<FunctorT(ArgumentT)>
    {
        typedef typename ResultT<ArgumentT,ValueT>::type type;
    };
}; // End of Unary_Return_Binary_Template_Bind_2nd struct

/**
 * A mix-in specifying that a functor is a binary functor whose return 
 * type is determined by the given ternary type function with the given
 * first argument.
 */
template <template<typename,typename,typename> 
                typename ResultT,
                typename ValueT>
struct Binary_Return_Ternary_Template_Bind_1st
{
    template <typename Arguments>
    struct result
    {
        typedef void type;
    };
    
    template <typename FunctorT,
              typename Argument1T,
              typename Argument2T>
    struct result<FunctorT(Argument1T,Argument2T)>
    {
        typedef typename ResultT<ValueT,Argument1T,Argument2T>::type type;
    };
}; // End of Binary_Return_Ternary_Template_Bind_1st struct

/**
 * A mix-in specifying that a functor is an unary functor 
 * whose return type is determined by the given ternary
 * type function with the given first and second arguments.
 */
template <template<typename,typename,typename> 
                        typename ResultT,
                        typename Value1T,
                        typename Value2T>
struct Unary_Return_Ternary_Template_Bind_1st_2nd
{
    template <typename Arguments>
    struct result
    {
        typedef void type;
    };
    
    template <typename FunctorT,
              typename ArgumentT>
    struct result<FunctorT(ArgumentT)>
    {
        typedef typename ResultT<Value1T,Value2T,ArgumentT>::type type;
    };
}; // End of Unary_Return_Ternary_Template_Bind_1st_2nd

/**
 * A mix-in specifying that a functor is an unary functor whose return
 * type is determined by the given ternary type function with the given
 * first and third arguments.
 */
template <template<typename,typename,typename> 
                typename ResultT,
                typename Value1T,
                typename Value3T>
struct Unary_Return_Ternary_Template_Bind_1st_3rd
{
    template <typename Arguments>
    struct result
    {
        typedef void type;
    };

    template <typename FunctorT,
              typename ArgumentT>
    struct result<FunctorT(ArgumentT)>
    {
      typedef typename ResultT<Value1T,ArgumentT,Value3T>::type type;
    };
}; // End of Unary_Return_Ternary_Template_Bind_1st_3rd struct

/***************************************************/
/*          Arithmetic Operator Functors           */
/***************************************************/

/**
 * Functor which negates the input argument
*/
struct Arg_Negation_Functor : Unary_Return_Same_Type
{
    template <typename ArgumentT>
    typename result<Arg_Negation_Functor(ArgumentT)>::type
        operator()( const ArgumentT& arg ) const
    {
        return -arg;
    }
}; // End of Argument_Negation_Functor struct

/**
 * Functor which performs the binary sum of 2 arguments
*/
struct Arg_Arg_Sum_Functor : Binary_Return_Template_Type<Sum_Type>
{
    template <typename Argument1T,
              typename Argument2T >
    typename Sum_Type<Argument1T,Argument2T>::type
        operator()( const Argument1T& arg1,
                    const Argument2T& arg2 ) const
    {
        return arg1 + arg2;
    }

    /**
     * Get name
     */
    static std::string name()
    {
        std::stringstream sout;
        sout << "Arg_Arg_Sum_Functor";
        return sout.str();
    }
    
}; // End of Arg_Arg_Sum_Functor struct

/**
 * Functor which performs the unary sum of an argument and value
*/
template <typename ValueT>
class Arg_Val_Sum_Functor : Unary_Return_Binary_Template_Bind_2nd<Sum_Type,ValueT>
{
    public:

        Arg_Val_Sum_Functor( const ValueT& value )
          : m_value( value )
        {}

        template <typename ArgumentT>
        typename Sum_Type<ArgumentT,ValueT>::type 
            operator()( const ArgumentT& arg ) const
        {
            return arg + m_value;
        }
    
    private:

        const ValueT m_value;
}; // End of Arg_Val_Sum_Functor class

/**
 * Unary sum of a value and an argument
*/
template <typename ValueT>
class Val_Arg_Sum_Functor : Unary_Return_Binary_Template_Bind_1st<Sum_Type,ValueT>
{
    public:

        Val_Arg_Sum_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        typename Sum_Type<ValueT,ArgumentT>::type 
            operator()( const ArgumentT& arg ) const
        {
            return m_value + arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Sum_Functor class

/**
 * Binary in-place sum of two arguments
 */
struct Arg_Arg_In_Place_Sum_Functor : Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Argument1T& operator()( Argument1T&       arg1,
                            const Argument2T& arg2 ) const
    {
        return arg1 = (Argument1T)( arg1 + arg2 );
    }
}; // End of Arg_Arg_In_Place_Sum_Functor struct

/**
 * Unary in-place sum of an argument and a value
 */
template <typename ValueT>
class Arg_Val_In_Place_Sum_Functor : Unary_Return_Same_Type
{
    public:

        Arg_Val_In_Place_Sum_Functor( const ValueT& value )
         : m_value( value ) {}

        template <typename ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return arg = (ArgumentT)( arg + m_value );
        }
    
    private:
        const ValueT m_value;
}; // End of Arg_Val_In_Place_Sum_Functor class

/**
 * Binary difference of two arguments
*/
struct Arg_Arg_Difference_Functor : Binary_Return_Template_Type<Difference_Type>
{
    template <typename Argument1T,
              typename Argument2T>
    typename Difference_Type<Argument1T,Argument2T>::type
        operator()( const Argument1T& arg1,
                    const Argument2T& arg2 ) const
    {
        return arg1 - arg2;
    }
}; // End of Arg_Arg_Difference_Functor struct

  
/**
 * Unary difference of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Difference_Functor : Unary_Return_Binary_Template_Bind_2nd<Difference_Type,ValueT>
{
    public:
        Arg_Val_Difference_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        typename Difference_Type<ArgumentT,ValueT>::type
            operator()( const ArgumentT& arg ) const
        {
            return arg - m_value;
        }

    private:
        const ValueT m_value;

}; // End of Arg_Val_Difference_Functor class

/**
 * Unary difference of a value and an argument
*/
template <typename ValueT>
class Val_Arg_Difference_Functor : Unary_Return_Binary_Template_Bind_1st<Difference_Type,ValueT>
{
    public:
        
        Val_Arg_Difference_Functor( const ValueT& value )
         : m_value( value ) {}

        template <typename ArgumentT>
        typename Difference_Type<ValueT,ArgumentT>::type
            operator()( const ArgumentT& arg ) const
        {
            return m_value - arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Difference_Functor class

/**
 * Binary in-place difference of two arguments
 */
struct Arg_Arg_In_Place_Difference_Functor : Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Argument1T& operator()( Argument1T&       arg1, 
                            const Argument2T& arg2 ) const
    {
        return arg1 = (Argument1T)( arg1 - arg2 );
    }
}; // End of Arg_Arg_In_Place_Difference_Functor struct

/**
 * Unary in-place difference of an argument and a value
 */
template <typename ValueT>
class Arg_Val_In_Place_Difference_Functor : Unary_Return_Same_Type
{
    public:
        Arg_Val_In_Place_Difference_Functor( const ValueT& value )
          : m_value( value ) {}
        
        template <typename ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return arg = (ArgumentT)( arg - m_value );
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_In_Place_Difference_Functor class

/**
 * Binary product of two arguments
*/
struct Arg_Arg_Product_Functor : Binary_Return_Template_Type<Product_Type>
{
    template <typename Argument1T,
              typename Argument2T>
    typename Product_Type<Argument1T,Argument2T>::type
        operator()( const Argument1T& arg1,
                    const Argument2T& arg2 ) const
    {
        return arg1 * arg2;
    }
}; // End of Arg_Arg_Product_Functor struct

/**
 * Unary product of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Product_Functor : Unary_Return_Binary_Template_Bind_2nd<Product_Type,ValueT>
{
    public:
        Arg_Val_Product_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        typename Product_Type<ArgumentT,ValueT>::type
            operator()( const ArgumentT& arg ) const
        {
            return arg * m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Product_Functor class

/**
 * Unary product of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Product_Functor : Unary_Return_Binary_Template_Bind_1st<Product_Type,ValueT>
{
    public:
        Val_Arg_Product_Functor( const ValueT& value )
          : m_value( value ) {}

        template <typename ArgumentT>
        typename Product_Type<ValueT,ArgumentT>::type
            operator()( const ArgumentT& arg ) const
        {
            return m_value * arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Product_Functor class

/**
 * Binary in-place product of two arguments
 */
struct Arg_Arg_In_Place_Product_Functor : Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Argument1T& operator()( Argument1T&       arg1, 
                            const Argument2T& arg2 ) const
    {
        return arg1 = (Argument1T)( arg1 * arg2 );
    }
}; // End of Arg_Arg_In_Place_Product_Functor struct

/**
 * Unary in-place product of an argument and a value
 */
template <typename ValueT>
class Arg_Val_In_Place_Product_Functor : Unary_Return_Same_Type
{
    public:
        Arg_Val_In_Place_Product_Functor( const ValueT& value )
          : m_value( value ) {}

        template <class ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return arg = (ArgumentT)( arg * m_value );
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_In_Place_Product_Functor class

/**
 * Binary quotient of two arguments
 */
struct Arg_Arg_Quotient_Functor : Binary_Return_Template_Type<Quotient_Type>
{
    template <typename Argument1T,
              typename Argument2T>
    typename Quotient_Type<Argument1T,Argument2T>::type
        operator()( const Argument1T& arg1,
                    const Argument2T& arg2 ) const
    {
        return arg1 / arg2;
    }
}; // End of Arg_Arg_Quotient_Functor struct

/**
 * Unary quotient of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Quotient_Functor : Unary_Return_Binary_Template_Bind_2nd<Quotient_Type,ValueT>
{
    public:
        Arg_Val_Quotient_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        typename Quotient_Type<ArgumentT,ValueT>::type
            operator()( const ArgumentT& arg ) const
        {
            return arg / m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Quotient_Functor class

/**
 * Unary quotient of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Quotient_Functor : Unary_Return_Binary_Template_Bind_1st<Quotient_Type,ValueT>
{
    public:
        Val_Arg_Quotient_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        typename Quotient_Type<ValueT,ArgumentT>::type
            operator()( const ArgumentT& arg ) const
        {
            return m_value / arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Quotient_Functor class

/**
 * Binary in-place quotient of two arguments
 */
struct Arg_Arg_In_Place_Quotient_Functor : Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Argument1T& operator()( Argument1T&       arg1,
                            const Argument2T& arg2 ) const
    {
        return arg1 = (Argument1T)( arg1 / arg2 );
    }
}; // End of Arg_Arg_In_Place_Quotient_Functor struct

/**
 * Unary in-place quotient of an argument and a value
 */
template <typename ValueT>
class Arg_Val_In_Place_Quotient_Functor : Unary_Return_Same_Type
{
    public:
        Arg_Val_In_Place_Quotient_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        ArgumentT& operator()( ArgumentT& arg ) const
        {
            return arg = (ArgumentT)( arg / m_value );
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_In_Place_Quotient_Functor class

/**
 * Safe binary quotient of two arguments
 */
struct Arg_Arg_Safe_Quotient_Functor : Binary_Return_Template_Type<Quotient_Type>
{
    template <typename Argument1T,
              typename Argument2T>
    typename Quotient_Type<Argument1T,Argument2T>::type
        operator()( const Argument1T& arg1,
                    const Argument2T& arg2 ) const
    {
        if( arg2 == Argument2T() )
        {
            return typename Quotient_Type<Argument1T,Argument2T>::type();
        }
        else
        {
            return ( arg1 / arg2 );
        }
    }
}; // End of Arg_Arg_Safe_Quotient_Functor struct

/**
 * Safe unary quotient of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Safe_Quotient_Functor : Unary_Return_Binary_Template_Bind_2nd<Quotient_Type,ValueT>
{
    public:
    
        Arg_Val_Safe_Quotient_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        typename Quotient_Type<ArgumentT,ValueT>::type
            operator()( const ArgumentT& arg ) const
        {
            if ( m_value == ValueT() )
            {
                return typename Quotient_Type<ArgumentT,ValueT>::type();
            }
            else
            {
                return ( arg / m_value );
            }
        }
    private:
        const ValueT m_value;
}; // End of Arg_Val_Safe_Quotient_Functor class

/**
 * Safe unary quotient of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Safe_Quotient_Functor : Unary_Return_Binary_Template_Bind_1st<Quotient_Type,ValueT>
{
    public:
        Val_Arg_Safe_Quotient_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        typename Quotient_Type<ValueT,ArgumentT>::type operator()( const ArgumentT& arg ) const
        {
            if ( arg==ArgumentT() )
            {
                return typename Quotient_Type<ValueT,ArgumentT>::type();
            }
            else
            {
                return ( m_value / arg );
            }
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Safe_Quotient_Functor class

/**
 * Safe binary in-place quotient of two arguments
 */
struct Arg_Arg_In_Place_Safe_Quotient_Functor : Binary_Return_1st_Type
{
    template <typename Argument1T,
              typename Argument2T>
    Argument1T& operator()( Argument1T&       arg1,
                            const Argument2T& arg2 ) const
    {
        if ( arg2 == Argument2T() )
        {
            return arg1 = Argument1T();
        }
        else
        {
            return arg1 = (Argument1T)( arg1 / arg2 );
        }
    }
}; // End of Arg_Arg_In_Place_Safe_Quotient_Functor struct

/**
 * Safe unary in-place quotient of an argument and a value
 */
template <typename ValueT>
class Arg_Val_In_Place_Safe_Quotient_Functor : Unary_Return_Same_Type
{
    public:
        Arg_Val_In_Place_Safe_Quotient_Functor( const ValueT& value )
           : m_value( value ) {}

    template <typename ArgumentT>
    ArgumentT& operator()( ArgumentT& arg ) const
    {
        if ( m_value == ValueT() )
        {
            return arg=ArgumentT();
        }
        else
        {
            return arg = (ArgumentT)( arg / m_value );
        }
    }

    private:
        const ValueT m_value;
}; // End of Arg_Val_In_Place_Safe_Quotient_Functor class

/**
 * Binary equality operator of two arguments
 */
struct Arg_Arg_Equality_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T,
              typename Argument2T>
    bool operator()( const Argument1T& arg1, 
                     const Argument2T& arg2 ) const
    {
        return arg1 == arg2;
    }
}; // End of Arg_Arg_Equality_Functor struct

/**
 * Unary equality operator of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Equality_Functor : Return_Fixed_Type<bool>
{
    public:
        Arg_Val_Equality_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg == m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Equality_Functor class

/**
 * Unary equality operator of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Equality_Functor : Return_Fixed_Type<bool>
{
    public:
        Val_Arg_Equality_Functor( const ValueT& value ) : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return m_value == arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Equality_Functor class

/**
 * Binary inequality operator of two arguments
 */
struct Arg_Arg_Inequality_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T, typename Argument2T>
    bool operator()( const Argument1T& arg1,
                     const Argument2T& arg2 ) const
    {
        return arg1 != arg2;
    }
}; // End of Arg_Arg_Inequality_Functor struct

/**
 * Unary inequality operator of an argument and a value
 */
template <typename ValueT>
struct Arg_Val_Inequality_Functor : Return_Fixed_Type<bool>
{
    public:
        Arg_Val_Inequality_Functor( const ValueT& value )
          : m_value( value ) {}

        template <class ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg != m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Inequality_Functor class

/**
 * Unary inequality operator of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Inequality_Functor : Return_Fixed_Type<bool>
{
    public:
        Val_Arg_Inequality_Functor( const ValueT& value )
             : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return m_value != arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Inequality_Functor class

/**
 * Binary less-than operator of two arguments
 */
struct Arg_Arg_Less_Than_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T,
              typename Argument2T>
    bool operator()( const Argument1T& arg1,
                     const Argument2T& arg2 ) const
    {
        return arg1 < arg2;
    }
}; // End of Arg_Arg_Less_Than_Functor struct

/**
 * Unary less-than operator of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Less_Than_Functor : Return_Fixed_Type<bool>
{
    public:
    
        Arg_Val_Less_Than_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg < m_value;
        }
    
    private:
        const ValueT m_value;

}; // End of Arg_Val_Less_Than_Functor class

/**
 * Unary less-than operator of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Less_Than_Functor : Return_Fixed_Type<bool>
{
    public:
        Val_Arg_Less_Than_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( ArgumentT const& arg ) const
        {
            return m_value < arg;
        }

      private:
            const ValueT m_value;
}; // End of Val_Arg_Less_Than_Functor class

/**
 * Binary less-than-or-equal operator of two arguments
 */
struct Arg_Arg_Less_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T,
              typename Argument2T>
    bool operator()( const Argument1T& arg1,
                     const Argument2T& arg2 ) const
    {
        return arg1 <= arg2;
    }
}; // End of Arg_Arg_Less_Than_Or_Equal_Functor struct

/**
 * Unary less-than-or-equal operator of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Less_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    public:
        Arg_Val_Less_Than_Or_Equal_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg <= m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Less_Than_Or_Equal_Functor class

/**
 * Unary less-than-or-equal operator of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Less_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    public:
        Val_Arg_Less_Than_Or_Equal_Functor( const ValueT& value )
           : m_value( value ) {}

        template <class ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return m_value <= arg;
        }
    
    private:
        const ValueT m_value;    
}; // End of Val_Arg_Less_Than_Or_Equal_Functor class

/**
 * Binary greater-than operator of 2 arguments
*/
struct Arg_Arg_Greater_Than_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T,
              typename Argument2T>
    bool operator()( const Argument1T& arg1,
                     const Argument2T& arg2 ) const
    {
        return arg1 > arg2;
    }
}; // End of Arg_Arg_Greater_Than_Functor struct

/**
 * Unary greater-than operator between an argument and a value
*/
template <typename ValueT>
class Arg_Val_Greater_Than_Functor : Return_Fixed_Type<bool>
{
    public:

        Arg_Val_Greater_Than_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg > m_value;
        }

    private:
        const ValueT m_value;
}; // End of Arg_Val_Greater_Than_Functor class

/**
 * Unary greater-than operator of a value and an argument
 */
template <typename ValueT>
class Val_Arg_Greater_Than_Functor : Return_Fixed_Type<bool>
{
    public:
        Val_Arg_Greater_Than_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return m_value > arg;
        }

    private:
        const ValueT m_value;
}; // End of Val_Arg_Greater_Than_Functor class

/**
 * Binary greater-than-or-equal operator of two arguments
 */
struct Arg_Arg_Greater_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    template <typename Argument1T,
              typename Argument2T>
    bool operator()( const Argument1T& arg1,
                     const Argument2T& arg2 ) const
    {
        return arg1 >= arg2;
    }
}; // End of Arg_Arg_Greater_Than_Or_Equal_Functor

/**
 * Unary greater-than-or-equal operator of an argument and a value
 */
template <typename ValueT>
class Arg_Val_Greater_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    public:
        Arg_Val_Greater_Than_Or_Equal_Functor( const ValueT& value )
           : m_value( value ) {}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return arg >= m_value;
        }
        
    private:
        const ValueT m_value;
}; // End of Arg_Val_Greater_Than_Or_Equal_Functor class

/**
 * Greater than or equal operator between a Value and an Argument
*/
template <typename ValueT>
class Val_Arg_Greater_Than_Or_Equal_Functor : Return_Fixed_Type<bool>
{
    public:

        Val_Arg_Greater_Than_Or_Equal_Functor( const ValueT& value )
          : m_value( value ){}

        template <typename ArgumentT>
        bool operator()( const ArgumentT& arg ) const
        {
            return m_value >= arg;
        }

    private:
    
        const ValueT m_value;
}; // End of Val_Arg_Greater_Than_Or_Equal_Functor class

/**
 * Functor which does nothing.
*/
struct NOP
{
    template <typename T>
    void operator()(T) const {}
}; // End of NOP Struct

} // End of tmns::math namespace