/**
 ******************************************************************************
 * @file    tftp.h
 * @author  William Xu
 * @version V1.0.0
 * @date    20-June-2015
 * @brief   This file provides other tftp API header file.
 ******************************************************************************
 *
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

#include <stdio.h>		/*for input and output */
#include <string.h>		/*for string functions */
#include <stdlib.h> 		/**/

typedef struct
{
    char filename[32];
    uint32_t filelen;
    uint32_t flashaddr; // the flash address of this file
    int flashtype; // SPI flash or Internal flash.
} tftp_file_info_t;

int tsend( tftp_file_info_t *fileinfo, uint32_t ipaddr );
/*a function to get a file from the server*/
int tget( tftp_file_info_t *fileinfo, uint32_t ipaddr );

