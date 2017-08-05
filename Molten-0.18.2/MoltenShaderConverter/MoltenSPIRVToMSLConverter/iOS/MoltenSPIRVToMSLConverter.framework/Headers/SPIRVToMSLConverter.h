/*
 * SPIRVToMSLConverter.h
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

#ifndef __SPIRVToMSLConverter_h_
#define __SPIRVToMSLConverter_h_ 1

#include "spirv.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace molten {


#pragma mark -
#pragma mark SPIRVToMSLConverterContext

	/** Options for converting SPIR-V to Metal Shading Language */
	typedef struct SPIRVToMSLConverterOptions {
		bool shouldFlipVertexY = true;
		bool isRenderingPoints = false;

        /** 
         * Returns whether the specified options match this one.
         * It does if all corresponding elements are equal.
         */
        bool matches(SPIRVToMSLConverterOptions& other);

	} SPIRVToMSLConverterOptions;

	/**
	 * Defines MSL characteristics of a vertex attribute at a particular location.
	 * The isUsedByShader flag is set to true during conversion of SPIR-V to MSL
	 * if the shader makes use of this vertex attribute.
	 */
	typedef struct MSLVertexAttribute {
		uint32_t location = 0;
		uint32_t mslBuffer = 0;
        uint32_t mslOffset = 0;
        uint32_t mslStride = 0;
        bool isPerInstance = false;

		bool isUsedByShader = false;

        /**
         * Returns whether the specified vertex attribute match this one.
         * It does if all corresponding elements except isUsedByShader are equal.
         */
        bool matches(MSLVertexAttribute& other);
        
	} MSLVertexAttribute;

	/**
	 * Matches the binding index of a MSL resource for a binding within a descriptor set.
	 * Taken together, the stage, desc_set and binding combine to form a reference to a resource
	 * descriptor used in a particular shading stage. Generally, only one of the buffer, texture,
	 * or sampler elements will be populated. The isUsedByShader flag is set to true during
	 * compilation of SPIR-V to MSL if the shader makes use of this vertex attribute.
	 */
	typedef struct MSLResourceBinding {
		spv::ExecutionModel stage;
		uint32_t descriptorSet = 0;
		uint32_t binding = 0;

		uint32_t mslBuffer = 0;
		uint32_t mslTexture = 0;
		uint32_t mslSampler = 0;

		bool isUsedByShader = false;

        /**
         * Returns whether the specified resource binding match this one.
         * It does if all corresponding elements except isUsedByShader are equal.
         */
        bool matches(MSLResourceBinding& other);

    } MSLResourceBinding;

	/** Context passed to the SPIRVToMSLConverter to map SPIR-V descriptors to Metal resource indices. */
	typedef struct SPIRVToMSLConverterContext {
		SPIRVToMSLConverterOptions options;
		std::vector<MSLVertexAttribute> vertexAttributes;
		std::vector<MSLResourceBinding> resourceBindings;

        /** Returns whether the vertex attribute at the specified index is used by the shader. */
        bool isVertexAttributeUsed(uint32_t vaIdx);

        /** Returns whether the vertex buffer at the specified Metal binding index is used by the shader. */
        bool isVertexBufferUsed(uint32_t mslBuffer);

        /**
         * Returns whether this context matches the other context. It does if the respective 
         * options match and any vertex attributes and resource bindings used by this context
         * can be found in the other context. Vertex attributes and resource bindings that are
         * in the other context but are not used by the shader that created this context, are ignored.
         */
        bool matches(SPIRVToMSLConverterContext& other);

        /** Aligns the usage of this context with that of the source context. */
        void alignUsageWith(SPIRVToMSLConverterContext& srcContext);

	} SPIRVToMSLConverterContext;

	/** Special constant used in a MSLResourceBinding descriptorSet element to indicate the bindings for the push constants. */
	static const uint32_t kPushConstDescSet = UINT32_MAX;

	/** Special constant used in a MSLResourceBinding binding element to indicate the bindings for the push constants. */
	static const uint32_t kPushConstBinding = 0;


#pragma mark -
#pragma mark SPIRVToMSLConverter

	/** Converts SPIR-V code to Metal Shading Language code. */
	class SPIRVToMSLConverter {

	public:

		/** Sets the SPIRV code. */
		void setSPIRV(const std::vector<uint32_t>& spirv);

		/**
		 * Sets the SPIRV code from the specified array of values.
		 * The length parameter indicates the number of uint values to store.
		 */
		void setSPIRV(const uint32_t* spirvCode, size_t length);

		/** Returns a reference to the SPIRV code, set by one of the setSPIRV() functions. */
		const std::vector<uint32_t>& getSPIRV();

		/**
		 * Converts SPIR-V code, set using setSPIRV() to MSL code, which can be retrieved using getMSL().
		 *
		 * The boolean flags indicate whether the original SPIR-V code, the resulting MSL code, 
         * and optionally, the original GLSL (as converted from the SPIR_V), should be logged 
         * to the result log of this converter. This can be useful during shader debugging.
		 */
		bool convert(SPIRVToMSLConverterContext& context,
                     bool shouldLogSPIRV = false,
                     bool shouldLogMSL = false,
                     bool shouldLogGLSL = false);

		/**
		 * Returns the Metal Shading Language source code
		 * most recently converted by the convert() function.
		 */
		const std::string& getMSL();

		/**
		 * Returns whether the most recent conversion was successful.
		 *
		 * The initial value of this property is NO. It is set to YES upon successful conversion.
		 */
		bool getWasConverted();

		/**
		 * Returns a human-readable log of the most recent conversion activity.
		 * This may be empty if the conversion was successful.
		 */
		const std::string& getResultLog();

	protected:
		void logMsg(const char* logMsg);
		bool logError(const char* errMsg);
		void logSPIRV(const char* opDesc);
		bool validateSPIRV();
        void logSource(std::string& src, const char* srcLang, const char* opDesc);

		std::vector<uint32_t> _spirv;
		std::string _msl;
		std::string _resultLog;
		bool _wasConverted = false;
	};


#pragma mark Support functions

	/**
	 * Cleans the specified shader function name so it can be used as as an MSL function name.
	 * The cleansed name is returned. The original name is left unmodified.
	 */
	std::string cleanMSLFuncName(const std::string& funcName);

	/** Appends the SPIR-V in human-readable form to the specified log string. */
	void logSPIRV(std::vector<uint32_t>& spirv, std::string& spvLog);

}
#endif
