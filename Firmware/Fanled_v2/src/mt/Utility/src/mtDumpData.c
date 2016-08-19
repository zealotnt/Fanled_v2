/*
 * mtDumpData.c
 *
 *  Created on: Aug 18, 2016
 *      Author: zealot
 */
#include "../inc/mtDumpData.h"

/**
 * @Function: mtSerialCmdDumpBufferDataRaw
 */
Void mtSerialCmdDumpBufferDataRaw(char *printStr, Void *pBuff, UInt16 wMsgLen)
{
#if (SUPPORT_DUMP_DEBUG_DATA == 1)
	UInt32 i;
	UInt8 *pDat = (UInt8 *)pBuff;

	DEBUG_SERIAL_PRINT(KLGRN "%s " KBOLD "(%d bytes)" KRBOLD ": ", printStr, wMsgLen);
	/* msgLen = 0 --> Cause Segmentation fault */
	if (wMsgLen != 0)
	{
		for(i = 0; i < wMsgLen; i++)
		{
			DEBUG_SERIAL_PRINT(" %2x", pDat[i]);
		}
	}
	DEBUG_SERIAL_PRINT(KRESET " \r\n");
#endif
}

/**
 * @Function: mtSerialCmdDumpBufferData
 */
Void mtSerialCmdDumpBufferData(char *printStr, Void *pBuff, UInt16 wMsgLen)
{
#if (SUPPORT_DUMP_DEBUG_DATA == 1)
	UInt32 i;
	UInt8 *pDat = (UInt8 *)pBuff;

	if (wMsgLen > MAX_SERIAL_DATAPAYLOAD_IN_PACKET_SIZE)
	{
		DEBUG_SERIAL_PRINT_NOTIFY_HANG("Invalid len \r\n");
	}

	DEBUG_SERIAL_PRINT(KLGRN "%s " KBOLD "(%d bytes)" KRBOLD ": ", printStr, wMsgLen);
	/* msgLen = 0 --> Cause Segmentation fault */
	if (wMsgLen != 0)
	{
		for(i = 0; i < wMsgLen - 1; i++)
		{
			DEBUG_SERIAL_PRINT(" %2x", pDat[i]);
		}
		DEBUG_SERIAL_PRINT(" [%2x]", pDat[MAX_SERIAL_DATAPAYLOAD_IN_PACKET_SIZE - 1]);
	}
	DEBUG_SERIAL_PRINT(KRESET " \r\n");

#endif
}

/************************* End of File ****************************************/
