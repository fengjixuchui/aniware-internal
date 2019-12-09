#pragma once

enum
{
	FLOW_OUTGOING,
	FLOW_INCOMING,
	MAX_FLOWS,
};

struct INetChannelInfo
{
	enum
	{
		GENERIC,
		LOCALPLAYER,
		OTHERPLAYERS,
		ENTITIES,
		SOUNDS,
		EVENTS,
		USERMESSAGES,
		ENTMESSAGES,
		VOICE,
		STRINGTABLE,
		MOVE,
		STRINGCMD,
		SIGNON,
		TOTAL,
	};

	VFUNC( 10, GetLatency( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
	VFUNC( 11, GetAverageLatency( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
	VFUNC( 12, GetAverageLoss( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
	VFUNC( 13, GetAverageChoke( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
	VFUNC( 14, GetAverageData( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
	VFUNC( 15, GetAveragePackets( const int flow ), float( __thiscall* )( void*, const int ) )( flow );
};