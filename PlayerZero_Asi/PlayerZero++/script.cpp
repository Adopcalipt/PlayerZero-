/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
		--Modified By Adoppocalipt 2023--
*/

#include "script.h"
#include "keyboard.h"
#include "PZSys.h"

using namespace std;
using namespace PZSys;
using namespace PZclass;
using namespace PZData;

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

int AddRelationship(string name)
{
	Hash hash = -1;

	PED::ADD_RELATIONSHIP_GROUP((LPSTR)name.c_str(), &hash);

	return hash;
}
Hash GetRelationship()
{
	return PED::GET_PED_RELATIONSHIP_GROUP_HASH(PLAYER::PLAYER_PED_ID());
}
void RelGroupMember(Ped Peddy, Hash group)
{
	LoggerLight("-GroupMember-");

	PED::SET_PED_RELATIONSHIP_GROUP_HASH(Peddy, group);
}

void EnableSnow(bool bEnable)
{
	if (getGameVersion() > eGameVersion::VER_1_0_2944_0_NOSTEAM)
	{
		invoke<Void>(0x6E9EF3A33C8899F8, bEnable);
		invoke<Void>(0xAEEDAD1420C65CC0, bEnable);
		invoke<Void>(0xA342A3763B3AFB6C, bEnable);
		invoke<Void>(0x4CC7F0FEA5283FE0, bEnable);

		if (bEnable)
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET("core_snow");
			if (STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("core_snow"))
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("core_snow");
		}
		else
			STREAMING::_REMOVE_NAMED_PTFX_ASSET("core_snow");
	}
}

void AddGraphics(string graphics)
{
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_xm_orbital");
}

void MoveEntity(Entity MyEnt, Vector3 position)
{
	ENTITY::SET_ENTITY_COORDS(MyEnt, position.x, position.y, position.z, 1, 0, 0, 1);
}
bool NotTheSame(Vector4 V1, Vector4 V2)
{
	bool bTrue = false;

	if (V1.X != V2.X)
		bTrue = true;

	if (V1.Y != V2.Y)
		bTrue = true;

	if (V1.Z != V2.Z)
		bTrue = true;

	if (V1.R != V2.R)
		bTrue = true;

	return bTrue;
}

string sVersion = "1.8";
string sSaveCont = GetDir() + "/Scripts/PlayerZero/SavedContacts.xml";
string sDisenable = GetDir() + "/PlayerZero/DisablePZ.txt";
string sXmasTree = "prop_xmas_ext";
string sMoneyPicker = "";
string IPhony = "CELLPHONE_IFRUIT";

int iNpcList = 0;
int ChatSHat = 0;
bool bHeistPop = true;
string sTarg = "";
int GetBack = 5;
int iAirCount = 0;
Ped ItsMe = NULL;
bool ItsSnowing = false;

bool ItsChristmas = false;
bool ItHalloween = false;

bool DropMoneys = false;
vector<MoneyBags> MoneyDrops {};

ClothX GetCloths(bool male)
{
	LoggerLight("-GetCloths-");

	ClothX cothing = FemaleDefault;

	string OutputFolder = GetDir() + "/Outfits";

	if (male)
	{
		cothing = MaleDefault;
		OutputFolder = GetDir() + "/Outfits/Male";
	}
	else
		OutputFolder = GetDir() + "/Outfits/Female";


	vector<string> Files = {};
	for (const auto& entry : std::filesystem::directory_iterator(OutputFolder))
		Files.push_back(entry.path().string());

	int iRando = 0;
	if (male)
		iRando = LessRandomInt("Outfits_01", 0, (int)Files.size() - 1);
	else
		iRando = LessRandomInt("Outfits_02", 0, (int)Files.size() - 1);

	string Cloths = ""; vector<int> ClothA = {};	vector<int> ClothB = {}; vector<int> ExtraA = {}; vector<int> ExtraB = {};
	int intList = 0;

	vector<string> MyColect = ReadFile(Files[iRando]);

	for (int i = 0; i < MyColect.size(); i++)
	{
		string line = MyColect[i];
		if (StringContains("Title", line))
		{
			Cloths = StingNumbersInt(line);
		}
		else if (StringContains("[ClothA]", line))
		{
			intList = 1;
		}
		else if (StringContains("[ClothB]", line))
		{
			intList = 2;
		}
		else if (StringContains("[ExtraA]", line))
		{
			intList = 3;
		}
		else if (StringContains("[ExtraB]", line))
		{
			intList = 4;
		}
		else if (StringContains("[FreeOverLay]", line))
		{
			intList = 5;
		}
		else if (StringContains("[Tattoo]", line))
		{
			intList = 6;
		}
		else if (intList == 1)
		{
			ClothA.push_back(StingNumbersInt(line));
		}
		else if (intList == 2)
		{
			int iSpot = StingNumbersInt(line);
			if (iSpot < 0)
				iSpot = 0;
			ClothB.push_back(iSpot);
		}
		else if (intList == 3)
		{
			ExtraA.push_back(StingNumbersInt(line));
		}
		else if (intList == 4)
		{
			int iSpot = StingNumbersInt(line);
			if (iSpot < 0)
				iSpot = 0;
			ExtraB.push_back(iSpot);
		}
	}

	cothing = ClothX(Cloths, ClothA, ClothB, ExtraA, ExtraB);

	return cothing;
}

void SetRelationType(bool friendly)
{
	if (friendly)
	{
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, GP_Player, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, Gp_Follow, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, Gp_Follow, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, Gp_Friend, Gp_Follow);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Follow, GP_Attack);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Follow, GP_Mental);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, GP_Player, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, Gp_Friend, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Friend, GP_Attack);

		if (MySettings.Aggression > 4)
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, GP_Player);
		}
		else
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Attack, GP_Player);
		}

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Friend, GP_Mental);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, GP_Mental);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Attack);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Player, GP_Mental);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Mental);
	}
	else
	{
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, GP_Player, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, Gp_Follow, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, Gp_Follow, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, Gp_Friend, Gp_Follow);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Follow, GP_Attack);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, Gp_Follow);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Follow, GP_Mental);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, GP_Player, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, Gp_Friend, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Friend, GP_Attack);

		if (MySettings.Aggression > 4)
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, GP_Player);
		}
		else
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Attack, GP_Player);
		}

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, Gp_Friend);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Gp_Friend, GP_Mental);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, GP_Mental);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Attack);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Player, GP_Mental);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Player);

		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Mental, GP_Mental);
	}
}
void LoadinData()
{
	LoggerLight("-LoadinData-");

	//Pz_Memo::LoadMemFunc();

	FindSettings(&MySettings);

	PZSetMinWait = MySettings.MinWait * 1000;
	PZSetMaxWait = MySettings.MaxWait * 1000;
	PZSetMinSession = MySettings.MinSession * 1000;
	PZSetMaxSession = MySettings.MaxSession * 1000;

	iNextPlayer = InGameTime() + RandomInt(PZSetMinWait, PZSetMaxWait);

	GP_Player = GetRelationship();
	FollowMe = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());
	Gp_Friend = AddRelationship("FrendlyNPCs");
	GP_Attack = AddRelationship("AttackNPCs");
	Gp_Follow = AddRelationship("FollowerNPCs");
	GP_Mental = AddRelationship("MentalNPCs");
	LoadLang();
	string Today = TimeDate();

	if (StringContains("Dec", Today))
	{
		if (StringContains(" 24 ", Today) || StringContains(" 25 ", Today) || StringContains(" 26 ", Today) || StringContains(" 27 ", Today) || StringContains(" 28 ", Today) || StringContains(" 29 ", Today) || StringContains(" 30 ", Today) || StringContains(" 31 ", Today))
		{
			ItsSnowing = true;
			ItsChristmas = true;
			EnableSnow(ItsSnowing);
			WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), MyHashKey("WEAPON_snowball"), 9999, false, true);
		}
	}
	else if (StringContains("Jan", Today))
	{
		if (StringContains(" 1 ", Today))
		{
			ItsSnowing = true;
			ItsChristmas = true;
			EnableSnow(ItsSnowing);
			WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), MyHashKey("WEAPON_snowball"), 9999, false, true);
		}
	}
	else if (StringContains("Oct", Today))
	{
		if (StringContains(" 31 ", Today))
			ItHalloween = true;
	}

	SetRelationType(MySettings.FriendlyFire);

	PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), FollowMe);
	PED::SET_GROUP_FORMATION(FollowMe, 3);
	
	if (!FileExists(HasOutfits))
		OutfitFolderTest();

	PhoneContacts = LoadContacts();
}

