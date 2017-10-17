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
#include "Utility/inc/md5.h"

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
#define FAT_FS_RETURN_IF_ERR(fr, func, status) fr = func; if (fr != FR_OK) {status = fr; goto exit;}


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
	Bool thereIsFile = False;

	*msgOutLen = 4;
	msgOut[3] = SD_END;
	from_file_num = msgIn[2];

	if (f_mount(&gFatFs, "", 0) == FR_OK)
	{
		FAT_FS_RETURN_IF_ERR(res, f_opendir(&mydir, "0:\\"), msgOut[2]);

		do
		{
			FAT_FS_RETURN_IF_ERR(res, f_readdir(&mydir, &myfno), msgOut[2]);

			/* Don't need to get old files */
			if (i < from_file_num)
			{
				i ++;
				continue;
			}

			if (myfno.fname[0])
			{
				thereIsFile = True;
				last_file_name_len = strlen(myfno.fname);
				if ((last_idx + last_file_name_len + 1) > MAX_SERIAL_DATA_EXCEPT_CMD)
				{
					/* We don't want the \n character in the last
					 * this will cause the split() method of Python cause 1 more null item in list */
					*msgOutLen = last_idx - 1;
					msgOut[3] = SD_STILL;
					goto exit;
				}

				memcpy((char *)&msgOut[last_idx], myfno.fname, last_file_name_len);

				/* Separate other file name with null character */
				msgOut[last_idx + last_file_name_len] = '\n';

				/* Prepare for next iteration */
				last_idx += last_file_name_len + 1;
			}
		}
		while (myfno.fname[0]);

		/* If it can go there, means the SD list has end */
		/* Minus 1: same reason as before
		 * Don't minus 1 if there is no file (python assume there is 4 byte return) */
		*msgOutLen = last_idx - thereIsFile;
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

	/* 2b: CMD + CTR code
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
	FAT_FS_RETURN_IF_ERR(fr, f_open(&sd_file, (char *)&msgIn[6], FA_READ), msgOut[2]);

	/* Offset file */
	FAT_FS_RETURN_IF_ERR(fr, f_lseek(&sd_file, file_offset), msgOut[2]);

	/* Read file */
	FAT_FS_RETURN_IF_ERR(fr,
	                     f_read(&sd_file,
	                    		 &msgOut[4],
	                    		 MAX_SERIAL_DATA_EXCEPT_CMD - 2,
	                    		 &bytes_read),
	                     msgOut[2]);

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
	FRESULT fr;
	UInt16 file_name_len;

	/* Check input param */
	file_name_len = strlen((char *)&msgIn[2]);

	/* 2b: CMD + CTR code
	 * 1b: Null of filename
	 * */
	if ((file_name_len + 2 + 1) != msgInLen)
	{
		msgOut[2] = API_PARAM_ERROR;
		goto exit;
	}

	FAT_FS_RETURN_IF_ERR(fr, f_unlink((char *)&msgIn[2]), msgOut[2]);

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdWriteFile(UInt8 *msgIn,
                                     UInt16 msgInLen,
                                     UInt8 *msgOut,
                                     UInt16 *msgOutLen)
{
	FIL sd_file;
	FRESULT fr;
	UInt16 file_name_len;
	UInt32 file_offset;
	UInt32 file_content_length;
	UInt32 file_written;

	/* Check input param */
	file_name_len = strlen((char *)&msgIn[10]);
	file_content_length = (UInt32)*((UInt32 *)&msgIn[6]);
	file_offset = (UInt32)*((UInt32 *)&msgIn[2]);

	/* 2b: CMD + CTR code
	 * 4b: file offset
	 * 4b: file content length
	 * xb: length of file name
	 * 1b: Null of filename
	 * xb; file contents
	 * */
	if ((2 + 4 + 4 + file_name_len + 1 + file_content_length) != msgInLen)
	{
		msgOut[2] = API_PARAM_ERROR;
		goto exit;
	}

	FAT_FS_RETURN_IF_ERR(fr, f_mount(&gFatFs, "", 0), msgOut[2]);

	/* Offset = 0 -> create file */
	if (file_offset == 0)
	{
try_open:
		/* Open file object */
		fr = f_open(&sd_file, (char *)&msgIn[10], FA_WRITE | FA_CREATE_NEW | FA_CREATE_ALWAYS);
		if (fr == FR_EXIST)
		{
			FAT_FS_RETURN_IF_ERR(fr, f_unlink((char *)&msgIn[10]), msgOut[2]);
			goto try_open;
		}
		if (fr != FR_OK)
		{
			msgOut[2] = fr;
			goto exit;
		}

		FAT_FS_RETURN_IF_ERR(fr, f_write(&sd_file, &msgIn[11 + file_name_len], file_content_length, &file_written), msgOut[2]);
	}
	/* Offset != 0 -> append to file */
	else
	{
		/* Open file object */
		FAT_FS_RETURN_IF_ERR(fr, f_open(&sd_file, (char *)&msgIn[10], FA_WRITE), msgOut[2]);

		FAT_FS_RETURN_IF_ERR(fr, f_lseek(&sd_file, file_offset), msgOut[2]);

		FAT_FS_RETURN_IF_ERR(fr, f_write(&sd_file, &msgIn[11 + file_name_len], file_content_length, &file_written), msgOut[2]);
	}

	/* If it can go there, everything is still ok, cleanup and sync */
	if (file_written != file_content_length)
	{
		msgOut[2] = API_SD_DISK_ERR;
		goto exit;
	}

	FAT_FS_RETURN_IF_ERR(fr, f_sync(&sd_file), msgOut[2]);

	FAT_FS_RETURN_IF_ERR(fr, f_close(&sd_file), msgOut[2]);

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdCheckFileMd5(UInt8 *msgIn,
                                        UInt16 msgInLen,
                                        UInt8 *msgOut,
                                        UInt16 *msgOutLen)
{
	FIL sd_file;
	FRESULT fr;
	UInt16 file_name_len;
	UInt32 bytes_read;
	mbedtls_md5_context ctx;
	UInt8 md5_result[16];

	/* Check input param */
	file_name_len = strlen((char *)&msgIn[2]);
	/* 2b: CMD + CTR code
	 * xb: length of file name
	 * 1b: Null of filename
	 * */
	if ((2 + file_name_len + 1) != msgInLen)
	{
		msgOut[2] = API_PARAM_ERROR;
		goto exit;
	}

	FAT_FS_RETURN_IF_ERR(fr, f_mount(&gFatFs, "", 0), msgOut[2]);

	/* Open file object */
	FAT_FS_RETURN_IF_ERR(fr, f_open(&sd_file, (char *)&msgIn[2], FA_READ), msgOut[2]);

	/* Init md5 calculation */
	mbedtls_md5_init(&ctx);
	mbedtls_md5_starts(&ctx);

	/* Loop through file and calculate the CRC value */
	do
	{
		FAT_FS_RETURN_IF_ERR(fr,
		                     f_read(&sd_file,
		                    		 &msgOut[2],
		                    		 MAX_SERIAL_DATA_EXCEPT_CMD - 1,
		                    		 &bytes_read),
		                     msgOut[2]);

		mbedtls_md5_update(&ctx, &msgOut[2], bytes_read);
	}
	while (bytes_read != 0);

	msgOut[2] = API_COMMAND_EXECUTE_SUCCESS;
	mbedtls_md5_finish(&ctx, md5_result);
	memcpy(&msgOut[3], md5_result, sizeof(md5_result));
	*msgOutLen = sizeof(md5_result) + 3;
	mbedtls_md5_free(&ctx);

exit:
	f_close(&sd_file);
	return MT_SUCCESS;
}
/************************* End of File ****************************************/
