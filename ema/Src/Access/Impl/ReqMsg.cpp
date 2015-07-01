/*|-----------------------------------------------------------------------------
 *|            This source code is provided under the Apache 2.0 license      --
 *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
 *|                See the project's LICENSE.md for details.                  --
 *|           Copyright Thomson Reuters 2015. All rights reserved.            --
 *|-----------------------------------------------------------------------------
 */

#include "ReqMsg.h"
#include "ReqMsgDecoder.h"
#include "ReqMsgEncoder.h"
#include "Utilities.h"
#include "GlobalPool.h"
#include "RdmUtilities.h"

using namespace thomsonreuters::ema::access;
using namespace thomsonreuters::ema::rdm;

extern const EmaString& getDTypeAsString( DataType::DataTypeEnum dType );

const EmaString TickByTickName( "TickByTick" );
const EmaString JustInTimeConflatedRateName( "JustInTimeConflatedrate" );
const EmaString BestConflatedRateName( "BestConflatedRate" );
const EmaString BestRateName( "BestRate" );
EmaString UnknownReqMsgQosRateName;

const EmaString RealTimeName( "RealTime" );
const EmaString BestDelayedTimelinessName( "BestDelayedTimeliness" );
const EmaString BestTimelinessName( "BestTimeliness" );
EmaString UnknownReqMsgQosTimelinessName;

ReqMsg::ReqMsg() :
 Msg(),
 _toString()
{
}

ReqMsg::~ReqMsg()
{
	if ( _pEncoder )
		g_pool._reqMsgEncoderPool.returnItem( static_cast<ReqMsgEncoder*>( _pEncoder ) );

	if ( _pDecoder )
		g_pool._reqMsgDecoderPool.returnItem( static_cast<ReqMsgDecoder*>( _pDecoder ) );
}

const EmaString& ReqMsg::getRateAsString() const
{
	switch ( getQosRate() )
	{
	case TickByTickEnum :
		return TickByTickName;
	case JustInTimeConflatedEnum :
		return JustInTimeConflatedRateName;
	case BestConflatedRateEnum :
		return BestConflatedRateName;
	case BestRateEnum :
		return BestRateName;
	default :
		return UnknownReqMsgQosRateName.set( "Rate on ReqMsg. Value = " ).append( (UInt64)getQosRate() );
	}
}

const EmaString& ReqMsg::getTimelinessAsString() const
{
	switch ( getQosTimeliness() )
	{
	case RealTimeEnum :
		return RealTimeName;
	case BestDelayedTimelinessEnum :
		return BestDelayedTimelinessName;
	case BestTimelinessEnum :
		return BestTimelinessName;
	default :
		return UnknownReqMsgQosTimelinessName.set( "QosTimeliness on ReqMsg. Value = " ).append( (UInt64)getQosTimeliness() );
	}
}

ReqMsg& ReqMsg::clear()
{
	if ( _pEncoder )
		_pEncoder->clear();

	return *this;
}

Data::DataCode ReqMsg::getCode() const
{
	return Data::NoCodeEnum;
}

DataType::DataTypeEnum ReqMsg::getDataType() const
{
	return DataType::ReqMsgEnum;
}

const EmaString& ReqMsg::toString() const
{
	return toString( 0 );
}

const EmaString& ReqMsg::toString(  UInt64 indent ) const
{
	const ReqMsgDecoder* pTempDecoder = static_cast<const ReqMsgDecoder*>( _pDecoder );

	addIndent( _toString.clear(), indent++ ).append( "ReqMsg" );
	addIndent( _toString, indent, true ).append( "streamId=\"" ).append( pTempDecoder->getStreamId() ).append( "\"" );
	addIndent( _toString, indent, true ).append( "domain=\"" ).append( rdmDomainToString( getDomainType() ) ).append( "\"" );

	indent--;
	if ( pTempDecoder->hasMsgKey() )
	{
		indent++;
		if ( pTempDecoder->hasName() )
			addIndent( _toString, indent, true ).append( "name=\"" ).append( pTempDecoder->getName() ).append( "\"" );

		if ( pTempDecoder->hasNameType() )
			addIndent( _toString, indent, true ).append( "nameType=\"" ).append( pTempDecoder->getNameType() ).append( "\"" );

		if ( pTempDecoder->hasServiceId() )
			addIndent( _toString, indent, true ).append( "serviceId=\"" ).append( pTempDecoder->getServiceId() ).append( "\"" );

		if ( pTempDecoder->hasServiceName() )
			addIndent( _toString, indent, true ).append( "serviceName=\"" ).append( pTempDecoder->getServiceName() ).append( "\"" );

		if ( pTempDecoder->hasFilter() )
			addIndent( _toString, indent, true ).append( "filter=\"" ).append( pTempDecoder->getFilter() ).append( "\"" );

		if ( pTempDecoder->hasId() )
			addIndent( _toString, indent, true ).append( "id=\"" ).append( pTempDecoder->getId() ).append( "\"" );
			
		indent--;
		if ( pTempDecoder->hasAttrib() )
		{
			indent++;
			addIndent( _toString, indent, true ).append( "Attrib dataType=\"" ).append( getDTypeAsString( pTempDecoder->getAttribData().getDataType() ) ).append( "\"\n" );

			indent++;
			_toString.append( pTempDecoder->getAttribData().toString( indent ) );
			indent--;

			addIndent( _toString, indent, true ).append( "AttribEnd" );
			indent--;
		}
	}

	if ( pTempDecoder->hasExtendedHeader() )
	{
		indent++;
		addIndent( _toString, indent, true ).append( "ExtendedHeader\n" );

		indent++;

		addIndent( _toString, indent );
		hexToString( _toString, pTempDecoder->getExtendedHeader() );

		indent--;

		addIndent( _toString, indent, true ).append( "ExtendedHeaderEnd" );
		indent--;
	}

	if ( pTempDecoder->hasPayload() )
	{
		indent++;
		addIndent( _toString, indent, true ).append( "Payload dataType=\"" ).append( getDTypeAsString( pTempDecoder->getPayloadData().getDataType() ) ).append( "\"\n" );

		indent++;
		_toString.append( pTempDecoder->getPayloadData().toString( indent ) );
		indent--;

		addIndent( _toString, indent, true ).append( "PayloadEnd" );
		indent--;
	}
			
	addIndent( _toString, indent, true ).append( "ReqMsgEnd\n" );

	return _toString;
}