bool GotThatName(const vector<string>* listing, string name)
{
	LoggerLight("GotThatName");
	bool b = false;
	for (int i = 0; i < (int)listing->size(); i++)
	{
		if (listing->at(i) == name)
		{
			b = true;
			break;
		}
	}

	return b;
}
int GotYourNumber(const vector<int>* listing, int num)
{
	LoggerLight("GotThatName");
	bool b = false;
	for (int i = 0; i < (int)listing->size(); i++)
	{
		if (listing->at(i) == num)
		{
			b = true;
			break;
		}
	}

	return b;
}
string FindNewName()
{
	LoggerLight("FindNewName");

	int iS = LessRandomInt("NameFind01", 0, (int)sListOpeniLet.size() - 1);
	string sName = sListOpeniLet[iS];

	iS = LessRandomInt("NameFind02", 0, (int)sListVowls.size() - 1);
	sName += sListVowls[iS];
	if (LessRandomInt("NameFind03", 0, 10) < 5)
	{
		iS = LessRandomInt("NameFind04", 0, (int)sListVowls.size() - 1);
		sName += sListVowls[iS];
	}
	iS = LessRandomInt("NameFind05", 0, 20);

	if (iS < 3)
	{
		if (MySettings.Aggression > 7)
			sName += sListPostfix[RandomInt(0, (int)sListPostfix.size() - 1)] + sListPadding[RandomInt(0, (int)sListPadding.size() - 1)];
	}
	else if (iS < 6)
	{
		string s = "";
		for (int i = 0; i < 3; i++)
			s += sListPostfix[RandomInt(0, (int)sListPostfix.size() - 1)];

		string sR = "";
		int iRev = (int)s.length() - 1;
		while (iRev > -1)
		{
			sR += s[iRev];
			iRev--;
		}
		sName = s + sName + sR;
	}
	else if (iS < 10)
	{
		sName += sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)] + sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)] + sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)];
	}
	else if (iS < 14)
	{
		sName += sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)] + sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)];
	}
	else if (iS < 16)
	{
		sName += sListPostfix[RandomInt(0, (int)sListPostfix.size() - 1)] + sListNumbers[RandomInt(0, (int)sListNumbers.size() - 1)];
	}
	return sName;
}
string SillyNameList()
{
	LoggerLight("SillyNameList");

	string name = FindNewName();

	while (GotThatName(&PlayerNames, name))
	{
		name = FindNewName();
		WAIT(1);
	}
	PlayerNames.push_back(name);

	return name;
}
int UniqueLevels()
{
	LoggerLight("UniqueLevels");

	int iNumber1 = LessRandomInt("Levels", 0, 10);

	int iNumber2 = LessRandomInt("Levels", 0, 10);

	int iNumber3 = LessRandomInt("Levels", 0, 10);

	return iNumber1 + (iNumber2 * 10) + (iNumber3 * 100);
}
void EraseBlip(Blip MyBlip)
{
	if (MyBlip != NULL)
	{
		if ((bool)UI::DOES_BLIP_EXIST(MyBlip))
			UI::REMOVE_BLIP(&MyBlip);
	}
}
FaceBank AddFace(bool male)
{
	int ShapeFirstID;
	int ShapeSecondID;
	if (male)
	{
		ShapeFirstID = LessRandomInt("AddFaceM01", 0, 20);
		ShapeSecondID = LessRandomInt("AddFaceM01", 0, 20);
	}
	else
	{
		ShapeFirstID = LessRandomInt("AddFaceF01", 21, 41);
		ShapeSecondID = LessRandomInt("AddFaceF01", 21, 41);
	}

	float ShapeMix = PZSys::RandomFloat(-0.9, 0.9);
	float SkinMix = PZSys::RandomFloat(0.9, 0.99);
	float ThirdMix = PZSys::RandomFloat(-0.9, 0.9);

	return FaceBank(ShapeFirstID, ShapeSecondID, ShapeMix, SkinMix, ThirdMix);
}
vector<FreeOverLay> AddOverLay(bool male)
{
	LoggerLight("AddOverLay");
	vector<FreeOverLay> YourOver = {};

	for (int i = 0; i < 12; i++)
	{
		int iColour = 0;
		int iChange = RandomInt(0, PED::_GET_NUM_HEAD_OVERLAY_VALUES(i));
		float fVar = RandomFloat(0.45, 0.99);

		if (i == 0)
		{
			iChange = RandomInt(0, iChange);
		}//Blemishes
		else if (i == 1)
		{
			if (male)
				iChange = RandomInt(0, iChange);
			else
				iChange = 255;
			iColour = 1;
		}//Facial Hair
		else if (i == 2)
		{
			iChange = RandomInt(0, iChange);
			iColour = 1;
		}//Eyebrows
		else if (i == 3)
		{
			iChange = 255;
		}//Ageing
		else if (i == 4)
		{
			int iFace = RandomInt(0, 50);
			if (iFace < 30)
			{
				iChange = RandomInt(0, 15);
			}
			else if (iFace < 45)
			{
				iChange = RandomInt(0, iChange);
				fVar = RandomFloat(0.85f, 0.99f);
			}
			else
				iChange = 255;
		}//Makeup
		else if (i == 5)
		{
			if (!male)
			{
				iChange = RandomInt(0, iChange);
				fVar = RandomFloat(0.15f, 0.39f);
			}
			else
				iChange = 255;
			iColour = 2;
		}//Blush
		else if (i == 6)
		{
			iChange = RandomInt(0, iChange);
		}//Complexion
		else if (i == 7)
		{
			iChange = 255;
		}//Sun Damage
		else if (i == 8)
		{
			if (!male)
				iChange = RandomInt(0, iChange);
			else
				iChange = 255;
			iColour = 2;
		}//Lipstick
		else if (i == 9)
		{
			iChange = RandomInt(0, iChange);
		}//Moles/Freckles
		else if (i == 10)
		{
			if (male)
				iChange = RandomInt(0, iChange);
			else
				iChange = 255;
			iColour = 1;
		}//Chest Hair
		else if (i == 11)
		{
			iChange = RandomInt(0, iChange);
		}//Body Blemishes

		int AddColour = 0;

		if (iColour > 0)
			AddColour = RandomInt(0, 64);

		YourOver.push_back(FreeOverLay(iChange, AddColour, fVar));
	}

	return YourOver;
}
vector<Tattoo> AddRandTats(bool male)
{
	LoggerLight("AddRandTats");
	vector<Tattoo> Tatlist = {};

	if (male)
	{
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats01[RandomInt(0, (int)maleTats01.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats02[RandomInt(0, (int)maleTats02.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats03[RandomInt(0, (int)maleTats03.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats04[RandomInt(0, (int)maleTats04.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats05[RandomInt(0, (int)maleTats05.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats06[RandomInt(0, (int)maleTats06.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats07[RandomInt(0, (int)maleTats07.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = maleTats08[RandomInt(0, (int)maleTats08.size() - 1)];
			Tatlist.push_back(T);
		}
	}
	else
	{
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats01[RandomInt(0, (int)femaleTats01.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats02[RandomInt(0, (int)femaleTats02.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats03[RandomInt(0, (int)femaleTats03.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats04[RandomInt(0, (int)femaleTats04.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats05[RandomInt(0, (int)femaleTats05.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats06[RandomInt(0, (int)femaleTats06.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats07[RandomInt(0, (int)femaleTats07.size() - 1)];
			Tatlist.push_back(T);
		}
		if (RandomInt(0, 10) < 5)
		{
			Tattoo T = femaleTats08[RandomInt(0, (int)femaleTats08.size() - 1)];
			Tatlist.push_back(T);
		}
	}

	return Tatlist;
}
ClothBank NewClothBank()
{
	if (LessRandomInt("GotMale", 0, 10) < 5)
	{
		bool Male = true;
		string Model = "mp_m_freemode_01";
		ClothX Cothing = GetCloths(true);
		HairSets MyHair = MHairsets[LessRandomInt("MCBank02", 0, (int)MHairsets.size() - 1)];
		return ClothBank(SillyNameList(), Model, Cothing, AddFace(Male), Male, MyHair, RandomInt(1, 61), RandomInt(1, 61), RandomInt(0, 10), AddOverLay(Male), AddRandTats(Male));
	}
	else
	{

		bool Male = false;
		string Model = "mp_f_freemode_01";
		ClothX Cothing = GetCloths(false);
		HairSets MyHair = FHairsets[LessRandomInt("FCBank02", 0, (int)FHairsets.size() - 1)];
		return ClothBank(SillyNameList(), Model, Cothing, AddFace(Male), Male, MyHair, RandomInt(1, 61), RandomInt(1, 61), RandomInt(0, 10), AddOverLay(Male), AddRandTats(Male));
	}
}
int OhMyBlip(Vehicle MyVehic)
{
	LoggerLight("OhMyBlip");

	int iBeLip = 0;
	if (MyVehic != NULL)
	{
		int iVehClass = VEHICLE::GET_VEHICLE_CLASS(MyVehic);

		if (iVehClass == 14)
			iBeLip = 427;
		else if (iVehClass == 15)
			iBeLip = 64;
		else if (iVehClass == 8)
			iBeLip = 226;
		else if (iVehClass == 16)
			iBeLip = 424;
		else if (iVehClass == 12)
			iBeLip = 616;
		else if (iVehClass == 20)//mule
			iBeLip = 477;
		else if (iVehClass == 10)//trucks
			iBeLip = 477;
		else if (iVehClass == 17)//buss
			iBeLip = 513;
		else if (iVehClass == 7)
			iBeLip = 595;
		else if (iVehClass == 6)
			iBeLip = 523;
		else if (iVehClass == 13)
			iBeLip = 376;
		else
			iBeLip = 225;


		for (int i = 0; i < vehBlips.size(); i++)
		{
			if ((bool)VEHICLE::IS_VEHICLE_MODEL(MyVehic, MyHashKey(vehBlips[i].VehicleKey)))
				iBeLip = vehBlips[i].BlipNo;
		}
	}

	return iBeLip;
}

void RemoveFromPedList(string played)
{
	int iPlace = ReteaveAfk(played);
	if (iPlace < (int)PedList.size() && iPlace > -1)
		PedList.erase(PedList.begin() + iPlace);
}
int FindUSeat(Vehicle vMe)
{
	bool bPass = true;
	for (int i = 0; i < (int)GunnerSeat.size(); i++)
	{
		if (MyHashKey(GunnerSeat[i]) == ENTITY::GET_ENTITY_MODEL(vMe))
		{
			bPass = false;
			break;
		}
	}

	int iSeats;
	if (bPass)
	{
		iSeats = 0;
		while (iSeats < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vMe))
		{
			if ((bool)VEHICLE::IS_VEHICLE_SEAT_FREE(vMe, iSeats))
				break;
			else
				iSeats++;
		}
	}
	else
	{
		iSeats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vMe);
		while (iSeats > -1)
		{
			if ((bool)VEHICLE::IS_VEHICLE_SEAT_FREE(vMe, iSeats))
				break;
			else
				iSeats--;
		}
	}

	return iSeats;
}
void WarptoAnyVeh(Vehicle Vhic, Ped Peddy, int iSeat)
{
	LoggerLight("WarptoAnyVeh");

	PED::SET_PED_INTO_VEHICLE(Peddy, Vhic, iSeat);
}
void GetOutVehicle(Ped Peddy)
{
	LoggerLight("-GetOutVehicle-");

	if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Peddy, 0))
		AI::TASK_LEAVE_VEHICLE(Peddy, PED::GET_VEHICLE_PED_IS_IN(Peddy, true), 4160);
}
void EmptyVeh(Vehicle Vhic)
{
	LoggerLight("PedActions.EmptyVeh");

	if ((bool)ENTITY::DOES_ENTITY_EXIST(Vhic))
	{
		int iSeats = 0;
		while (iSeats < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(Vhic))
		{
			if (!(bool)VEHICLE::IS_VEHICLE_SEAT_FREE(Vhic, iSeats))
				GetOutVehicle(VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vhic, iSeats));
			iSeats += 1;
		}
	}
}
bool InSameVeh(Ped Peddy)
{
	bool bIn = false;
	if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Peddy, 0))
	{
		Vehicle Vic = PED::GET_VEHICLE_PED_IS_IN(Peddy, true);
		if ((bool)PED::IS_PED_IN_VEHICLE(PLAYER::PLAYER_PED_ID(), Vic, false))
			bIn = true;
	}
	return bIn;
}

void ResetPlayer(PlayerBrain* brain, bool Delete)
{
	if (brain->IsPlane || brain->IsHeli)
		iAirCount--;

	if (brain->ThisVeh != NULL)
	{
		EmptyVeh(brain->ThisVeh);
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&brain->ThisVeh);
		brain->ThisVeh = NULL;
	}

	if (brain->Oppressor != NULL)
	{
		EmptyVeh(brain->Oppressor);
		ENTITY::DELETE_ENTITY(&brain->Oppressor);
		brain->Oppressor = NULL;
	}

	if (brain->XmasTree != NULL)
	{
		ENTITY::DELETE_ENTITY(&brain->XmasTree);
		brain->XmasTree = NULL;
	}

	if (brain->PiggyBackin)
	{
		brain->PiggyBackin = false;
		ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), 0, 0);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	if (brain->AirTranspport)
	{
		bPlanePort = false;
		brain->AirTranspport = false;
		brain->TimeOn = 0;
	}

	if (brain->BusDriver)
	{
		bBusDrivin = false;
		brain->BusDriver = false;
		brain->TimeOn = 0;
	}
	
	if (brain->RentaCop)
	{
		bRentoCop = false;
		brain->RentaCop = false;
		brain->TimeOn = 0;
	}

	EraseBlip(brain->ThisBlip);

	if (Delete)
	{
		if (brain->ThisPed != NULL)
		{
			if ((bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisPed))
			{
				ENTITY::DETACH_ENTITY(brain->ThisPed, 0, 0);
				PED::REMOVE_PED_FROM_GROUP(brain->ThisPed);

				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&brain->ThisPed);
			}
		}
	}
}

void PedCleaning(PZclass::PlayerBrain* played, string sOff, bool bDelete)
{
	LoggerLight("ClearUp.PedCleaning, MyName == " + played->MyName);
	BottomLeft("~h~" + played->MyName + "~s~ " + sOff);
	ResetPlayer(played, true);
	PedList[iNpcList].TimeToGo = true;
}

void BlipFiler(Blip MyBlip, int iBlippy, string sName, int iColour)
{
	UI::SET_BLIP_SPRITE(MyBlip, iBlippy);
	UI::SET_BLIP_AS_SHORT_RANGE(MyBlip, true);
	UI::SET_BLIP_CATEGORY(MyBlip, 2);
	UI::SET_BLIP_COLOUR(MyBlip, iColour);

	string name = " Player: " + sName;

	if (sName != "")
	{
		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)name.c_str());
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(MyBlip);

		//UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		//UI::_ADD_TEXT_COMPONENT_STRING(" Player: " + sName);
		//UI::END_TEXT_COMMAND_SET_BLIP_NAME( MyBlip);
	}
}
Blip PedBlimp(Blip CurBlip, Ped pEdd, int iBlippy, string sName, int iColour, bool heading)
{
	LoggerLight("PedBlimp, iBlippy == " + std::to_string(iBlippy) + ", sName == " + sName + ", iColour" + std::to_string(iColour));

	EraseBlip(CurBlip);

	Blip MyBlip = UI::ADD_BLIP_FOR_ENTITY(pEdd);;

	BlipFiler(MyBlip, iBlippy, sName, iColour);
	if (heading)
		UI::_SET_BLIP_SHOW_HEADING_INDICATOR(MyBlip, true);

	return MyBlip;
}
Blip LocalBlip(Blip CurBlip, Vector4 Vlocal, int iBlippy, string sName, int iColour)
{
	LoggerLight("BuildObjects, iBlippy == " + std::to_string(iBlippy) + ", sName == " + sName);

	EraseBlip(CurBlip);
	Blip MyBlip = UI::ADD_BLIP_FOR_COORD(Vlocal.X, Vlocal.Y, Vlocal.Z);
	BlipFiler(MyBlip, iBlippy, sName, iColour);
	return MyBlip;
}
void BlipingBlip(PZclass::PlayerBrain* brain)
{
	if (PZData::MySettings.NoRadar || brain->OffRadarBool)
	{
		if (brain->ThisBlip != NULL)
		{
			EraseBlip(brain->ThisBlip);
			brain->ThisBlip = NULL;
		}
	}
	else
	{
		if ((bool)PED::IS_PED_IN_ANY_VEHICLE(brain->ThisPed, 0))
		{
			if (brain->DirBlip)
			{
				brain->DirBlip = false;
				if (brain->Driver && brain->ThisVeh != NULL)
					brain->ThisBlip = PedBlimp(brain->ThisBlip, brain->ThisPed, OhMyBlip(brain->ThisVeh), brain->MyName, brain->BlipColour, brain->DirBlip);
				else
				{
					EraseBlip(brain->ThisBlip);
					brain->ThisBlip = NULL;
				}
			}
		}
		else
		{
			if (!brain->DirBlip)
			{
				brain->DirBlip = true;
				brain->ThisBlip = PedBlimp(brain->ThisBlip, brain->ThisPed, 1, brain->MyName, brain->BlipColour, brain->DirBlip);
				UI::_SET_BLIP_SHOW_HEADING_INDICATOR(brain->ThisBlip, 1);
			}
			else if (brain->ThisBlip == NULL)
			{
				brain->ThisBlip = PedBlimp(brain->ThisBlip, brain->ThisPed, 1, brain->MyName, brain->BlipColour, brain->DirBlip);
				UI::_SET_BLIP_SHOW_HEADING_INDICATOR(brain->ThisBlip, 1);
			}
		}
	}
}

void AddMonies(int iAmount)
{
	Hash H1 = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());
	int iPedCred = -1;
	if (H1 == -1686040670)
		iPedCred = MyHashKey("SP2_TOTAL_CASH");
	else if (H1 == -1692214353)
		iPedCred = MyHashKey("SP1_TOTAL_CASH");
	else if (H1 == 225514697)
		iPedCred = MyHashKey("SP0_TOTAL_CASH");
	int Credit = 0;
	STATS::STAT_GET_INT(iPedCred, &Credit, -1);
	Credit += iAmount;
	STATS::STAT_SET_INT(iPedCred, Credit, 1);
}
void PlayerDump(bool KeepFriend)
{
	LoggerLight("-PlayerDump-");

	for (int i = 0; i < (int)MakeFrenz.size(); i++)
	{
		if (KeepFriend)
		{
			if (!MakeFrenz[i].Brains.Follower)
				MakeFrenz[i].Brains.TimeOn = 0;
		}
		else
			MakeFrenz[i].Brains.TimeOn = 0;
	}

	for (int i = 0; i < (int)MakeCarz.size(); i++)
	{
		if (KeepFriend)
		{
			if (!MakeCarz[i].Brains.Follower)
				MakeCarz[i].Brains.TimeOn = 0;
		}
		else
			MakeCarz[i].Brains.TimeOn = 0;
	}

	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (KeepFriend)
		{
			if (!PedList[i].Follower)
				PedList[i].TimeOn = 0;
		}
		else
			PedList[i].TimeOn = 0;
	}

	for (int i = 0; i < (int)AFKList.size(); i++)
		AFKList[i].TimeOn = 0;
}
void LaggOut(bool KeepFriend)
{
	LoggerLight("-LaggOut-");
	bPlanePort = false;
	bBusDrivin = false;
	bRentoCop = false;

	PlayerDump(KeepFriend);

	ClearYourFriend(false);

	iAirCount = 0;
}
void RandomChat(string sPed, int itype, int iNat)
{
	string sRan = sPed + " [All] ";

	if (iNat == 1)
	{
		if (itype == 1)
			sRan += ShitGreet01[LessRandomInt("ShitGreet01", 0, (int)ShitGreet01.size() - 1)];
		else if (itype == 2)
			sRan += ShitGreet02[LessRandomInt("ShitGreet02", 0, (int)ShitGreet02.size() - 1)];
		else if (itype == 3)
			sRan += ShitGreet03[LessRandomInt("ShitGreet03", 0, (int)ShitGreet03.size() - 1)];
		else if (itype == 4)
			sRan += ShitGreet04[LessRandomInt("ShitGreet04", 0, (int)ShitGreet04.size() - 1)];
		else if (itype == 5)
			sRan += ShitGreet05[LessRandomInt("ShitGreet05", 0, (int)ShitGreet05.size() - 1)];
	}
	else if (iNat == 2)
	{
		if (itype == 1)
			sRan += ShitTalk01[LessRandomInt("ShitTalk01", 0, (int)ShitTalk01.size() - 1)];
		else if (itype == 2)
			sRan += ShitTalk02[LessRandomInt("ShitTalk02", 0, (int)ShitTalk02.size() - 1)];
		else if (itype == 3)
			sRan += ShitTalk03[LessRandomInt("ShitTalk03", 0, (int)ShitTalk03.size() - 1)];
		else if (itype == 4)
			sRan += ShitTalk04[LessRandomInt("ShitTalk04", 0, (int)ShitTalk04.size() - 1)];
		else if (itype == 5)
			sRan += ShitTalk05[LessRandomInt("ShitTalk05", 0, (int)ShitTalk05.size() - 1)];
	}
	else if (iNat == 3)
	{
		sRan += NoobTalk01[LessRandomInt("NoobTalk01", 0, (int)NoobTalk01.size() - 1)];
	}
	else if (iNat == 4)
	{
		sRan += AppTalk01[LessRandomInt("AppTalk01", 0, (int)AppTalk01.size() - 1)];
	}
	else if (iNat == 5)
	{
		sRan += CrazyTalk01[LessRandomInt("CrazyTalk01", 0, (int)CrazyTalk01.size() - 1)];
	}

	int iTime = InGameTime() + 5000;

	ShitTalk.push_back(ShitTalking(sRan, iTime));

	if ((int)sRan.length() > 43)
		ShitTalk.push_back(ShitTalking("", iTime));
}
float GroundHight(Vector3 postion)
{
	float GHight = postion.z;
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(postion.x, postion.y, postion.z, &GHight, 1);
	return GHight;
}
void StayOnGround(Vehicle Vhick)
{
	LoggerLight("StayOnGround");
	while (!VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vhick))
	{
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vhick);
		WAIT(10);
	}
}
int PlayerZinSesh(bool IncludeLoad)
{
	if (IncludeLoad)
		return (int)PedList.size() + (int)AFKList.size() + (int)MakeCarz.size() + (int)MakeFrenz.size();
	else
		return (int)PedList.size() + (int)AFKList.size();
}
vector<Vector3> BuildFlightPath(Vector3 vStart)
{

	vector<Vector3> landSand = vector<Vector3>{
		 NewVector3(225.8934, 2841.527, 200.0402),
		 NewVector3(796.0992, 3011.926, 90.13193),
		 NewVector3(1495.307, 3187.998, 41.04951),
		 NewVector3(1655.049, 3249.205, 41.21964),
		 NewVector3(1561.392, 3160.818, 41.1649),
		 NewVector3(1334.507, 2924.953, 98.35621),
		 NewVector3(798.5253, 2388.362, 282.331),
		 NewVector3(413.3483, 2034.074, 425.4946),
		 NewVector3(-175.5016, 1448.899, 598.845),
		 NewVector3(-349.6658, -187.2563, 398.4032)
	};
	vector<Vector3> landLS = vector<Vector3>{
		 NewVector3(-1002.727, -1650.774, 134.2087),
		 NewVector3(-1193.304, -1941.04, 59.51603),
		 NewVector3(-1571.467, -2617.15, 14.57554),
		 NewVector3(-1612.011, -2789.524, 14.62421),
		 NewVector3(-1532.074, -2835.987, 14.58676),
		 NewVector3(-991.79, -3147.605, 90.8317),
		 NewVector3(-440.5424, -3123.56, 232.0305),
		 NewVector3(-88.80471, -2403.97, 234.262),
		 NewVector3(-18.7144, -1591.593, 351.0859),
		 NewVector3(29.53166, 219.7558, 581.6113),
		 NewVector3(-169.9742, 1746.14, 484.2034)
	};

	float f1 = DistanceTo(vStart, landSand[0]);
	float f2 = DistanceTo(vStart, landLS[0]);

	if (f1 < f2)
	{
		FlyMeToo = landLS[0];
		return landSand;
	}
	else
	{
		FlyMeToo = landSand[0];
		return landLS;
	}
}
vector<int> RandVehModsist()
{
	vector<int> RandMods = {};

	for (int i = 0; i < 67; i++)
		RandMods.push_back(-1);


	LoggerLight("RandVehModsist Count == " + std::to_string((int)RandMods.size()));

	return RandMods;
}
Prop BuildProps(string sObject, Vector3 vPos, Vector3 vRot, bool bPush, bool bAddtolist)
{
	LoggerLight("BuildProps,  sObject == " + sObject);
	Prop Plop = OBJECT::CREATE_OBJECT(MyHashKey(sObject), vPos.x, vPos.y, vPos.z, 1, 1, 1);
	ENTITY::SET_ENTITY_COORDS(Plop, vPos.x, vPos.y, vPos.z, 0, 0, 0, 1);
	ENTITY::SET_ENTITY_ROTATION(Plop, vRot.x, vRot.y, vRot.z, 2, 1);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Plop, 1, 1);
	if (bPush)
		ENTITY::APPLY_FORCE_TO_ENTITY(Plop, 1, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1, 1, 1, 1, 1, 1);

	return Plop;
}
void OrbExp(Ped PFired, Vector3 Pos1, Vector3 Pos2, Vector3 Pos3, Vector3 Pos4, Vector3 Pos5)
{
	LoggerLight("PedActions.OrbExp");

	FIRE::ADD_OWNED_EXPLOSION(PFired, Pos2.x, Pos2.y, Pos2.z, 49, 1, true, false, 1);
	FIRE::ADD_OWNED_EXPLOSION(PFired, Pos3.x, Pos3.y, Pos3.z, 49, 1, true, false, 1);
	FIRE::ADD_OWNED_EXPLOSION(PFired, Pos4.x, Pos4.y, Pos4.z, 49, 1, true, false, 1);
	FIRE::ADD_OWNED_EXPLOSION(PFired, Pos5.x, Pos5.y, Pos5.z, 49, 1, true, false, 1);
	FIRE::ADD_OWNED_EXPLOSION(PFired, Pos1.x, Pos1.y, Pos1.z, 54, 1, true, false, 1);

	AUDIO::PLAY_SOUND_FROM_COORD(-1, "DLC_XM_Explosions_Orbital_Cannon", Pos1.x, Pos1.y, Pos1.z, 0, 0, 1, 0);
	AddGraphics("scr_xm_orbital");
}
void OrbLoad(string sWhoDidit, bool bPlayerStrike)
{
	LoggerLight("PedActions.OrbLoad, sWhoDidit == " + sWhoDidit);

	if (bPlayerStrike)
		BottomLeft(PZLang[100] + sWhoDidit + PZLang[101]);
	else
		BottomLeft(sWhoDidit + PZLang[102]);

	BigMessage(PZLang[103], "", 3);
}
void FireOrb(string sId, Ped Target, bool bPlayerStrike)
{
	LoggerLight("FireOrb, sId == " + sId);

	Ped pFired = PLAYER::PLAYER_PED_ID();

	int MyBrian = ReteaveBrain(sId);

	if (MyBrian != -1 && MyBrian < (int)PedList.size())
	{
		if (!bPlayerStrike)
		{
			PedList[MyBrian].ThisBlip = LocalBlip(PedList[MyBrian].ThisBlip, OrbStrike[RandomInt(0, 8)], 590, PedList[MyBrian].MyName, 1);

			pFired = PedList[MyBrian].ThisPed;
			WAIT(7500);
		}

		Vector3 TargetPos = ENTITY::GET_ENTITY_COORDS(Target, true);
		float GHight = GroundHight(TargetPos);
		if (GHight < TargetPos.z)
		{
			Vector3 TargF = FowardOf(Target, 5, true);
			Vector3 TargB = FowardOf(Target, 5, false);
			Vector3 TargR = RightOf(Target, 5, true);
			Vector3 TargL = RightOf(Target, 5, false);
			OrbExp(pFired, TargetPos, TargF, TargB, TargR, TargL);

			OrbLoad(PedList[MyBrian].MyName, bPlayerStrike);
			WAIT(4000);

			if (MyBrian != -1)
			{
				PedList[MyBrian].TimeOn = 0;
			}
		}
	}
}
bool VehicleEnter = false;

const std::vector<std::string> SeatTest = {
	"CUBAN800", //>
	"DODO", //>
	"DUSTER", //>
	"LUXOR", //>
	"LUXOR2", //><!-- Luxor Deluxe -->
	"MAMMATUS", //>
	"MILJET", //>
	"ROGUE",
	"NIMBUS", //>
	"SHAMAL", //>
	"VELUM", //>
	"VELUM2", //><!-- Velum 5-Seater -->
	"VESTRA", //>
	"MOGUL", //>
	"SEABREEZE"
};
const std::vector<std::string> Kamakasi = {
	"CUBAN800", //>
	"DODO", //>
	"DUSTER", //>
	"LUXOR", //>
	"LUXOR2", //><!-- Luxor Deluxe -->
	"MAMMATUS", //>
	"MILJET", //>
	"NIMBUS", //>
	"BOMBUSHKA", //><!-- RM-10 Bombushka -->
	"ALKONOST", //><!-- RO-86 Alkonost -->
	"SHAMAL", //>
	"TITAN", //>
	"VELUM", //>
	"VELUM2", //><!-- Velum 5-Seater -->
	"VESTRA", //>
	"VOLATOL", //>
	"ALPHAZ1", //>
	"BESRA", //>
	"HOWARD", //><!-- Howard NX-25 -->
	"STUNT", //><!-- Mallard -->
	"LAZER", //><!-- P-996 LAZER -->
	"ROGUE", //>
	"MICROLIGHT", //><!-- Ultralight -->
	"MOLOTOK" //><!-- V-65 Molotok -->
};
bool IsItThePlane(int iCheckList, Vehicle vMe)
{
	bool result = false;
	if (iCheckList == 1)
	{
		if (!bPlanePort)
		{
			for (int i = 0; i < SeatTest.size(); i++)
			{
				if ((bool)VEHICLE::IS_VEHICLE_MODEL(vMe, MyHashKey(SeatTest[i])))
				{
					result = true;
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < Kamakasi.size(); i++)
		{
			if ((bool)VEHICLE::IS_VEHICLE_MODEL(vMe, MyHashKey(Kamakasi[i])))
			{
				result = true;
				break;
			}
		}
	}


	return result;
}

Vector4 FindingShops(Ped peddy)
{
	Vector3 PedPos = ENTITY::GET_ENTITY_COORDS(peddy, true);
	float fDis = 5000.0;
	int This = 0;
	for (int i = 0; i < (int)ShopsNTings.size(); i++)
	{
		float fit = DistanceTo(ShopsNTings[i], PedPos);
		if (fit < fDis)
		{
			fDis = fit;
			This = i;
		}
	}
	return ShopsNTings[This];
}
int YourGunNum()
{
	int iGun = 0;

	if (MySettings.Aggression > 2)
	{
		if (MySettings.SpaceWeaps)
			iGun = RandomInt(2, 10);
		else
			iGun = RandomInt(2, 9);
	}

	return iGun;
}
void GunningIt(Ped Peddy, int iGun)
{
	LoggerLight("GunningIt");
	WEAPON::REMOVE_ALL_PED_WEAPONS(Peddy, 0);

	vector<string> sWeapList = {};

	if (iGun == 1)
	{
		sWeapList.push_back("WEAPON_dagger");  //0x92A27487",
		sWeapList.push_back("WEAPON_hammer");  //0x4E875F73",
		sWeapList.push_back("WEAPON_battleaxe");  //0xCD274149",
		sWeapList.push_back("WEAPON_golfclub");  //0x440E4788",
		sWeapList.push_back("WEAPON_machete");  //0xDD5DF8D9",
	}
	else if (iGun == 2)
	{
		sWeapList.push_back("WEAPON_dagger");  //0x92A27487",
		sWeapList.push_back("WEAPON_pipebomb");  //0xBA45E8B8",
		sWeapList.push_back("WEAPON_navyrevolver");  //0x917F6C8C"
		sWeapList.push_back("WEAPON_combatpdw");  //0xA3D4D34",
		sWeapList.push_back("WEAPON_sawnoffshotgun");  //0x7846A318",
		sWeapList.push_back("WEAPON_sniperrifle");  //0x5FC3C11",
	}
	else if (iGun == 3)
	{
		sWeapList.push_back("WEAPON_hammer");  //0x4E875F73",
		sWeapList.push_back("WEAPON_revolver");  //0xC1B3C3D1",
		sWeapList.push_back("WEAPON_smg");  //0x2BE6766B",
		sWeapList.push_back("WEAPON_pumpshotgun");  //0x1D073A89",
		sWeapList.push_back("WEAPON_advancedrifle");  //0xAF113F99",
	}
	else if (iGun == 4)
	{
		sWeapList.push_back("WEAPON_battleaxe");  //0xCD274149",
		sWeapList.push_back("WEAPON_molotov");  //0x24B17070",
		sWeapList.push_back("WEAPON_stungun");  //0x3656C8C1",
		sWeapList.push_back("WEAPON_microsmg");  //0x13532244",
		sWeapList.push_back("WEAPON_musket");  //0xA89CB99E",
		sWeapList.push_back("WEAPON_gusenberg");  //0x61012683"--69
	}
	else if (iGun == 5)
	{
		sWeapList.push_back("WEAPON_golfclub");  //0x440E4788",
		sWeapList.push_back("WEAPON_grenade");  //0x93E220BD",
		sWeapList.push_back("WEAPON_appistol");  //0x22D8FE39",
		sWeapList.push_back("WEAPON_assaultshotgun");  //0xE284C527",
		sWeapList.push_back("WEAPON_mg");  //0x9D07F764",
	}
	else if (iGun == 6)
	{
		sWeapList.push_back("WEAPON_machete");  //0xDD5DF8D9",
		sWeapList.push_back("WEAPON_heavypistol");  //0xD205520E",
		sWeapList.push_back("WEAPON_microsmg");  //0x13532244",
		sWeapList.push_back("WEAPON_specialcarbine");  //0xC0A3098D",

	}
	else if (iGun == 7)
	{
		sWeapList.push_back("WEAPON_flashlight");  //0x8BB05FD7",
		sWeapList.push_back("WEAPON_GADGETPISTOL");  //0xAF3696A1",--new to cayo ppero
		sWeapList.push_back("WEAPON_MILITARYRIFLE");  //0x624FE830"--65
		sWeapList.push_back("WEAPON_COMBATSHOTGUN");  //0x5A96BA4--54
	}
	else if (iGun == 8)
	{
		sWeapList.push_back("WEAPON_marksmanrifle");  //0xC734385A",
		sWeapList.push_back("WEAPON_marksmanrifle_mk2");  //0x6A6C02E0"--74
	}
	else if (iGun == 9)
	{
		sWeapList.push_back("WEAPON_pistol_mk2");  //0xBFE256D4",---------19
		sWeapList.push_back("WEAPON_snspistol_mk2");  //0x88374054",---24
		sWeapList.push_back("WEAPON_revolver_mk2");  //0xCB96392F",----29
		sWeapList.push_back("WEAPON_pumpshotgun_mk2");  //0x555AF99A",-----------46
		sWeapList.push_back("WEAPON_assaultrifle_mk2");  //0x394F415C",-------56
		sWeapList.push_back("WEAPON_carbinerifle_mk2");  //0xFAD1F1C9",------58
		sWeapList.push_back("WEAPON_specialcarbine_mk2");  //0x969C3D67",------61
		sWeapList.push_back("WEAPON_bullpuprifle_mk2");  //0x84D6FAFD",----63
		sWeapList.push_back("WEAPON_combatmg_mk2");  //0xDBBD7280",------68
		sWeapList.push_back("WEAPON_heavysniper_mk2");  //0xA914799",---72
		sWeapList.push_back("WEAPON_marksmanrifle_mk2");  //0x6A6C02E0"--74
	}
	else if (iGun == 10)
	{
		sWeapList.push_back("WEAPON_raypistol");  //0xAF3696A1",--36
		sWeapList.push_back("WEAPON_raycarbine");  //0x476BF155"--44
		sWeapList.push_back("weapon_rayminigun");
	}
	else if (iGun == 15)
	{
		sWeapList.push_back("WEAPON_nightstick");  //0xAF3696A1",--36
		sWeapList.push_back("WEAPON_pistol");  //0x476BF155"--44
		sWeapList.push_back("WEAPON_pumpshotgun");
	}

	for (int i = 0; i < sWeapList.size(); i++)
		WEAPON::GIVE_WEAPON_TO_PED(Peddy, MyHashKey(sWeapList[i]), 9999, false, true);
}
void GetInVehicle(Ped Peddy, Vehicle Vhick, int Seat, bool clearSeat)
{
	LoggerLight("PlayerEnterVeh");

	iFindingTime = InGameTime() + 1000;

	if (clearSeat)
		GetOutVehicle(VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vhick, Seat));

	if (Seat != -1)
	{
		int ThreePass = 3;
		AI::TASK_OPEN_VEHICLE_DOOR(Peddy, Vhick, -1, Seat, 1.5f);
		while (!PED::IS_PED_GETTING_INTO_A_VEHICLE(Peddy))
		{
			WAIT(1000);
			AI::TASK_ENTER_VEHICLE(Peddy, Vhick, -1, Seat, 1.50f, 1, 0);
			ThreePass--;
			if (ThreePass < 0)
				break;
		}

		if (ThreePass < 1)
			WarptoAnyVeh(Vhick, Peddy, Seat);
	}
}
void PlayerEnterVeh(Vehicle Vhick)
{
	VehicleEnter = true;
	int iSeats = FindUSeat(Vhick);
	GetInVehicle(PLAYER::PLAYER_PED_ID(), Vhick, iSeats, false);
	VehicleEnter = false;
}
void ForceAnim(Ped peddy, string sAnimDict, string sAnimName, Vector3 AnPos, Vector3 AnRot)
{
	LoggerLight("ForceAnim, sAnimName == " + sAnimName);

	AI::CLEAR_PED_TASKS(peddy);
	STREAMING::REQUEST_ANIM_DICT((LPSTR)sAnimDict.c_str());
	while (!STREAMING::HAS_ANIM_DICT_LOADED((LPSTR)sAnimDict.c_str()))
		WAIT(1);
	AI::TASK_PLAY_ANIM_ADVANCED(peddy, (LPSTR)sAnimDict.c_str(), (LPSTR)sAnimName.c_str(), AnPos.x, AnPos.y, AnPos.z, AnRot.x, AnRot.y, AnRot.z, 8.0, 0.00, -1, 1, 0.01, 0, 0);
	STREAMING::REMOVE_ANIM_DICT((LPSTR)sAnimDict.c_str());
}
void ForceAnim(Ped peddy, string sAnimDict, string sAnimName, Vector4 AnPos)
{
	LoggerLight("ForceAnimv4, sAnimName == " + sAnimName);
	ForceAnim(peddy, sAnimDict, sAnimName, NewVector3(AnPos.X, AnPos.Y, AnPos.Z), NewVector3(0.0f, 0.0f, AnPos.R));
}
void FollowPed(Ped Target, Ped Follower)
{
	AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(Follower, Target, 0, 0, 0, 45, -1, 10, 1);
}
int LandingGear(Vehicle Vic)
{
	//	0: Deployed
	//	1 : Closing
	//	2 : Opening
	//	3 : Retracted
	return VEHICLE::_GET_VEHICLE_LANDING_GEAR(Vic);
}
void LandNearHeli(Ped Peddy, Vehicle vHick, Vector3 vTarget)
{
	AI::CLEAR_PED_TASKS(Peddy);
	float HeliDesX = vTarget.x;
	float HeliDesY = vTarget.y;
	float HeliDesZ = vTarget.z;
	float HeliSpeed = 35;
	float HeliLandArea = 10;
	Vector3 VHickPos = ENTITY::GET_ENTITY_COORDS(vHick, true);
	float dx = VHickPos.x - HeliDesX;
	float dy = VHickPos.y - HeliDesY;

	float HeliDirect = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(dx, dy) - 180.00f;


	AI::TASK_HELI_MISSION(Peddy, vHick, 0, 0, HeliDesX, HeliDesY, HeliDesZ, 20, HeliSpeed, HeliLandArea, HeliDirect, -1, -1, -1, 32);

	PED::SET_PED_FIRING_PATTERN(Peddy, MyHashKey("FIRING_PATTERN_BURST_FIRE_HELI"));

	PED::SET_PED_KEEP_TASK(Peddy, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Peddy, true);
}
void LandNearPlane(Ped Peddy, Vehicle vHick, Vector3 vStart, Vector3 vFinish)
{
	AI::CLEAR_PED_TASKS(Peddy);
	AI::TASK_PLANE_LAND(Peddy, vHick, vStart.x, vStart.y, vStart.z, vFinish.x, vFinish.y, vFinish.z);
	PED::SET_PED_KEEP_TASK(Peddy, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Peddy, true);
}
void WalkHere(Ped Peddy, Vector4 Dest)
{
	AI::CLEAR_PED_TASKS(Peddy);
	AI::TASK_FOLLOW_NAV_MESH_TO_COORD(Peddy, Dest.X, Dest.Y, Dest.Z, 1.0, -1, 0.0, false, 0.0);
	PED::SET_PED_KEEP_TASK(Peddy, true);
}
void RunHere(Ped Peddy, Vector3 Dest)
{
	AI::CLEAR_PED_TASKS(Peddy);
	AI::TASK_FOLLOW_NAV_MESH_TO_COORD(Peddy, Dest.x, Dest.y, Dest.z, 2.0, -1, 0.0, false, 0.0);
	PED::SET_PED_KEEP_TASK(Peddy, true);
}
void DriveAround(Ped Peddy)
{
	AI::CLEAR_PED_TASKS(Peddy);
	if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Peddy, 0))
	{
		if (Peddy == VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_USING(Peddy), -1))
		{
			Vehicle Vick = PED::GET_VEHICLE_PED_IS_IN(Peddy, false);

			AI::TASK_VEHICLE_DRIVE_WANDER(Peddy, Vick, 25, 262972);
			PED::SET_PED_KEEP_TASK(Peddy, true);
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Peddy, true);
		}
	}

}
void DriveToooDest(Ped Peddy, Vehicle Vick, Vector3 Vme, float fSpeed, bool Blocking, bool RunOver, bool PlaneTaxi)
{
	AI::CLEAR_PED_TASKS(Peddy);
	if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Peddy, 0))
	{
		if (RunOver)
			AI::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(Peddy, Vick, Vme.x, Vme.y, Vme.z, fSpeed, 262972, 0.1f);
		else if (PlaneTaxi)
			AI::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(Peddy, Vick, Vme.x, Vme.y, Vme.z, fSpeed, 16777216, 1.0f);
		else
			AI::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(Peddy, Vick, Vme.x, Vme.y, Vme.z, fSpeed, 1073742652, 5.0f);//536871355

		PED::SET_PED_KEEP_TASK(Peddy, true);
		if (Blocking)
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Peddy, true);
	}
}
void DriveBye(Ped Peddy, Vehicle Vick, Ped Target, bool Driver)
{
	AI::CLEAR_PED_TASKS(Peddy);
	if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Peddy, 0))
	{
		AI::TASK_VEHICLE_SHOOT_AT_PED(Peddy, Target, 5);
		if (Driver)
		{
			if ((bool)PED::IS_PED_IN_ANY_VEHICLE(Target, 0))
				AI::TASK_VEHICLE_CHASE(Peddy, Target);
			else
				DriveToooDest(Peddy, Vick, ENTITY::GET_ENTITY_COORDS(Target, true), 45.0f, true, true, false);
		}
	}
}
void FlyHeli(Ped Pedd, Vehicle Vhick, Vector3 vHeliDest, float fSpeed, float flanding)
{
	ENTITY::FREEZE_ENTITY_POSITION(Vhick, false);
	AI::CLEAR_PED_TASKS(Pedd);
	float HeliDesX = vHeliDest.x;
	float HeliDesY = vHeliDest.y;
	float HeliDesZ = vHeliDest.z;
	float HeliSpeed = fSpeed;
	float HeliLandArea = flanding;

	Vector3 VHickPos = ENTITY::GET_ENTITY_COORDS(Pedd, true);

	float dx = VHickPos.x - HeliDesX;
	float dy = VHickPos.y - HeliDesY;
	float HeliDirect = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(dx, dy) - 180.00f;

	AI::TASK_HELI_MISSION(Pedd, Vhick, 0, 0, HeliDesX, HeliDesY, HeliDesZ, 9, HeliSpeed, HeliLandArea, HeliDirect, -1, -1, -1, 0);
	PED::SET_PED_FIRING_PATTERN(Pedd, MyHashKey("FIRING_PATTERN_BURST_FIRE_HELI"));
	PED::SET_PED_KEEP_TASK(Pedd, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Pedd, true);
}
void FlyPlane(Ped Pedd, Vehicle Vhick, Vector3 vPlaneDest, Ped AttackPed)
{
	float fAngle = GetAngle(ENTITY::GET_ENTITY_COORDS(Vhick, true), vPlaneDest);
	AI::CLEAR_PED_TASKS(Pedd);

	if (AttackPed != NULL)
	{
		if (IsItThePlane(2, Vhick))
			AI::TASK_PLANE_MISSION(Pedd, Vhick, 0, 0, vPlaneDest.x, vPlaneDest.y, vPlaneDest.z, 4, 20.0f, 50.0f, fAngle, vPlaneDest.z + 10.0f, vPlaneDest.z - 10.0f);
		else
			AI::TASK_PLANE_MISSION(Pedd, Vhick, 0, AttackPed, 0, 0, 0, 6, 0.0f, 0.0f, fAngle, 5000.0f, -10.0f);
		PED::SET_PED_FLEE_ATTRIBUTES(Pedd, 0, true);
	}
	else
		AI::TASK_PLANE_MISSION(Pedd, Vhick, 0, 0, vPlaneDest.x, vPlaneDest.y, vPlaneDest.z, 4, 20.0f, 50.0f, fAngle, vPlaneDest.z + 100.0f, vPlaneDest.z -100.0f);

	PED::SET_PED_KEEP_TASK(Pedd, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Pedd, true);
}
void PedDoGetIn(Vehicle GetV, Ped Peddy, string sId)
{
	int iSeats = FindUSeat(GetV);
	Vector3 PedPos = ENTITY::GET_ENTITY_COORDS(Peddy, true);
	Vector3 VehPos = ENTITY::GET_ENTITY_COORDS(GetV, true);

	if (iSeats > -1)
	{
		if (DistanceTo(PedPos, VehPos) < 65.00)
		{
			GetInVehicle(Peddy, GetV, iSeats, false);
		}
		else
			WarptoAnyVeh(GetV, Peddy, iSeats);
	}
}
void GreefWar(Ped Peddy, Ped Victim)
{
	if (Victim != NULL)
	{
		AI::CLEAR_PED_TASKS(Peddy);
		PED::SET_PED_FLEE_ATTRIBUTES(Peddy, 0, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(Peddy, 46, true);

		AI::TASK_COMBAT_PED(Peddy, Victim, 0, 16);
	}
}
void FightPlayer(PZclass::PlayerBrain* brain)
{
	bool bInVeh = false;

	if (brain->Driver || brain->Passenger)
		bInVeh = true;

	brain->BlipColour = 1;
	brain->Friendly = false;
	brain->WanBeFriends = false;
	brain->ApprochPlayer = false;
	brain->TimeOn += 120000;

	UI::SET_BLIP_COLOUR(brain->ThisBlip, 1);
	PED::REMOVE_PED_FROM_GROUP(brain->ThisPed);

	if (MySettings.Aggression > 8)
		RelGroupMember(brain->ThisPed, GP_Mental);
	else
		RelGroupMember(brain->ThisPed, GP_Attack);

	/*
	AI::CLEAR_PED_TASKS_IMMEDIATELY(brain->ThisPed);
	if (!bInVeh)
		GreefWar(brain->ThisPed, PLAYER::PLAYER_PED_ID());
	else
		DriveBye(brain->ThisPed, PED::GET_VEHICLE_PED_IS_IN(brain->ThisPed, false), PLAYER::PLAYER_PED_ID(), brain->Driver);
	*/
}

void FightTogether(Vehicle myVeh, Ped myTarget)
{
	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (PedList[i].Passenger)
		{
			if (PedList[i].ThisVeh == myVeh)
				DriveBye(PedList[i].ThisPed, myVeh, myTarget, false);
		}
	}
}
Ped FindAFight(Ped Attacker, bool Friend, bool inPlane, bool inHeli)
{
	Ped Fight = NULL;
	float Dist = 4000.0f;
	int You = -1;

	if (inPlane)
	{
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (MySettings.Aggression > 9)
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed)
				{
					if (PedList[i].IsPlane || PedList[i].IsHeli)
						You = i;
				}
			}
			else
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].Friendly != Friend && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && PedList[i].IsPlane)
				{
					if (PedList[i].IsPlane || PedList[i].IsHeli)
						You = i;
				}
			}
		}

		if (You == -1)
		{
			for (int i = 0; i < (int)PedList.size(); i++)
			{
				if (MySettings.Aggression > 9)
				{
					if (PedList[i].ThisPed != Attacker && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed)
					{
						if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
						{
							Dist = DistanceTo(PedList[i].ThisPed, Attacker);
							You = i;
						}
					}
				}
				else
				{
					if (PedList[i].ThisPed != Attacker && PedList[i].Friendly != Friend && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed)
					{
						if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
						{
							Dist = DistanceTo(PedList[i].ThisPed, Attacker);
							You = i;
						}
					}
				}
			}
		}
	}
	else if (inHeli)
	{
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (MySettings.Aggression > 9)
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && PedList[i].IsHeli)
					You = i;
			}
			else
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].Friendly != Friend && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && PedList[i].IsHeli)
					You = i;
			}
		}

		if (You == -1)
		{
			for (int i = 0; i < (int)PedList.size(); i++)
			{
				if (MySettings.Aggression > 9)
				{
					if (PedList[i].ThisPed != Attacker && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && !PedList[i].IsPlane)
					{
						if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
						{
							Dist = DistanceTo(PedList[i].ThisPed, Attacker);
							You = i;
						}
					}
				}
				else
				{
					if (PedList[i].ThisPed != Attacker && PedList[i].Friendly != Friend && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && !PedList[i].IsPlane)
					{
						if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
						{
							Dist = DistanceTo(PedList[i].ThisPed, Attacker);
							You = i;
						}
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (MySettings.Aggression > 9)
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && !PedList[i].IsPlane && !PedList[i].IsHeli)
				{
					if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
					{
						Dist = DistanceTo(PedList[i].ThisPed, Attacker);
						You = i;
					}
				}
			}
			else
			{
				if (PedList[i].ThisPed != Attacker && PedList[i].Friendly != Friend && PedList[i].ThisPed != NULL && !PedList[i].YoDeeeed && !PedList[i].IsPlane && !PedList[i].IsHeli)
				{
					if (DistanceTo(PedList[i].ThisPed, Attacker) < Dist)
					{
						Dist = DistanceTo(PedList[i].ThisPed, Attacker);
						You = i;
					}
				}
			}
		}
	}

	if (!Friend && MySettings.Aggression > 5)
	{
		if (You != -1)
		{
			if (DistanceTo(PLAYER::PLAYER_PED_ID(), Attacker) < Dist)
				Fight = PLAYER::PLAYER_PED_ID();
			else
				Fight = PedList[You].ThisPed;
		}
		else
			Fight = PLAYER::PLAYER_PED_ID();
	}
	else if (You != -1)
		Fight = PedList[You].ThisPed;

	return Fight;
}
void PickFight(Ped Attacker, Vehicle Plane, Ped fight, int PrefVeh)
{
	if (fight != NULL)
	{
		Vector3 FIghterPos = PZSys::EntityPosition(fight);

		if (PrefVeh == 3 || PrefVeh == 5)
			FlyPlane(Attacker, Plane, FIghterPos, fight);
		else if (PrefVeh == 2 || PrefVeh == 4)
			FlyHeli(Attacker, Plane, FIghterPos, 45.00, 0.00);
		else
			DriveBye(Attacker, Plane, fight, true);
	}
}
int iPasiveMode = -1;
Ped YourKilller(Ped Victim)
{
	Entity ThisEnt = PED::_GET_PED_KILLER(Victim);
	if (ThisEnt != NULL)
	{
		if ((bool)ENTITY::IS_ENTITY_A_PED(ThisEnt))
		{

		}
		else if ((bool)ENTITY::IS_ENTITY_A_VEHICLE(ThisEnt))
		{
			ThisEnt = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ThisEnt, -1);
		}
		else
		{
			ThisEnt = NULL;
		}
	}
	return ThisEnt;
}
int WhoShotMe(Ped MeDie)
{
	int iShoot = -1;
	Entity Killer = YourKilller(MeDie);

	if (Killer == PLAYER::PLAYER_PED_ID())
	{
		iShoot = -10;
	}
	else
	{
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (Killer != NULL)
			{
				if (Killer == PedList[i].ThisPed)
				{
					iShoot = i;
					break;
				}
			}
		}
	}

	return iShoot;
}
int BackOffPig()
{
	LoggerLight("BackOffPig");

	int iAm = -1;
	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (PedList[i].PiggyBackin)
		{
			iAm = i;
			break;
		}
	}
	return iAm;
}
int iFolPos;
void OhDoKeepUp(Ped Peddy)
{
	LoggerLight("OhDoKeepUp");
	AI::CLEAR_PED_TASKS(Peddy);

	float fXpos = -2.50;
	float fYpos = 1.50;

	if (iFolPos == 1)
	{
		fXpos = -2.50;
		fYpos = 0.00;
	}
	else if (iFolPos == 2)
	{
		fXpos = -2.50;
		fYpos = -2.50;
	}
	else if (iFolPos == 3)
	{
		fXpos = 2.50;
		fYpos = 0.00;
	}
	else if (iFolPos == 4)
	{
		fXpos = 1.50;
		fYpos = 0.00;
	}
	else if (iFolPos == 5)
	{
		fXpos = -1.50;
		fYpos = 0.00;
	}
	else if (iFolPos == 6)
	{
		fXpos = 2.50;
		fYpos = -2.50;
	}
	else if (iFolPos == 7)
	{
		fXpos = -1.50;
		fYpos = -2.50;
		iFolPos = 0;
	}

	iFolPos++;
	AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(Peddy, PLAYER::PLAYER_PED_ID(), fXpos, fYpos, 0.0, 1.0, -1, 0.0, 1);
	//PED::SET_PED_KEEP_TASK(Peddy, 1);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Peddy, 0);
}
void FolllowTheLeader(Ped Peddy)
{
	LoggerLight("FolllowTheLeader");

	PED::REMOVE_PED_FROM_GROUP(Peddy);

	RelGroupMember(Peddy, Gp_Follow);
	PED::SET_PED_AS_GROUP_MEMBER(Peddy, FollowMe);//was as FollowMe me now as Gp_

	if ((bool)PED::IS_PED_GROUP_MEMBER(Peddy, FollowMe))
		LoggerLight("IS_PED_GROUP_MEMBER == true");

	OhDoKeepUp(Peddy);
}
int NearHiest()
{
	int iNear = -1;
	Vector3 PlayPos = PlayerPosi();

	for (int i = 0; i < (int)HeistDrop.size(); i++)
	{
		if (DistanceTo(HeistDrop[i], PlayPos) < 55.0)
		{
			iNear = i;
			break;
		}
	}
	return iNear;
}

