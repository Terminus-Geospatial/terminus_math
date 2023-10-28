/**
 * @file    MatrixN.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Terminus Libraries
#include "Matrix.hpp"
#include "Sub_Matrix.hpp"

namespace tmns::math {

/**
 * Dynamicall allocated, arbitrary-dimension matrix class.
 */
template <typename ElementT>
class Matrix<ElementT,0,0> : public Matrix_Base<Matrix<ElementT> >
{
    public:

        /// @brief  Array Type
        using array_type = std::vector<ElementT>;
    
        /// @brief Underlying Element / Value Type
        using value_type = ElementT;

        /// @brief Reference Value Type
        using reference_type = ElementT&;

        /// @brief Const Reference Value Type
        using const_reference_type = ElementT const&;

        /// @brief Iterator Type
        using iter_t = typename array_type::iterator;

        /// @brief Const Iterator Type
        using const_iter_t = typename array_type::const_iterator;

        /**
         * Constructs a default (zero-size) matrix
         */
        Matrix() = default;

        /**
         * Constructs a matrix of zeros with the given size
         */
        Matrix( size_t rows,
                size_t cols )
            : m_rows( rows ),
              m_cols( cols ),
              m_data( rows * cols )
        {
            std::fill( m_data.begin(),
                       m_data.end(),
                       0 );
        }

        /**
         * Constructs a matrix of the given size from given 
         * densely-packed row-mjor data.  This constructor copies the
         * data.  If you wish to make a shallow proxy object instead,
         * Matrix_Proxy
         */
        Matrix( size_t          rows,
                size_t          cols,
                const ElementT* data )
            : m_data( data, data + rows * cols ),
              m_rows( rows ),
              m_cols( cols )
        {
        }

        /**
         * Constructs a matrix of the given size from given 
         * densely-packed row-mjor data.  This constructor copies the
         * data.  If you wish to make a shallow proxy object instead,
         * Matrix_Proxy
         */
        Matrix( size_t                  rows,
                size_t                  cols,
                std::vector<value_type> data )
            : m_data( data.begin(), data.end() ),
              m_rows( rows ),
              m_cols( cols )
        {
        }

        /**
         * Standard Copy Constructor
         * @note Provided in case the next constructor accidentally auto-resolves without
         *       this
        */
       Matrix( const Matrix& mat )
           : m_data( mat.m_data ),
             m_rows( mat.m_rows ),
             m_cols( mat.m_cols )
        {
        }
        
        /**
         * Generalized copy constructor for any base type
         */
        template <typename OtherMatrixT>
        Matrix( const Matrix_Base<OtherMatrixT>& mat )
            : m_data( mat.impl().begin(),
                      mat.impl().end() ),
              m_rows( mat.impl().rows() ),
              m_cols( mat.impl().cols() )
        {
        }

        /**
         * Copy assignment operator
         */
        Matrix& operator = ( const Matrix& mat )
        {
            // Make a copy of the data
            Matrix temp( mat );
            m_rows = temp.m_rows;
            m_cols = temp.m_cols;
            m_data = temp.m_data;
            return (*this);
        }

