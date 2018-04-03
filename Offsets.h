#include "Includes.h"

#define OFFSET_GAMECONTEXT 0x14428A188
#define OFFSET_GAMERENDERER 0x1444F90C8

LPCSTR GameName = "STAR WARS BATTLEFRONT II";



namespace Offsets {
	// Created with ReClass.NET by KN4CK3R


	class ClientSoldierHealthComponent
	{
	public:
		char pad_0000[32]; //0x0000
		float Health; //0x0020
		float m_flMaxHealth; //0x0024
		char pad_0024[20]; //0x0024
		//float MaxHealth; //0x0038
	};

	class ClientSoldierPrediction
	{
	public:
		char pad_0000[32]; //0x0000
		D3DXVECTOR3 Location; //0x0020
		char pad_0001[4];
		D3DXVECTOR3 Velocity; //0x0030
	};

	class ClientAimEntity
	{
	public:
		char pad_0000[224]; //0x0000
		D3DXVECTOR4 CachedRayCast; //0x00E0
	};

	class ClientSoldier
	{
	public:
		char pad_0000[616]; //0x0000
		DWORD64 ClientSoldierHealthComponent; //0x0268
		char pad_0270[512]; //0x0270
		float N000006E3; //0x0470
		float HeightOffset; //0x0474
		char pad_0478[224]; //0x0478
		float N00000700; //0x0558
		float Yaw; //0x055C
		float Pitch; //0x0560
		char pad_0564[404]; //0x0564
		DWORD64 ClientSoldierPrediction; //0x06F8
		char pad_0700[704]; //0x0700
		DWORD64 ClientAimEntity; //0x09C0
		DWORD64 ClientSoldierWeapon; //0x09C8
		char pad_09D0[40]; //0x09D0
		uint8_t Occluded; //0x09F8
		char pad_09F9[1607]; //0x09F9
	}; //Size: 0x1040


	class nameClass
	{
	public:
		char Name[20]; //0x0000
		char pad_0014[64]; //0x0014
	}; //Size: 0x0054


	class ClientPlayer
	{
	public:
		char pad_0000[24]; //0x0000
		DWORD64 NameClass; //0x0018
		char pad_0020[56]; //0x0020
		uint32_t Team; //0x0058
		char pad_005C[420]; //0x005C
		DWORD64 AttachedControllable; //0x0200
		char pad_0208[8]; //0x0208
		DWORD64 ControlledControllable; //0x0210
	//	char pad_0218[3624]; //0x0218
	}; //Size: 0x1040



	class mppPlayer
	{
	public:
		DWORD64 PlayerArray[60]; //0x0000
	}; 

	class PlayerManager
	{
	public:
		char pad_0000[1384]; //0x0000
		DWORD64 LocalPlayer; //0x0568
		char pad_0570[504]; //0x0570
		DWORD64 mppPlayer; //0x0768
	};

	class GameContext
	{
	public:
		char pad_0000[88]; //0x0000
		DWORD64 PlayerManager; //0x0058
	};

	class ClientGameContext
	{
	public:
		DWORD64 GameContext; //0x0000
	};


	class RenderView
	{
	public:
		char pad_0000[800]; //0x0000
		D3DXVECTOR3 CameraPos; //0x0320
		char pad_032C[260]; //0x032C
		D3DXMATRIXA16 viewProj; //0x0430
		//char pad_0470[288]; //0x0470
	}; //Size: 0x0590


	class GameRenderer
	{
	public:
		char pad_0000[1336]; //0x0000
		DWORD64 RenderView; //0x0538
	};

	class StaticGameRenderer
	{
	public:
		DWORD64 GameRenderer; //0x0000
	}; 

}