void OnlineDress(Ped Pedx, ClothX* MyCloths)
{
	LoggerLight("OnlineDress");

	PED::CLEAR_ALL_PED_PROPS(Pedx);

	for (int i = 0; i < MyCloths->ClothA.size(); i++)
	{
		if (MyCloths->ClothA[i] != -10)
			PED::SET_PED_COMPONENT_VARIATION(Pedx, i, MyCloths->ClothA[i], MyCloths->ClothB[i], 2);

	}

	for (int i = 0; i < MyCloths->ExtraA.size(); i++)
	{
		if (MyCloths->ExtraA[i] != -10)
			PED::SET_PED_PROP_INDEX(Pedx, i, MyCloths->ExtraA[i], MyCloths->ExtraB[i], false);
	}
}
void OnlineFaces(Ped Pedx, ClothBank* pFixtures)
{
	LoggerLight("OnlineFaces");
	//****************FaceShape/Colour****************
	PED::SET_PED_HEAD_BLEND_DATA(Pedx, pFixtures->MyFaces.ShapeFirstID, pFixtures->MyFaces.ShapeSecondID, pFixtures->MyFaces.ShapeThirdID, pFixtures->MyFaces.SkinFirstID, pFixtures->MyFaces.SkinSecondID, pFixtures->MyFaces.SkinThirdID, pFixtures->MyFaces.ShapeMix, pFixtures->MyFaces.SkinMix, pFixtures->MyFaces.ThirdMix, 0);

	if (ItsChristmas)
	{
		if (pFixtures->Male)
		{
			PZclass::ClothX Outfit = XmasOut_M[LessRandomInt("Crimb01", 0, (int)XmasOut_M.size() - 1)];
			OnlineDress(Pedx, &Outfit);
		}
		else
		{
			PZclass::ClothX Outfit = XmasOut_F[LessRandomInt("Crimb01", 0, (int)XmasOut_F.size() - 1)];
			OnlineDress(Pedx, &Outfit);
		}
	}
	else if (ItHalloween)
	{
		if (pFixtures->Male)
		{
			int iRan = LessRandomInt("Hallow01", 0, 11);
			PZclass::ClothX Outfit = PZclass::ClothX("MaleArena WarSpace_Horror", { 0, 141, 0, 164, 108, 0, 33, 0, 15, 0, 0, 277 }, { 0, iRan, 0, iRan, iRan, 0, 0, 0, 0, 0, 0, iRan }, { -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1 });
			OnlineDress(Pedx, &Outfit);
		}
		else
		{
			int iRan = LessRandomInt("Hallow01", 0, 11);
			PZclass::ClothX Outfit = PZclass::ClothX("FemaleArena WarSpace_Horror", { 21, 141, 0, 205, 115, 0, 34, 0, 6, 0, 0, 290 }, { 0, iRan, 0, iRan, iRan, 0, 0, 0, 0, 0, 0, iRan }, { -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1 });
			OnlineDress(Pedx, &Outfit);
		}
	}
	else
		OnlineDress(Pedx, &pFixtures->Cothing);

	PED::SET_PED_COMPONENT_VARIATION(Pedx, 2, pFixtures->MyHair.Comp, pFixtures->MyHair.Text, 2);

	if (pFixtures->MyHair.Over != -1)
		PED::_APPLY_PED_OVERLAY(Pedx, pFixtures->MyHair.OverLib, pFixtures->MyHair.Over);
	PED::_SET_PED_HAIR_COLOR(Pedx, pFixtures->HairColour, pFixtures->HairStreaks);

	//****************Face****************
	for (int i = 0; i < pFixtures->MyOverlay.size(); i++)
	{
		int iColour = 0;

		if (i == 1)
		{
			iColour = 1;
		}//Facial Hair
		else if (i == 2)
		{
			iColour = 1;
		}//Eyebrows
		else if (i == 5)
		{
			iColour = 2;
		}//Blush
		else if (i == 8)
		{
			iColour = 2;
		}//Lipstick
		else if (i == 10)
		{
			iColour = 1;
		}//Chest Hair

		int iChange = pFixtures->MyOverlay[i].Overlay;
		int AddColour = pFixtures->MyOverlay[i].OverCol;
		float fVar = pFixtures->MyOverlay[i].OverOpc;

		PED::SET_PED_HEAD_OVERLAY(Pedx, i, iChange, fVar);

		if (iColour > 0)
			PED::_SET_PED_HEAD_OVERLAY_COLOR(Pedx, i, iColour, AddColour, 0);
	}
	//****************Tattoos****************
	for (int i = 0; i < pFixtures->MyTattoo.size(); i++)
		PED::_APPLY_PED_OVERLAY(Pedx, MyHashKey(pFixtures->MyTattoo[i].BaseName), MyHashKey(pFixtures->MyTattoo[i].TatName));
}

