/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.c
**
** Module name       : template
**
**
** Summary:
**
**= History ====================================================================
** @date 	Feb 23, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "../inc/mtSerialCmdParser.h"
#include "../inc/mtSerialHandler.h"
#include "../inc/mtSerialPorting.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#ifndef SUPPORT_DUMP_DEBUG_DATA
#define SUPPORT_DUMP_DEBUG_DATA		0
#endif

#define SERIAL_FI_RESP_SECONDBYTE	SERIAL_FI_RF_PROCESSOR_TO_DEVICE
#define OWN_FI						SERIAL_FI_DEVICE_TO_RF_PROCESSOR

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
serialQueuePayload_t gQueuePayload;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
static UInt32 dwTotalByteRcv = 0;
/* 8-bit CRC with polynomial x^8+x^6+x^3+x^2+1, 0x14D.
   Chosen based on Koopman, et al. (0xA6 in his notation = 0x14D >> 1):
   http://www.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf
   Get from READER SERIAL APPLICATION PROGRAMMER'S INTERFACE
   Appendix A - CRC-8 Algorithm
 */
const static UInt8 crc8_table[256] =
{
	0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
	0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
	0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
	0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
	0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
	0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
	0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
	0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
	0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
	0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
	0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
	0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
	0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
	0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
	0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
	0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
	0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
	0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
	0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
	0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
	0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
	0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
	0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
	0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
	0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
	0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
	0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
	0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
	0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
	0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
	0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
	0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
static Void mtSerialCmd_ResetRcvStateMachine(serialQueuePayload_t *qBuff);

static int mtSerialCmd_CheckValidFISecondByte(UInt8 bByte);

static serialErrorType_t mtSerialCmd_CheckValidLen(serialQueuePayload_t *pBuffer, UInt32 *pdwDataLen);

static mtErrorCode_t mtSerialCmd_CheckValidData(serialQueuePayload_t *pBuffer);

static mtErrorCode_t checkValidTerminationPacket(serialQueuePayload_t *pBuffer, UInt8 *pPacketType);

static UInt8 mtSerialCmd_UtilsCRC8Calculate(Void *pBuffIn, UInt32 dwLen);

static mtErrorCode_t mtSerialCmdSendACK(UInt8 bTarget);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
/**
 * @Function: mtSerialCmd_ResetRcvStateMachine
 */
static Void mtSerialCmd_ResetRcvStateMachine(serialQueuePayload_t *qBuff)
{
	qBuff->lenMonitoring = qBuff->rcvLen;
	qBuff->rcvLen = 0;
	qBuff->rcvState = RCV_IDLE;
	mtSerialPort_FlushRxData();
}

/**
 * @Function: checkValid_FI_SecondByte
 */
static int mtSerialCmd_CheckValidFISecondByte(UInt8 byte)
{
	return (byte == SERIAL_FI_DEVICE_TO_RF_PROCESSOR) ? 1 : 0;
}

/**
 * @Function: mtSerialCmd_CheckValidLen
 */
static serialErrorType_t mtSerialCmd_CheckValidLen(serialQueuePayload_t *pBuffer, UInt32 *pdwDataLen)
{
	serialErrorType_t retVal = ERROR_NONE;
	serialHeaderLen_t *pDat = (serialHeaderLen_t *)&pBuffer->serialDataFrame.Len;

	UInt8 bCalculatedLCS;

	bCalculatedLCS = pDat->Lenl ^ pDat->Lenm;

	if (bCalculatedLCS == pDat->LCS)
	{
		*pdwDataLen = pDat->Lenl + (pDat->Lenm << 8);

		/* Check max value of Data bytes (512 bytes) */
		if ((pDat->Lenm == 0xFF) && (pDat->Lenl == 0x00))
		{
			/* ACK packet, length = 0 */
			*pdwDataLen = 0;
			retVal = ERROR_NONE;
		}
		else if (pDat->Lenm == 0xFF)
		{
			/* NOTE: NACK packet, let the handling thread choose what to do */
			*pdwDataLen = 0;
			retVal = ERROR_NONE;
		}
		else if (*pdwDataLen <= MAX_SERIAL_DATA_EXCEPT_CMD)
		{
			retVal = ERROR_NONE;
		}
		else
		{
			retVal = ERROR_LEN_TOO_BIG;
		}
	}
	else
	{
		retVal = ERROR_INVALID_LEN_LCS;
	}
	return retVal;
}

/**
 * @Function: mtSerialCmd_CheckValidData
 */
static mtErrorCode_t mtSerialCmd_CheckValidData(serialQueuePayload_t *pBuffer)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	UInt16 wLen;
	UInt8 bCalculatedVal;

	wLen = pBuffer->serialDataFrame.Len.Lenl + (pBuffer->serialDataFrame.Len.Lenm << 8);
	bCalculatedVal = mtSerialCmd_UtilsCRC8Calculate(&pBuffer->serialDataFrame.Len, wLen + 3);

	if (bCalculatedVal != pBuffer->serialDataFrame.Data.Dt[wLen - 2])
	{
		retVal = MT_ERROR;
	}

	return retVal;
}

