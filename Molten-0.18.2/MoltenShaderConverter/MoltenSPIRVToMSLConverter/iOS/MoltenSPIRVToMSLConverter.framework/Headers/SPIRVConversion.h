/*
 * SPIRVConversion.h
 *
 * Copyright (c) 2014-2017 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the Molten distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */

#ifndef __SPIRVConversion_h_
#define __SPIRVConversion_h_ 1

#ifdef __cplusplus
extern "C" {
#endif	//  __cplusplus


#include <stdlib.h>

	
/** This file contains convenience functions for converting SPIR-V to MSL, callable from standard C code. */


/**
 * Convenience function that converts the specified SPIR-V code to Metal Shading Language (MSL),
 * source code, and returns whether the conversion was successful.
 *
 * If the pMSL parameter is not NULL, this function allocates space for the converted 
 * MSL source code, and returns a pointer to that MSL code in the location indicated 
 * by this parameter. It is the responsibility of the caller to free() the memory returned 
 * in this parameter.
 *
 * If the pResultLog parameter is not NULL, a pointer to the contents of the converter
 * results log will be set at the location pointed to by the pResultLog parameter.
 * It is the responsibility of the caller to free() the memory returned in this parameter.
 *
 * The boolean flags indicate whether the original SPIR-V code and resulting MSL source code
 * should be logged to the converter results log. This can be useful during shader debugging.
 */
bool mlnConvertSPIRVToMSL(uint32_t* spvCode,
                          size_t spvLength,
                          char** pMSL,
                          char** pResultLog,
                          bool shouldLogSPIRV,
                          bool shouldLogMSL);

/**
 * Convenience function that converts SPIR-V code in the specified file to 
 * Metal Shading Language (MSL) source code. The file path should either be 
 * absolute or relative to the resource directory.
 *
 * If the pMSL parameter is not NULL, this function allocates space for the converted
 * MSL source code, and returns a pointer to that MSL code in the location indicated
 * by this parameter. It is the responsibility of the caller to free() the memory returned
 * in this parameter.
 *
 * If the pResultLog parameter is not NULL, a pointer to the contents of the converter
 * results log will be set at the location pointed to by the pResultLog parameter.
 * It is the responsibility of the caller to free() the memory returned in this parameter.
 *
 * The boolean flags indicate whether the original SPIR-V code and resulting MSL source code
 * should be logged to the converter results log. This can be useful during shader debugging.
 */
bool mlnConvertSPIRVFileToMSL(const char* spvFilepath,
                              char** pMSL,
                              char** pResultLog,
                              bool shouldLogSPIRV,
                              bool shouldLogMSL);


#ifdef __cplusplus
}
#endif	//  __cplusplus

#endif
