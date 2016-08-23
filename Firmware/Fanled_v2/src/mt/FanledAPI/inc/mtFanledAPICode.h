/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPICode.h
**
** Module name       : FanledAPI
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
#include "FanledAPI/inc/mtFanledAPIFwUpgrade.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define MT_DATA_RES_LEN_MIN				3

#define CMD_CODE_BASIC				0x8B

#define CTR_CODE_GETVERSION			0x00
#define CTR_CODE_GETSET_RTC			0x02
/* free to add */
#define CTR_CODE_BL_GET_LAST_ERR	0x20
#define CTR_CODE_UPGRADEFW			0x30
#define CTR_CODE_FW_DOWNLOAD		0x32
#define CTR_CODE_FW_CHECKSUM		0x34
#define CTR_CODE_ERASE_APP			0x36
/* free to add */
#define CTR_CODE_PROTOCOL_TEST		0x70

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
/*Result code*/
typedef enum
{
	API_COMMAND_EXECUTE_SUCCESS			= 0x00, /*!<  Command executed successfully */
	API_COMMAND_EXECUTE_FAIL			= 0xFF, /*!<  Command execution failed */
	API_PARAM_ERROR						= 0xFE, /*!<  Invalid value of a command parameter */
	API_CHECK_CRC_ERROR					= 0xC1, /*!<  CRC check error */
	API_COMMAND_NOT_SUPPORTED			= 0xBE, /*!<  Command not supported */
} mtFanledApiResultCode;


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
#if (DEBUG)
#define API_INFO(...)				printf(__VA_ARGS__)
#define API_ERR(...)				do {printf(KRED KBOLD __VA_ARGS__ ); printf(KRESET "\r\n");} while(0);
#else
#define API_INFO(...)
#define API_ERR(...)
#endif

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
