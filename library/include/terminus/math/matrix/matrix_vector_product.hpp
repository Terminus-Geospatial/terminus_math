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
 * @file    matrix_vector_product.hpp
 * @author  Marvin Smith
 * @date    10/13/2023
*/
#pragma once

// Terminus Libraries
#include "../vector/VectorN.hpp"
#include "../vector/Vector_Proxy.hpp"
#include "Matrix_Operations.hpp"
#include "Matrix_Proxy.hpp"

namespace tmns::math {

/**
 * A class representing a product of a matrix and a vector.
 */
template <typename MatrixT,
          typename VectorT,
          bool TransposeN>
class Matrix_Vector_Product : public Vector_Base<Matrix_Vector_Product<MatrixT,VectorT,TransposeN> >
{
    public:

        using value_type = typename Product_Type<typename MatrixT::value_type,
                                                 typename VectorT::value_type>::type;

        using reference_type = value_type;

        using const_reference_type = value_type;

        /**
         * Matrix Vector Product Constructor
         */
        Matrix_Vector_Product( const MatrixT& m,
                               const VectorT& v )
            : m_matrix(m),
              m_vector(v){}

        /**
         * Get the size of the matrix/vector product
         */
        size_t size() const
        {
            return ( TransposeN ) ? ( m_matrix.cols() ) : ( m_matrix.rows() );
        }

        reference_type operator()( size_t i ) const requires ( TransposeN )
        {
            return VectorT::dot( select_col( m_matrix, i ),
                                 m_vector );
        }

        reference_type operator()( size_t i ) const requires ( !TransposeN )
        {
            return dot( select_row( m_matrix, i ),
                        m_vector );
        }

        class Iterator : public boost::iterator_facade<Iterator,
                                                       value_type,
                                                       boost::random_access_traversal_tag,
                                                       value_type>
        {
            public:

                Iterator( const Matrix_Vector_Product& mvp,
                          size_t                       index )
                    : m_mvp( mvp ),
                      m_index( index ){}

            private:

                bool equal( Iterator const& iter ) const
                {
                    return m_index == iter.m_index;
                }

                typename Iterator::difference_type distance_to( const Iterator& iter ) const
                {
                    return iter.m_index - m_index;
                }

                void increment()
                {
                    ++m_index;
                }

                void decrement()
                {
                    --m_index;
                }

                void advance( typename Iterator::difference_type n )
                {
                    m_index += n;
                }

                typename Iterator::reference dereference() const
                {
                    return m_mvp( m_index );
                }

                friend class boost::iterator_core_access;

                Matrix_Vector_Product const& m_mvp;

                size_t m_index;

        }; // End of Iterator Class

        using iter_t = Iterator;

        using const_iter_t = Iterator;


        /**
         * Get the beginning iterator position
         */
        Iterator begin() const
        {
            return Iterator( *this, 0 );
        }

        /**
         * Get the end iterator position
         */
        Iterator end() const
        {
            return Iterator( *this, size() );
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Vector_Product<" << MatrixT::name() << ","
                 << VectorT::name() << "," << std::boolalpha << TransposeN <<  ">";
            return sout.str();
        }

    private:

        template <typename MatT>
        struct Matrix_Closure
        {
            using type = Matrix<typename MatT::value_type>;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<Matrix<ElementT,RowsN,ColsN>>
        {
            using type = Matrix<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<const Matrix<ElementT,RowsN,ColsN>>
        {
            using type = Matrix<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<Matrix_Proxy<ElementT,RowsN,ColsN>>
        {
            using type = Matrix_Proxy<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<const Matrix_Proxy<ElementT,RowsN,ColsN> >
        {
            using type = Matrix_Proxy<ElementT,RowsN,ColsN> const&;
        };

        template <typename VecT>
        struct Vector_Closure
        {
            using type = Vector_<typename VecT::value_type,
                                 Vector_Size<VecT>::value>;
        };

        template <typename ElementT,
                  size_t   SizeN>
        struct Vector_Closure<Vector_<ElementT,SizeN>>
        {
            using type = Vector_<ElementT,SizeN> const&;
        };

        template <typename ElementT,
                  size_t   SizeN>
        struct Vector_Closure<const Vector_<ElementT,SizeN>>
        {
            using type = Vector_<ElementT,SizeN>const&;
        };

        template <typename ElementT,
                  size_t SizeN>
        struct Vector_Closure<Vector_Proxy<ElementT,SizeN> >
        {
            using type = Vector_Proxy<ElementT,SizeN> const&;
        };

        template <typename ElementT,
                  size_t   SizeN>
        struct Vector_Closure<const Vector_Proxy<ElementT,SizeN>>
        {
            using type = Vector_Proxy<ElementT,SizeN> const&;
        };

        /// @brief Underlying matrix data
        typename Matrix_Closure<MatrixT>::type m_matrix;

        /// @brief Underlying vector data
        typename Vector_Closure<VectorT>::type m_vector;

}; // End of Matrix_Vector_Product class

} // End of tmns::math namespace