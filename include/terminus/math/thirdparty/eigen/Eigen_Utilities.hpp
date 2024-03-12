/**
 * @file    Eigen_Utilities.hpp
 * @author  Marvin Smith
 * @date    10/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/matrix.hpp>
#include <terminus/math/vector/VectorN.hpp>

// Eigen Libraries
#include <Eigen/Dense>

namespace tmns::math::eigen {

/**
 * Convert from Terminus Matrix to Eigen Matrix
 * 
 * @note This is the general Matrix<> type
*/
template <typename InputTmnsMatrixType>
struct TMNS_to_Eigen_Picker
{
    using ValueT = InputTmnsMatrixType::value_type;

    using EigenT = ::Eigen::Matrix<ValueT,
                                   ::Eigen::Dynamic,
                                   ::Eigen::Dynamic>;

    /**
     * Get the pointer
    */
    static ValueT* to_ptr( const InputTmnsMatrixType& mat )
    {
        return mat.data();
    }

    static EigenT map( const MatrixN<ValueT>& mat )
    {
        return Eigen::Map<EigenT>( to_ptr( mat.transpose() ),
                                   mat.rows(),
                                   mat.cols() );
    }

}; // End of TMNS_to_Eigen_Picker

/**
 * Convert from Terminus Matrix to Eigen Matrix
 * 
 * This is the MatrixN<> dynamically-sized variant
*/
template <typename MatrixValueT>
struct TMNS_to_Eigen_Picker<MatrixN<MatrixValueT>>
{
    using ValueT = MatrixValueT;

    using EigenT = ::Eigen::Matrix<MatrixValueT,
                                   ::Eigen::Dynamic,
                                   ::Eigen::Dynamic,
                                   ::Eigen::RowMajor>;
    
    /**
     * Get the pointer
    */
    static ValueT* to_ptr( const MatrixN<MatrixValueT>& mat )
    {
        return const_cast<ValueT*>( mat.data() );
    }

    static EigenT map( const MatrixN<ValueT>& mat )
    {
        MatrixN<ValueT> matT = mat; //transpose(mat);
        Eigen::Map<EigenT> result( to_ptr( matT ),
                                   mat.rows(),
                                   mat.cols() );
        return std::move( result );
    }

}; // End of TMNS_to_Eigen_Picker (MatrixN<ValueT>)

/**
 * Convert from Const Terminus Matrix to Eigen Matrix
*/
template <typename MatrixValueT>
struct TMNS_to_Eigen_Picker<const MatrixN<MatrixValueT>>
{
    using ValueT = MatrixValueT;

    using EigenT = ::Eigen::Matrix<MatrixValueT,
                                   ::Eigen::Dynamic,
                                   ::Eigen::Dynamic>;

    /**
     * Get the pointer
    */
    static ValueT* to_ptr( const MatrixValueT& mat )
    {
        return mat.data();
    }

    static EigenT map( const MatrixN<ValueT>& mat )
    {
        return Eigen::Map<EigenT>( to_ptr( mat ),
                                   mat.rows(),
                                   mat.cols() );
    }
    
}; // End of TMNS_to_Eigen_Picker (MatrixN<double>)

/**
 * Picker for a Terminus Dynamic Vector to Eigen Dynamic Vector
*/
template <typename VectorValueT>
struct TMNS_to_Eigen_Picker<VectorN<VectorValueT>>
{
    using ValueT = VectorValueT;

    using EigenT = ::Eigen::Vector<ValueT,
                                   ::Eigen::Dynamic>;
    
    /**
     * Get the pointer
    */
    static ValueT* to_ptr( const VectorN<VectorValueT>& vec )
    {
        return const_cast<ValueT*>( vec.data().data() );
    }

    static EigenT map( const VectorN<VectorValueT>& vec )
    {
        return Eigen::Map<EigenT>( to_ptr( vec ),
                                   vec.size() );
    }

}; // End of TMNS_to_Eigen_Picker (VectorN<double>)

template <typename VectorValueT>
struct TMNS_to_Eigen_Picker<const VectorN<VectorValueT>>
{
    using ValueT = VectorValueT;

    using EigenT = ::Eigen::Vector<ValueT,
                                   ::Eigen::Dynamic>;
    
    /**
     * Get the pointer
    */
    static ValueT* to_ptr( const VectorN<VectorValueT>& vec )
    {
        return vec.data().data();
    }

    static EigenT map( const VectorN<VectorValueT>& vec )
    {
        return Eigen::Map<EigenT>( to_ptr( vec ),
                                   vec.size() );
    }
    
}; // End of TMNS_to_Eigen_Picker (MatrixN<double>)

/**
 * @brief Function to convert a Terminus matrix into an Eigen matrix.
 * This should remain type-neutral, as the TMNS_to_Eigen_Picker will implement the
 * specializations 
*/
template <typename EigenT,
          typename InputMatrixT>
ImageResult<EigenT> to_eigen( const InputMatrixT& mat )
{
    // Construct the end result
    return outcome::ok<EigenT>( TMNS_to_Eigen_Picker<InputMatrixT>::map( mat ) );
}


/**
 * Convert an Eigen Matrix into a Terminus MatrixN.
*/
template <typename MatrixT,
          typename EigenMatrixT>
ImageResult<MatrixN<typename MatrixT::value_type>> from_eigen( const EigenMatrixT& mat ) 
    requires (std::is_same<MatrixT,MatrixN<typename MatrixT::value_type>>::value)
{
    // @fixme This is the utter worst way.  Someday I'll solve Eigen's Map<> class
    MatrixT output( mat.rows(), mat.cols() );
    auto it = output.begin();
    auto temp = mat.transpose().reshaped(); //<Eigen::RowMajor>();
    for( const auto& a : temp )
    {
        (*it++) = a;
    }
    return outcome::ok<MatrixT>( output );
}

/**
 * Convert Eigen Vector to Terminus Vector
*/
template <typename VectorT,
          typename EigenT>
ImageResult<VectorN<typename VectorT::value_type>> from_eigen( const EigenT& in )
    requires std::is_same<VectorT,VectorN<typename VectorT::value_type>>::value
{
    using ValueT = typename VectorT::value_type;
    return outcome::ok<VectorN<ValueT>>( VectorT( in.data(),
                                                  in.size() ) );
}

} // End of tmns::math::eigen namespace