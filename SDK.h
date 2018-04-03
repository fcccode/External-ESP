#include "Memory.h"
#include "Offsets.h"


#pragma warning(disable:4996)

RPM mem;





class Player {
private:
	Offsets::ClientPlayer pClientPlayer;
public:
	Player() {}
	Player(DWORD64 _ptr) {
		this->ptr = _ptr;
		pClientPlayer = mem.read<Offsets::ClientPlayer>(this->ptr);
		Offsets::ClientSoldier pClientSoldier = mem.read<Offsets::ClientSoldier>(pClientPlayer.ControlledControllable);
		Offsets::nameClass pnameclass = mem.read<Offsets::nameClass>(pClientPlayer.NameClass);
		Offsets::ClientSoldierHealthComponent pHealthComponent = mem.read<Offsets::ClientSoldierHealthComponent>(pClientSoldier.ClientSoldierHealthComponent);
		Offsets::ClientSoldierPrediction pSoldierPrediction = mem.read<Offsets::ClientSoldierPrediction>(pClientSoldier.ClientSoldierPrediction);
		try {
			if (pClientPlayer.NameClass != 0) {
				if (pnameclass.Name != 0) {
					std::wstring tmpname = std::wstring(&pnameclass.Name[0], &pnameclass.Name[20]);
					auto czech = wcstok(&tmpname[0], L" ");
					if (czech != nullptr)
						this->Name = czech;
				}
			}
		}
		catch(const std::exception &exc){}
		
		this->Health = pHealthComponent.Health;
		this->MaxHealth = pHealthComponent.m_flMaxHealth;
		this->Location = pSoldierPrediction.Location;
		this->Velocity = pSoldierPrediction.Velocity;
		this->Pitch = pClientSoldier.Pitch;
		this->Yaw = abs(pClientSoldier.Yaw);
		this->Visible = 1 - pClientSoldier.Occluded;
		this->SoldierHeight = pClientSoldier.HeightOffset;
		D3DXVECTOR3 HeadPos = this->Location;
		HeadPos.y += (this->SoldierHeight * .9);
		this->AimHead = HeadPos;
		D3DXVECTOR3 BodyPos = this->Location;
		BodyPos.y += (this->SoldierHeight * .7);
	//	std::cout << this->SoldierHeight << std::endl;
		this->AimBody = BodyPos;
		(pClientPlayer.AttachedControllable == 0) ? this->InVehicle = false : this->InVehicle = true;
		this->Team = pClientPlayer.Team;
		this->pWeapon = pClientSoldier.ClientSoldierWeapon;
	}
	std::wstring Name = L"";
	DWORD64 ptr;
	FLOAT Health = 0;
	FLOAT MaxHealth = 0;
	int Team = 0;
	D3DXVECTOR3 Location = D3DXVECTOR3();
	D3DXVECTOR3 Velocity = D3DXVECTOR3();
	BOOL Visible = false;
	FLOAT Pitch = 0;
	FLOAT Yaw = 0;
	D3DXVECTOR3 AimBody = D3DXVECTOR3();
	D3DXVECTOR3 AimHead = D3DXVECTOR3();
	FLOAT SoldierHeight = 0;
	FLOAT Distance = 0;
	BOOL InVehicle = false;
	DWORD64 pWeapon;
	BOOL IsValid() {
		if ((this->Location.x != NULL) &&
			(this->Team != 0) &&
			(this->Health > NULL) &&
			(this->ptr != NULL) &&
			(this->pClientPlayer.ControlledControllable != 0) &&
			(this->pClientPlayer.AttachedControllable == 0) &&
			(!this->InVehicle)) return true;
		return false;
	}
};

class Manager {
private:
	Offsets::StaticGameRenderer pGR = mem.read<Offsets::StaticGameRenderer>(OFFSET_GAMERENDERER);
	Offsets::GameRenderer pGameRenderer = mem.read<Offsets::GameRenderer>(pGR.GameRenderer);
	Offsets::RenderView pRenderView = mem.read<Offsets::RenderView>(pGameRenderer.RenderView);
public:
	RECT rect;
	Player LocalPlayer = NULL;
	std::vector<Player> Players = std::vector<Player>();
	Player ClosestVisibleEnemy = NULL;


