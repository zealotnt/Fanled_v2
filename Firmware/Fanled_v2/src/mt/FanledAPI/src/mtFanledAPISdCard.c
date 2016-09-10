/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPISdCard.c
**
** Module name       : FanledAPI
**
**
** Summary:
**
**= History ====================================================================
** @date 	Sep 5, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "mtInclude.h"
#include <string.h>

#include "../inc/mtFanledAPICode.h"
#include "ff.h"
#include "App/inc/mtAppDef.h"
#include "UartHandler/inc/mtSerialCmdParser.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define SD_END				0x00
#define SD_STILL			0x01

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
/* SD Card specific Variables */
extern FATFS gFatFs;
extern DIR mydir;
extern FILINFO myfno;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
mtErrorCode_t mtFanledApiSdListFile(UInt8 *msgIn,
                                    UInt16 msgInLen,
                                    UInt8 *msgOut,
                                    UInt16 *msgOutLen)
{
	FRESULT res = FR_DISK_ERR;
	UInt8 last_file_name_len = 0;
	UInt16 last_idx = 4;
	UInt8 from_file_num;
	UInt8 i = 0;

	*msgOutLen = 4;
	msgOut[3] = SD_END;
	from_file_num = msgIn[2];

	if (f_mount(0, &gFatFs) == FR_OK)
	{
		res = f_opendir(&mydir, "0:\\");
		if (res != FR_OK)
		{
			msgOut[2] = res;
			goto exit;
		}

		do
		{
			res = f_readdir(&mydir, &myfno);
			if (res != FR_OK)
			{
				msgOut[2] = res;
				goto exit;
			}

			/* Don't need to get old files */
			if (i < from_file_num)
			{
				i ++;
				continue;
			}

			if (myfno.fname[0])
			{
				last_file_name_len = strlen(myfno.lfname);
				if ((last_idx + last_file_name_len + 1) > MAX_SERIAL_DATA_EXCEPT_CMD)
				{
					/* We don't want the \n character in the last
					 * this will cause the split() method of Python cause 1 more null item in list */
					*msgOutLen = last_idx - 1;
					msgOut[3] = SD_STILL;
					goto exit;
				}

				memcpy((char *)&msgOut[last_idx], myfno.lfname, last_file_name_len);

				/* Separate other file name with null character */
				msgOut[last_idx + last_file_name_len] = '\n';

				/* Prepare for next iteration */
				last_idx += last_file_name_len + 1;
			}
		}
		while (myfno.fname[0]);

		/* If it can go there, means the SD list has end */
		/* Minus 1: same reason as before */
		*msgOutLen = last_idx - 1;
		msgOut[3] = SD_END;
	}

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdInspect(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdReadFile(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen)
{
	FIL sd_file;
	FRESULT fr;
	UInt32 bytes_read;
	UInt16 file_name_len;
	UInt32 file_offset;

	/* Check input param */
	file_name_len = strlen((char *)&msgIn[6]);

	/* 2b: Cmd + Ctr code
	 * 4b: File offset
	 * 1b: Null of filename
	 * */
	if ((file_name_len + 2 + 4 + 1) != msgInLen)
	{
		msgOut[2] = API_PARAM_ERROR;
		goto exit;
	}

	/* CmdCode + CtrCode + Status + isEnd */
	file_offset = (UInt32)*((UInt32 *)&msgIn[2]);
	*msgOutLen = 4;

	/* Open file object */
	fr = f_open(&sd_file, (char *)&msgIn[6], FA_READ);
	if (fr != FR_OK)
	{
		msgOut[2] = fr;
		goto exit;
	}

	/* Offset file */
	fr = f_lseek(&sd_file, file_offset);
	if (fr != FR_OK)
	{
		msgOut[2] = fr;
		goto exit;
	}

	/* Read file */
	fr = f_read(&sd_file,
	       &msgOut[4],
	       MAX_SERIAL_DATA_EXCEPT_CMD - 2,
	       &bytes_read);
	if (fr != FR_OK)
	{
		msgOut[2] = fr;
		goto exit;
	}

	/* Return value to host */
	msgOut[3] = SD_END;
	if (bytes_read == (MAX_SERIAL_DATA_EXCEPT_CMD - 2))
	{
		msgOut[3] = SD_STILL;
	}
	*msgOutLen += bytes_read;

exit:
	f_close(&sd_file);
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdDeleleFile(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdWriteFile(UInt8 *msgIn,
                                     UInt16 msgInLen,
                                     UInt8 *msgOut,
                                     UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}
/************************* End of File ****************************************/