        /**
         * Generalize Assignment Operator
         */
        template <typename OtherMatrixT>
        Matrix& operator = ( const Matrix_Base<OtherMatrixT>& mat )
        {
            // Make a copy of the data
            Matrix temp( mat );
            m_rows = temp.m_rows;
            m_cols = temp.m_cols;
            m_data = temp.m_data;
            return (*this);
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary VW matrix expressions.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename OtherMatrixT>
        Matrix& operator = ( const Matrix_No_Tmp<OtherMatrixT>& mat )
        {
            if( mat.impl().rows() == rows() && 
                mat.impl().cols() == cols() )
            {
                std::copy( mat.impl().begin(),
                           mat.impl().end(),
                           begin() );
                return *this;
            }
            else 
            {
                std::cerr << "Matrix must have dimensions of " << rows() << " rows by "
                          << cols() << " cols.  Actual: " << mat.rows() << " rows x " 
                          << mat.cols() << " cols";
                return *this = mat.impl();
            } 
        }

        /**
         * Get matrix rows
         */
        size_t rows() const
        {
            return m_rows;
        }

        /**
         * Get matrix columns 
         */
        size_t cols() const
        {
            return m_cols;
        }

        /**
         * Resize the matrix.  Elements in memory are preserved when
         * specified.
         */
        void set_size( size_t rows,
                       size_t cols,
                       bool   preserve = false )
        {
            if( preserve )
            {
                // Create new array
                array_type other( rows * cols );
                
                size_t mr = (std::min)( rows, m_rows );
                size_t mc = (std::min)( cols, m_cols );
                for( size_t r=0; r<mr; ++r )
                for( size_t c=0; c<mc; ++c )
                {
                    other[ r * cols + c ] = m_data[ r * m_cols + c ];
                }

                m_data.swap( other );
            }
            else
            {
                m_data.resize( rows * cols, false );
            }
            m_rows = rows;
            m_cols = cols;
        }

        /**
         * Access an element
         */
        value_type& operator()( size_t row,
                                size_t col )
        {
            return m_data[ row * m_cols + col ];
        }

        /**
         * Access an element
         */
        value_type const& operator()( size_t row,
                                      size_t col ) const
        {
            return m_data[ row * m_cols + col ];
        }

        /**
         * Get pointer to underlying data
         */
        value_type* data()
        {
            return &(operator()(0,0));
        }
        
        /**
         * Get pointer to underlying data
         */
        const value_type* data() const
        {
            return &(operator()(0,0));
        }

        /**
         * Get the beginning of the matrix
         */
        iter_t begin()
        {
            return m_data.begin();
        }
        
        /**
         * Get the beginning of the matrix (const)
         */
        const_iter_t begin() const
        {
            return m_data.begin();
        }

        /**
         * Get the end of the matrix
         */
        iter_t end()
        {
            return m_data.end();
        }

        /**
         * Get the end of the matrix (const)
         */
        const_iter_t end() const
        {
            return m_data.end();
        }

        /**
         * Get the Determinant
         * General case
         */
        value_type determinant() const
        {
            value_type result = value_type();

            // Make sure the matrix is square
            if( rows() != cols() )
            {
                tmns::log::error( ADD_CURRENT_LOC(), 
                                  "Matrix must be square.  Actual: ",
                                  rows(), " x ", cols() );
                return result;
            }
            
            std::stack<std::pair<Matrix<value_type>,value_type> > s;

            s.push( std::make_pair( (*this), 1 ) );
            while( !s.empty() )
            {
                auto a = s.top().first;
                value_type scale = s.top().second;

                s.pop();
                
                // Make sure matrix is square
                if( a.rows() != a.cols() )
                {
                    tmns::log::error( ADD_CURRENT_LOC(), 
                                  "Matrix must be square.  Actual: ",
                                  a.rows(), " x ", a.cols() );
                    return result;
                }
                
                size_t dim = a.rows();
                Matrix<value_type> sub;
                switch( dim )
                {
                    case 0:
                        break;
                    case 1:
                        result += scale * a( 0, 0 );
                        break;
                    case 2:
                        result += scale * ( a( 0, 0 ) * a( 1, 1 ) - a( 0, 1 ) * a( 1, 0 ) );
                        break;
                    default:
                        {
                            sub = submatrix( a, 1, 1, dim - 1, dim - 1 );
                            s.push( std::make_pair( sub, scale * a( 0, 0 ) ) );
                            scale *= -1;
                        }
                        for( size_t i = 1; i < ( dim - 1 ); ++i )
                        {
                            submatrix( sub, 0, 0, dim-1, i )       = submatrix( a, 1,   0, dim-1, i );
                            submatrix( sub, 0, i, dim-1, dim-i-1 ) = submatrix( a, 1, i+1, dim-1, dim-i-1 );
                            s.push( std::make_pair( sub, scale * a( 0, i ) ) );
                            scale *= -1;
                        }
                        {
                            sub = submatrix( a, 1, 0, dim-1, dim-1 );
                            s.push( std::make_pair( sub, scale * a( 0, dim - 1 ) ) );
                        }
                        break;
                } // End of switch statement
            }
            return result;
        }

        /**
         * Inverse Matrix
         */
        Matrix<value_type,0,0> inverse() const
        {
            value_type zero            = value_type();
            size_t sz                  = cols();
            Matrix<value_type,0,0> buf = (*this);

            // Initialize the permutation
            VectorN<size_t> pm( sz );
            for( size_t i = 0; i < sz; ++i )
            {
                pm(i) = i;
            }

            // Perform LU decomposition with partial pivoting
            for( size_t i = 0; i < sz; ++i )
            {
                Matrix_Col<Matrix<value_type> > mci( buf, i );
                Matrix_Row<Matrix<value_type> > mri( buf, i );
      
                size_t i_norm_inf = i + index_norm_inf( subvector( mci, i, sz - i ) );
      
                if( buf( i_norm_inf, i ) == zero )
                {
                    throw std::runtime_error( "Matrix is singular in inverse()" );
                }
        
                if( i_norm_inf != i )
                {
                    size_t pbuf = pm(i);
                    pm(i) = pm(i_norm_inf);
                    pm(i_norm_inf) = pbuf;
                    VectorN<value_type> rowbuf = mri;
                    mri = select_row(buf,i_norm_inf);
                    select_row( buf, i_norm_inf ) = rowbuf;
                }
                if ( i != sz - 1 )
                {
                    subvector( mci, i+1, sz-i-1 ) /= buf( i, i );
                    submatrix( buf, i+1, i+1, sz-i-1, sz-i-1 ) -= outer_prod( subvector( mci, i+1, sz-i-1 ),
                                                                              subvector( mri, i+1, sz-i-1 ) );
                }
            }

            // Build up a permuted identity matrix
            Matrix<value_type> inverse_mat( sz, sz );
            for( size_t i = 0; i < sz; ++i )
            {
                inverse_mat( i, pm(i) ) = value_type(1);
            }

            // Divide by the lower-triangular term
            for( size_t i = 0; i < sz; ++i ){
            for( size_t j = 0; j < sz; ++j ){
                value_type t = inverse_mat( i, j );
                if( t != zero )
                {
                    for( size_t k = i+1; k < sz; ++k )
                    {
                        inverse_mat( k, j ) -= buf( k, i ) * t;
                    }
                }
            }} // End of lower-triangle division

            // Divide by the upper-triangular term
            for ( ssize_t i = sz - 1; i >= 0; --i ){
            for ( ssize_t j = sz - 1; j >= 0; --j ){
                value_type t = inverse_mat(i,j) /= buf(i,i);
                if( t != zero )
                {
                    for( ssize_t k = i-1; k >= 0; --k )
                    {
                        inverse_mat(k,j) -= buf(k,i) * t;
                    }
                }
            }}

            return inverse_mat;
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix<" << Data_Type_Name<ElementT>::name()
                 << ",0,0>";
            return sout.str();
        }

    private:

        /// @brief Underlying array data
        array_type m_data;

        /// @brief Matrix Rows
        size_t m_rows { 0 };

        /// @brief Matrix Cols
        size_t m_cols { 0 };

};  // End of Matrix<ElementT,0,0> class

// Rename Matrix to MatrixN for this case
template <typename ElementT>
using MatrixN = Matrix<ElementT,0,0>;

} // End of tmns::math namespace