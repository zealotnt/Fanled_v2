/*
 * mtFanledAPICode.h
 *
 *  Created on: Aug 18, 2016
 *      Author: zealot
 */
#include "../inc/mtFanledAPICode.h"

#define APDU_CMD_CODE(buff)		(buff[0]) 	/*!< Get Command Code from buffer data*/
#define APDU_CTR_CODE(buff)		(buff[1])	/*!< Get Control Code from buffer data*/
#define APDU_ANT_CFG(buff)		(buff[2])	/*!< Get Antenna Configuration */

Void mtFanledApiSetResponseCode(UInt8 *pInBuf, UInt8 *pOutBuf, UInt16 *pOutBufLen)
{
	*pOutBufLen = MT_DATA_RES_LEN_MIN;			/*Initialize length of message response*/
	pOutBuf[0] = APDU_CMD_CODE(pInBuf);			/*Response code*/
	pOutBuf[1] = APDU_CTR_CODE(pInBuf)+1;		/*Response control code*/
	pOutBuf[2] = API_COMMAND_EXECUTE_SUCCESS;	/*Result code*/
}