void MaxOutAllModsNoWheels(Vehicle Vehic, int cT)
{
	LoggerLight("MaxOutAllModsNoWheels");

	VEHICLE::SET_VEHICLE_MOD_KIT(Vehic, 0);
	for (int i = 0; i < 50; i++)
	{
		int iSpoilher = VEHICLE::GET_NUM_VEHICLE_MODS(Vehic, i);

		if (i == 18 || i == 22 || i == 23 || i == 24)
		{

		}
		else
		{
			iSpoilher -= 1;
			VEHICLE::SET_VEHICLE_MOD(Vehic, i, iSpoilher, true);
		}
	}

	if (cT != 13 && cT != 14 && cT != 15 && cT != 16)
	{
		VEHICLE::TOGGLE_VEHICLE_MOD(Vehic, 18, true);
		VEHICLE::TOGGLE_VEHICLE_MOD(Vehic, 22, true);
	}
	else if (cT == 15 || cT == 16)
		VEHICLE::_SET_VEHICLE_LANDING_GEAR(Vehic, 3);
}
void MakeModsNotWar(Vehicle Vehic, vector<int> MyMods)
{
	LoggerLight("MakeModsNotWar");

	VEHICLE::SET_VEHICLE_MOD_KIT(Vehic, 0);

	for (int i = 0; i < MyMods.size(); i++)
	{
		if (MyMods[i] == -10)
		{

		}
		else if (i == 48)
		{
			int iSpoilher = -1;
			if (MyMods[i] == -1)
			{
				iSpoilher = VEHICLE::GET_NUM_VEHICLE_MODS(Vehic, i) - 1;
				if (iSpoilher < 1)
					iSpoilher = VEHICLE::GET_VEHICLE_LIVERY_COUNT(Vehic) - 1;

				if (iSpoilher > 0)
				{
					MyMods[i] = RandomInt(0, iSpoilher);
				}
			}

			if (iSpoilher > -1 || MyMods[i] > -1)
			{
				VEHICLE::SET_VEHICLE_LIVERY(Vehic, MyMods[i]);
				VEHICLE::SET_VEHICLE_MOD(Vehic, i, MyMods[i], true);
			}
		}
		else if (i == 66)
		{
			if (MyMods[i] == -1)
			{
				int iCheckHere = RandomInt(0, 159);
				MyMods[i] = iCheckHere;
			}
		}
		else if (i == 67)
		{
			if (MyMods[i] == -1)
			{
				int iCheckHere = RandomInt(0, 159);
				VEHICLE::SET_VEHICLE_COLOURS(Vehic, MyMods[i - 1], iCheckHere);
			}
			else
			{
				VEHICLE::SET_VEHICLE_COLOURS(Vehic, MyMods[i - 1], MyMods[i]);
			}
		}
		else if (MyMods[i] == -1)
		{
			int iSpoilher = VEHICLE::GET_NUM_VEHICLE_MODS(Vehic, i) - 1;
			if (iSpoilher > 0)
			{
				int iCheckHere = RandomInt(0, iSpoilher);
				VEHICLE::SET_VEHICLE_MOD(Vehic, i, iCheckHere, true);
			}
		}
		else
		{
			VEHICLE::SET_VEHICLE_MOD(Vehic, i, MyMods[i], true);
		}
	}
}
Vehicle OppresiveDump(Vector4 VecLocal)
{
	Vehicle BuildVehicle = NULL;
	int iVehHash = MyHashKey("oppressor2");

	if ((bool)STREAMING::IS_MODEL_IN_CDIMAGE(iVehHash) && (bool)STREAMING::IS_MODEL_A_VEHICLE(iVehHash))
	{
		STREAMING::REQUEST_MODEL(iVehHash);
		while (!(bool)STREAMING::HAS_MODEL_LOADED(iVehHash))
			WAIT(1);

		BuildVehicle = VEHICLE::CREATE_VEHICLE(iVehHash, VecLocal.X, VecLocal.Y, VecLocal.Z, VecLocal.R, true, true);
		MaxOutAllModsNoWheels(BuildVehicle, 15);

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(BuildVehicle, 1, 1);
	}

	return BuildVehicle;
}
Ped PlayerPedGen(Vector4 vLocal, PlayerBrain* thisBrain)
{
	LoggerLight("PlayerPedGen = " + thisBrain->MyName);

	Ped thisPed = NULL;

	WAIT(100);
	Hash model = MyHashKey(thisBrain->PFMySetting.Model);
	STREAMING::REQUEST_MODEL(model);// Check if the model is valid

	if ((bool)STREAMING::IS_MODEL_IN_CDIMAGE(model) && (bool)STREAMING::IS_MODEL_VALID(model))
	{
		while (!STREAMING::HAS_MODEL_LOADED(model))
			WAIT(10);

		thisPed = PED::CREATE_PED(4, model, vLocal.X, vLocal.Y, vLocal.Z, vLocal.R, true, false);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

		if ((bool)ENTITY::DOES_ENTITY_EXIST(thisPed))
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(thisPed, 1, 1);

			if (MySettings.Aggression > 5)
				PED::SET_PED_ACCURACY(thisPed, MySettings.AccMax);
			else
				PED::SET_PED_ACCURACY(thisPed, MySettings.AccMin);

			int iHeal = RandomInt(200, 300);
			PED::SET_PED_MAX_HEALTH(thisPed, iHeal);
			ENTITY::SET_ENTITY_HEALTH(thisPed, iHeal);
			thisBrain->HeadTag = UI::_CREATE_HEAD_DISPLAY(thisPed, (LPSTR)thisBrain->MyName.c_str(), false, false, "", 1);//
			UI::_0xEE76FF7E6A0166B0(thisBrain->HeadTag, true);
			UI::_0xA67F9C46D612B6F1(thisBrain->HeadTag, true);

			if (MySettings.Aggression > 1)
				PED::SET_PED_COMBAT_ABILITY(thisPed, 150);
			else
				PED::SET_PED_COMBAT_ABILITY(thisPed, 10);

			if (!thisBrain->Driver)
			{
				PED::SET_PED_COMBAT_MOVEMENT(thisPed, 2);
				AI::SET_PED_PATH_CAN_USE_CLIMBOVERS(thisPed, true);
				AI::SET_PED_PATH_CAN_USE_LADDERS(thisPed, true);
				AI::SET_PED_PATH_CAN_DROP_FROM_HEIGHT(thisPed, true);
				AI::SET_PED_PATH_PREFER_TO_AVOID_WATER(thisPed, false);
				PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 0, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 1, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 3, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 5, true);

				if (MySettings.Aggression > 1)
				{
					PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 46, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(thisPed, 2, true);
					if (MySettings.Aggression > 2)
						PED::SET_PED_CAN_SWITCH_WEAPON(thisPed, true);
				}
			}
			
			thisBrain->ThisPed = thisPed;

			OnlineFaces(thisPed, &thisBrain->PFMySetting);

			GunningIt(thisPed, thisBrain->GunSelect);

			if (thisBrain->MyIdentity == "")
			{
				thisBrain->TimeOn = InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession);
				thisBrain->Level = UniqueLevels();
				thisBrain->MyIdentity = thisBrain->MyName + std::to_string(thisBrain->Level);
			}

			if (thisBrain->Follower)
			{
				thisBrain->Friendly = true;
				if (!thisBrain->Driver && !thisBrain->Passenger)
					FolllowTheLeader(thisPed);
				else
					RelGroupMember(thisPed, Gp_Follow);
			}
			else if (thisBrain->Friendly)
			{
				RelGroupMember(thisPed, Gp_Friend);
				//if (!thisBrain->Driver || !thisBrain->Passenger)
				//	WalkHere(thisPed, FindingShops(thisPed));
			}
			else
				FightPlayer(thisBrain);

			
			if (thisBrain->Oppressor != NULL)
			{
				PED::SET_PED_ACCURACY(thisPed, 100);
				thisBrain->DirBlip = false;
				thisBrain->ThisBlip = PedBlimp(NULL, thisPed, 639, thisBrain->MyName, thisBrain->BlipColour, false);
			}
			else if (thisBrain->Driver)
			{
				float fAggi = (float)MySettings.Aggression / 100;
				PED::SET_DRIVER_ABILITY(thisPed, 1.00f);
				PED::SET_DRIVER_AGGRESSIVENESS(thisPed, fAggi);
				PED::SET_PED_STEERS_AROUND_VEHICLES(thisPed, true);
				thisBrain->DirBlip = true;
			}
			else if (thisBrain->Passenger)
				thisBrain->DirBlip = true;
			else
				thisBrain->DirBlip = false;


			if (thisBrain->IsPlane)
			{
				if (thisBrain->ThisVeh != NULL)
					ENTITY::FREEZE_ENTITY_POSITION(thisBrain->ThisVeh, false);

			}
		}
		else
			thisPed = NULL;
	}
	else
		thisPed = NULL;

	return thisPed;
}

Hash RandVeh(int iVechList)
{
	LoggerLight("RandVeh, iVechList == " + std::to_string(iVechList));

	Hash sVeh = 0;

	if (iVechList == 1)
	{
		if (AddnVeh.size() > 0)
			sVeh = AddnVeh[LessRandomInt("RandVeh01", 0, (int)AddnVeh.size() - 1)];
		else
			sVeh = MyHashKey(PreVeh_01[LessRandomInt("RandVeh01", 0, (int)PreVeh_01.size() - 1)]);
	}
	else if (iVechList == 2)
		sVeh = MyHashKey(PreVeh_02[LessRandomInt("RandVeh02", 0, (int)PreVeh_02.size() - 1)]);
	else if (iVechList == 3)
		sVeh = MyHashKey(PreVeh_03[LessRandomInt("RandVeh03", 0, (int)PreVeh_03.size() - 1)]);
	else if (iVechList == 4)
		sVeh = MyHashKey(PreVeh_04[LessRandomInt("RandVeh04", 0, (int)PreVeh_04.size() - 1)]);
	else if (iVechList == 5)
		sVeh = MyHashKey(PreVeh_05[LessRandomInt("RandVeh05", 0, (int)PreVeh_05.size() - 1)]);
	else if (iVechList == 6)
		sVeh = MyHashKey(PreVeh_06[LessRandomInt("RandVeh06", 0, (int)PreVeh_06.size() - 1)]);
	else if (iVechList == 7)
		sVeh = MyHashKey(PreVeh_07[LessRandomInt("RandVeh07", 0, (int)PreVeh_07.size() - 1)]);
	else if (iVechList == 8 || iVechList == 999)
		sVeh = MyHashKey("HYDRA");
	else if (iVechList == 9)
		sVeh = MyHashKey("oppressor2");
	else if (iVechList == 15)
		sVeh = MyHashKey("police5");
	else
		sVeh = MyHashKey("ZENTORNO");

	return sVeh;
}
bool IsItARealVehicle(Hash vehName)
{
	LoggerLight("IsItARealVehicle");

	bool bIsReal = false;
	if ((bool)STREAMING::IS_MODEL_A_VEHICLE(vehName))
		bIsReal = true;

	return bIsReal;
}
bool HasASeat(Vehicle vMe)
{
	bool bIn = false;
	if (vMe != NULL)
	{
		if (FindUSeat(vMe) > -1)
			bIn = true;
	}
	return bIn;
}

Vehicle SpawnVehicleProp(Vector3 VecLocal)
{
	Vehicle BuildVehicle = NULL;

	LoggerLight("SpawnVehicle");

	Hash VehModel = RandVeh(7);

	if (!IsItARealVehicle(VehModel))
		VehModel = MyHashKey("MAMBA");

	if ((bool)STREAMING::IS_MODEL_IN_CDIMAGE(VehModel) && (bool)STREAMING::IS_MODEL_A_VEHICLE(VehModel))
	{
		STREAMING::REQUEST_MODEL(VehModel);
		while (!(bool)STREAMING::HAS_MODEL_LOADED(VehModel))
			WAIT(1);

		BuildVehicle = VEHICLE::CREATE_VEHICLE(VehModel, VecLocal.x, VecLocal.y, VecLocal.z, 0.0, true, true);

		ENTITY::APPLY_FORCE_TO_ENTITY(BuildVehicle, 1, 0, 0, 1, 0, 0, 1, 1, false, true, true, false, true);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(VehModel);
	}
	return BuildVehicle;
}
Vehicle SpawnVehicle(int iVehList, Vector4 VecLocal, bool bAddPlayer, PlayerBrain* myBrains, bool bAsProp, bool bCanFill)
{
	LoggerLight("SpawnVehicle");

	Vehicle BuildVehicle = NULL;
	WAIT(100);

	Hash VehModel = RandVeh(iVehList);

	if (!IsItARealVehicle(VehModel))
		VehModel = MyHashKey("MAMBA");

	if ((bool)STREAMING::IS_MODEL_IN_CDIMAGE(VehModel) && (bool)STREAMING::IS_MODEL_A_VEHICLE(VehModel))
	{
		STREAMING::REQUEST_MODEL(VehModel);
		while (!(bool)STREAMING::HAS_MODEL_LOADED(VehModel))
			WAIT(1);

		BuildVehicle = VEHICLE::CREATE_VEHICLE(VehModel, VecLocal.X, VecLocal.Y, VecLocal.Z, VecLocal.R, true, true);

		int iClass = VEHICLE::GET_VEHICLE_CLASS(BuildVehicle);
		if (iClass == 15 || iClass == 16)
			MaxOutAllModsNoWheels(BuildVehicle, iClass);
		else
			MakeModsNotWar(BuildVehicle, RandVehModsist());



		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(BuildVehicle, 1, 1);

		myBrains->ThisVeh = BuildVehicle;
		myBrains->Driver = true;
		myBrains->FindPlayer = 0;

		if (!HasASeat(BuildVehicle))
			myBrains->ApprochPlayer = false;

		if (iVehList == 8)
		{
			myBrains->IsPlane = true;
			Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(BuildVehicle, true);
			MyPos.z = 550.00;
			myBrains->Oppressor = OppresiveDump(Vector4(MyPos.x, MyPos.y, MyPos.z, 0.0));
			ENTITY::ATTACH_ENTITY_TO_ENTITY(myBrains->Oppressor, BuildVehicle, PED::GET_PED_BONE_INDEX(BuildVehicle, 0), 0.00, 3.32999945, -0.10, 0.00, 0.00, 0.00, false, false, false, false, 2, true);
			ENTITY::SET_ENTITY_ALPHA(BuildVehicle, 0, 0);
			MoveEntity(BuildVehicle, MyPos);
		}
		else if (myBrains->Friendly && IsItThePlane(1, BuildVehicle))
		{
			myBrains->PlaneLand = 10;
			myBrains->AirTranspport = true;
			myBrains->ApprochPlayer = false;
			bPlanePort = true;
			bCanFill = false;
		}

		if (bAddPlayer)
		{
			myBrains->ThisPed = PlayerPedGen(VecLocal, myBrains);
			WarptoAnyVeh(BuildVehicle, myBrains->ThisPed, -1);
			GunningIt(myBrains->ThisPed, 6);
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(myBrains->ThisPed, 1);

			VEHICLE::SET_VEHICLE_ENGINE_ON(BuildVehicle, 1, 1, 0);
			int iSpare = -1;
			if (!myBrains->Friendly)
				iSpare = 0;
			int iSeating = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(BuildVehicle);
			if (bCanFill && PlayerZinSesh(true) + iSeating + iSpare < MySettings.MaxPlayers)
			{
				for (int i = 0; i < iSeating + iSpare; i++)
				{
					LoggerLight("Fill Vehicle with peds");
					int iLevel = UniqueLevels();

					ClothBank MB = NewClothBank();
					int iGetGunnin = YourGunNum();
					PlayerBrain newBrain = PlayerBrain(MB.Name, MB.Name + std::to_string(iLevel), MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), iLevel, false, myBrains->Nationality, iGetGunnin, 0);
					newBrain.Driver = false;
					newBrain.Passenger = true;
					newBrain.ThisVeh = BuildVehicle;
					newBrain.Friendly = myBrains->Friendly;
					PedList.push_back(newBrain);
					Ped CarPed = PlayerPedGen(VecLocal, &PedList[PedList.size() - 1]);
					WarptoAnyVeh(BuildVehicle, CarPed, i);
					GunningIt(CarPed, 6);
				}
			}
		}
		else
			WarptoAnyVeh(BuildVehicle, myBrains->ThisPed, -1);


		Vector3 PP = PlayerPosi();
		PP.z += 150.0;
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(VehModel);
	}
	else
		BuildVehicle = NULL;

	return BuildVehicle;
}