const EmaBuffer& ReqMsg::getAsHex() const
{
	return static_cast<const ReqMsgDecoder*>( _pDecoder )->getHexBuffer();
}

bool ReqMsg::hasPriority() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->hasPriority();
}

bool ReqMsg::hasQos() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->hasQos();
}

bool ReqMsg::hasView() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->hasView();
}

bool ReqMsg::hasBatch() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->hasBatch();
}

bool ReqMsg::hasServiceName() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->hasServiceName();
}

UInt8 ReqMsg::getPriorityClass() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getPriorityClass();
}

UInt16 ReqMsg::getPriorityCount() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getPriorityCount();
}

UInt32 ReqMsg::getQosTimeliness() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getTimeliness();
}

UInt32 ReqMsg::getQosRate() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getRate();
}

bool ReqMsg::getInitialImage() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getInitialImage();
}

bool ReqMsg::getInterestAfterRefresh() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getInterestAfterRefresh();
}

bool ReqMsg::getConflatedInUpdates() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getConflatedInUpdates();
}

bool ReqMsg::getPause() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getPause();
}

bool ReqMsg::getPrivateStream() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getPrivateStream();
}

const EmaString& ReqMsg::getServiceName() const
{
	return static_cast<const ReqMsgDecoder*>(_pDecoder)->getServiceName();
}

Decoder& ReqMsg::getDecoder()
{
	if ( !_pDecoder )
		Msg::setDecoder( g_pool._reqMsgDecoderPool.getItem() );

	return *_pDecoder;
}

ReqMsg& ReqMsg::name( const EmaString& name )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->name( name );
	return *this;
}

ReqMsg& ReqMsg::nameType( UInt8 nameType )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->nameType( nameType );
	return *this;
}

ReqMsg& ReqMsg::serviceName( const EmaString& serviceName )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->serviceName( serviceName );
	return *this;
}

ReqMsg& ReqMsg::serviceId( UInt32 serviceId )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->serviceId( serviceId );
	return *this;
}

ReqMsg& ReqMsg::id( Int32 id )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->identifier( id );
	return *this;
}

ReqMsg& ReqMsg::filter( UInt32 filter )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->filter( filter );
	return *this;
}

ReqMsg& ReqMsg::streamId( Int32 streamId )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->streamId( streamId );
	return *this;
}

ReqMsg& ReqMsg::domainType( UInt16 domainType )
{
	if ( domainType > 255 )
	{
		EmaString temp( "Passed in DomainType is out of range." );
		throwDtuException( domainType, temp );
		return *this;
	}

	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->domainType( (UInt8)domainType );
	return *this;
}

ReqMsg& ReqMsg::priority( UInt8 priorityClass, UInt16 priorityCount )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->priority( priorityClass, priorityCount );
	return *this;
}

ReqMsg& ReqMsg::qos( UInt32 timeliness, UInt32 rate )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->qos( timeliness, rate );
	return *this;
}

ReqMsg& ReqMsg::attrib( const ComplexType& data )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->attrib( data );
	return *this;
}

ReqMsg& ReqMsg::payload( const ComplexType& data )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	_pEncoder->payload( data );
	return *this;
}

ReqMsg& ReqMsg::extendedHeader( const EmaBuffer& Buffer )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->extendedHeader( Buffer );
	return *this;
}

ReqMsg& ReqMsg::initialImage( bool initialImage )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->initialImage( initialImage );
	return *this;
}

ReqMsg& ReqMsg::interestAfterRefresh( bool interestAfterRefresh )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->interestAfterRefresh( interestAfterRefresh );
	return *this;
}

ReqMsg& ReqMsg::pause( bool pause )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->pause( pause );
	return *this;
}

ReqMsg& ReqMsg::conflatedInUpdates( bool conflatedInUpdates )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->conflatedInUpdates( conflatedInUpdates );
	return *this;
}

ReqMsg& ReqMsg::privateStream( bool privateStream )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._reqMsgEncoderPool.getItem();

	static_cast<ReqMsgEncoder*>(_pEncoder)->privateStream( privateStream );
	return *this;
}
