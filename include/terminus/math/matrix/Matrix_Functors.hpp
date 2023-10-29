/**
 * @file    Matrix_Functors.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Terminus Libraries
#include "Matrix_Traits.hpp"

// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::math {

/**
 * @class Matrix_Unary_Functor<MatrixT,FunctorT>
 * An unary elementwise matrix function class.
 */
template <typename MatrixT,
          typename FunctorT>
class Matrix_Unary_Functor : public Matrix_Base<Matrix_Unary_Functor<MatrixT,FunctorT> >
{
    public:
    
        // Applied value type
        using value_type = std::invoke_result_t<FunctorT, typename MatrixT::value_type>;

        /// Reference Result Type
        using reference_type = value_type;

        /// Const Reference Type
        using const_reference_type = value_type;
        
        /**
         * Constructor
         */
        Matrix_Unary_Functor( const MatrixT& matrix )
            : m_matrix( matrix )
        {}

        /**
         * Argument Parameterized Constructor
         */
        template <typename Argument1>
        Matrix_Unary_Functor( const MatrixT& matrix,
                              Argument1      a1 )
            : m_matrix( matrix ),
              m_functor( a1 )
        {}

        /**
         * Get underlying matrix
         */
        MatrixT const& child() const
        {
            return m_matrix;
        }

        /**
         * Get matrix rows
         */
        size_t rows() const
        {
            return m_matrix.rows();
        }

        /**
         * Get matrix columns
         */
        size_t cols() const
        {
            return m_matrix.cols();
        }

        /**
         * Function Operator
         */
        reference_type operator()( size_t i, size_t j ) const
        {
            return m_functor( child()( i, j ) );
        }

        /**
         * Return all diagonal components
         */
        VectorN<value_type> diagonal() const
        {
            VectorN<value_type> output( std::min( rows(), cols() ) );
            for( size_t x = 0; x < output.size(); x++ )
            {
                output[x] = this->operator()( x, x );
            }
            return output;
        }

        /**
         * Iterator overload
        */
        class Iterator : public boost::iterator_facade<Iterator,
                                                       value_type,
                                                       boost::random_access_traversal_tag,
                                                       value_type>
        {
            public:
                
                /**
                 * Constructor
                 */
                Iterator( const typename MatrixT::const_iter_t& iter,
                          const FunctorT&                       functor )
                    : m_iterator( iter ),
                      m_functor( functor )
                {}

            private:
                
                /**
                 * Equal Operator
                 */
                bool equal( const Iterator& iter ) const
                {
                    return m_iterator == iter.m_iterator;
                }

                typename Iterator::difference_type distance_to( const Iterator& iter ) const
                {
                    return iter.m_iterator - m_iterator;
                }

                void increment()
                {
                    ++m_iterator;
                }

                void decrement()
                {
                    --m_iterator;
                }

                void advance( typename Iterator::difference_type n )
                {
                    m_iterator += n;
                }

                typename Iterator::reference dereference() const
                {
                    return m_functor( *m_iterator );
                }

                friend class boost::iterator_core_access;

                /// @brief Internal Iterator Position / Offset
                typename MatrixT::const_iter_t m_iterator;

                /// @brief Functor to apply
                FunctorT m_functor;

        }; // End of Iterator class

        using iter_t = Iterator;
        using const_iter_t = iter_t;

        iter_t begin() const
        {
            return Iterator( child().begin(), m_functor );
        }

        const_iter_t end  () const
        {
            return Iterator( child().end(), m_functor );
        }

    private:

        /// @brief Underlying matrix reference
        MatrixT const& m_matrix;

        /// @brief Functor to apply
        FunctorT m_functor;
};

/**
 * Matrix Row Traits
 */
template <typename MatrixT,
          typename FunctorT>
struct Matrix_Rows<Matrix_Unary_Functor<MatrixT,FunctorT> >
{
    static const size_t value = Matrix_Rows<MatrixT>::value;
};

/**
 * Matrix Column Traits
*/
template <typename MatrixT,
          typename FunctorT>
struct Matrix_Cols<Matrix_Unary_Functor<MatrixT,FunctorT> >
{
    static const size_t value = Matrix_Cols<MatrixT>::value;
};


/**
 * @class Matrix_Binary_Functor<Matrix1T,Matrix2T,FunctorT>
 * A binary elementwise matrix function class.
 */
template <typename Matrix1T,
          typename Matrix2T,
          typename FunctorT>
class Matrix_Binary_Functor : public Matrix_Base<Matrix_Binary_Functor<Matrix1T,Matrix2T,FunctorT> >
{
    public:

        /// Value Type
        using value_type = std::invoke_result_t<FunctorT, 
                                                typename Matrix1T::value_type,
                                                typename Matrix2T::value_type>;

        /// Reference Type
        using reference_type = value_type;

        /// @brief Const Reference Type
        using const_reference_type = value_type;

        /**
         * Constructor
         */
        Matrix_Binary_Functor( const Matrix1T& matrix1,
                               const Matrix2T& matrix2 )
            : m_matrix1( matrix1 ),
              m_matrix2( matrix2 ),
              m_functor()
        {
            if( matrix1.rows() != matrix2.rows() ||
                matrix1.cols() != matrix2.cols() )
            {
                std::cerr << "error: Matrices don't have a matching size. Mat 1 ( "
                          << matrix1.rows() << ", " << matrix1.cols() << ") vs Mat 2 ("
                          << matrix2.rows() << ", " << matrix2.cols() << ").  Expect a crash.";
            }
        }

