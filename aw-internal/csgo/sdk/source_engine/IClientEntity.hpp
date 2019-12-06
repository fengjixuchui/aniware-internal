#pragma once

using CBaseHandle = unsigned long;

class IClientUnknown;
class ClientClass;
class bf_read;

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void NotifyShouldTransmit( int state ) = 0;
	virtual void OnPreDataChanged( int updateType ) = 0;
	virtual void OnDataChanged( int updateType ) = 0;
	virtual void PreDataUpdate( int updateType ) = 0;
	virtual void PostDataUpdate( int updateType ) = 0;
	virtual void __unkn() = 0;
	virtual bool IsDormant() = 0;
	virtual int EntIndex() const = 0;
	virtual void ReceiveMessage( int classID, bf_read& msg ) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void SetDestroyedOnRecreateEntities() = 0;
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual math::vec3_t& GetRenderOrigin( void ) = 0;
	virtual math::vec3_t& GetRenderAngles( void ) = 0;
	virtual bool ShouldDraw( void ) = 0;
	virtual int GetRenderFlags( void ) = 0;
	virtual void Unused( void ) const {}
	virtual ClientShadowHandle_t GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual const void* GetModel() const = 0;
	virtual int DrawModel( int flags, const uint8_t& instance ) = 0;
	virtual int GetBody() = 0;
	virtual void GetColorModulation( float* color ) = 0;
	virtual bool LODTest() = 0;
	virtual bool SetupBones( math::matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime ) = 0;
	virtual void SetupWeights( const math::matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights,
							   float* pFlexDelayedWeights ) = 0;
	virtual void DoAnimationEvents( void ) = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void GetRenderBounds( math::vec3_t& mins, math::vec3_t& maxs ) = 0;
	virtual void GetRenderBoundsWorldspace( math::vec3_t& mins, math::vec3_t& maxs ) = 0;
	virtual void GetShadowRenderBounds( math::vec3_t& mins, math::vec3_t& maxs, int shadowType ) = 0;
	virtual bool ShouldReceiveProjectedTextures( int flags ) = 0;
	virtual bool GetShadowCastDistance( float* pDist, int shadowType ) const = 0;
	virtual bool GetShadowCastDirection( math::vec3_t* pDirection, int shadowType ) const = 0;
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty( bool bDirty ) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int ShadowCastType() = 0;
	virtual void CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t GetModelInstance() = 0;
	virtual const math::matrix3x4_t& RenderableToWorldTransform() = 0;
	virtual int LookupAttachment( const char* pAttachmentName ) = 0;
	virtual bool GetAttachment( int number, math::vec3_t& origin, math::vec3_t& angles ) = 0;
	virtual bool GetAttachment( int number, math::matrix3x4_t& matrix ) = 0;
	virtual float* GetRenderClipPlane( void ) = 0;
	virtual int GetSkin() = 0;
	virtual void OnThreadedDrawSetup() = 0;
	virtual bool UsesFlexDelayedWeights() = 0;
	virtual void RecordToolMessage() = 0;
	virtual bool ShouldDrawForSplitScreenUser( int nSlot ) = 0;
	virtual uint8_t OverrideAlphaModulation( uint8_t nAlpha ) = 0;
	virtual uint8_t OverrideShadowAlphaModulation( uint8_t nAlpha ) = 0;
};


class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
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
	virtual void pad0();
	virtual math::vec3_t& OBBMins() const;
	virtual math::vec3_t& OBBMaxs() const;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

enum
{
	FL_ONGROUND = 1 << 0,
	FL_DUCKING = 1 << 1,
	FL_WATERJUMP = 1 << 2,
	FL_FROZEN = 1 << 5,
	FL_FAKECLIENT = 1 << 8,
};

enum 
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG558,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034
};

enum
{
	ALIVE = 0,
	DYING,
	DEAD,
	RESPAWNABLE,
	DISCARDBODY,
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
public:
	void Release() override = 0;
};