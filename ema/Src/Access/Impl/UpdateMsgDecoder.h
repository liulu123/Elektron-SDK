/*|-----------------------------------------------------------------------------
 *|            This source code is provided under the Apache 2.0 license      --
 *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
 *|                See the project's LICENSE.md for details.                  --
 *|           Copyright Thomson Reuters 2015. All rights reserved.            --
 *|-----------------------------------------------------------------------------
 */

#ifndef __thomsonreuters_ema_access_UpdateMsgDecoder_h
#define __thomsonreuters_ema_access_UpdateMsgDecoder_h

#include "EmaPool.h"
#include "MsgDecoder.h"
#include "UpdateMsg.h"
#include "EmaStringInt.h"
#include "EmaBufferInt.h"

namespace thomsonreuters {

namespace ema {

namespace access {

class UpdateMsgDecoder : public MsgDecoder
{
public :

	UpdateMsgDecoder();

	~UpdateMsgDecoder();

	void setRsslData( UInt8 , UInt8 , RsslMsg* , const RsslDataDictionary* );

	void setRsslData( UInt8 , UInt8 , RsslBuffer* , const RsslDataDictionary* , void* );

	void setRsslData( RsslDecodeIterator* , RsslBuffer* );

	bool hasMsgKey() const;

	bool hasName() const;

	bool hasNameType() const;

	bool hasServiceId() const;

	bool hasServiceName() const;

	bool hasId() const;

	bool hasFilter() const;

	bool hasAttrib() const;

	bool hasPayload() const;

	bool hasExtendedHeader() const;

	Int32 getStreamId() const;

	UInt16 getDomainType() const;

	const EmaString& getName() const;

	UInt8 getNameType() const;

	UInt32 getServiceId() const;

	const EmaString& getServiceName() const;

	Int32 getId() const;

	UInt32 getFilter() const;

	const EmaBuffer& getExtendedHeader() const;

	bool hasSeqNum() const;

	bool hasPermissionData() const;

	bool hasConflated() const;

	bool hasPublisherId() const;

	UInt8 getUpdateTypeNum() const;

	UInt32 getSeqNum() const;

	const EmaBuffer& getPermissionData() const;

	UInt16 getConflatedTime() const;

	UInt16 getConflatedCount() const;

	UInt32 getPublisherIdUserId() const;

	UInt32 getPublisherIdUserAddress() const;

	bool getDoNotCache() const;

	bool getDoNotConflate() const;

	bool getDoNotRipple() const;

	void setServiceName( const char* , UInt32 , bool nullTerm = true );

	const EmaBuffer& getHexBuffer() const;

private :

	RsslMsg								_rsslMsg;

	RsslMsg*							_pRsslMsg;

	mutable EmaStringInt				_name;

	mutable EmaStringInt				_serviceName;

	mutable EmaBufferInt				_extHeader;

	mutable EmaBufferInt				_permission;

	mutable EmaBufferInt				_hexBuffer;

	mutable bool						_serviceNameSet;

	UInt8								_rsslMajVer;

	UInt8								_rsslMinVer;

	mutable OmmError::ErrorCode			_errorCode;
};

class UpdateMsgDecoderPool : public DecoderPool< UpdateMsgDecoder >
{
public :

	UpdateMsgDecoderPool( unsigned int size = 5 ) : DecoderPool< UpdateMsgDecoder >( size ) {};

	~UpdateMsgDecoderPool() {}

private :

	UpdateMsgDecoderPool( const UpdateMsgDecoderPool& );
	UpdateMsgDecoderPool& operator=( const UpdateMsgDecoderPool& );
};

}

}

}

#endif // __thomsonreuters_ema_access_UpdateMsgDecoder_h
