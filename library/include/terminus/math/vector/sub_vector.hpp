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
 * @file    sub_vector.hpp
 * @author  Marvin Smith
 * @date    10/24/2023
 */
#pragma once

namespace tmns::math {

/*
 * class SubVector<VectorT>
 * An dynamically-sized subvector class.
 */
template <typename VectorT>
class Sub_Vector : public Vector_Base<Sub_Vector<VectorT>>
{
    public:
        using value_type = typename VectorT::value_type;

        using reference_type = std::conditional<std::is_const_v<VectorT>,
                                                typename VectorT::const_reference_type,
                                                typename VectorT::reference_type>;

        using const_reference_type = typename VectorT::const_reference_type;

        using iter_t = std::conditional<std::is_const_v<VectorT>,
                                        typename VectorT::const_iter_t,
                                        typename VectorT::iter_t>;

        using const_iter_t = typename VectorT::const_iter_t;

        /**
         * Constructor
         * @param v Vector to use as underlying reference data.
         * @param pos Starting position for all lookups.
         * @param size Max size of the array.
        */
        Sub_Vector( VectorT& v,
                    size_t   pos,
                    size_t   size )
            : m_vector(v),
              m_pos(pos),
              m_size(size) {}

        /**
         * Standard copy assignment operator.
         */
        Sub_Vector& operator = ( const Sub_Vector& v )
        {
            if( v.impl().size() != m_size )
            {
                std::stringstream sout;
                sout << "Vectors must have same size in subvector assignment";
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.begin(),
                       v.end(),
                       m_vector.begin() );
            return *this;
        }

        /**
         * Generalized assignment operator, from arbitrary vector expressions.
         */
        template <typename OtherT>
        Sub_Vector& operator = ( const Vector_Base<OtherT>& v )
        {
            if( v.impl().size() != m_size )
            {
                std::stringstream sout;
                sout << "Vectors must have same size in subvector assignment";
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.impl().begin(),
                       v.impl().end(),
                       m_vector.begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary vector expressions.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename OtherT>
        Sub_Vector& operator = ( const Vector_No_Tmp<OtherT>& v )
        {
            if( v.impl().size() != m_size )
            {
                std::stringstream sout;
                sout << "Vectors must have same size in subvector assignment";
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.impl().begin(),
                       v.impl().end(),
                       m_vector.begin() );
            return *this;
        }

        VectorT& child()
        {
            return m_vector;
        }

        VectorT const& child() const
        {
            return m_vector;
        }

        size_t size() const
        {
            return m_size;
        }

        void fill( value_type val )
        {
            for (size_t i=0; i<m_size; ++i)
            {
                this->operator()(i) = val;
            }
        }

        reference_type operator()( size_t i )
        {
            return child()(m_pos+i);
        }

        const_reference_type operator()( size_t i ) const { return child()(m_pos+i); }

        reference_type operator[]( size_t i )
        {
            return child()(m_pos+i);
        }

        const_reference_type operator[]( size_t i ) const
        {
            return child()(m_pos+i);
        }

        iter_t begin()
        {
            return child().begin() + m_pos;\
        }

        const_iter_t begin() const
        {
            return child().begin() + m_pos;
        }

        iter_t end()
        {
            return child().begin() + m_pos + m_size;
        }

        const_iter_t end() const
        {
            return child().begin() + m_pos + m_size;
        }

    private:

      VectorT& m_vector;
      size_t   m_pos;
      size_t   m_size;
};

template <typename VectorT>
Sub_Vector<VectorT> subvector( Vector_Base<VectorT>& vector,
                               size_t                pos,
                               size_t                size )
{
    return Sub_Vector<VectorT>( vector.impl(), pos, size );
}

template <typename VectorT>
Sub_Vector<const VectorT> subvector( const Vector_Base<VectorT>& vector,
                                     size_t                      pos,
                                     size_t                      size )
{
    return Sub_Vector<const VectorT>( vector.impl(), pos, size );
}

} // End of tmns::math namespace