/**
 * @Function: mtSerialCmd_CheckValidACK
 */
static mtErrorCode_t checkValidTerminationPacket(serialQueuePayload_t *buffer, UInt8 *packetType)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	if (buffer->serialDataFrame.IF.FIl == SERIAL_FI_FIRSTBYTE &&
	        mtSerialCmd_CheckValidFISecondByte(buffer->serialDataFrame.IF.FIm) &&
	        buffer->serialDataFrame.Len.Lenl == 0 &&
	        buffer->serialDataFrame.Len.Lenm == 0xFF &&
	        buffer->serialDataFrame.Len.LCS == 0xFF )
	{
		*packetType = SERIAL_PACKET_ACK;
	}
	else if (buffer->serialDataFrame.IF.FIl == SERIAL_FI_FIRSTBYTE &&
	         mtSerialCmd_CheckValidFISecondByte(buffer->serialDataFrame.IF.FIm) &&
	         buffer->serialDataFrame.Len.Lenl != 0 &&
	         buffer->serialDataFrame.Len.Lenm == 0xFF &&
	         buffer->serialDataFrame.Len.LCS == (buffer->serialDataFrame.Len.Lenl ^ buffer->serialDataFrame.Len.Lenm))
	{
		*packetType = SERIAL_PACKET_NACK;
	}
	else
	{
		UInt16 pckLen = buffer->serialDataFrame.Len.Lenl + (buffer->serialDataFrame.Len.Lenm << 8);
		if (buffer->lenMonitoring == (pckLen + 6) &&
		        buffer->serialDataFrame.Len.LCS == (buffer->serialDataFrame.Len.Lenl ^ buffer->serialDataFrame.Len.Lenm))
		{
			*packetType = SERIAL_PACKET_DATA;
		}
		else
		{
			*packetType = SERIAL_PACKET_UNKNOWN;
			retVal = MT_ERROR;
		}
	}
	return retVal;
}

/**
 * @Function: mtSerialCmd_UtilsCRC8Calculate
 */
static UInt8 mtSerialCmd_UtilsCRC8Calculate(Void *pBuffIn, UInt32 dwLen)
{
	UInt8 bCrc = 0;
	UInt8 *pData = (UInt8 *)pBuffIn;

	while (dwLen--)
	{
		bCrc = crc8_table[bCrc ^ *pData];
		pData += 1;
	}

	return bCrc;
}

/**
 * @Function: mtSerialCmdSendACK
 */
static mtErrorCode_t mtSerialCmdSendACK(UInt8 bTarget)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	serialHeaderFI_t ackIF;
	serialHeaderLen_t ackLen;

	ackIF.FIl = SERIAL_FI_FIRSTBYTE;
	ackIF.FIm = bTarget;
	ackLen.Lenl = 0x00;
	ackLen.Lenm = 0xFF;
	ackLen.LCS = 0xFF;

	retVal = mtSerialPort_Write((UInt8 *)&ackIF, sizeof(serialHeaderFI_t));
	retVal |= mtSerialPort_Write((UInt8 *)&ackLen, sizeof(serialHeaderLen_t));

	return retVal;
}
/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
/**
 * @Function: mtSerialCmdRcvStateHandling
 */
