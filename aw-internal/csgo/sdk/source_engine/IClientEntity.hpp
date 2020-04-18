#pragma once

using CBaseHandle = unsigned long;

class IClientUnknown;
class ClientClass;
class bf_read;

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown( ) = 0;
	virtual void Release( ) = 0;
	virtual ClientClass* GetClientClass( ) = 0;
	virtual void NotifyShouldTransmit( int state ) = 0;
	virtual void OnPreDataChanged( int updateType ) = 0;
	virtual void OnDataChanged( int updateType ) = 0;
	virtual void PreDataUpdate( int updateType ) = 0;
	virtual void PostDataUpdate( int updateType ) = 0;
	virtual void __unkn( ) = 0;
	virtual bool IsDormant( ) = 0;
	virtual int Index( ) const = 0;
	virtual void ReceiveMessage( int classID, bf_read& msg ) = 0;
	virtual void* GetDataTableBasePtr( ) = 0;
	virtual void SetDestroyedOnRecreateEntities( ) = 0;
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown( ) = 0;
	virtual math::vec3_t& GetRenderOrigin( void ) = 0;
	virtual math::vec3_t& GetRenderAngles( void ) = 0;
	virtual bool ShouldDraw( void ) = 0;
	virtual int GetRenderFlags( void ) = 0;
	virtual void Unused( void ) const {}
	virtual ClientShadowHandle_t GetShadowHandle( ) const = 0;
	virtual ClientRenderHandle_t& RenderHandle( ) = 0;
	virtual const void* GetModel( ) const = 0;
	virtual int DrawModel( int flags, const uint8_t& instance ) = 0;
	virtual int GetBody( ) = 0;
	virtual void GetColorModulation( float* color ) = 0;
	virtual bool LODTest( ) = 0;
	virtual bool SetupBones( math::matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime ) = 0;
	virtual void SetupWeights( const math::matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights,
							   float* pFlexDelayedWeights ) = 0;
	virtual void DoAnimationEvents( void ) = 0;
	virtual void* GetPVSNotifyInterface( ) = 0;
	virtual void GetRenderBounds( math::vec3_t& mins, math::vec3_t& maxs ) = 0;
	virtual void GetRenderBoundsWorldspace( math::vec3_t& mins, math::vec3_t& maxs ) = 0;
	virtual void GetShadowRenderBounds( math::vec3_t& mins, math::vec3_t& maxs, int shadowType ) = 0;
	virtual bool ShouldReceiveProjectedTextures( int flags ) = 0;
	virtual bool GetShadowCastDistance( float* pDist, int shadowType ) const = 0;
	virtual bool GetShadowCastDirection( math::vec3_t* pDirection, int shadowType ) const = 0;
	virtual bool IsShadowDirty( ) = 0;
	virtual void MarkShadowDirty( bool bDirty ) = 0;
	virtual IClientRenderable* GetShadowParent( ) = 0;
	virtual IClientRenderable* FirstShadowChild( ) = 0;
	virtual IClientRenderable* NextShadowPeer( ) = 0;
	virtual int ShadowCastType( ) = 0;
	virtual void CreateModelInstance( ) = 0;
	virtual ModelInstanceHandle_t GetModelInstance( ) = 0;
	virtual const math::matrix3x4_t& RenderableToWorldTransform( ) = 0;
	virtual int LookupAttachment( const char* pAttachmentName ) = 0;
	virtual bool GetAttachment( int number, math::vec3_t& origin, math::vec3_t& angles ) = 0;
	virtual bool GetAttachment( int number, math::matrix3x4_t& matrix ) = 0;
	virtual float* GetRenderClipPlane( void ) = 0;
	virtual int GetSkin( ) = 0;
	virtual void OnThreadedDrawSetup( ) = 0;
	virtual bool UsesFlexDelayedWeights( ) = 0;
	virtual void RecordToolMessage( ) = 0;
	virtual bool ShouldDrawForSplitScreenUser( int nSlot ) = 0;
	virtual uint8_t OverrideAlphaModulation( uint8_t nAlpha ) = 0;
	virtual uint8_t OverrideShadowAlphaModulation( uint8_t nAlpha ) = 0;
};


class IHandleEntity
{
public:
	virtual ~IHandleEntity( ) {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const CBaseHandle& GetRefEHandle( ) const = 0;
};

class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class ICollideable
{
public:
	virtual void pad0( );
	virtual math::vec3_t& OBBMins( ) const;
	virtual math::vec3_t& OBBMaxs( ) const;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable( ) = 0;
	virtual IClientNetworkable* GetClientNetworkable( ) = 0;
	virtual IClientRenderable* GetClientRenderable( ) = 0;
	virtual IClientEntity* GetIClientEntity( ) = 0;
	virtual C_BaseEntity* GetBaseEntity( ) = 0;
	virtual IClientThinkable* GetClientThinkable( ) = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty( ) = 0;
};

enum Flags_t : int
{
	fl_onground = 1 << 0,
	fl_ducking = 1 << 1,
	fl_waterjump = 1 << 2,
	fl_frozen = 1 << 5,
	fl_fakeclient = 1 << 8,
};

enum Weapon_t : int
{
	weapon_none = 0,
	weapon_deagle,
	weapon_elite,
	weapon_fiveseven,
	weapon_glock,
	weapon_ak47 = 7,
	weapon_aug,
	weapon_awp,
	weapon_famas,
	weapon_g3sg1,
	weapon_galilar = 13,
	weapon_m249,
	weapon_m4a1 = 16,
	weapon_mac10,
	weapon_p90 = 19,
	weapon_mp5sd = 23,
	weapon_ump45,
	weapon_xm1014,
	weapon_bizon,
	weapon_mag7,
	weapon_negev,
	weapon_sawedoff,
	weapon_tec9,
	weapon_taser,
	weapon_hkp2000,
	weapon_mp7,
	weapon_mp9,
	weapon_nova,
	weapon_p250,
	weapon_shield,
	weapon_scar20,
	weapon_sg558,
	weapon_ssg08,
	weapon_knifegg,
	weapon_knife,
	weapon_flashbang,
	weapon_hegrenade,
	weapon_smokegrenade,
	weapon_molotov,
	weapon_decoy,
	weapon_incgrenade,
	weapon_c4,
	weapon_healthshot = 57,
	weapon_knife_t = 59,
	weapon_m4a1_silencer,
	weapon_usp_silencer,
	weapon_cz75a = 63,
	weapon_revolver,
	weapon_tagrenade = 68,
	weapon_fists,
	weapon_breachcharge,
	weapon_tablet = 72,
	weapon_melee = 74,
	weapon_axe,
	weapon_hammer,
	weapon_spanner = 78,
	weapon_knife_ghost = 80,
	weapon_firebomb,
	weapon_diversion,
	weapon_frag_grenade,
	weapon_snowball,
	weapon_bumpmine,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut,
	weapon_knife_karambit,
	weapon_knife_m9_bayonet,
	weapon_knife_tactical,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly,
	weapon_knife_push,
	weapon_knife_ursus = 519,
	weapon_knife_gypsy_jackknife,
	weapon_knife_stiletto = 522,
	weapon_knife_widowmaker,
	glove_studded_bloodhound = 5027,
	glove_t_side = 5028,
	glove_ct_side = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_wrap = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034
};

enum BodyState_t : int
{
	alive = 0,
	dying,
	dead,
	respawnable,
	discardbody,
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
public:
	void Release( ) override = 0;
};