void HeistDrips(int iMyArea)
{
	LoggerLight("HeistDrips, iMyArea == " + std::to_string(iMyArea));
	vector<Vector4> VectorList = {};

	if (iMyArea == 0)
	{
		VectorList.push_back(Vector4(-1105.577, -1692.11, 4.345489, 0.0));
		VectorList.push_back(Vector4(-1107.141, -1690.495, 4.353377, 0.0));     //1
		VectorList.push_back(Vector4(-1105.481, -1690.545, 4.325913, 0.0));     //2--Vesp Beach
		VectorList.push_back(Vector4(716.5444, -979.9716, 24.11811, 0.0));      //3+Darnel
	}
	else if (iMyArea == 1)
	{

		VectorList.push_back(Vector4(60.53763, 8.939384, 69.14648, 0.0));      //4
		VectorList.push_back(Vector4(58.98491, 8.12832, 69.18693, 0.0));        //5
		VectorList.push_back(Vector4(60.66815, 6.700741, 69.12641, 0.0));     //6
		VectorList.push_back(Vector4(61.93483, 7.88855, 69.09691, 0.0));       //7--Vinwood

	}
	else if (iMyArea == 2)
	{

		VectorList.push_back(Vector4(718.9022, -980.336, 24.12285, 0.0));      //8
		VectorList.push_back(Vector4(717.793, -982.8883, 24.13336, 0.0));     //9
		VectorList.push_back(Vector4(715.0596, -982.1675, 24.1188, 0.0));      //10
		VectorList.push_back(Vector4(716.5444, -979.9716, 24.11811, 0.0));    //11--Darnels LaMessa

	}
	else if (iMyArea == 3)
	{
		VectorList.push_back(Vector4(1681.823, 4817.896, 42.01214, 0.0));    //12
		VectorList.push_back(Vector4(1681.932, 4819.233, 42.03329, 0.0));     //13
		VectorList.push_back(Vector4(1681.289, 4820.775, 42.05544, 0.0));    //14
		VectorList.push_back(Vector4(1681.09, 4822.904, 42.05639, 0.0));       //15--Grapesead

	}
	else
	{

		VectorList.push_back(Vector4(-1038.972, -2736.403, 20.16928, 0.0));   //16
		VectorList.push_back(Vector4(-1037.887, -2738.665, 20.16928, 0.0));    //17
		VectorList.push_back(Vector4(-1035.784, -2738.059, 20.16928, 0.0));    //18
		VectorList.push_back(Vector4(-1036.624, -2735.942, 20.16928, 0.0));   //19--Airport

	}

	int iNat = RandomInt(1, 5);

	for (int i = 0; i < (int)VectorList.size() - 1; i++)
	{
		int iLevel = UniqueLevels();
		int iGetGunnin = YourGunNum();
		ClothBank MB = NewClothBank();
		PlayerBrain newBrain = PlayerBrain(MB.Name, MB.Name + std::to_string(iLevel), MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), iLevel, false, iNat, iGetGunnin, 0);
		PedList.push_back(newBrain);
		PlayerPedGen(VectorList[i], &PedList[PedList.size() - 1]);
	}
	int iLevel = UniqueLevels();

	int iGetGunnin = YourGunNum();
	ClothBank MB = NewClothBank();
	PlayerBrain newBrain = PlayerBrain(MB.Name, MB.Name + std::to_string(iLevel), MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), iLevel, false, iNat, iGetGunnin, 0);
	PedList.push_back(newBrain);
	Ped Bob = PlayerPedGen(VectorList[(int)VectorList.size() - 1], &PedList[PedList.size() - 1]);
	WAIT(1200);

	FIRE::ADD_OWNED_EXPLOSION(Bob, VectorList[0].X, VectorList[0].Y, VectorList[0].Z, 0, 1, true, false, 1);

	bHeistPop = false;
}

void CreatePlayer()
{
	LoggerLight("CreatePlayer");

	if (iAirCount < 0)
		iAirCount = 0;

	int iLevel = UniqueLevels();
	int iGetGunnin = YourGunNum();
	ClothBank MB = NewClothBank();
	int iNat = RandomInt(1, 5);
	PlayerBrain newBrain = PlayerBrain(MB.Name, MB.Name + std::to_string(iLevel), MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), iLevel, false, iNat, iGetGunnin, 0);

	bool bCanFill = false;
	int iBrain = LessRandomInt("CreatePlayerz01", 1, 10);

	if (MySettings.Aggression == 0)
	{
		newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 1)
	{
		if (iBrain < 5)
			newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 2)
	{
		if (iBrain < 2)
			newBrain.Friendly = false;
		else if (iBrain > 8)
			newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 3)
	{
		if (iBrain < 3)
			newBrain.Friendly = false;
		else if (iBrain > 8)
			newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 4)
	{
		if (iBrain < 4)
			newBrain.Friendly = false;
		else if (iBrain > 9)
			newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 5)
	{
		if (iBrain < 5)
			newBrain.Friendly = false;
		else if (iBrain > 9)
			newBrain.SessionJumper = true;
	}
	else if (MySettings.Aggression == 6)
	{
		if (iBrain < 6)
			newBrain.Friendly = false;
	}
	else if (MySettings.Aggression == 7)
	{
		if (iBrain < 7)
			newBrain.Friendly = false;
	}
	else if (MySettings.Aggression == 8)
	{
		if (iBrain < 8)
			newBrain.Friendly = false;
	}
	else if (MySettings.Aggression == 9)
	{
		if (iBrain < 9)
			newBrain.Friendly = false;
	}
	else
		newBrain.Friendly = false;

	if (!newBrain.Friendly)
		newBrain.BlipColour = 1;

	if (MySettings.NoRadar)
	{
		newBrain.OffRadarBool = true;
		newBrain.OffRadar = -1;
	}

	if (LessRandomInt("iBrain02", 1, 10) < 4 || MySettings.Aggression < 3 || newBrain.SessionJumper)
	{
		LoggerLight("CreatePlayer on foot");

		if (!newBrain.SessionJumper && newBrain.Friendly)
			newBrain.ApprochPlayer = true;
		FindPed MyFinda = FindPed(35.00f, 220.00f, newBrain);
		MakeFrenz.push_back(MyFinda);
	}
	else
	{
		LoggerLight("CreatePlayer Vehicle");

		int iTypeO = LessRandomInt("iBrain03", 1, 60);

		if (MySettings.Aggression < 4)
		{
			if (iAirCount < MySettings.AirVeh)
			{
				if (iTypeO < 5 && newBrain.Friendly)
				{
					iAirCount++;
					newBrain.PrefredVehicle = 3;//Plane
					newBrain.IsPlane = true;
				}
				else if (iTypeO < 20 && newBrain.Friendly)
				{
					iAirCount++;
					newBrain.PrefredVehicle = 2;//Heli 
					newBrain.IsHeli = true;
					bCanFill = true;
				}
				else
				{
					newBrain.PrefredVehicle = 1;//Veh				
					bCanFill = true;
				}
			}
			else
			{
				newBrain.PrefredVehicle = 1;//Veh				
				bCanFill = true;
			}
		}
		else
		{
			if (iAirCount < MySettings.AirVeh)
			{
				if (iTypeO < 5 && MySettings.Aggression > 6)
				{
					iAirCount++;
					newBrain.IsPlane = true;
					newBrain.PrefredVehicle = 8;//Oppressor
				}
				else if (iTypeO < 15)
				{
					newBrain.PrefredVehicle = 6;//ArmoredVeh
					bCanFill = true;
				}
				else if (iTypeO < 30)
				{
					iAirCount++;
					newBrain.IsPlane = true;
					newBrain.PrefredVehicle = 5;//AttPlane
				}
				else if (iTypeO < 40)
				{
					iAirCount++;
					newBrain.IsHeli = true;
					newBrain.PrefredVehicle = 4;
					bCanFill = true;
				}
				else if (iTypeO < 50 && newBrain.Friendly)
				{
					iAirCount++;
					newBrain.IsPlane = true;
					newBrain.PrefredVehicle = 3;//Plane
				}
				else if (iTypeO < 55 && newBrain.Friendly)
				{
					iAirCount++;
					newBrain.IsHeli = true;
					newBrain.PrefredVehicle = 2;//Heli 
					bCanFill = true;
				}
				else
				{
					bCanFill = true;
					newBrain.PrefredVehicle = 1;//Veh
				}
			}
			else
			{
				bCanFill = true;
				if (iTypeO < 25)
					newBrain.PrefredVehicle = 6;//ArmoredVeh
				else
					newBrain.PrefredVehicle = 1;//Veh
			}

			if (!bRentoCop && LessRandomInt("RentoCop", 1, 20) < 2)
			{
				bRentoCop = true;
				newBrain.Friendly = true;
				newBrain.IsPlane = false;
				newBrain.IsHeli = false;
				newBrain.BlipColour = 0;
				if (newBrain.PFMySetting.Male)
					newBrain.PFMySetting.Cothing = PZclass::ClothX("Cop", { 0, 0, -1, 0, 35, 0, 25, 0, 58, 0, 0, 55 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1 });
				else
					newBrain.PFMySetting.Cothing = PZclass::ClothX("Cop", { 0, 0, -1, 14, 34, 0, 25, 0, 35, 0, 0, 48 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1 });
				newBrain.PrefredVehicle = 15;
				newBrain.GunSelect = 15;
				newBrain.RentaCop = true;
				bCanFill = false;
			}
		}

		FindVeh myCar = FindVeh(15.00f, 145.00f, true, bCanFill, newBrain);
		MakeCarz.push_back(myCar);
	}
}
void InABuilding()
{
	LoggerLight("InABuilding");

	int iMit = LessRandomInt("InABuilding", 0, (int)AFKPlayers.size() - 1);

	string sName = SillyNameList();
	Blip FakeB = LocalBlip(NULL, AFKPlayers[iMit], 417, sName, 0);
	int iNat = RandomInt(1, 5);
	AfkPlayer MyAfk = AfkPlayer(FakeB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), iNat, iMit, sName, sName, UniqueLevels());
	AFKList.push_back(MyAfk);
}
void NewPlayer()
{
	LoggerLight("NewPlayer");
	iNextPlayer = InGameTime() + RandomInt(PZSetMinWait, PZSetMaxWait);

	int iHeister = NearHiest();

	if (PlayerZinSesh(true) + 5 < MySettings.MaxPlayers && iHeister != -1 && bHeistPop)
		HeistDrips(iHeister);
	else
	{
		if (LessRandomInt("NewPlayer", 1, 10) < 8)
			CreatePlayer();
		else
			InABuilding();
	}
}
void DropObjects(Vector3 vTarget)
{
	LoggerLight("DropObjects");

	string sObject;
	Entity Plop;
	if (LessRandomInt("DropObjects", 1, 10) < 5)
	{
		sObject = DropProplist[LessRandomInt("GetObject", 0, (int)DropProplist.size() - 1)];
		Plop = BuildProps(sObject, vTarget, NewVector3(0.0, 0.0, 0.0), true, false);
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&Plop);

	}
	else
	{
		Plop = SpawnVehicleProp(vTarget);
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&Plop);
	}
}
const std::vector<std::string> fluids = {
	"scr_solomon3",
	"scr_trev4_747_blood_splash",
	"cut_trevor1",
	"cs_trev1_blood_pool",
};
void EasyWayOut(Ped Vic)
{
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(Vic, MyHashKey("WEAPON_pistol"), 1, true);
	WEAPON::SET_PED_AMMO(Vic, MyHashKey("WEAPON_pistol"), 100);
	WEAPON::SET_CURRENT_PED_WEAPON(Vic, MyHashKey("WEAPON_pistol"), true);
	WAIT(450);
	ForceAnim(Vic, "mp_suicide", "pistol", GetPosV4(Vic));
	AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "GENERIC_CURSE_HIGH", Vic, 0, 0, 0);
	WAIT(700);
	int WaitHere = InGameTime() + 200;
	while (true)
	{
		if (WaitHere < InGameTime())
			break;
		if ((bool)STREAMING::HAS_NAMED_PTFX_ASSET_LOADED((LPSTR)fluids[0].c_str()))
		{
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL((LPSTR)fluids[0].c_str());
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY((LPSTR)fluids[1].c_str(), Vic, 0.0f, 0.15f, 0.75f, 0.0f, 0.0f, 90.0f, 1.0f, false, false, false);
		}
		else
			STREAMING::REQUEST_NAMED_PTFX_ASSET((LPSTR)fluids[0].c_str());

		if ((bool)STREAMING::HAS_NAMED_PTFX_ASSET_LOADED((LPSTR)fluids[2].c_str()))
		{
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL((LPSTR)fluids[2].c_str());
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY((LPSTR)fluids[3].c_str(), Vic, 0.0f, 0.15f, 0.75f, 0.0f, 0.0f, 90.0f, 0.5f, false, false, false);
		}
		else
			STREAMING::REQUEST_NAMED_PTFX_ASSET((LPSTR)fluids[2].c_str());

		WAIT(1);
	}
	ENTITY::SET_ENTITY_HEALTH(Vic, 1);
}
void PlayHorn(Vehicle Vic, int duration)
{
	VEHICLE::START_VEHICLE_HORN(Vic, duration, MyHashKey("HELDDOWN"), 0);
}
void FollowOn(PlayerBrain* brain, bool inVeh, bool PlayVeh)
{
	if (brain != NULL)
	{
		brain->WanBeFriends = false;
		brain->ApprochPlayer = false;
		brain->TimeOn += 600000;
		brain->Follower = true;
		brain->BlipColour = 38;
		if (brain->ThisBlip != NULL)
			UI::SET_BLIP_COLOUR(brain->ThisBlip, 38);

		if (inVeh)
		{
			brain->EnterVehQue = false;
			brain->FindPlayer = InGameTime() + 7000;
			RelGroupMember(brain->ThisPed, Gp_Follow);
			PlayerEnterVeh(brain->ThisVeh);
		}
		else
		{
			if (PlayVeh)
			{
				brain->EnterVehQue = true;
				PedDoGetIn(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), brain->ThisPed, brain->MyIdentity);
				RelGroupMember(brain->ThisPed, Gp_Follow);
			}
			else
			{
				brain->EnterVehQue = false;
				FolllowTheLeader(brain->ThisPed);
			}
		}
	}
	YouFriend = PZclass::JoinMe("Empty");
}
void WillYouBeMyFriend()
{
	if (!(bool)ENTITY::DOES_ENTITY_EXIST(YouFriend.MyBrain->ThisPed))
		ClearYourFriend(true);
	else if ((bool)ENTITY::IS_ENTITY_DEAD(YouFriend.MyBrain->ThisPed))
		ClearYourFriend(true);
	else if (YouFriend.MyBrain->Driver)
	{
		if (!(bool)ENTITY::DOES_ENTITY_EXIST(YouFriend.MyBrain->ThisVeh))
			ClearYourFriend(true);
		else if ((bool)ENTITY::IS_ENTITY_DEAD(YouFriend.MyBrain->ThisVeh))
			ClearYourFriend(true);
		else if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && (bool)PED::IS_PED_IN_ANY_VEHICLE(YouFriend.MyBrain->ThisPed, 0) && DistanceTo(PlayerPosi(), ENTITY::GET_ENTITY_COORDS(YouFriend.MyBrain->ThisPed, true)) < 35.0f)
		{
			if (YouFriend.Horny)
			{
				YouFriend.Horny = false;
				PlayHorn(YouFriend.MyBrain->ThisVeh, RandomInt(3000, 8000));
			}
			TopLeft(PZLang[91] + ControlSym[23] + PZLang[93] + ". " + PZLang[91] + ControlSym[46] + PZLang[94]);
			AddMarker(YouFriend.MyBrain->ThisVeh);
			if (ButtonDown(23, true))
			{
				if (YouFriend.Planes)
				{
					PlayerEnterVeh(YouFriend.MyBrain->ThisVeh);
					YouFriend.MyBrain->PlaneLand = 5;
					ClearYourFriend(false);
				}
				else if (MySettings.Aggression < 9)
					FollowOn(YouFriend.MyBrain, true, false);
				else
				{
					FightPlayer(YouFriend.MyBrain);
					ClearYourFriend(false);
				}
			}
			else if (ButtonDown(46, false))
			{
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(YouFriend.MyBrain->ThisPed, false);	
				ClearYourFriend(false);
			}
		}
		else
		{
			if (YouFriend.MyBrain->PlaneLand == 5)
				YouFriend.MyBrain->PlaneLand = 7;
			ClearYourFriend(true);
		}
	}
	else
	{
		if ((bool)PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			if (DistanceTo(PlayerPosi(), ENTITY::GET_ENTITY_COORDS(YouFriend.MyBrain->ThisPed, true)) < 15.0f)
			{
				TopLeft(PZLang[91] + ControlSym[86] + PZLang[95]);
				AddMarker(YouFriend.MyBrain->ThisPed);
				if (ButtonDown(86, false))
				{
					if (MySettings.Aggression < 9)
						FollowOn(YouFriend.MyBrain, false, true);
					else
					{
						FightPlayer(YouFriend.MyBrain);
						ClearYourFriend(false);
					}
				}
			}
			else if (DistanceTo(PlayerPosi(), ENTITY::GET_ENTITY_COORDS(YouFriend.MyBrain->ThisPed, true)) > 25.0f)
				ClearYourFriend(true);
		}
		else
		{
			if (DistanceTo(PlayerPosi(), ENTITY::GET_ENTITY_COORDS(YouFriend.MyBrain->ThisPed, true)) < 10.0f)
			{
				TopLeft(PZLang[91] + ControlSym[47] + PZLang[96] + ". " + PZLang[91] + ControlSym[46] + PZLang[94]);
				AddMarker(YouFriend.MyBrain->ThisPed);
				if (ButtonDown(47, true))
				{
					if (MySettings.Aggression < 9)
						FollowOn(YouFriend.MyBrain, false, false);
					else
					{
						FightPlayer(YouFriend.MyBrain);
						ClearYourFriend(false);
					}
				}
				else if (ButtonDown(46, false))
					ClearYourFriend(false);
			}
			else if (DistanceTo(PlayerPosi(), ENTITY::GET_ENTITY_COORDS(YouFriend.MyBrain->ThisPed, true)) > 25.0f)
				ClearYourFriend(true);
		}
	}
}
void PlayersKiller()
{
	int iKiller = -1;
	Ped WhoShot = PED::_GET_PED_KILLER(PLAYER::PLAYER_PED_ID());
	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (PedList[i].ThisPed == WhoShot)
		{
			PedList[i].Kills += 1;
			iKiller = i;
			break;
		}
	}

	while ((bool)ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()))
		WAIT(100);

	if (iKiller != -1)
	{
		BottomLeft(PZLang[97] + std::to_string(PedList[iKiller].Kills) + " - " + std::to_string(PedList[iKiller].Killed) + " " + PedList[iKiller].MyName);

		if (MySettings.Aggression < 2)
		{
			PED::REMOVE_PED_FROM_GROUP(PedList[iKiller].ThisPed);
			GetOutVehicle(PedList[iKiller].ThisPed);
			PedList[iKiller].TimeOn = 0;
		}
	}

	ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
	ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 120, false);
	iPasiveMode = InGameTime() + 15000;

	PlayDead = false;
}
int BagTimes = 0;
void ThemMoneyBAgs()
{
	if ((int)MoneyDrops.size() > 0)
	{
		if (BagTimes < InGameTime())
		{
			BagTimes = InGameTime() + 500;
			for (int i = 0; i < (int)MoneyDrops.size(); i++)
			{
				if (MoneyDrops[i].BagUp)
				{
					if (MoneyDrops[i].TimeOn < InGameTime())
					{
						if ((int)MoneyDrops[i].TheseBags.size() > 0)
						{
							for (int ii = 0; ii < (int)MoneyDrops[i].TheseBags.size(); ii++)
							{
								if ((bool)ENTITY::DOES_ENTITY_EXIST(MoneyDrops[i].TheseBags[ii]))
									ENTITY::DELETE_ENTITY(&MoneyDrops[i].TheseBags[ii]);

							}
						}
						int iJoin = ReteaveBrain(MoneyDrops[i].PedId);
						if (iJoin != -1)
						{
							PedList[iJoin].Friendly = true;
							PedList[iJoin].BlipColour = 0;

							PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PedList[iJoin].ThisPed, false);
							UI::SET_BLIP_COLOUR(PedList[iJoin].ThisBlip, 0);
							PED::REMOVE_PED_FROM_GROUP(PedList[iJoin].ThisPed);
							PED::SET_PED_RELATIONSHIP_GROUP_HASH(PedList[iJoin].ThisPed, Gp_Friend);
							AI::CLEAR_PED_TASKS_IMMEDIATELY(PedList[iJoin].ThisPed);
						}
						MoneyDrops.erase(MoneyDrops.begin() + i);
					}
				}
				else
				{
					if (MoneyDrops[i].TimeOn < InGameTime())
					{
						MoneyDrops[i].BagUp = true;
						MoneyDrops[i].TimeOn = InGameTime() + 30000;
					}
					else
					{
						Vector4 AboutHere = InAreaOf(ENTITY::GET_ENTITY_COORDS(MoneyDrops[i].ThisPed, true), 0.01f, 4.0f);
						Prop MoreBag = BuildProps(MoneyDrops[i].Bags, NewVector3(AboutHere.X, AboutHere.Y, AboutHere.Z + 5.0f), NewVector3(0.0, 0.0, 0.0), true, false);
						MoneyDrops[i].TheseBags.push_back(MoreBag);

						if (MoneyDrops[i].CollectBag)
						{
							int iJoin = ReteaveBrain(MoneyDrops[i].PedId);
							if (iJoin != -1)
							{
								PedList[iJoin].MoneyBags = true;
								RunHere(MoneyDrops[i].ThisPed, ENTITY::GET_ENTITY_COORDS(MoneyDrops[i].TheseBags[0], false));
								PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(MoneyDrops[i].ThisPed, true);
							}
							MoneyDrops[i].CollectBag = false;
						}
					}
				}

			}
		}
		else
		{
			for (int i = 0; i < (int)MoneyDrops.size(); i++)
			{
				if ((bool)ENTITY::IS_ENTITY_DEAD(MoneyDrops[i].ThisPed))
				{
					if ((int)MoneyDrops[i].TheseBags.size() > 0)
					{
						for (int ii = 0; ii < (int)MoneyDrops[i].TheseBags.size(); ii++)
						{
							if ((bool)ENTITY::DOES_ENTITY_EXIST(MoneyDrops[i].TheseBags[ii]))
								ENTITY::DELETE_ENTITY(&MoneyDrops[i].TheseBags[ii]);

						}
					}
					MoneyDrops.erase(MoneyDrops.begin() + i);
				}
				else if (MoneyDrops[i].TheseBags.size() > 0)
				{
					if (DistanceTo(ENTITY::GET_ENTITY_COORDS(MoneyDrops[i].ThisPed, true), ENTITY::GET_ENTITY_COORDS(MoneyDrops[i].TheseBags[0], true)) < 0.5)
					{
						ENTITY::DELETE_ENTITY(&MoneyDrops[i].TheseBags[0]);
						MoneyDrops[i].TheseBags.erase(MoneyDrops[i].TheseBags.begin());
					}
					else
					{
						RunHere(MoneyDrops[i].ThisPed, ENTITY::GET_ENTITY_COORDS(MoneyDrops[i].TheseBags[0], false));
						PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(MoneyDrops[i].ThisPed, true);
					}
				}
			}
		}
	}
	else
		DropMoneys = false;
}
Prop WindMill;
void EclipsWindMill()
{
	LoggerLight("AddEclipsWindMill");
	if (WindMill == NULL)
		WindMill = BuildProps("prop_windmill_01", NewVector3(-832.50, 290.95, 83.00), NewVector3(-90.00, 94.72, 0.00), false, false);
	else
	{
		ENTITY::DELETE_ENTITY(&WindMill);
		WindMill = NULL;
	}
}
bool KeepFrieands = true;

