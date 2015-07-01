/*|-----------------------------------------------------------------------------
 *|            This source code is provided under the Apache 2.0 license      --
 *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
 *|                See the project's LICENSE.md for details.                  --
 *|           Copyright Thomson Reuters 2015. All rights reserved.            --
 *|-----------------------------------------------------------------------------
 */

#include "RefreshMsg.h"
#include "RefreshMsgDecoder.h"
#include "RefreshMsgEncoder.h"
#include "OmmState.h"
#include "EmaBufferInt.h"
#include "Utilities.h"
#include "GlobalPool.h"
#include "RdmUtilities.h"

using namespace thomsonreuters::ema::access;
using namespace thomsonreuters::ema::rdm;

extern const EmaString& getDTypeAsString( DataType::DataTypeEnum dType );

RefreshMsg::RefreshMsg() :
 Msg(),
 _toString()
{
}

RefreshMsg::~RefreshMsg()
{
	if ( _pEncoder )
		g_pool._refreshMsgEncoderPool.returnItem( static_cast<RefreshMsgEncoder*>( _pEncoder ) );

	if ( _pDecoder )
		g_pool._refreshMsgDecoderPool.returnItem( static_cast<RefreshMsgDecoder*>( _pDecoder ) );
}

RefreshMsg& RefreshMsg::clear()
{
	if ( _pEncoder )
		_pEncoder->clear();

	return *this;
}

Data::DataCode RefreshMsg::getCode() const
{
	return Data::NoCodeEnum;
}

DataType::DataTypeEnum RefreshMsg::getDataType() const
{
	return DataType::RefreshMsgEnum;
}

const EmaString& RefreshMsg::toString() const
{
	return toString( 0 );
}

const EmaString& RefreshMsg::toString( UInt64 indent ) const
{
	const RefreshMsgDecoder* pTempDecoder = static_cast<const RefreshMsgDecoder*>( _pDecoder );

	addIndent( _toString.clear(), indent++ ).append( "RefreshMsg" );
	addIndent( _toString, indent, true ).append( "streamId=\"" ).append( pTempDecoder->getStreamId() ).append( "\"" );
	addIndent( _toString, indent, true ).append( "domain=\"" ).append( rdmDomainToString( getDomainType() ) ).append( "\"" );			

	if ( pTempDecoder->getSolicited() )
		addIndent( _toString, indent, true ).append( "solicited" );

	if ( pTempDecoder->getComplete() )
		addIndent( _toString, indent, true ).append( "RefreshComplete" );

	addIndent( _toString, indent, true ).append( "state=\"" ).append( pTempDecoder->getState().toString() ).append( "\"" );

	EmaString temp;
	hexToString( temp, pTempDecoder->getItemGroup() );
	addIndent( _toString, indent, true ).append( "itemGroup=\"" ).append( temp ).append( "\"" );

	if ( pTempDecoder->hasPermissionData() )
	{
		EmaString temp;
		hexToString( temp, pTempDecoder->getPermissionData() );
		addIndent( _toString, indent, true ).append( "permissionData=\"" ).append( temp ).append( "\"" );
	}

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

	if ( _pDecoder->hasPayload() )
	{
		indent++;
		addIndent( _toString, indent, true ).append( "Payload dataType=\"" ).append( getDTypeAsString( pTempDecoder->getPayloadData().getDataType() ) ).append( "\"\n" );

		indent++;
		_toString.append( pTempDecoder->getPayloadData().toString( indent ) );
		indent--;

		addIndent( _toString, indent, true ).append( "PayloadEnd" );
		indent--;
	}
			
	addIndent( _toString, indent, true ).append( "RefreshMsgEnd\n" );

	return _toString;
}

const EmaBuffer& RefreshMsg::getAsHex() const
{
	return static_cast<const RefreshMsgDecoder*>( _pDecoder )->getHexBuffer();
}

bool RefreshMsg::hasQos() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasQos();
}

bool RefreshMsg::hasSeqNum() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasSeqNum();
}

bool RefreshMsg::hasPartNum() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasPartNum();
}

bool RefreshMsg::hasPermissionData() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasPermissionData();
}

bool RefreshMsg::hasPublisherId() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasPublisherId();
}

bool RefreshMsg::hasServiceName() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->hasServiceName();
}

const OmmState& RefreshMsg::getState() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getState();
}

const OmmQos& RefreshMsg::getQos() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getQos();
}

UInt32 RefreshMsg::getSeqNum() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getSeqNum();
}

UInt16 RefreshMsg::getPartNum() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getPartNum();
}