mtSerialRcvRoutineDecision_t mtSerialCmdRcvStateHandling(UInt8 bData, serialQueuePayload_t *qBuff, UInt32 *pdwTotalDataLen)
{
	mtSerialRcvRoutineDecision_t retVal = ROUTINE_RET_NO_CHANGE;
	UInt8 *pDat = (UInt8 *)&qBuff->serialDataFrame;
	receiveRoutineState_t *rcvRoutineState = &qBuff->rcvState;

	mtGetCurrentTime(&gReloadTimeAtRcvRoutine);
	mtMutexLock(&gRcvVarChangeMutex);

	if (qBuff->rcvLen < sizeof(serialRcvFrame_t))
	{
		/* Reset buffer if this is first step of state machine */
		if (*rcvRoutineState == RCV_IDLE)
		{
			*pdwTotalDataLen = 0;
			mtSerialCmd_ResetRcvStateMachine(qBuff);
			mtGetCurrentTime(&timingFirstRcvByte);
		}

		/* In RCV_ERROR and RCV_DONE state, we discard received character */
//		if (*rcvRoutineState ==  RCV_ERROR || *rcvRoutineState ==  RCV_DONE)
		{
			pDat[qBuff->rcvLen] = bData;
			qBuff->rcvLen++;
			qBuff->lenMonitoring++;
			dwTotalByteRcv++;
		}
		switch (*rcvRoutineState)
		{
			case RCV_IDLE:
			{
				mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());
				if (bData == SERIAL_FI_FIRSTBYTE)
				{
					*rcvRoutineState = RCV_IF;
					qBuff->errorFlag = ERROR_NONE;
				}
				else
				{
					*rcvRoutineState = RCV_ERROR;
					DEBUG_SERIAL_PRINT("FirstWrongChar=%2x\r\n", bData);
					qBuff->errorFlag = ERROR_INVALID_IF1;
				}
			}
			break;

			case RCV_IF:
			{
				mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());
				if (mtSerialCmd_CheckValidFISecondByte(bData))
				{
					*rcvRoutineState = RCV_LEN;
				}
				else
				{
					*rcvRoutineState = RCV_ERROR;
					qBuff->errorFlag = ERROR_INVALID_IF2;
				}
			}
			break;

			case RCV_LEN:
			{
				mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());
				if (qBuff->rcvLen < SERIAL_NUM_OF_LENGTH_BYTE + sizeof(serialHeaderFI_t))
				{
					/* Simply receive until get enough number of bytes */
					break;
				}
				else
				{
					serialErrorType_t lenValid;
					lenValid = mtSerialCmd_CheckValidLen(qBuff, pdwTotalDataLen);
					if ((lenValid == MT_SUCCESS) && (*pdwTotalDataLen == 0))
					{
						qBuff->errorFlag = RCV_SUCCESS;
						*rcvRoutineState = RCV_DONE;
						mtSerialPort_InterByteTimerDisable();

						// NOTE: Not use RCV_DONE state;
						mtSerialCmd_ResetRcvStateMachine(qBuff);
						/* Finish receiving packet (ACK or NACK),
						 * push msg queue to HandlingThread */
						retVal = ROUTINE_RET_PUSH_DATA;
					}
					else if (lenValid == MT_SUCCESS)
					{
						*rcvRoutineState = RCV_DATA;
					}
					else
					{
						*rcvRoutineState = RCV_ERROR;
						qBuff->errorFlag = lenValid;
					}
				}
			}
			break;

			case RCV_DATA:
			{
				mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());

				/* We receive Data bytes and CRC8 byte here */
				if (qBuff->rcvLen < (*pdwTotalDataLen + 1 + sizeof(serialHeaderFI_t) + sizeof(serialHeaderLen_t)))
				{
					/* Simply receive until get enough number of bytes */
					break;
				}
				else
				{
					mtErrorCode_t dataValid;
					dataValid = mtSerialCmd_CheckValidData(qBuff);
					if (dataValid == MT_SUCCESS)
					{
						qBuff->serialDataFrame.CRC8 = bData;
						qBuff->errorFlag = RCV_SUCCESS;
						*rcvRoutineState = RCV_DONE;
						mtSerialPort_InterByteTimerDisable();

						// NOTE: Not use RCV_DONE state;
						mtSerialCmd_ResetRcvStateMachine(qBuff);
						/* Finish receiving data, push message queue now */
						retVal = ROUTINE_RET_PUSH_DATA;
					}
					else
					{
						*rcvRoutineState = RCV_ERROR;
						qBuff->errorFlag = ERROR_INVALID_CRC;
					}
				}
			}
			break;

			case RCV_DONE:
			{
				/* Wait until HandlingThread finish processing Message Queue */
				/* FIXME: should we handle incoming byte when we are staying in RCV_DONE State ?*/
			}
			break;

			case RCV_ERROR:
			{
				/* Wait until Inter-byte-timer trigger !! */
				mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());
			}
			break;

			default:
				break;
		}
	}
	else
	{
		/* Buffer overflow */
		if (qBuff->errorFlag != ERROR_SERIAL_BUFFER_OVERFLOW)
		{
			DEBUG_SERIAL_PRINT_NOTIFY_BAD("\r\n Buffer overflow \r\n");
		}

		*rcvRoutineState = RCV_ERROR;
		qBuff->lenMonitoring++;
		qBuff->errorFlag = ERROR_SERIAL_BUFFER_OVERFLOW;
		mtSerialPort_InterByteTimerReload(/*&cmdRcvInterByteTimer,*/ mtSerialPort_InterByteTimerGetInverval());
		retVal = ROUTINE_RET_NO_CHANGE;
	}
	mtMutexUnlock(&gRcvVarChangeMutex);

	return retVal;
}

