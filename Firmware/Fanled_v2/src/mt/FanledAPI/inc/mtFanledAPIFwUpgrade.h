/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPIFwUpgrade.h
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

#ifndef MTFANLEDAPIFWUPGRADE_H_
#define MTFANLEDAPIFWUPGRADE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define MAX_DOWNLOAD_PACKET_LEN			480

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
#pragma pack(1)
/*!
 *	\brief Download Command Packet data structure
 */
typedef struct
{
	UInt8 CmdCode;
	UInt8 ControlCode;
	UInt16 PacketNumber;
	UInt16 PacketLen;
	UInt32 PacketData[0];
} mtDownloadCmdPacket_t;

/*!
 *	\brief Download Response Packet data structure
 */
typedef struct
{
	UInt8 RespCode;
	UInt8 RespControl;
	UInt8 Status;
	UInt16 PacketNumber;
} mtDownloadResponsePacket_t;

typedef struct
{
	UInt8 CmdCode;
	UInt8 ControlCode;
	UInt16 FirmwareSize;
	UInt32 CRC32;
} mtChecksumPacket_t;

#pragma pack()

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
mtErrorCode_t mtFanledApiBlGetLastErr(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiRequestFirmwareUpgrade(UInt8 *msgIn,
        UInt16 msgInLen,
        UInt8 *msgOut,
        UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiFirmwareEraseApp(UInt8 *msgIn,
        UInt16 msgInLen,
        UInt8 *msgOut,
        UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiFirmwareDownload(UInt8 *msgIn,
        UInt16 msgInLen,
        UInt8 *msgOut,
        UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiFirmwareChecksum(UInt8 *msgIn,
        UInt16 msgInLen,
        UInt8 *msgOut,
        UInt16 *msgOutLen);

#ifdef __cplusplus
}
#endif

#endif /* MTFANLEDAPIFWUPGRADE_H_ */
