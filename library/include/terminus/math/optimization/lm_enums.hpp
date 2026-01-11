/**
 * @file    LM_Enums.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
 */
#pragma once

namespace tmns::math::optimize {

enum class LM_STATUS_CODE { ERROR_DID_NOT_CONVERGE        = -1,
                            ERROR_STATUS_UNKNOWN          = 0,
                            ERROR_CONVERGED_ABS_TOLERANCE = 1,
                            ERROR_CONVERGED_REL_TOLERANCE = 2 };


} // End of tmns::math::optimize namespace