void ProcessAfk(PZclass::AfkPlayer* brain)
{
	if (ChatSHat < InGameTime())
	{
		ChatSHat = InGameTime() + RandomInt(2000, 12000);
		if (RandomInt(1, 50) < 15 && ShitTalk.size() < 5)
			RandomChat(brain->MyName, 2, brain->Nationality);
	}
}
void ProcessPZ(PZclass::PlayerBrain* brain)
{
	if (brain->ThisPed == NULL)
	{

	}
	else
	{
		if (brain->YoDeeeed)
		{
			if (brain->DeathSequence == 1 || brain->DeathSequence == 3 || brain->DeathSequence == 5 || brain->DeathSequence == 7)
			{
				if (brain->DeathTime < InGameTime())
				{
					ENTITY::SET_ENTITY_ALPHA(brain->ThisPed, 80, false);
					brain->DeathSequence++;
					brain->DeathTime = InGameTime() + 500;
				}
			}
			else if (brain->DeathSequence == 2 || brain->DeathSequence == 4 || brain->DeathSequence == 6)
			{
				if (brain->DeathTime < InGameTime())
				{
					ENTITY::SET_ENTITY_ALPHA(brain->ThisPed, 255, false);
					brain->DeathSequence++;
					brain->DeathTime = InGameTime() + 500;
				}
			}
			else if (brain->DeathSequence == 8)
			{
				if (brain->DeathTime < InGameTime())
				{
					LoggerLight("-BringoutDead-");
					if (MySettings.BackChat)
					{
						if (ChatSHat < InGameTime())
						{
							ChatSHat = InGameTime() + RandomInt(500, 12000);
							if (RandomInt(1, 50) < 35 && ShitTalk.size() < 5)
							{
								RandomChat(brain->MyName, 5, brain->Nationality);
							}
						}
					}
					brain->DeathSequence = 0;
					brain->FindPlayer = 0;
					brain->DeathTime = 0;
					ENTITY::DELETE_ENTITY(&brain->ThisPed);
					brain->ThisPed = NULL;
					brain->YoDeeeed = false;

					if (brain->Killed > RandomInt(8, 22) || MySettings.Aggression < 2)
						brain->TimeOn = 0;

					PlayerPedGen(FindPedSpPoint(InAreaOf(PlayerPosi(), 35.0f, 50.0f)), brain);
				}
			}
		}
		else
		{
			int GameTime = InGameTime();
			Ped ThePlayer = PLAYER::PLAYER_PED_ID();
			Ped PlayZero = brain->ThisPed;
			Vector3 PlayerPos = PlayerPosi();
			Vector3 PedPos = ENTITY::GET_ENTITY_COORDS(PlayZero, false);

			if ((bool)ENTITY::IS_ENTITY_DEAD(PlayZero))
			{
				ResetPlayer(brain, false);

				int iDie = WhoShotMe(PlayZero);

				if (iDie == -10)
				{
					if (brain->Bounty)
						AddMonies(7000);

					brain->Friendly = false;
					brain->BlipColour = 1;
					brain->ApprochPlayer = false;
					brain->Follower = false;
					brain->Killed += 1;

					BottomLeft(PZLang[97] + std::to_string(brain->Killed) + " - " + std::to_string(brain->Kills) + " " + brain->MyName);

				}
				else if (iDie != -1)
					BottomLeft(PedList[iDie].MyName + PZLang[98] + brain->MyName);
				else
				{
					if (brain->EWO)
					{
						brain->EWO = false;
						BottomLeft(brain->MyName + " " + PZLang[108]);
					}
					else
						BottomLeft(brain->MyName + " " + PZLang[81]);
				}

				brain->DeathSequence = 1;
				brain->DeathTime = GameTime + 10000;
				brain->TimeOn += 60000;
				brain->ThisEnemy = NULL;
				brain->Bounty = false;
				brain->MoneyBags = false;
				brain->InCombat = false;
				brain->SessionJumper = false;
				brain->EnterVehQue = false;
				brain->Driver = false;
				brain->Passenger = false;
				brain->Befallen = false;
				brain->OffRadarBool = false;
				brain->IsPlane = false;
				brain->IsHeli = false;
				brain->GrabVeh = false;

				if (brain->Friendly || brain->Follower && iDie != -10)
				{
					brain->ApprochPlayer = true;
					brain->WanBeFriends = false;
				}

				PED::REMOVE_PED_FROM_GROUP(PlayZero);

				brain->YoDeeeed = true;
			}
			else
			{
				BlipingBlip(brain);

				if (brain->TimeOn < GameTime)
				{
					GetOutVehicle(PlayZero);
					PedCleaning(brain, PZLang[82], false);
				}
				else if (PedPos.z + 10 < GroundHight(PedPos))
				{
					if (brain->Befallen)
					{
						if (brain->DeathTime < GameTime)
							ENTITY::SET_ENTITY_HEALTH(PlayZero, -1);
					}
					else
					{
						brain->Befallen = true;
						brain->DeathTime = GameTime + 5000;
					}
				}
				else if (brain->Befallen)
					brain->Befallen = false;
				else if (brain->SessionJumper)
				{
					if (DistanceTo(PedPos, PlayerPos) < 10.00f)
					{
						PED::REMOVE_PED_FROM_GROUP(PlayZero);
						PedCleaning(brain, PZLang[83], true);
						brain->TimeToGo = true;
					}
				}
				else if (brain->MoneyBags)
				{

				}
				else if (brain->WanBeFriends)
				{
					if ((bool)ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(PlayZero, ThePlayer, 1) || (bool)PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(ThePlayer, PlayZero))
					{
						brain->WanBeFriends = false;

						if (YouFriend.MyIdentity != "Empty")
						{
							if (YouFriend.MyIdentity == brain->MyIdentity)
								ClearYourFriend(false);
						}
						if (MySettings.Aggression > 1)
							FightPlayer(brain);
						else
							PED::SET_PED_FLEE_ATTRIBUTES(PlayZero, 32, 1);
					}
					else
					{
						if (brain->FindPlayer < GameTime)
						{
							if (brain->IsPlane)
								brain->ApprochPlayer = false;
							else if (brain->IsHeli)
								LandNearHeli(PlayZero, brain->ThisVeh, PlayerPos);
							else if (brain->Driver)
								DriveToooDest(PlayZero, brain->ThisVeh, PlayerPosi(), 35.0f, true, false, false);
							else
								WalkHere(PlayZero, FindingShops(PlayZero));

							brain->FindPlayer = GameTime + 5000;
						}
					}
				}
				else if (brain->AirTranspport)
				{
					if (brain->PlaneLand == 10)
					{
						if ((bool)PED::IS_PED_IN_VEHICLE(PlayZero, brain->ThisVeh, false))
						{
							if ((int)landPlane.size() < 1)
								landPlane = BuildFlightPath(PlayerPos);
							ENTITY::FREEZE_ENTITY_POSITION(brain->ThisVeh, false);
							PED::SET_PED_CAN_BE_DRAGGED_OUT(PlayZero, false);
							AI::SET_DRIVE_TASK_DRIVING_STYLE(PlayZero, 4194304);// 16777216
							brain->FlightPath = 0;
							FlyPlane(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], NULL);
							brain->PlaneLand--;
						}
					}
					else if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0) || brain->ThisVeh == NULL)
					{
						brain->Driver = false;
						brain->TimeOn = 0;
					}
					else if (brain->PlaneLand == 9)
					{
						if (DistanceTo(landPlane[brain->FlightPath], PedPos) < 150.0f)
						{
							LandNearPlane(PlayZero, brain->ThisVeh, landPlane[1], landPlane[2]);
							brain->FlightPath++;
							brain->FlightPath++;
							brain->PlaneLand--;
						}
						else
							FlyPlane(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], NULL);
					}
					else if (brain->PlaneLand == 8)
					{
						if (DistanceTo(landPlane[2], PedPos) < 10.0f)
						{
							ENTITY::FREEZE_ENTITY_POSITION(brain->ThisVeh, true);
							brain->PlaneLand--;
						}
					}
					else if (brain->PlaneLand == 7)
					{
						if (DistanceTo(PlayerPos, PedPos) < 20.0f)
						{
							LoggerLight("TakeOff...7");
							StayOnGround(brain->ThisVeh);
							ClearYourFriend(true);

							YouFriend.MyBrain = brain;
							YouFriend.MyIdentity = brain->MyIdentity;
							YouFriend.Planes = true;
							YouFriend.Horny = false;
							brain->TimeOn = InGameTime() + 1200000;
							brain->PlaneLand--;
						}
					}
					else if (brain->PlaneLand == 6)
					{

					}
					else if (brain->PlaneLand == 5)
					{
						if ((bool)PED::IS_PED_IN_VEHICLE(ThePlayer, brain->ThisVeh, false))
						{
							LoggerLight("TakeOff...5");
							ENTITY::FREEZE_ENTITY_POSITION(brain->ThisVeh, false);
							brain->FlightPath++;
							DriveToooDest(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], 12.0f, false, true, true);
							brain->PlaneLand--;
						}
					}
					else if (brain->PlaneLand == 4)
					{
						if (DistanceTo(landPlane[brain->FlightPath], PedPos) < 5.0f)
						{
							LoggerLight("TakeOff...4");
							brain->FlightPath++;
							DriveToooDest(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], 12.0f, true, true, true);
							brain->PlaneLand--;
						}
					}
					else if (brain->PlaneLand == 3)
					{
						if (DistanceTo(landPlane[brain->FlightPath], PedPos) < 5.0f)
						{
							LoggerLight("TakeOff...3");
							brain->FlightPath++;
							FlyPlane(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], NULL);
							brain->PlaneLand--;
						}
					}
					else if (brain->PlaneLand == 2)
					{
						if (DistanceTo(landPlane[brain->FlightPath], PedPos) < 150)
						{
							LoggerLight("TakeOff...2");
							if (LandingGear(brain->ThisVeh) == 0)
								VEHICLE::_SET_VEHICLE_LANDING_GEAR(brain->ThisVeh, 1);
							brain->FlightPath++;
							if (brain->FlightPath < (int)landPlane.size())
								FlyPlane(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], NULL);
							else
							{
								brain->FlightPath = 0;
								landPlane = BuildFlightPath(FlyMeToo);
								FlyPlane(PlayZero, brain->ThisVeh, landPlane[0], NULL);
								brain->PlaneLand = 9;
							}
						}
						else
							FlyPlane(PlayZero, brain->ThisVeh, landPlane[brain->FlightPath], NULL);
					}
				}
				else if (brain->RentaCop)
				{
					if (brain->ThisVeh == NULL || ENTITY::IS_ENTITY_DEAD(brain->ThisVeh))
					{
						EasyWayOut(PlayZero);
						brain->Driver = false;
						brain->TimeOn = 0;
					}
					else
					{
						
						if (brain->ThisEnemy == ThePlayer)
						{
							if ((bool)PED::IS_PED_IN_ANY_VEHICLE(ThePlayer, 0))
							{
								Vehicle PlayVeh = PED::GET_VEHICLE_PED_IS_IN(ThePlayer, false);
								if (VEHICLE::IS_VEHICLE_STOPPED(PlayVeh))
								{
									if (DistanceTo(PlayZero, ThePlayer) < 30.0f)
									{
										AI::CLEAR_PED_TASKS(PlayZero);
										GetOutVehicle(PlayZero);
										AI::TASK_ARREST_PED(PlayZero, ThePlayer);
									}
								}
							}
							else
							{
								brain->ThisEnemy = NULL;
								AI::CLEAR_PED_TASKS(PlayZero);
							}
						}
						else if (brain->ThisEnemy != NULL)
						{
							if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
								brain->ThisEnemy = NULL;
							else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
								brain->ThisEnemy = NULL;
						}
						else
						{
							if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0))
							{
								if (brain->FindPlayer < GameTime)
								{
									brain->FindPlayer = GameTime + 5000;
									GetInVehicle(PlayZero, brain->ThisVeh, -1, true);
								}
							}
							else if (brain->FindPlayer < GameTime)
							{
								brain->FindPlayer = GameTime + 5000;
								if ((bool)PED::IS_PED_IN_ANY_VEHICLE(ThePlayer, 0))
								{
									brain->ThisEnemy = ThePlayer;
									AI::TASK_VEHICLE_CHASE(PlayZero, ThePlayer);
									VEHICLE::SET_VEHICLE_SIREN(brain->ThisVeh, true);
								}
								else
								{
									brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, false, false);
									if (brain->ThisEnemy != NULL)
									{
										VEHICLE::SET_VEHICLE_SIREN(brain->ThisVeh, true);
										PickFight(PlayZero, brain->ThisVeh, brain->ThisEnemy, brain->PrefredVehicle);
									}
									else
										VEHICLE::SET_VEHICLE_SIREN(brain->ThisVeh, false);
								}
							}
						}
					}
				}
				else if (brain->Follower)
				{
					if (brain->SessionGreating && MySettings.BackChat)
					{
						if (RandomInt(1, 10) < 5)
						{
							ChatSHat = GameTime + RandomInt(500, 12000);
							brain->SessionGreating = false;
							RandomChat(brain->MyName, 1, brain->Nationality);
						}
					}

					if (brain->ApprochPlayer)
					{
						if (brain->Driver)
						{
							if (brain->ThisVeh == NULL)
							{
								brain->ApprochPlayer = false;
								brain->Driver = false;
							}
							else if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0))
							{
								if ((bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisVeh))
								{
									ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&brain->ThisVeh);
									brain->ThisVeh = NULL;
								}
							}
							else
							{
								if (DistanceTo(PedPos, PlayerPos) < 30.0f)
								{
									YouFriend.MyBrain = brain;
									YouFriend.MyIdentity = brain->MyIdentity;

									if (!brain->IsHeli)
										AI::CLEAR_PED_TASKS(PlayZero);

									brain->WanBeFriends = true;
									brain->ApprochPlayer = false;
								}
								else if (brain->FindPlayer < GameTime)
								{
									if (brain->IsPlane)
										brain->ApprochPlayer = false;
									else if (brain->IsHeli)
									{
										Vector3 VHere = ENTITY::GET_ENTITY_FORWARD_VECTOR(ThePlayer);
										VHere.x = PlayerPos.x + VHere.x * 5;
										VHere.y = PlayerPos.y + VHere.y * 5;
										VHere.z = PlayerPos.z + VHere.z * 5;
										LandNearHeli(PlayZero, brain->ThisVeh, VHere);
									}
									else
										DriveToooDest(PlayZero, brain->ThisVeh, PlayerPosi(), 35.0f, true, false, false);

									brain->FindPlayer = GameTime + 5000;
								}
							}
						}
						else
						{
							brain->ApprochPlayer = false;
							brain->Passenger = false;
						}
					}
					else if ((bool)PED::IS_PED_IN_ANY_VEHICLE(ThePlayer, 0))
					{
						if (brain->Driver)
						{
							if (brain->FindPlayer < GameTime)
							{
								brain->FindPlayer = GameTime + 5000;
								if ((bool)UI::IS_WAYPOINT_ACTIVE())
								{
									if (MyWayPoint().x != LetsGoHere.x)
									{
										LetsGoHere = MyWayPoint();

										if (brain->IsPlane)
										{

										}
										else if (brain->IsHeli)
										{
											Vector3 vecHigh = NewVector3(LetsGoHere.x, LetsGoHere.y, LetsGoHere.z + 50.0f);
											if (DistanceTo(LetsGoHere, PedPos) < 75)
											{
												LandNearHeli(PlayZero, brain->ThisVeh, LetsGoHere);
												brain->FindPlayer = GameTime + 120000;
											}
											else
												FlyHeli(PlayZero, brain->ThisVeh, vecHigh, 45, 0);
										}
										else
											DriveToooDest(PlayZero, brain->ThisVeh, LetsGoHere, 35.0f, true, false, false);
									}
								}
								else
								{
									if (brain->ThisEnemy != NULL)
									{
										if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
											brain->ThisEnemy = NULL;
										else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
											brain->ThisEnemy = NULL;
									}
									else
									{
										brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, brain->IsPlane, brain->IsHeli);
										if (brain->ThisEnemy != NULL)
											PickFight(PlayZero, brain->ThisVeh, brain->ThisEnemy, brain->PrefredVehicle);
									}
								}
							}
						}
						else if (brain->Passenger)
						{

						}
						else
						{
							brain->Passenger = true;
							Vehicle Vick = PED::GET_VEHICLE_PED_IS_IN(ThePlayer, false);
							PedDoGetIn(Vick, PlayZero, brain->MyIdentity);
							brain->TimeOn += 60000;
						}
					}
					else
					{
						if (brain->FindPlayer < GameTime)
						{
							brain->FindPlayer = GameTime + 5000;
							if (brain->Driver && !VehicleEnter)
							{
								if (DistanceTo(PlayerPos, PedPos) < 25)
								{
									GetOutVehicle(PlayZero);

									PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PlayZero, false);
									brain->Driver = false;

									FolllowTheLeader(PlayZero);
								}
								else if (brain->ThisVeh != NULL)
								{
									if (brain->FindPlayer < GameTime)
									{
										if (brain->IsPlane)
											brain->ApprochPlayer = false;
										else if (brain->IsHeli)
											LandNearHeli(PlayZero, brain->ThisVeh, PlayerPos);
										else
											DriveToooDest(PlayZero, brain->ThisVeh, PlayerPosi(), 45.0f, true, false, false);

										brain->FindPlayer = GameTime + 5000;
									}
								}
								else
								{
									brain->Driver = false;
								}
							}
							else if (brain->Passenger)
							{
								GetOutVehicle(PlayZero);

								PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PlayZero, false);
								brain->Passenger = false;
								FolllowTheLeader(PlayZero);
							}
							else if (DistanceTo(PlayerPos, PedPos) > 150.00 && !PED::IS_PED_FALLING(ThePlayer) && !PED::IS_PED_IN_PARACHUTE_FREE_FALL(ThePlayer) && PED::GET_PED_PARACHUTE_STATE(ThePlayer) < 1)
							{
								MoveEntity(PlayZero, PlayerPos);
								FolllowTheLeader(PlayZero);
							}
						}
					}
				}
				else if (brain->Friendly)
				{
					if (MySettings.BackChat)
					{
						if (ChatSHat < GameTime)
						{
							ChatSHat = GameTime + RandomInt(500, 12000);
							if (RandomInt(1, 50) < 15 && ShitTalk.size() < 5)
								RandomChat(brain->MyName, 2, brain->Nationality);
						}
					}

					if ((bool)ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(PlayZero, ThePlayer, 1) || (bool)PED::IS_PED_IN_COMBAT(PlayZero, ThePlayer) || (bool)PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(ThePlayer, PlayZero))
					{
						brain->ApprochPlayer = false;
						brain->WanBeFriends = false;
						PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PlayZero, false);
						if (brain->Follower)
						{
							PED::REMOVE_PED_FROM_GROUP(PlayZero);
							brain->Follower = false;
						}

						if (MySettings.Aggression > 1)
							FightPlayer(brain);
						else
							PED::SET_PED_FLEE_ATTRIBUTES(PlayZero, 32, 1);
					}
					else
					{
						if (brain->Passenger)
						{
							if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0))
							{
								brain->ThisVeh = NULL;
								brain->Passenger = false;
							}
						}
						else if (brain->Driver)
						{
							if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0) || brain->ThisVeh == NULL)
								brain->Driver = false;
							else if (!brain->IsPlane && brain->ApprochPlayer && YouFriend.MyIdentity == "Empty" && DistanceTo(PlayerPos, PedPos) < 50.0f)
							{
								YouFriend.MyBrain = brain;
								YouFriend.MyIdentity = brain->MyIdentity;

								if (brain->IsHeli)
									YouFriend.Horny = false;

								brain->WanBeFriends = true;
								brain->ApprochPlayer = false;
							}
							else
							{
								if (MySettings.Aggression < 2)
								{
									if (brain->FindPlayer < GameTime)
									{
										DriveAround(PlayZero);
										brain->FindPlayer = GameTime + 25000;
									}
								}
								else
								{
									if (brain->FindPlayer < GameTime)
									{
										brain->FindPlayer = GameTime + 5000;
										if (brain->ThisEnemy != NULL)
										{
											if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
												brain->ThisEnemy = NULL;
											else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
												brain->ThisEnemy = NULL;
										}
										else
										{
											brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, brain->IsPlane, brain->IsHeli);
											if (brain->ThisEnemy != NULL)
											{
												brain->EnemyPos = DistanceTo((Entity)brain->ThisEnemy, PedPos) - 1.0f;
												PickFight(PlayZero, brain->ThisVeh, brain->ThisEnemy, brain->PrefredVehicle);
												FightTogether(brain->ThisVeh, brain->ThisEnemy);
											}
										}
									}
								}
							}
						}
						else
						{
							if (DistanceTo(PedPos, PlayerPos) < 7.0f)
							{
								if (brain->ApprochPlayer && YouFriend.MyIdentity == "Empty")
								{
									if (!brain->WanBeFriends)
									{
										YouFriend.MyBrain = brain;
										YouFriend.MyIdentity = brain->MyIdentity;

										brain->WanBeFriends = true;
										brain->ApprochPlayer = false;
									}
								}
							}
							else if (brain->FindPlayer < GameTime)
							{
								brain->FindPlayer = GameTime + 1000;

								if (MySettings.Aggression > 2)
								{
									if (brain->ThisEnemy != NULL)
									{
										if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
											brain->ThisEnemy = NULL;
										else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
											brain->ThisEnemy = NULL;
										else if (DistanceTo((Entity)brain->ThisEnemy, PedPos) > brain->EnemyPos)
										{
											if (MySettings.Aggression > 5)
											{
												brain->EWO = true;
												EasyWayOut(PlayZero);
											}
											else
												brain->TimeOn = 0;
										}
									}
									else
									{
										brain->FindPlayer = GameTime + RandomInt(10000, 15000);
										brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, brain->IsPlane, brain->IsHeli);
										if (brain->ThisEnemy == NULL)
											WalkHere(PlayZero, FindingShops(PlayZero));
										else
										{
											brain->EnemyPos = DistanceTo((Entity)brain->ThisEnemy, PedPos) - 1.0f;
											GreefWar(PlayZero, brain->ThisEnemy);
										}
									}
								}
								else
								{
									brain->FindPlayer = GameTime + RandomInt(50000, 100000);
									WalkHere(PlayZero, FindingShops(PlayZero));
								}
							}
						}
					}
				}
				else
				{
					if (MySettings.BackChat)
					{
						if (ChatSHat < GameTime)
						{
							ChatSHat = GameTime + RandomInt(500, 12000);
							if (RandomInt(1, 50) < 15 && ShitTalk.size() < 5)
							{
								RandomChat(brain->MyName, 5, 0);
							}
						}
					}

					if (brain->Driver)
					{
						if (brain->ThisVeh == PED::GET_VEHICLE_PED_IS_IN(ThePlayer, false))
						{
							brain->Driver = false;
							GetOutVehicle(PlayZero);
						}
						else if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0))
							brain->Driver = false;
						else if (brain->FindPlayer < GameTime)
						{
							if (brain->ThisEnemy != NULL)
							{
								if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
									brain->ThisEnemy = NULL;
								else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
									brain->ThisEnemy = NULL;
							}
							else
							{
								if (brain->FindPlayer < GameTime)
								{
									brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, brain->IsPlane, brain->IsHeli);
									PickFight(PlayZero, brain->ThisVeh, brain->ThisEnemy, brain->PrefredVehicle);
									brain->FindPlayer = GameTime + 5000;
								}
							}
						}
					}
					else if (brain->Passenger)
					{
						if (!(bool)PED::IS_PED_IN_ANY_VEHICLE(PlayZero, 0))
							brain->Passenger = false;
					}
					else
					{
						if (brain->FindPlayer < GameTime)
						{
							brain->FindPlayer = GameTime + 1000;

							if (brain->ThisEnemy != NULL)
							{
								if (!(bool)ENTITY::DOES_ENTITY_EXIST(brain->ThisEnemy))
									brain->ThisEnemy = NULL;
								else if ((bool)ENTITY::IS_ENTITY_DEAD(brain->ThisEnemy))
									brain->ThisEnemy = NULL;
								else if (DistanceTo((Entity)brain->ThisEnemy, PedPos) > brain->EnemyPos)
								{
									if (MySettings.Aggression > 5)
									{
										brain->EWO = true;
										EasyWayOut(PlayZero);
									}
									else
										brain->TimeOn = 0;
								}
							}
							else
							{
								brain->FindPlayer = GameTime + RandomInt(10000, 15000);
								brain->ThisEnemy = FindAFight(PlayZero, brain->Friendly, brain->IsPlane, brain->IsHeli);
								if (brain->ThisEnemy == NULL)
									WalkHere(PlayZero, FindingShops(PlayZero));
								else
								{
									brain->EnemyPos = DistanceTo((Entity)brain->ThisEnemy, PedPos) - 1.0f;
									GreefWar(PlayZero, brain->ThisEnemy);
								}
							}
						}
						else if ((bool)PED::IS_PED_IN_COMBAT(ThePlayer, PlayZero) && brain->ThisEnemy != ThePlayer)
						{
							brain->ThisEnemy = ThePlayer;
							brain->EnemyPos = DistanceTo((Entity)brain->ThisEnemy, PedPos) + 10.0f;
							GreefWar(PlayZero, brain->ThisEnemy);
						}
					}
				}
			}
		}
	}
}
void PlayerZerosAI()
{
	if (PlayerZinSesh(true) > MySettings.MaxPlayers)
	{
		if ((int)AFKList.size() > 0)
			AFKList[0].TimeOn = 0;
		else if ((int)PedList.size() > 0)
			PedList[0].TimeOn = 0;
	}
	
	if ((int)PedList.size() > 0)
	{
		if (iNpcList < (int)PedList.size())
		{
			if (PedList[iNpcList].TimeToGo)
				PedList.erase(PedList.begin() + iNpcList);
			else
				ProcessPZ(&PedList[iNpcList]);
			iNpcList++;
		}
		else
			iNpcList = 0;
	}

	if ((int)AFKList.size() > 0)
	{
		if (iBlpList < (int)AFKList.size())
		{
			if (AFKList[iBlpList].TimeOn < InGameTime())
			{
				if ((bool)UI::DOES_BLIP_EXIST(AFKList[iBlpList].ThisBlip))
					UI::REMOVE_BLIP(&AFKList[iBlpList].ThisBlip);

				BottomLeft("~h~" + AFKList[iBlpList].MyName + "~s~ left");
				AFKList.erase(AFKList.begin() + iBlpList);
			}
			else if (MySettings.BackChat)
				ProcessAfk(&AFKList[iBlpList]);
			iBlpList++;
		}
		else
			iBlpList = 0;
	}

	if (!BypassAll)
	{
		if (!MySettings.InviteOnly)
		{
			if (iNextPlayer < InGameTime() && PlayerZinSesh(true) < MySettings.MaxPlayers)
				NewPlayer();
		}
	}
}
int Scaleform_PLAYER_LIST()
{
	LoggerLight("Scaleform_PLAYER_LIST");

	int iList = GRAPHICS::REQUEST_SCALEFORM_MOVIE("INSTRUCTIONAL_BUTTONS");

	while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(iList))
		WAIT(1);

	GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(iList, "CLEAR_ALL");//Was---_CALL_SCALEFORM_MOVIE_FUNCTION_VOID
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iList, "TOGGLE_MOUSE_BUTTONS");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(0);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

	GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(iList, "CREATE_CONTAINER");//Was---_CALL_SCALEFORM_MOVIE_FUNCTION_VOID
	int iAddOns = 0;

	for (int i = 0; i < PedList.size(); i++)
	{
		int iFailed = 0;
		string sPlayer = PedList[i].MyName;
		while (sPlayer.size() < 14 && iFailed < 10)
		{
			sPlayer = sPlayer + " ";
			WAIT(1);
			iFailed += 1;
		}
		sPlayer = sPlayer + std::to_string(PedList[i].Level);

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iList, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(iAddOns);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((LPSTR)sPlayer.c_str());
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		iAddOns += 1;
	}
	for (int i = 0; i < AFKList.size(); i++)
	{
		int iFailed = 0;
		string sPlayer = AFKList[i].MyName;
		while (sPlayer.size() < 14 && iFailed < 10)
		{
			sPlayer = sPlayer + " ";
			WAIT(1);
			iFailed += 1;
		}
		sPlayer = sPlayer + std::to_string(AFKList[i].Level);


		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iList, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(iAddOns);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((LPSTR)sPlayer.c_str());
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		iAddOns += 1;
	}

	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iList, "SET_DATA_SLOT");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(iAddOns);

	if (MySettings.InviteOnly)
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Mod Disabled");
	else
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Players in Session ;  ");

	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iList, "DRAW_INSTRUCTIONAL_BUTTONS");
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(1);
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

	iDisplay = InGameTime() + 8000;
	TopLeft(PZLang[91] + ControlSym[MySettings.Control_Open] + PZLang[92]);
}
void ScanArea()
{
	if (MakeFrenz.size() > 0)
	{
		Vector4 Frend = FindPedSpPoint(FindPedSpPoint(InAreaOf(PlayerPosi(), 35.0f, 50.0f)));

		PedList.push_back(MakeFrenz[MakeFrenz.size() - 1].Brains);
		PlayerPedGen(Frend, &PedList[PedList.size() - 1]);
		MakeFrenz.pop_back();
		LoggerLight("MakeFrenz Count == " + std::to_string((int)MakeFrenz.size()));
	}

	if (MakeCarz.size() > 0)
	{
		Vector4 Carz = FindVehSpPoint(FindPedSpPoint(InAreaOf(PlayerPosi(), 45.0f, 90.0f)));

		PedList.push_back(MakeCarz[MakeCarz.size() - 1].Brains);
		SpawnVehicle(MakeCarz[MakeCarz.size() - 1].Brains.PrefredVehicle, Carz, MakeCarz[MakeCarz.size() - 1].AddPlayer, &PedList[PedList.size() - 1], false, MakeCarz[MakeCarz.size() - 1].CanFill);
		MakeCarz.pop_back();
	}
}

