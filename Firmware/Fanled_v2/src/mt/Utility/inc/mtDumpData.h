/*
 * mtDumpData.h
 *
 *  Created on: Aug 18, 2016
 *      Author: zealot
 */

#ifndef MTDUMPDATA_H_
#define MTDUMPDATA_H_

#include "mtInclude.h"

/*!
 * \brief Dump buffer
 * \param printStr string that will be print before dumping the data
 * \param buff buffer that will be dumped
 * \param msgLen len of message that will be dumped
 */
Void mtSerialCmdDumpBufferDataRaw(char *printStr, Void *buff, UInt16 msgLen);

/*!
 * \brief Dump buffer, with intention that the buffer contain Command/Response data
 * \param printStr string that will be print before dumping the data
 * \param buff buffer that will be dumped
 * \param msgLen len of message that will be dumped
 */
Void mtSerialCmdDumpBufferData(char *printStr, Void *buff, UInt16 msgLen);

#endif /* MTDUMPDATA_H_ */