/**
 * @Function: mtSerialCmdSendPacket
 */
mtErrorCode_t mtSerialCmdSendPacket(serialRcvFrame_t *packet)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	/* Avoid using malloc and free, cause MAXIM's compiler not support it */
	UInt16 wLenToWrite = packet->Len.Lenl + (packet->Len.Lenm << 8);

	mtSerialPort_Write((UInt8 *)&packet->IF, wLenToWrite + 5);
	mtSerialPort_Write((UInt8 *)&packet->CRC8, 1);

//	DumpBufferData("Sending", packet, lenToWrite + 6);
	return retVal;
}

/**
 * @Function: mtSerialCmdDataLinkHandlingThread
 */
Void mtSerialCmdDataLinkHandlingThread(serialQueuePayload_t sQueuePayload)
{
	static serialRcvFrame_t sResponseFrame;
	UInt8 sPacketType = SERIAL_PACKET_UNKNOWN;
	UInt16 msgInLen;

	if (sQueuePayload.errorFlag != RCV_SUCCESS)
	{
		mtSerialCmd_ResetRcvStateMachine(&gQueuePayload);
	}

	switch (sQueuePayload.type)
	{
		case DATA_TYPE:
		{
			UInt16 msgOutLen = 0;
			if (sQueuePayload.errorFlag != RCV_SUCCESS)
			{
				DEBUG_SERIAL_PRINT_NOTIFY_BAD("Rcv error, code: %d \r\n", sQueuePayload.errorFlag);
				goto exit;
			}

			if (sQueuePayload.serialDataFrame.IF.FIm != OWN_FI)
			{
				DEBUG_SERIAL_PRINT("This Cmd is not for me, just ignore it \r\n");
				goto exit;
			}

			checkValidTerminationPacket(&sQueuePayload, &sPacketType);
			switch (sPacketType)
			{
				case SERIAL_PACKET_DATA:
				{
					/* Reply ACK first */
					mtSerialCmdSendACK(SERIAL_FI_RESP_SECONDBYTE);
					msgInLen = sQueuePayload.serialDataFrame.Len.Lenl + (sQueuePayload.serialDataFrame.Len.Lenm << 8);

					/* Process cmd */
					mtSerialProcessCmdPacket((UInt8 *)&sQueuePayload.serialDataFrame.Data,
					                         msgInLen,
					                         (UInt8 *)&sResponseFrame.Data,
					                         &msgOutLen);

					/* Fill up necessary bytes */
					sResponseFrame.IF.FIl = SERIAL_FI_FIRSTBYTE;
					sResponseFrame.IF.FIm = SERIAL_FI_RESP_SECONDBYTE;
					sResponseFrame.Len.Lenl = msgOutLen & 0xff;
					sResponseFrame.Len.Lenm = (msgOutLen & 0xff00) >> 8;
					sResponseFrame.Len.LCS = sResponseFrame.Len.Lenl ^ sResponseFrame.Len.Lenm;
					sResponseFrame.CRC8 = mtSerialCmd_UtilsCRC8Calculate(&sResponseFrame.Len, msgOutLen + 3);
					mtSerialCmdSendPacket(&sResponseFrame);
					break;
				}
				case SERIAL_PACKET_NACK:
				{
					/* Re-send last packet if receive any NACK packet */
					mtSerialCmdSendPacket(&sResponseFrame);
					break;
				}
				default:
					break;
			}
		}
		break;

		default:
			break;
	}
exit:
	return;
}