        /**
         * Argument-Capable Constructor
         */
        template <typename Argument1>
        Matrix_Binary_Functor( const Matrix1T& matrix1,
                               const Matrix2T& matrix2,
                               Argument1       a1 )
            : m_matrix1( matrix1 ),
              m_matrix2( matrix2 ),
              m_functor( a1 )
        {
            if( matrix1.rows() != matrix2.rows() ||
                matrix1.cols() != matrix2.cols() )
            {
                std::cerr << "error: Matrices don't have a matching size. Mat 1 ( "
                          << matrix1.rows() << ", " << matrix1.cols() << ") vs Mat 2 ("
                          << matrix2.rows() << ", " << matrix2.cols() << ").  Expect a crash.";
            }
        }

        /**
         * Get first matrix
         */
        Matrix1T const& child1() const
        {
            return m_matrix1;
        }
    
        /**
         * Get second matrix
         */
        Matrix2T const& child2() const
        {
            return m_matrix2;
        }

        /**
         * Get matrix rows
         */
        size_t rows() const
        {
            return m_matrix1.rows();
        }

        /**
         * Get matrix columns
         */
        size_t cols() const
        {
            return m_matrix1.cols();
        }

        /**
         * Apply the functor
         */
        reference_type operator()( size_t i, size_t j ) const
        {
            return m_functor( child1()( i, j ),
                              child2()( i, j ) );
        }

        /**
         * Return all diagonal components
         */
        VectorN<value_type> diagonal() const
        {
            VectorN<value_type> output( std::min( rows(), cols() ) );
            for( size_t x = 0; x < output.size(); x++ )
            {
                output[x] = this->operator()( x, x );
            }
            return output;
        }

        /**
         * Iterator Overload
        */
        class Iterator : public boost::iterator_facade<Iterator,
                                                       value_type,
                                                       boost::random_access_traversal_tag,
                                                       value_type>
        {
            public:
                
                /**
                 * Constructor
                */
                Iterator( typename Matrix1T::const_iter_t const& iter1,
                          typename Matrix2T::const_iter_t const& iter2,
                          const FunctorT&                        functor )
                    : m_iter1( iter1 ),
                      m_iter2( iter2 ),
                      m_functor( functor )
                {}

            private:

                // Give boost internal access
                friend class boost::iterator_core_access;

                bool equal( Iterator const& iter ) const
                {
                    return ( m_iter1 == iter.m_iter1 ) && ( m_iter2 == iter.m_iter2 );
                }
            
               /**
                 * Compute the distance to the other iterator
                */
                typename Iterator::difference_type distance_to( Iterator const &iter ) const
                {
                    return iter.m_iter1 - m_iter1;
                }
            
                /**
                 * Increment the iterator
                 */
                void increment()
                {
                    ++m_iter1;
                    ++m_iter2;
                }
            
                /**
                 * Decrement the iterator
                 */
                void decrement()
                {
                    --m_iter1;
                    --m_iter2;
                }
            
                /**
                 * Increment the iterator by the requested amount
                 */
                void advance( typename Iterator::difference_type n )
                {
                    m_iter1 += n;
                    m_iter2 += n;
                }
            
                /**
                 * Dereference the iterator, applying the functor
                */
                typename Iterator::reference dereference() const
                {
                    return m_functor( *m_iter1,
                                      *m_iter2 );
                }

                /// @brief Reference to first matrix
                typename Matrix1T::const_iter_t m_iter1;

                /// @brief Reference to second matrix
                typename Matrix2T::const_iter_t m_iter2;

                /// @brief Functor to apply to matrices
                FunctorT m_functor;

        }; // End of iterator class

        /// @brief  Iterator Type
        using iter_t = Iterator;

        /// @brief Const Iterator Type
        using const_iter_t = Iterator;
        
        /**
         * Starting Iterator Position
        */
        iter_t begin() const
        {
            return iter_t( child1().begin(),
                           child2().begin(),
                           m_functor );
        }
    
        /**
         * End Iterator Position
         */
        iter_t end() const
        {
            return iter_t( child1().end(),
                           child2().end(),
                           m_functor );
        }


    private:

        /// @brief First Matrix
        Matrix1T const& m_matrix1;

        /// @brief Second Matrix
        Matrix2T const& m_matrix2;

        /// @brief Functor to be applied
        FunctorT m_functor;
};

/**
 * Matrix Row Trait
 */
template <typename Matrix1T,
          typename Matrix2T,
          typename FunctorT>
struct Matrix_Rows<Matrix_Binary_Functor<Matrix1T,Matrix2T,FunctorT>>
{
    static const size_t value = (Matrix_Rows<Matrix1T>::value != 0 ) ? 
                                    ( Matrix_Rows<Matrix1T>::value ) : 
                                    ( Matrix_Rows<Matrix2T>::value );
};

/**
 * Matrix Column Trait
 */
template <typename Matrix1T,
          typename Matrix2T,
          typename FunctorT>
struct Matrix_Cols<Matrix_Binary_Functor<Matrix1T,Matrix2T,FunctorT>>
{
    static const size_t value = (Matrix_Cols<Matrix1T>::value != 0 ) ? 
                                    ( Matrix_Cols<Matrix1T>::value ) : 
                                    ( Matrix_Cols<Matrix2T>::value );
};

} // End of tmns::math namespace