void HaCK001(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				Vector3 LandHere = FowardOf(PLAYER::PLAYER_PED_ID(), 5, true);
				MoveEntity(PedList[iPedLocate].ThisPed, LandHere);
			}
		}
		else
		{
			Vector3 LandHere = FowardOf(PLAYER::PLAYER_PED_ID(), 5, true);
			ClothBank MB = NewClothBank();
			int iNat = RandomInt(1, 5);
			int iGetGunnin = YourGunNum();
			PlayerBrain newBrain = PlayerBrain(AFKList[iPedLocate].MyName, AFKList[iPedLocate].MyIdentity, MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), AFKList[iPedLocate].Level, false, iNat, iGetGunnin, 0);
			PedList.push_back(newBrain);
			PlayerPedGen(Vector4(LandHere.x, LandHere.y, LandHere.z, 0.0), &PedList[PedList.size() - 1]);
			AFKList.erase(AFKList.begin() + iPedLocate);
		}
	}
}
void HaCK002(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				FIRE::START_ENTITY_FIRE(PedList[iPedLocate].ThisPed);

				FightPlayer(&PedList[iPedLocate]);
			}
		}
		else
		{
			BottomLeft(AFKList[iPedLocate].MyName + " " + PZLang[81]);
			AFKList[iPedLocate].TimeOn = 0;
		}
	}
}
void HaCK003(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			PedList[iPedLocate].TimeOn = 0;
		}
		else
		{
			AFKList[iPedLocate].TimeOn = 0;
		}
	}
}
void HaCK004(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				Vector3 Above = ENTITY::GET_ENTITY_COORDS(PedList[iPedLocate].ThisPed, true);
				Above.z += 25.0;
				DropObjects(Above);
			}
		}
		else
		{

		}
	}
}
void HaCK005(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				FireOrb(PedList[iPedLocate].MyIdentity, PedList[iPedLocate].ThisPed, true);

				FightPlayer(&PedList[iPedLocate]);
			}
		}
		else
		{

		}
	}
}
void HaCK006(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);
	int iAmPig = BackOffPig();
	Ped ThePlayer = PLAYER::PLAYER_PED_ID();

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				if (iPedLocate == iAmPig)
				{
					ENTITY::DETACH_ENTITY(ThePlayer, 0, 0);
					AI::CLEAR_PED_TASKS_IMMEDIATELY(ThePlayer);
					PedList[iAmPig].PiggyBackin = false;
				}
				else
				{
					if (iAmPig != -1)
					{
						ENTITY::DETACH_ENTITY(ThePlayer, 0, 0);
						AI::CLEAR_PED_TASKS_IMMEDIATELY(ThePlayer);
						PedList[iAmPig].PiggyBackin = false;
					}

					ForceAnim(ThePlayer, "amb@code_human_in_bus_passenger_idles@female@sit@idle_a", "idle_a", PlayerPosi(), NewVector3(0.0, 0.0, 0.0));
					ENTITY::ATTACH_ENTITY_TO_ENTITY(ThePlayer, PedList[iPedLocate].ThisPed, 31086, 0.10, 0.15, 0.61, 0.00, 0.00, 180.00, 0, 0, 0, 0, 2, 1);
					PedList[iPedLocate].PiggyBackin = true;
				}
			}
		}
		else
		{
			if (iAmPig != -1)
			{
				ENTITY::DETACH_ENTITY(ThePlayer, 0, 0);
				AI::CLEAR_PED_TASKS_IMMEDIATELY(ThePlayer);
				PedList[iAmPig].PiggyBackin = false;
			}

			Vector3 LandHere = FowardOf(ThePlayer, 5, true);
			ClothBank MB = NewClothBank();
			int iGetGunnin = YourGunNum();
			PlayerBrain newBrain = PlayerBrain(AFKList[iPedLocate].MyName, AFKList[iPedLocate].MyIdentity, MB, InGameTime() + RandomInt(PZSetMinSession, PZSetMaxSession), AFKList[iPedLocate].Level, false, AFKList[iPedLocate].Nationality, iGetGunnin, 0);
			newBrain.PiggyBackin = true;
			PedList.push_back(newBrain);
			Ped Bob = PlayerPedGen(Vector4(LandHere.x, LandHere.y, LandHere.z, 0.0), &PedList[PedList.size() -1]);
			AFKList.erase(AFKList.begin() + iPedLocate);
			ForceAnim(ThePlayer, "amb@code_human_in_bus_passenger_idles@female@sit@idle_a", "idle_a", PlayerPosi(), NewVector3(0.0, 0.0, 0.0));
			ENTITY::ATTACH_ENTITY_TO_ENTITY(ThePlayer, Bob, 31086, 0.10, 0.15, 0.61, 0.00, 0.00, 180.00, 0, 0, 0, 0, 2, 1);
		}
	}
}
void HaCK007(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			DropMoneys = true;
			if (PedList[iPedLocate].ThisPed != NULL)
				MoneyDrops.push_back(MoneyBags(PedList[iPedLocate].ThisPed, PedList[iPedLocate].MyIdentity, InGameTime() + 5000));
		}
		else
		{

		}
	}
}
void HaCK008(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			PedList[iPedLocate].ThisBlip = PedBlimp(PedList[iPedLocate].ThisBlip, PedList[iPedLocate].ThisPed, 303, PedList[iPedLocate].MyName, 1, false);
			PedList[iPedLocate].Bounty = true;
		}
		else
		{
			AFKList[iPedLocate].ThisBlip = LocalBlip(AFKList[iPedLocate].ThisBlip, AFKPlayers[AFKList[iPedLocate].App], 303, AFKList[iPedLocate].MyName, 1);
		}
	}
}
void HaCK009(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisVeh != NULL && PedList[iPedLocate].ThisPed != NULL)
				GetOutVehicle(PedList[iPedLocate].ThisPed);
		}
		else
		{

		}
	}
}
void HaCK010(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			GetOutVehicle(PLAYER::PLAYER_PED_ID());

			if (PedList[iPedLocate].ThisVeh != NULL)
				GetInVehicle(PLAYER::PLAYER_PED_ID(), PedList[iPedLocate].ThisVeh, 0, true);
		}
		else
		{

		}
	}
}
void HaCK011(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].XmasTree != NULL)
			{
				ENTITY::DELETE_ENTITY(&PedList[iPedLocate].XmasTree);
				PedList[iPedLocate].XmasTree = NULL;
			}
			else if (PedList[iPedLocate].ThisPed != NULL)
			{
				Vector3 Vme = ENTITY::GET_ENTITY_COORDS(PedList[iPedLocate].ThisPed, 0);
				PedList[iPedLocate].XmasTree = BuildProps("prop_xmas_tree_int", Vme, NewVector3(0.0, 90.0, 0.0), false, false);
				ENTITY::SET_ENTITY_COLLISION(PedList[iPedLocate].XmasTree, false, false);
				ENTITY::ATTACH_ENTITY_TO_ENTITY(PedList[iPedLocate].XmasTree, PedList[iPedLocate].ThisPed, PED::GET_PED_BONE_INDEX(PedList[iPedLocate].ThisPed, 24818), 0.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0, 0, 0, 1, 2, 1);
			}
		}
		else
		{

		}
	}
}
void HaCK012(string ThisPed)
{
	bool bRain = true;
	int iPedLocate = ReteaveBrain(ThisPed);

	if (iPedLocate == -1)
	{
		bRain = false;
		iPedLocate = ReteaveAfk(ThisPed);
	}

	if (iPedLocate != -1)
	{
		if (bRain)
		{
			if (PedList[iPedLocate].ThisPed != NULL)
			{
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(PedList[iPedLocate].ThisPed, true);
				PedList[iPedLocate].EWO = true;
				EasyWayOut(PedList[iPedLocate].ThisPed);

				FightPlayer(&PedList[iPedLocate]);
			}
		}
		else
		{
			BottomLeft(AFKList[iPedLocate].MyName + PZLang[99]);
			AFKList[iPedLocate].TimeOn = 0;
		}
	}
}

bool NotWanted = false;
vector<PZclass::PhoneContact*> YourFriends = {};

void BurnPlayers()
{
	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (!PedList[i].Follower)
			HaCK002(PedList[i].MyIdentity);
		if (DistanceTo(PedList[i].ThisPed, PLAYER::PLAYER_PED_ID()) > 70.0)
			ENTITY::SET_ENTITY_HEALTH(PedList[i].ThisPed, 0);
	}

	for (int i = 0; i < (int)AFKList.size(); i++)
	{
		BottomLeft("~h~" + AFKList[i].MyName + "~s~ " + PZLang[81]);
		AFKList[i].TimeOn = 0;
	}
}
void CashFlow()
{
	for (int i = 0; i < (int)PedList.size(); i++)
		HaCK007(PedList[i].MyIdentity);
}
void GetingOffHere()
{
	for (int i = 0; i < (int)PedList.size(); i++)
	{
		if (PedList[i].ThisPed != NULL)
			GetOutVehicle(PedList[i].ThisPed);
	}
}
void HaCKThiSPeD(string ThisPed, string name, bool vHick, bool InVick, bool InApp)
{
	int MyPick = -1;
	if (InApp)
	{
		MyPick = MyMenuSys(PZLang[61], vector<string>{ PZLang[69], PZLang[70], PZLang[71], PZLang[72], PZLang[74] });
		if (MyPick == 0)
			HaCK001(ThisPed);//"Bring Player to Self",
		else if (MyPick == 1)
			HaCK002(ThisPed);//"Burn Player",
		else if (MyPick == 2)
			HaCK003(ThisPed);//"Crash Player",
		else if (MyPick == 3)
			HaCK008(ThisPed);//"Add Bounty to Player",
		else if (MyPick == 4)
			HaCK012(ThisPed);//"EWO",
	}
	else if (vHick)
	{
		MyPick = MyMenuSys(PZLang[61], vector<string>{ PZLang[69], PZLang[70], PZLang[71], PZLang[72], PZLang[73], PZLang[75], PZLang[76], PZLang[77], PZLang[74] });
		if (MyPick == 0)
			HaCK001(ThisPed);//"Bring Player to Self",
		else if (MyPick == 1)
			HaCK002(ThisPed);//"Burn Player",
		else if (MyPick == 2)
			HaCK003(ThisPed);//"Crash Player",
		else if (MyPick == 3)
			HaCK008(ThisPed);//"Add Bounty to Player",
		else if (MyPick == 4)
			HaCK004(ThisPed);//"Drop Objects On Player",
		else if (MyPick == 5)
			HaCK009(ThisPed);//"Eject Player Vehicle",
		else if (MyPick == 6)
			HaCK010(ThisPed);//"Enter Player Vehicle",
		else if (MyPick == 7)
			HaCK011(ThisPed);//"Add Christmas Tree",
		else if (MyPick == 8)
			HaCK012(ThisPed);//"EWO",
	}
	else if (InVick)
	{
		MyPick = MyMenuSys(PZLang[61], vector<string>{ PZLang[69], PZLang[70], PZLang[71], PZLang[72], PZLang[73], PZLang[75], PZLang[77], PZLang[74] });
		if (MyPick == 0)
			HaCK001(ThisPed);//"Bring Player to Self",
		else if (MyPick == 1)
			HaCK002(ThisPed);//"Burn Player",
		else if (MyPick == 2)
			HaCK003(ThisPed);//"Crash Player",
		else if (MyPick == 3)
			HaCK008(ThisPed);//"Add Bounty to Player",
		else if (MyPick == 4)
			HaCK004(ThisPed);//"Drop Objects On Player",
		else if (MyPick == 5)
			HaCK009(ThisPed);//"Eject Player Vehicle",
		else if (MyPick == 6)
			HaCK011(ThisPed);//"Add Christmas Tree",
		else if (MyPick == 7)
			HaCK012(ThisPed);//"EWO",
	}
	else
	{
		MyPick = MyMenuSys(PZLang[61], vector<string>{ PZLang[69], PZLang[70], PZLang[71], PZLang[72], PZLang[73], PZLang[78], PZLang[79], PZLang[80], PZLang[77], PZLang[74] });

		if (MyPick == 0)
			HaCK001(ThisPed);//"Bring Player to Self",
		else if (MyPick == 1)
			HaCK002(ThisPed);//"Burn Player",
		else if (MyPick == 2)
			HaCK003(ThisPed);//"Crash Player",
		else if (MyPick == 3)
			HaCK008(ThisPed);//"Add Bounty to Player",
		else if (MyPick == 4)
			HaCK004(ThisPed);//"Drop Objects On Player",
		else if (MyPick == 5)
			HaCK005(ThisPed);//"Orbital Strike Player",
		else if (MyPick == 6)
			HaCK006(ThisPed);//"PiggyBack Player",
		else if (MyPick == 7)
			HaCK007(ThisPed);//"Drop Moneys on Player",
		else if (MyPick == 8)
			HaCK011(ThisPed);//"Add Christmas Tree",
		else if (MyPick == 9)
			HaCK012(ThisPed);//"EWO",
	}
}
void TrollPlayMenu()
{
	iMenuSys = 0;
	vector<string> PlayerZName = {};
	vector<string> PlayerZPos = {};

	for (int i = 0; i < PedList.size(); i++)
	{
		string sAdd = "";
		if (PedList[i].Follower)
			sAdd = " ("+ PZLang[68] +")";

		PlayerZName.push_back(PedList[i].MyName + sAdd);
		PlayerZPos.push_back(PedList[i].MyIdentity);
	}

	for (int i = 0; i < AFKList.size(); i++)
	{
		PlayerZName.push_back(AFKList[i].MyName);
		PlayerZPos.push_back(AFKList[i].MyIdentity);
	}

	int MyPick = MyMenuSys(PZLang[61], PlayerZName);

	if (MyPick < (int)PlayerZPos.size() && MyPick > -1)
	{
		int iPedLocate = ReteaveBrain(PlayerZPos[MyPick]);
		if (iPedLocate != -1)
		{
			bool GotSeats = false;
			bool InVick = PedList[iPedLocate].Passenger;

			if (PedList[iPedLocate].ThisVeh != NULL)
			{
				if (VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(PedList[iPedLocate].ThisVeh) > 0)
					GotSeats = true;
				else
					InVick = true;
			}
			HaCKThiSPeD(PlayerZPos[MyPick], PlayerZName[MyPick], GotSeats, InVick, false);
		}
		else
			HaCKThiSPeD(PlayerZPos[MyPick], PlayerZName[MyPick], false, false, true);
	}
}
bool bSnow = false;
bool HideTrafic = false;

