/**
 * @file    Matrix_Col.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

/// Terminus Libraries
#include "../vector/Vector_Base.hpp"

/// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::math {

/**
 * Structure for representing column semantics
 */
template <typename MatrixT>
class Matrix_Col : public Vector_Base<Matrix_Col<MatrixT> >
{
    private:

        /**
         * Forward declar iterator type
         */
        template <typename IteratorT>
        class Iterator;

    public:

        /// @brief Underlying Datatype
        using value_type = typename MatrixT::value_type;

        /// @brief Reference Type
        using reference_type = std::conditional_t<std::is_const_v<MatrixT>,
                                                  typename MatrixT::const_reference_type,
                                                  typename MatrixT::reference_type>;
        
        /// @brief Const Reference Type
        using const_reference_type = typename MatrixT::const_reference_type;

        /// @brief Iterator Type
        using iter_t = std::conditional_t<std::is_const_v<MatrixT>,
                                          Iterator<typename MatrixT::const_iter_t>,
                                          Iterator<typename MatrixT::iter_t>>;
        
        /// @brief Const Iterator Type
        using const_iter_t = Iterator<typename MatrixT::const_iter_t>;

        /**
         * @brief Constructor
         */
        Matrix_Col( MatrixT& matrix,
                    size_t   col )
            : m_matrix( matrix ),
              m_col( col )
        {
        }

        /**
         * @brief Standard Copy Assignment Operator
         */
        Matrix_Col&  operator = ( const Matrix_Col& v )
        {
            if( v.size() != size() )
            {
                std::stringstream sout;
                sout << "Vectors must have same size.  This: " << size() 
                     << ", other: " << v.size();
                throw std::runtime_error( sout.str() );
            }

            VectorN<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized assignment operator
         */
        template <typename OtherT>
        Matrix_Col& operator = ( const Vector_Base<OtherT>& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vectors must have same size.  This: " << size() 
                     << ", other: " << v.impl().size();
                throw std::runtime_error( sout.str() );
            }

            VectorN<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename OtherT>
        Matrix_Col& operator = ( const Vector_No_Tmp<OtherT>& v )
        {
            if( v.size() != size() )
            {
                std::stringstream sout;
                sout << "Vectors must have same size.  This: " << size() 
                     << ", other: " << v.size();
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.impl().begin(), v.impl().end(), begin() );
            return *this;
        }

        /**
         * @brief Get the underlying Matrix reference
         */
        MatrixT& child()
        {
            return m_matrix;
        }

        /**
         * @brief Get the underlying Matrix reference (const)
         */
        MatrixT const& child() const
        {
            return m_matrix;
        }

        /**
         * @brief Get the matrix size
         */
        size_t size() const
        {
            return child().rows();
        }

        /**
         * Function Operator.  Return the matrix value
         */
        reference_type operator()( size_t idx )
        {
            return child()( idx, m_col );
        }

        /**
         * @brief Function Operator.  Return the matrix value. (Const)
         */
        const_reference_type operator()( size_t idx ) const
        {
            return child()( idx, m_col );
        }

        /**
         * @brief Index Operator.  Return the matrix value.
         */
        reference_type operator[]( size_t idx )
        {
            if( idx > child().rows() )
            {
                std::stringstream sout;
                sout << "Requested row (" << idx << ") > Rows (" << child().rows() << ")";
                throw std::runtime_error( sout.str() );
            }
            return child()( idx, m_col );
        }

        /**
         * @brief Index Operator.  Return the matrix value.  (Const)
         */
        const_reference_type operator[]( size_t idx ) const
        {
            return child()( idx, m_col );
        }

        /**
         * @brief Get the beginning of the matrix.
         */
        iter_t begin()
        {
            return iter_t( child().begin() + m_col,
                           child().cols() );
        }

        /**
         * @brief Get the beginning of the matrix (Const)
         */
        const_iter_t begin() const
        {
            return const_iter_t( child().begin() + m_col,
                                 child().cols() );
        }

        /**
         * @brief Get the end of the matrix
         */
        iter_t end()
        {
            return begin() + size(); 
        }
        
        /**
         * @brief Get the end of the matrix
         */
        const_iter_t end() const
        {
            return begin() + size();
        }

        /**
         * Get the name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Col (" << MatrixT::name() << ")";
            return sout.str();
        }

        /**
         * Print to string
        */
        std::string to_string() const
        {
            std::stringstream sout;
            sout << name() << " (Col: " << m_col << ", Size: " << size() << "): ";
            for( size_t i = 0; i < size(); i++ )
            {
                if( i != 0 )
                {
                    sout << ", ";
                }
                sout << this->operator()(i);
            }
            return sout.str();
        }


    private:

        /**
         * Internal Iterator Type for efficient access
         */
        template <typename IteratorT>
        class Iterator : public boost::iterator_facade<Iterator<IteratorT>,
                                                       typename std::iterator_traits<IteratorT>::value_type,
                                                       boost::random_access_traversal_tag,
                                                       typename std::iterator_traits<IteratorT>::reference,
                                                       typename std::iterator_traits<IteratorT>::difference_type>
        {
            public:
                
                /// @brief Iterator diff type
                using difference_type = typename Iterator::difference_type;

                /**
                 * @brief Constructor given iterator and stride
                 */
                Iterator( const IteratorT& iter,
                          difference_type  stride )
                    : m_iter( iter ),
                      m_stride( stride ) {}
    
            private:

                /// @brief  Allow boost access to internals
                friend class boost::iterator_core_access;

                /// @brief  Underlying iterator
                IteratorT m_iter;

                /// @brief Stride (aka jump distance)
                difference_type m_stride;

                /**
                 * @brief Check if two iterators are at the same position
                 */
                bool equal( const Iterator& iter ) const
                {
                    return m_iter == iter.m_iter;
                }

                /**
                 * @brief Check the distance between 2 iterators
                 */
                difference_type distance_to( const Iterator& iter ) const
                {
                    return ( iter.m_iter - m_iter ) / m_stride;
                }

                /**
                 * @brief Increment the iterator to the next position.
                 */
                void increment()
                {
                    m_iter += m_stride;
                }

                /**
                 * @brief Decrement the iterator to the previous position.
                 */
                void decrement()
                {
                    m_iter -= m_stride;
                }

                /**
                 * @brief Advance the iterator to the new specified position
                 */
                void advance( difference_type n )
                {
                    m_iter += n * m_stride;
                }

                /**
                 * @brief Dereference the iterator, returning the value it points to.
                 */
                typename Iterator::reference dereference() const
                {
                    return *m_iter;
                }

        }; // End of Iterator class

        /// @brief  Underlying Matrix
        MatrixT& m_matrix;

        /// @brief  Column ID
        size_t m_col;

}; // End of Matrix_Col class

} // End of tmns::math namespace