	Manager(RECT _rect) {
		this->rect = _rect;
		Offsets::ClientGameContext pStaticGameContext = mem.read<Offsets::ClientGameContext>(OFFSET_GAMECONTEXT);
		Offsets::GameContext pGameContext = mem.read<Offsets::GameContext>(pStaticGameContext.GameContext);
		Offsets::PlayerManager pPlayerManager = mem.read<Offsets::PlayerManager>(pGameContext.PlayerManager);
		this->LocalPlayer = Player(pPlayerManager.LocalPlayer);
		for (int i = 1; i < 60; i++) {
			Player player(mem.read<Offsets::mppPlayer>(pPlayerManager.mppPlayer).PlayerArray[i]);
			if (player.ptr == 0);// break;
			if (player.IsValid() && player.ptr != this->LocalPlayer.ptr) {
				player.Distance = D3DXVec3Length(&(this->LocalPlayer.Location - player.Location));
				this->Players.push_back(player);
			}
		}
		float closest = 10000;
		for (int i = 0; i < this->Players.size(); i++) {
			if (this->Players[i].Visible) {
				if ((this->Players[i].Distance < closest)) {
					if (this->Players[i].Team != this->LocalPlayer.Team) {
						ClosestVisibleEnemy = this->Players[i];
						closest = this->Players[i].Distance;
					}
				}
			}
		}
	}
	

	D3DXVECTOR3 CameraPos() {
		return this->pRenderView.CameraPos;
	}
	D3DXVECTOR2 CalcAim(D3DXVECTOR3 target)
	{
		D3DXVECTOR3 player = this->CameraPos();
		float a = target.x - player.x;
		float b = target.z - player.z;
		float yaw = (float)atan(a / b);
		if (b < 0) {
			yaw += (float)M_PI;
		}
		if (yaw < 0) {
			yaw = (float)(M_PI * 2) + yaw;
		}
		yaw = ((float)M_PI * 2) - yaw;
		float z = target.y - player.y;
		float c = (float)sqrt(a * a + b * b);
		float pitch = (float)atan(z / c);
		return D3DXVECTOR2(pitch, yaw);
	}
	bool WorldToScreen(D3DXVECTOR3 &vLocVec4)
	{
		D3DXMATRIXA16 ViewProj = pRenderView.viewProj;
		float mX = (rect.right - rect.left) / 2;
		float mY = (rect.bottom - rect.top) / 2;

		float w =
			ViewProj(0, 3) * vLocVec4.x +
			ViewProj(1, 3) * vLocVec4.y +
			ViewProj(2, 3) * vLocVec4.z +
			ViewProj(3, 3);

		if (w < 0.65f)
		{
			vLocVec4.z = w;
			return false;
		}

		float x =
			ViewProj(0, 0) * vLocVec4.x +
			ViewProj(1, 0) * vLocVec4.y +
			ViewProj(2, 0) * vLocVec4.z +
			ViewProj(3, 0);

		float y =
			ViewProj(0, 1) * vLocVec4.x +
			ViewProj(1, 1) * vLocVec4.y +
			ViewProj(2, 1) * vLocVec4.z +
			ViewProj(3, 1);

		vLocVec4.x = (mX + mX * x / w);
		vLocVec4.y = (mY - mY * y / w);
		vLocVec4.z = w;
		return true;
	}

};


class Inputs {
	bool mouse_move(int x, int y)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;	//50;
		input.mi.dx = x*(65536 / 1920);
		input.mi.dy = y*(65536 / 1080);
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
		SendInput(1, &input, sizeof(input));
		return true;
	}
};

float Distance3D(D3DXVECTOR3  v1, D3DXVECTOR3 v2)
{
	float x_d = (v2.x - v1.x);
	float y_d = (v2.y - v1.y);
	float z_d = (v2.z - v1.z);
	return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}

