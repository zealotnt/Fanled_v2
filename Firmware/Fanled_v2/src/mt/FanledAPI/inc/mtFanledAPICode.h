/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.h
**
** Module name       : template
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef MTFANLEDAPICODE_H_
#define MTFANLEDAPICODE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include "mtInclude.h"
#include "FanledAPI/inc/mtFanledAPIBasic.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define MT_DATA_RES_LEN_MIN				3

#define FANLED_API_CMD_CODE_BASIC		0x8B

#define FANLED_API_CMD_CTR_GETVERSION	0x00

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
/*Result code*/
typedef enum
{
	API_COMMAND_EXECUTE_SUCCESS			= 0x00, /*!<  Command executed successfully */
	API_COMMAND_EXECUTE_FAIL			= 0xFF, /*!<  Command execution failed */
	API_PARAM_ERROR						= 0xFE, /*!<  Invalid value of a command parameter */
	API_MULTI_CARD_DETECTED				= 0xFC, /*!<  Multiple card detected */
	API_NO_CARD_DETECTED_OR_REMOVED		= 0xFB, /*!<  No card detected or card removed */
	API_CARD_INIT_ERROR					= 0xEF, /*!<  Error initializing the card */
	API_CARD_OPERATION_ERROR			= 0xD3, /*!<  Error in card operation (on the card) */
	API_CARD_OCCURRED_ERROR				= 0xD2, /*!<  An error occurred on the card */
	API_CARD_OPERATION_INTERRUPTED		= 0xD1, /*!<  Card operation interrupted */
	API_RF_RECEIVER_OCCURED_ERROR		= 0xC2, /*!<  Error occurred in the reader RF receiver */
	API_CARD_CHECK_MSG_CRC_ERROR		= 0xC1, /*!<  Card response message CRC check error */
	API_CARD_NO_RESPONSE				= 0xC0, /*!<  No response from card */
	API_COMMAND_NOT_SUPPORTED			= 0xBE, /*!<  Command not supported */
	API_READER_OPERATION_TIMEOUT		= 0xBC, /*!<  Reader operation timeout */
	API_COMMAND_CARD_SYNTAX_ERROR		= 0x7F, /*!<  Command or Card syntax error, including error in card in CRC */
} mtFanledApiResultCode;


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
Void mtFanledApiSetResponseCode(UInt8 *pInBuf, UInt8 *pOutBuf, UInt16 *pOutBufLen);

#ifdef __cplusplus
}
#endif

#endif /* MTFANLEDAPICODE_H_ */
