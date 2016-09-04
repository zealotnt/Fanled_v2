/**
 * @addtogroup mlsInclude
 * @{
 */

/**
 * @file mtErrors.h
 * @brief	Error code definitions
 *
 * This file contains error code definitions which are used by all function that
 * return error code in project
 * @author
 */

#ifndef __MTERRORS_H__
#define __MTERRORS_H__

#include "mtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef Int32 mtErrorCode_t;

/*---------------------------------------------------
 * Common error codes
 * --------------------------------------------------*/
#define MT_SUCCESS                    		0 	/** Global success return status   */
#define MT_ERROR							-1	/** Global error return status*/
#define MT_ERR_BAD_FORMAT					-2
#define MT_ERR_BAD_PARAM					-3

#ifdef __cplusplus
}
#endif

#endif /**__MTERRORS_H__*/


/*@}*/