/**
 * @Function: mtSerialCmd_InterByteTimeOutHandling
 */
Void mtSerialCmd_InterByteTimeOutHandling(Void *pParam)
{
	serialQueuePayload_t *pQueue = (serialQueuePayload_t *)pParam;

	mtMutexLock(&gRcvVarChangeMutex);
	if ((pQueue->errorFlag == ERROR_NONE) && (pQueue->rcvState != RCV_ERROR) && (pQueue->rcvState != RCV_IDLE))
	{
		/* Timeout when receiving bytes --> wait another timeout
		 * until Host finish sending its rubbish data */
		DEBUG_SERIAL_PRINT_NOTIFY_BAD("\r\nError timeout !!!, curState=%d, numOfByteRcv=%d \r\n",
		                              pQueue->rcvState, pQueue->rcvLen);
		pQueue->errorFlag = ERROR_INTER_BYTE_TIMEOUT;
		pQueue->rcvState = RCV_ERROR;
		mtSerialPort_InterByteTimerReload(mtSerialPort_InterByteTimerGetInverval());
	}
	else if (pQueue->rcvState == RCV_ERROR)
	{
		if (pQueue->errorFlag == ERROR_INTER_BYTE_TIMEOUT)
		{
			dwTotalByteRcv = 0;
		}
		/* Timeout when rcvRoutine are already in RCV_ERROR,
		 * just PUSH error msg queue to Handling Thread */
		DEBUG_SERIAL_PRINT_NOTIFY_BAD("\r\nTimeout when errored !!!, "
		                              "errType=%d ,curState=%d, numOfByteRcv=%d \r\n",
		                              pQueue->errorFlag, pQueue->rcvState, pQueue->lenMonitoring);
		pQueue->type = DATA_TYPE;
		mtSerialCmd_ResetRcvStateMachine(&gQueuePayload);
	}
	else
	{
		if (pQueue->rcvState == RCV_IDLE)
		{
			DEBUG_SERIAL_PRINT_NOTIFY_BAD("Timeout after rcv done !!!, just ignore it \r\n");
		}
		else
		{
			DEBUG_SERIAL_PRINT_NOTIFY_HANG("Don't know what is going on, hang !!! \r\n");
		}
	}
	mtMutexUnlock(&gRcvVarChangeMutex);
}

/************************* End of File ****************************************/