const EmaBuffer& RefreshMsg::getItemGroup() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getItemGroup();
}

const EmaBuffer& RefreshMsg::getPermissionData() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getPermissionData();
}

UInt32 RefreshMsg::getPublisherIdUserId() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getPublisherIdUserId();
}

UInt32 RefreshMsg::getPublisherIdUserAddress() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getPublisherIdUserAddress();
}

bool RefreshMsg::getDoNotCache() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getDoNotCache();
}

bool RefreshMsg::getSolicited() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getSolicited();
}

bool RefreshMsg::getComplete() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getComplete();
}

bool RefreshMsg::getClearCache() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getClearCache();
}

bool RefreshMsg::getPrivateStream() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getPrivateStream();
}

const EmaString& RefreshMsg::getServiceName() const
{
	return static_cast<const RefreshMsgDecoder*>(_pDecoder)->getServiceName();
}

Decoder& RefreshMsg::getDecoder()
{
	if ( !_pDecoder )
		Msg::setDecoder( g_pool._refreshMsgDecoderPool.getItem() );

	return *_pDecoder;
}

RefreshMsg& RefreshMsg::streamId( Int32 streamId )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->streamId( streamId );
	return *this;
}

RefreshMsg& RefreshMsg::domainType( UInt16 domainType )
{
	if ( domainType > 255 )
	{
		EmaString temp( "Passed in DomainType is out of range." );
		throwDtuException( domainType, temp );
		return *this;
	}

	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->domainType( (UInt8)domainType );
	return *this;
}

RefreshMsg& RefreshMsg::name( const EmaString& name )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->name( name );
	return *this;
}

RefreshMsg& RefreshMsg::nameType( UInt8 nameType )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->nameType( nameType );
	return *this;
}

RefreshMsg& RefreshMsg::serviceName( const EmaString& serviceName )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->serviceName( serviceName );
	return *this;
}

RefreshMsg& RefreshMsg::serviceId( UInt32 serviceId )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->serviceId( serviceId );
	return *this;
}

RefreshMsg& RefreshMsg::id( Int32 id )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->identifier( id );
	return *this;
}

RefreshMsg& RefreshMsg::filter( UInt32 filter )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->filter( filter );
	return *this;
}

RefreshMsg& RefreshMsg::qos( UInt32 timeliness, UInt32 rate )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->qos( timeliness, rate );
	return *this;
}

RefreshMsg& RefreshMsg::state( OmmState::StreamState streamState, OmmState::DataState dataState,
						UInt8 statusCode, const EmaString& statusText )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->state( streamState, dataState, statusCode, statusText );
	return *this;
}

RefreshMsg& RefreshMsg::seqNum( UInt32 seqNum )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->seqNum( seqNum );
	return *this;
}

RefreshMsg& RefreshMsg::partNum( UInt16 partNum )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->partNum( partNum );
	return *this;
}

RefreshMsg& RefreshMsg::itemGroup( const EmaBuffer& itemGroup )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->itemGroup( itemGroup );
	return *this;
}

RefreshMsg& RefreshMsg::permissionData( const EmaBuffer& permissionData )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->permissionData( permissionData );
	return *this;
}

RefreshMsg& RefreshMsg::publisherId( UInt32 userId, UInt32 userAddress )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->publisherId( userId, userAddress );
	return *this;
}

RefreshMsg& RefreshMsg::attrib( const ComplexType& data )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->attrib( data );
	return *this;
}

RefreshMsg& RefreshMsg::payload( const ComplexType& data )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	_pEncoder->payload( data );
	return *this;
}

RefreshMsg& RefreshMsg::extendedHeader( const EmaBuffer& buffer )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->extendedHeader( buffer );
	return *this;
}

RefreshMsg& RefreshMsg::solicited( bool solicited )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->solicited( solicited );
	return *this;
}

RefreshMsg& RefreshMsg::doNotCache( bool doNotCache )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->doNotCache( doNotCache );
	return *this;
}

RefreshMsg& RefreshMsg::clearCache( bool clearCache )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->clearCache( clearCache );
	return *this;
}

RefreshMsg& RefreshMsg::complete( bool complete )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->complete( complete );
	return *this;
}

RefreshMsg& RefreshMsg::privateStream( bool privateStream )
{
	if ( !_pEncoder )
		_pEncoder = g_pool._refreshMsgEncoderPool.getItem();

	static_cast<RefreshMsgEncoder*>(_pEncoder)->privateStream( privateStream );
	return *this;
}
