//***************************************************************************
//
//  Copyright (c) 2001 - 2006 Intel Corporation
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//***************************************************************************

/**
	@file	CIFXAnimationModifierDecoder.h

			Declaration of the CIFXAnimationModifierDecoder.
			The CIFXAnimationModifierDecoder is used by the CIFXLoadManager to
			load skeleton modifier params into the scene graph.
			CIFXAnimationModifierDecoder exposes a IFXDecoder interface to the
			CIFXLoadManager for this purpose.
*/

#ifndef CIFXAnimationModifierDecoder_H
#define CIFXAnimationModifierDecoder_H

#include "IFXDecoderX.h"
#include "IFXDataBlockQueueX.h"
#include "IFXAnimationModifier.h"
#include "IFXAutoRelease.h"
#include "IFXSceneGraph.h"
#include "IFXModifierBaseDecoder.h"

class CIFXAnimationModifierDecoder : virtual public IFXDecoderX,
											 public IFXModifierBaseDecoder
{
public:
	// Factory function.
	friend IFXRESULT IFXAPI_CALLTYPE CIFXAnimationModifierDecoder_Factory( IFXREFIID interfaceId, void** ppInterface );

	// IFXUnknown
	virtual U32 IFXAPI  AddRef( void );
	virtual U32 IFXAPI  Release( void );
	virtual IFXRESULT IFXAPI  QueryInterface( IFXREFIID interfaceId, void** ppInterface );

	// IFXDecoderX
	virtual void IFXAPI  InitializeX( const IFXLoadConfig &lc );
	virtual void IFXAPI  PutNextBlockX(IFXDataBlockX &rDataBlockX) ;
	virtual void IFXAPI  TransferX(IFXRESULT &rWarningPartialTransfer) ;

private:
	// Member functions.
    CIFXAnimationModifierDecoder();
	virtual ~CIFXAnimationModifierDecoder();

	// Member data.
	IFXDECLAREMEMBER(IFXAnimationModifier,m_pBonesModifier);

	F64 m_unitScale;
};

#endif
