#pragma once

struct CCSWeaponData
{
	char pad_vtable[ 0x4 ]; 
	char* consoleName; 
	char pad_0[ 0xc ];
	int32_t iMaxClip1; 
	int32_t iMaxClip2; 
	int32_t iDefaultClip1; 
	int32_t iDefaultClip2; 
	int32_t iPrimaryReserveAmmoMax; 
	int32_t iSecondaryReserveAmmoMax; 
	char* szWorldModel; 
	char* szViewModel; 
	char* szDroppedModel; 
	char pad_9[ 0x50 ];
	char* szHudName; 
	char* szWeaponName; 
	char pad_11[ 0x2 ];
	bool bIsMeleeWeapon; 
	char pad_12[ 0x9 ];
	float_t flWeaponWeight; 
	char pad_13[ 0x2c ];
	int32_t iWeaponType; 
	int32_t iWeaponPrice; 
	int32_t iKillAward; 
	char pad_16[ 0x4 ];
	float_t flCycleTime; 
	float_t flCycleTimeAlt; 
	char pad_18[ 0x8 ];
	bool bFullAuto; 
	char pad_19[ 0x3 ];
	int32_t iDamage; 
	float_t flArmorRatio; 
	int32_t iBullets; 
	float_t flPenetration; 
	char pad_23[ 0x8 ];
	float_t flWeaponRange; 
	float_t flRangeModifier; 
	float_t flThrowVelocity; 
	char pad_26[ 0xc ];
	bool bHasSilencer; 
	char pad_27[ 0xb ];
	char* szBulletType; 
	float_t flMaxSpeed; 
	char pad_29[ 0x50 ];
	int32_t iRecoilSeed; 
};