void TrollMenu()
{
	iMenuSys = 1;
	int MyPick = 0;

	string sWind = PZLang[57];
	if (WindMill != NULL)
		sWind = PZLang[58];

	string sWanted = PZLang[59];
	if (NotWanted)
		sWanted = PZLang[60];

	string SnowBlow = PZLang[106];
	if (ItsSnowing)
		SnowBlow = PZLang[107];

	MyPick = MyMenuSys(PZLang[61], vector<string>{ sWind, PZLang[62], PZLang[63], PZLang[64], PZLang[65], PZLang[66], sWanted, SnowBlow });

	if (MyPick == 0)
	{
		EclipsWindMill();
		iMenuSys = 2;
	}
	else if (MyPick == 1)
	{
		if (PlayerZinSesh(false) > 0)
			iMenuSys = 3;
		else
		{
			MyMenuSys(PZLang[61], vector<string>{ PZLang[67] });
			iMenuSys = 2;
		}
	}
	else if (MyPick == 2)
	{
		if (PlayerZinSesh(false) > 0)
			BurnPlayers();
		else
		{
			MyMenuSys(PZLang[61], vector<string>{ PZLang[67] });
			iMenuSys = 2;
		}
	}
	else if (MyPick == 3)
	{
		if (PlayerZinSesh(false) > 0)
			CashFlow();
		else
		{
			MyMenuSys(PZLang[61], vector<string>{ PZLang[67] });
			iMenuSys = 2;
		}
	}
	else if (MyPick == 4)
	{
		if (PlayerZinSesh(false) > 0)
			GetingOffHere();
		else
		{
			MyMenuSys(PZLang[61], vector<string>{ PZLang[67] });
			iMenuSys = 2;
		}
	}
	else if (MyPick == 5)
	{
		EasyWayOut(PLAYER::PLAYER_PED_ID());
		BottomLeft(PZLang[109]);
		iMenuSys = 0;
	}
	else if (MyPick == 6)
	{
		NotWanted = !NotWanted;
		iMenuSys = 2;
	}
	else if (MyPick == 7)
	{
		ItsSnowing = !ItsSnowing;
		EnableSnow(ItsSnowing);
		WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), MyHashKey("WEAPON_snowball"), 9999, false, true);
	}
	else if (MyPick == 8)
	{
		HideTrafic = !HideTrafic;
	}
}
const std::vector<std::string> TranlatesTo = {
	"English",
	"French",
	"German",
	"Italian",
	"Spanish",
	"Portuguese",
	"Polish",
	"Russian",
	"Korean",
	"Chinese",
	"Japanese",
	"Mexican",
	"ChineseSimplifyed"
};
void PlayerZeroMenu()
{
	iMenuSys = 0;

	vector<int> Output;

	string Invite = PZLang[2];
	if (MySettings.InviteOnly)
		Invite = PZLang[3];

	string Notice = PZLang[4];
	if (MySettings.NoNotify)
		Notice = PZLang[5];

	string Blipnig = PZLang[6];
	if (MySettings.NoRadar)
		Blipnig = PZLang[7];

	string Weaps = PZLang[8];
	if (MySettings.SpaceWeaps)
		Weaps = PZLang[9];

	string Tags = PZLang[10];
	if (!MySettings.NameTags)
		Tags = PZLang[11];

	string Pass = PZLang[12];
	if (!MySettings.PassiveMode)
		Pass = PZLang[13];

	string LFiend = PZLang[14];
	if (!MySettings.FriendlyFire)
		LFiend = PZLang[15];

	string TalkinBollox = PZLang[104];
	if (MySettings.BackChat)
		TalkinBollox = PZLang[105];

	string LLogs = PZLang[16];
	if (!MySettings.Debugger)
		LLogs = PZLang[17];

	if (PZData::MySettings.Pz_Lang == -1)
		PZData::MySettings.Pz_Lang = 0;

	int MyPick = MyMenuSys(PZLang[18], vector<string>{ PZLang[19], PZLang[20], PZLang[21], Invite, PZLang[22], PZLang[23], PZLang[24], PZLang[25], PZLang[26], PZLang[27], Notice, Blipnig, Weaps, Tags, Pass, LFiend, TalkinBollox, LLogs, PZLang[28], TranlatesTo[PZData::MySettings.Pz_Lang]});

	if (MyPick == 0)
		iMenuSys = 2;
	else if (MyPick == 1)
	{
		string PC = PZLang[29];
		if (!MySettings.MobileData)
			PC = PZLang[30];

		int Cont01 = MyMenuSys(PZLang[20], vector<string>{ PZLang[31], PZLang[32], PC });

		if (Cont01 == 0)
		{
			vector<string> Name_List = {};
			vector<string> Id_List = {};

			for (int i = 0; i < (int)PedList.size(); i++)
			{
				if (PedList[i].Follower && !PedList[i].IsInContacts)
				{
					Name_List.push_back(PedList[i].MyName);
					Id_List.push_back(PedList[i].MyIdentity);
				}
			}
			if ((int)Name_List.size() > 0)
			{
				int Cont02 = MyMenuSys(PZLang[20], Name_List);

				if (Cont02 > -1 && Cont02 < (int)Name_List.size())
				{
					int iNewCon = ReteaveBrain(Id_List[Cont02]);
					if (iNewCon != -1)
					{
						PedList[iNewCon].IsInContacts = true;
						SaveContacts(PhoneContact(Name_List[Cont02], PedList[iNewCon]));
					}
					iMenuSys = 1;
				}
			}
			else
			{
				int Cont02 = MyMenuSys(PZLang[20], vector<string>{ PZLang[33] });
				iMenuSys = 1;
			}
		}
		else if (Cont01 == 1)
		{
			vector<string> Name_List = {};
			vector<string> Id_List = {};

			for (int i = 0; i < (int)PhoneContacts.size(); i++)
			{
				if (!PlayerOnline(PhoneContacts[i].YourFriend.MyIdentity))
				{
					Name_List.push_back(PhoneContacts[i].YourFriend.MyName);
					Id_List.push_back(PhoneContacts[i].YourFriend.MyIdentity);
				}
			}

			if ((int)Name_List.size() > 0)
			{
				int Cont02 = MyMenuSys(PZLang[20], Name_List);

				if (Cont02 > -1 && Cont02 < (int)Id_List.size())
				{
					int Cont03 = MyMenuSys(Name_List[Cont02], vector<string>{PZLang[34], PZLang[35]});
					if (Cont03 == 0)
					{
						int Id = RetreaveCont(Id_List[Cont02]);
						if (Id != -1 && Id < PhoneContacts.size())
							ContactInSession(&PhoneContacts[Id]);
					}
					else if (Cont03 == 1)
					{
						if (Cont02 < (int)PhoneContacts.size() && Cont02 > -1)
						{
							FileRemoval(GetDir() + "/PlayerZero/Contacts/" + PhoneContacts[Cont02].Name + ".ini");
							PhoneContacts.erase(PhoneContacts.begin() + Cont02);
						}
					}
				}
			}
			else
			{
				int Cont02 = MyMenuSys(PZLang[20], vector<string>{ PZLang[36] });
				iMenuSys = 1;
			}
		}
		else
		{
			MySettings.MobileData = !MySettings.MobileData;
			iMenuSys = 1;
		}
	}
	else if (MyPick == 2)
	{
		KeepFrieands = false;
		LaggOut(KeepFrieands);//ClearSession
	}
	else if (MyPick == 3)
	{
		if (!BypassAll)
		{
			KeepFrieands = true;
			MySettings.InviteOnly = !MySettings.InviteOnly;
			LaggOut(KeepFrieands);
		}
	}
	else if (MyPick == 4)
	{
		Output = MyMenuSysPlus(PZLang[37], vector<MenuNumbers>{ MenuNumbers(PZLang[38], 0, 11, MySettings.Aggression) });

		MySettings.Aggression = Output[0];

		if (MySettings.Aggression > 4)
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, GP_Attack, GP_Player);
		}
		else
		{
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Player, GP_Attack);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, GP_Attack, GP_Player);
		}

		iMenuSys = 1;
	}
	else if (MyPick == 5)
	{
		Output = MyMenuSysPlus(PZLang[39], vector<MenuNumbers>{ MenuNumbers(PZLang[40], 5, 30, MySettings.MaxPlayers) });

		MySettings.MaxPlayers = Output[0];
		iMenuSys = 1;
	}
	else if (MyPick == 6)
	{
		Output = MyMenuSysPlus(PZLang[41], vector<MenuNumbers>{ MenuNumbers(PZLang[42], 0, MySettings.MaxWait, MySettings.MinWait), MenuNumbers(PZLang[43], MySettings.MinWait, 3600, MySettings.MaxWait) });
		MySettings.MinWait = Output[0];
		MySettings.MaxWait = Output[1];
		iMenuSys = 1;
	}
	else if (MyPick == 7)
	{
		Output = MyMenuSysPlus(PZLang[44], vector<MenuNumbers>{ MenuNumbers(PZLang[42], 10, MySettings.MaxSession, MySettings.MinSession), MenuNumbers(PZLang[43], MySettings.MinSession, 7200, MySettings.MaxSession) }), vector<int> {MySettings.MinSession, MySettings.MaxSession};
		MySettings.MinSession = Output[0];
		MySettings.MaxSession = Output[1];
		iMenuSys = 1;
	}
	else if (MyPick == 8)
	{
		Output = MyMenuSysPlus(PZLang[45], vector<MenuNumbers>{ MenuNumbers(PZLang[46], 0, MySettings.AccMax, MySettings.AccMin), MenuNumbers(PZLang[47], MySettings.AccMin, 100, MySettings.AccMax) });
		MySettings.AccMin = Output[0];
		MySettings.AccMax = Output[1];
		iMenuSys = 1;
	}
	else if (MyPick == 9)
	{
		Output = MyMenuSysPlus(PZLang[48], vector<MenuNumbers>{ MenuNumbers(PZLang[49], 0, MySettings.MaxPlayers, MySettings.AirVeh) });
		MySettings.AirVeh = Output[0];
		iMenuSys = 1;
	}
	else if (MyPick == 10)
	{
		MySettings.NoNotify = !MySettings.NoNotify;
		iMenuSys = 1;
	}
	else if (MyPick == 11)
	{
		MySettings.NoRadar = !MySettings.NoRadar;
		iMenuSys = 1;
	}
	else if (MyPick == 12)
	{
		MySettings.SpaceWeaps = !MySettings.SpaceWeaps;
		iMenuSys = 1;
	}
	else if (MyPick == 13)
	{
		MySettings.NameTags = !MySettings.NameTags;
		iMenuSys = 1;
	}
	else if (MyPick == 14)
	{
		MySettings.PassiveMode = !MySettings.PassiveMode;
		iMenuSys = 1;
	}
	else if (MyPick == 15)
	{
		MySettings.FriendlyFire = !MySettings.FriendlyFire;
		SetRelationType(MySettings.FriendlyFire);
		iMenuSys = 1;
	}
	else if (MyPick == 16)
	{
		MySettings.BackChat = !MySettings.BackChat;
		iMenuSys = 1;
	}
	else if (MyPick == 17)
	{
		MySettings.Debugger = !MySettings.Debugger;
		iMenuSys = 1;
	}
	else if (MyPick == 18)
	{
		int iRun = MyMenuSys(PZLang[50], vector<string>{ PZLang[53], PZLang[54] });
		if (iRun == 0)
		{
			int iCont = MyMenuSys(PZLang[50], vector<string>{ PZLang[51], PZLang[52], PZLang[55], PZLang[56] });
			if (iCont == 0)
			{
				PZData::MySettings.Control_Players = FindKeyBinds(true);
			}
			else if (iCont == 1)
			{
				PZData::MySettings.Control_Open = FindKeyBinds(true);
			}
			else if (iCont == 2)
			{
				PZData::MySettings.Control_A_Clear = FindKeyBinds(true);
				PZData::MySettings.Control_B_Clear = FindKeyBinds(true);
			}
			else if (iCont == 3)
			{
				PZData::MySettings.Control_A_Invite = FindKeyBinds(true);
				PZData::MySettings.Control_B_Invite = FindKeyBinds(true);
			}
		}
		else if (iRun == 1)
		{
			int iKeyed = MyMenuSys(PZLang[50], vector<string>{ PZLang[51], PZLang[52], PZLang[55], PZLang[56] });
			if (iKeyed == 0)
			{
				PZData::MySettings.Keys_Players = FindKeyBinds(false);
			}
			else if (iKeyed == 1)
			{
				PZData::MySettings.Keys_Open = FindKeyBinds(false);
			}
			else if (iKeyed == 2)
			{
				PZData::MySettings.Keys_Clear = FindKeyBinds(false);
			}
			else if (iKeyed == 3)
			{
				PZData::MySettings.Keys_Invite = FindKeyBinds(false);
			}
		}
	}
	else if (MyPick == 19)
	{
		PZData::MySettings.Pz_Lang = MyMenuSys(TranlatesTo[PZData::MySettings.Pz_Lang], TranlatesTo);
		LoadLang();
		iMenuSys = 1;
	}
	
	ReBuildIni(&MySettings);
}
void MenuLoops()
{
	while (true)
	{
		if (iMenuSys == 1)
			PlayerZeroMenu();
		else if (iMenuSys == 2)
			TrollMenu();
		else if (iMenuSys == 3)
			TrollPlayMenu();
		else
			break;
		WAIT(1);
	}
	bMenuOpen = false;
}

bool PhonesOff = false;
int SlowScan = 0;
int ClosingPhone = 0;
int ScanForBy = 0;
int SnowScan = 0;
int HidyHo = 0;
void ChaterBox()
{
	while (true)
	{
		if (!bStartTheMod)
		{
			if (ShitTalk.size() > 0)
			{
				for (int i = 0; i < ShitTalk.size(); i++)
					RightSide(ShitTalk[i].Talk, i);

				for (int i = 0; i < ShitTalk.size(); i++)
				{
					if (ShitTalk[i].TalkTime < InGameTime())
						ShitTalk.erase(ShitTalk.begin() + i);
				}
			}
			if (MySettings.NameTags)
			{
				if ((int)PedList.size() > 0)
				{
					for (int i = 0; i < (int)PedList.size(); i++)
					{
						if (PedList[i].ThisPed != NULL)
						{
							if ((bool)ENTITY::IS_ENTITY_ON_SCREEN(PedList[i].ThisPed) && !PedList[i].YoDeeeed && !PedList[i].Passenger)
							{
								Vector3 PedPos = RightOf(PedList[i].ThisPed, 0.5, false);
								showPlayerLabel(NewVector3(PedPos.x, PedPos.y, PedPos.z + 1.0f), 10.0, vector<string>{PedList[i].MyName});
							}
						}
					}
				}
			}
			if (MySettings.PassiveMode)
			{
				if (iPasiveMode != -1)
				{
					if (iPasiveMode < InGameTime())
					{
						iPasiveMode = -1;
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 0);
						ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255, false);
						CAM::DO_SCREEN_FADE_IN(1);
						PlayDead = false;
					}
				}
				else
				{
					if ((bool)ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()))
					{
						PlayDead = true;
						PlayersKiller();
					}
				}
			}
			if (NotWanted)
			{
				if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0)
				{
					PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
				}
			}
			if (ItsSnowing)
			{
				if (SnowScan < InGameTime())
				{
					SnowScan = InGameTime() + 1000;
					GRAPHICS::_SET_FORCE_PED_FOOTSTEPS_TRACKS(true);
					GRAPHICS::_SET_FORCE_VEHICLE_TRAILS(true);
				}
			}
			if (HideTrafic)
			{
				if (HidyHo < InGameTime())
				{
					HidyHo = InGameTime() + 500;
					GhostTown();
				}
			}
		}

		WAIT(1);
	}
}

DWORD waitTime = 0;
void main()
{
	if (FileExists(sDisenable))
		FileRemoval(sDisenable);

	if (FileExists(ZeroYank))
		FileRemoval(ZeroYank);

	if (FileExists(ZeroCayo))
		FileRemoval(ZeroCayo);

	if (FileExists(sSnowie))
		FileRemoval(sSnowie);

	while (true)
	{
		if (bStartTheMod)
		{
			if (!(bool)DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
			{
				LoggerLight("bStartTheMod");
				bStartTheMod = false;
				LoadinData();

				if (FirstRun)
				{
					WAIT(10000);
					ShitTalk.push_back(ShitTalking(PZLang[0], InGameTime() + 10000));
					ShitTalk.push_back(ShitTalking("", InGameTime() + 10000));
					ShitTalk.push_back(ShitTalking(PZLang[1], InGameTime() + 10000));
					iMenuSys = 1;
					MenuLoops();
				}
			}
			WAIT(10000);
		}
		else
		{
			DWORD maxTickCount = GetTickCount() + waitTime;
			do
			{
				if (!bMenuOpen)
				{
					if (IsIsSafe(MySettings.Control_Players) || WHileKeyDown(MySettings.Keys_Players))
						DisplayPlayers();
					else if (ButtonDown(MySettings.Control_A_Clear, false) && ButtonDown(MySettings.Control_B_Clear, false))
						LaggOut(false);
					else if (WHileKeyDown(MySettings.Keys_Clear))
						LaggOut(false);
					else if (!BypassAll && ButtonDown(MySettings.Control_A_Invite, false) && ButtonDown(MySettings.Control_B_Invite, false))
					{
						MySettings.InviteOnly = !MySettings.InviteOnly;
						LaggOut(true);
						ReBuildIni(&MySettings);
					}
					else if (WHileKeyDown(MySettings.Keys_Invite))
					{
						MySettings.InviteOnly = !MySettings.InviteOnly;
						LaggOut(true);
						ReBuildIni(&MySettings);
					}
					else if (WHileKeyDown(MySettings.Keys_Open) || bTrigMenu)
					{
						bTrigMenu = false;
						bMenuOpen = true;
						iMenuSys = 1;
						MenuLoops();
					}
				}

				if (MySettings.MobileData && (int)PhoneContacts.size() > 0)
				{
					if (ClosingPhone < InGameTime())
						PhoneFreeking();
				}

				if (DropMoneys)
					ThemMoneyBAgs();

				WAIT(0);
			} while (GetTickCount() < maxTickCount);
			waitTime = 0;

			if (!BypassAll)
			{
				if (ScanForBy < InGameTime())
				{
					ScanForBy = InGameTime() + 5000;
					if (FileExists(sDisenable))
					{
						LaggOut(true);
						BypassAll = true;
					}
				}
				else
				{
					if (YouFriend.MyIdentity != "Empty")
						WillYouBeMyFriend();
				}
			}
			else if (ScanForBy < InGameTime())
			{
				ScanForBy = InGameTime() + 5000;
				if (!FileExists(sDisenable))
					BypassAll = false;
			}

			if (SlowScan < InGameTime())
			{
				SlowScan = InGameTime() + 2000;
				ScanArea();
			}
			
			PlayerZerosAI();
			WAIT(1);
		}
	}
}

void AddChaterBox()
{
	srand(GetTickCount());
	ChaterBox();
}
void ScriptMain()
{
	srand(GetTickCount());
	main();
}