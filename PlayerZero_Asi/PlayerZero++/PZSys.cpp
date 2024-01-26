#include "PZSys.h"
#include "keyboard.h"

using namespace std;
using namespace PZData;

#include <Windows.h>
#include <Psapi.h>
#include <cstdlib>
#include <sys/stat.h>
#include <string>
#include <codecvt>
#include <array>
#include <locale>
#include <ctime>
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>

#pragma warning(disable : 4244 4305) // double <-> float conversions

namespace PZSys
{
	std::string ConvertWideToANSI(const std::wstring& wstr)
	{
		int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
		std::string str(count, 0);
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
		return str;
	}
	std::string ConvertWideToUtf8(const std::wstring& wstr)
	{
		int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
		std::string str(count, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
		return str;
	}
	std::string GetExeFileName()
	{
		//char* buffer[MAX_PATH];
		//GetModuleFileName(NULL, buffer, McAX_PATH);

		WCHAR buffer[MAX_PATH]{};
		GetModuleFileNameW(NULL, buffer, ARRAYSIZE(buffer));

		return ConvertWideToUtf8(std::wstring(buffer));
	}
	string GetDir()
	{
		std::string f = GetExeFileName();
		std::string Dir = f.substr(0, f.find_last_of("\\/"));

		return Dir;
	}
	string TimeDate()
	{
		time_t curr_time;
		curr_time = time(NULL);
		string tm = ctime(&curr_time);
		tm[tm.length() - 1] = char(32);//32 is this fails testing the _s version..
		return tm;
	}
	bool FileExists(string filename)
	{
		try
		{
			ifstream Infield(filename);
			return Infield.good();
		}
		catch (...)
		{
			return false;
		}
	}

	string sLogThis = GetDir() + "/PlayerZero/LoggerLight.txt";
	bool startLogs = false;

	bool ListContains(std::vector<int>* List, int item)
	{
		bool b = false;
		for (int i = 0; i < List->size(); i++)
		{
			if (List->at(i) == item)
				b = true;
		}
		return b;
	}
	bool FileRemoval(std::string filename)
	{
		bool Removed = false;
		try {
			if (std::filesystem::remove(filename))
				Removed = true;
		}
		catch (const std::filesystem::filesystem_error& err)
		{
			Removed = false;
		}
		return Removed;
	}
	void LoggerLight(string text)
	{
		if (startLogs && PZData::MySettings.Debugger)
		{
			ofstream MyFile(sLogThis, std::ios_base::app | std::ios_base::out);
			string Mess = TimeDate() + text;

			MyFile << Mess;
			MyFile << "\n";

			MyFile.close();
		}
		else
		{
			startLogs = true;

			ofstream MyFile(sLogThis);

			string Mess = TimeDate() + text;
			MyFile << Mess;
			MyFile << "\n";
			MyFile.close();
		}
	}
	void WriteFile(string file, vector<string> text)
	{
		ofstream MyFile(file);
		for (int i = 0; i < text.size(); i++)
		{
			MyFile << text[i];
			MyFile << "\n";
		}
		MyFile.close();
	}
	vector<string> IntoString(vector<int> text)
	{
		vector<string> output = {};

		for (int i = 0; i < text.size(); i++)
			output.push_back(std::to_string(text[i]));

		return output;
	}
	vector<string> ReadFile(string fileName)
	{
		string myText;
		vector<string> textList = {};

		if (FileExists(fileName))
		{
			ifstream MyReadFile(fileName);

			while (getline(MyReadFile, myText))
			{
				// Output the text from the file
				textList.push_back(myText);
			}
			MyReadFile.close();
		}

		return textList;
	}
	bool StringContains(string line, string wholeString)
	{
		bool Contain = false;
		int iLine = 0;
		string Word = "";
		for (int i = 0; i < wholeString.length(); i++)
		{
			if (line[iLine] == wholeString[i] && iLine < line.length())
			{
				Contain = true;
				Word += wholeString[i];
				iLine++;
			}
			else if (iLine > 0)
			{
				if (line == Word)
					break;
				else
				{
					iLine = 0;
					Word = "";

					if (line[0] == wholeString[i])
					{
						Contain = true;
						Word += wholeString[i];
						iLine++;
					}
				}
			}
		}

		if (Contain)
		{
			if (line != Word)
				Contain = false;
		}

		return Contain;
	}
	bool StringContains(char line, string wholeString)
	{
		bool Contain = false;
		for (int i = 0; i < wholeString.length(); i++)
		{
			if (line == wholeString[i])
			{
				Contain = true;
				break;
			}
		}
		return Contain;
	}
	int FindCharicter(char chars, string wholeString)
	{
		int iLine = -1;
		for (int i = 0; i < wholeString.length(); i++)
		{
			if (chars == wholeString[i])
			{
				iLine = i;
				break;
			}
		}
		return iLine;
	}
	int FindCharicter(string chars, string wholeString)
	{
		int iLine = -1;
		for (int i = 0; i < wholeString.length(); i++)
		{
			if (chars[0] == wholeString[i])
			{
				iLine = i;
				break;
			}
		}
		return iLine;
	}
	int StingNumbersInt(string line)
	{
		int iNum = 0;
		int iTimes = 0;
		bool bNegative = false;
		for (int i = (int)line.size(); i > -1; i--)
		{
			bool bSkip = false;
			int iAdd = 0;
			char sComp = line[i];

			if (sComp == char(45))
				bNegative = true;
			else if (sComp == char(49))
			{
				bNegative = false;
				iAdd = 1;
			}
			else if (sComp == char(50))
			{
				bNegative = false;
				iAdd = 2;
			}
			else if (sComp == char(51))
			{
				bNegative = false;
				iAdd = 3;
			}
			else if (sComp == char(52))
			{
				bNegative = false;
				iAdd = 4;
			}
			else if (sComp == char(53))
			{
				bNegative = false;
				iAdd = 5;
			}
			else if (sComp == char(54))
			{
				bNegative = false;
				iAdd = 6;
			}
			else if (sComp == char(55))
			{
				bNegative = false;
				iAdd = 7;
			}
			else if (sComp == char(56))
			{
				bNegative = false;
				iAdd = 8;
			}
			else if (sComp == char(57))
			{
				bNegative = false;
				iAdd = 9;
			}
			else if (sComp == char(48))
			{

			}
			else
				bSkip = true;

			if (!bSkip)
			{
				if (iTimes == 0)
				{
					iNum = iAdd;
					iTimes = 1;
				}
				else
					iNum += iAdd * iTimes;
				iTimes *= 10;
			}
		}

		if (bNegative)
			iNum = iNum * (-1);

		return iNum;
	}
	string AfterEqual(string tag)
	{
		int iSt = 0;
		string out;
		for (int i = 0; i < tag.length(); i++)
		{
			if (tag[i] == char(61))
			{
				iSt = i + 1;
				break;
			}
		}
		for (int i = iSt; i < tag.length(); i++)
			out += tag[i];

		return out;
	}
	string BoolToString(bool b)
	{
		if (b)
			return "true";
		else
			return "false";
	}
	float StingNumbersFloat(string line)
	{
		float fNum = 0;
		bool bNegative = false;
		bool DecDone = false;
		if (StringContains(".", line))
		{
			int iTimes = 0;
			for (int i = (int)line.size(); i > -1; i--)
			{
				bool bSkip = false;
				float fAdd = 0;
				char sComp = line[i];
				if (sComp == char(45))
					bNegative = true;
				else if (sComp == char(46))
				{
					fNum = fNum / iTimes;
					iTimes = 0;
					bSkip = true;
					DecDone = true;
				}
				else if (sComp == char(48))
				{
					if (DecDone)
					{
						DecDone = false;
						bSkip = true;
					}
				}
				else if (sComp == char(49))
				{
					bNegative = false;
					fAdd = 1;
				}
				else if (sComp == char(50))
				{
					bNegative = false;
					fAdd = 2;
				}
				else if (sComp == char(51))
				{
					bNegative = false;
					fAdd = 3;
				}
				else if (sComp == char(52))
				{
					bNegative = false;
					fAdd = 4;
				}
				else if (sComp == char(53))
				{
					bNegative = false;
					fAdd = 5;
				}
				else if (sComp == char(54))
				{
					bNegative = false;
					fAdd = 6;
				}
				else if (sComp == char(55))
				{
					bNegative = false;
					fAdd = 7;
				}
				else if (sComp == char(56))
				{
					bNegative = false;
					fAdd = 8;
				}
				else if (sComp == char(57))
				{
					bNegative = false;
					fAdd = 9;
				}
				else
					bSkip = true;

				if (!bSkip)
				{
					if (iTimes == 0)
					{
						fNum = fAdd;
						iTimes = 1;
					}
					else
						fNum += fAdd * iTimes;
					iTimes *= 10;
				}
			}

			if (bNegative)
				fNum = fNum * (-1);
		}
		else
			fNum = (float)StingNumbersInt(line);

		return fNum;
	}
	bool StringBool(string line)
	{
		if (StringContains("True", line))
			return true;
		else if (StringContains("true", line))
			return true;
		else if (StringContains("TRUE", line))
			return true;
		else
			return false;
	}
	float TwoDecimal(string Number)
	{
		int iSize = (int)Number.length();
		string Output = "";
		for (int i = 0; i < iSize; i++)
		{
			Output += Number[i];
			if (Number[i] == char(46) && i + 3 < Number.length())
				iSize = i + 3;
		}
		return StingNumbersFloat(Output);
	}
	int DayOfWeek()
	{
		int iDay;
		string t = TimeDate();
		if (StringContains("Mon", t))
			iDay = 1;
		else if (StringContains("Tue", t))
			iDay = 2;
		else if (StringContains("Wed", t))
			iDay = 3;
		else if (StringContains("Thu", t))
			iDay = 4;
		else if (StringContains("Fri", t))
			iDay = 5;
		else
			iDay = 6;

		return iDay;
	}
	int ReteaveAfk(string sId)
	{
		LoggerLight("PlayerAI.ReteaveAfk, sId == " + sId);

		int iAm = -1;
		for (int i = 0; i < (int)AFKList.size(); i++)
		{
			if (AFKList[i].MyIdentity == sId)
			{
				iAm = i;
				break;
			}
		}
		return iAm;
	}
	int ReteaveBrain(string sId)
	{
		LoggerLight("ReteaveBrain, sId == " + sId);

		int iAm = -1;
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (PedList[i].MyIdentity == sId)
			{
				iAm = i;
				break;
			}
		}
		return iAm;
	}
	int RetreaveCont(string sId)
	{
		LoggerLight("RetreaveCont, sId == " + sId);

		int iAm = -1;
		for (int i = 0; i < (int)PhoneContacts.size(); i++)
		{
			if (PhoneContacts[i].YourFriend.MyIdentity == sId)
			{
				iAm = i;
				break;
			}
		}
		return iAm;
	}
	bool InLoadList(string sId)
	{
		bool b = false;

		for (int i = 0; i < (int)MakeFrenz.size(); i++)
		{
			if (MakeFrenz[i].Brains.MyIdentity == sId)
			{
				b = true;
				break;
			}
		}
		if (!b)
		{
			for (int i = 0; i < (int)MakeCarz.size(); i++)
			{
				if (MakeCarz[i].Brains.MyIdentity == sId)
				{
					b = true;
					break;
				}
			}
		}
		return b;
	}
	void StartScript(string scriptName, int buffer)
	{
		SCRIPT::REQUEST_SCRIPT((LPSTR)scriptName.c_str());
		while (!SCRIPT::HAS_SCRIPT_LOADED((LPSTR)scriptName.c_str()))
		{
			SCRIPT::REQUEST_SCRIPT((LPSTR)scriptName.c_str());
			WAIT(1);
		}

		SYSTEM::START_NEW_SCRIPT((LPSTR)scriptName.c_str(), buffer);
		SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED((LPSTR)scriptName.c_str());
	}

	float RandomFloat(float min, float max)
	{
		if (min < max)
		{
			string BigNumbers = std::to_string(GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(min, max));
			float f = TwoDecimal(BigNumbers);
			return f;
		}
		else
			return min;
	}
	bool ERando = false;
	int RandomInt(int min, int max)
	{
		int EllRando = max - min;
		if (EllRando > 10)
		{
			if (ERando)
			{
				min /= 2;
				max /= 2;
			}
			else
			{
				if (min > -1 && min > max / 2)
				{
					min = max / 2;
				}
			}
			ERando = !ERando;
		}

		if (min < max)
		{
			srand((unsigned)time(NULL));

			return min + (rand() % max);
		}
		else
			return min;
	}
	vector<int> NewNums(int min, int max)
	{
		vector<int> Nums = {};
		for (int i = min; i < max + 1; i++)
			Nums.push_back(i);
		return Nums;
	}
	bool DirTest = true;
	int LessRandomInt(string sName, int min, int max)
	{
		if (DirTest)
		{
			LoggerLight("LessRandomInt, Direct Test");
			string OutputFolder = GetDir() + "/PlayerZero/Randoms";
			if (CreateDirectoryA((LPSTR)OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				LoggerLight("LessRandomInt, Direct Working");
				DirTest = false;
			}
			else
			{
				LoggerLight("LessRandomInt, Direct failed");
				if (min < max || min == max)
				{

				}
				else
				{
					max = RandomInt(min, max);
					min = max;
				}
			}

		}
		LoggerLight("LessRandomInt, id == " + sName + ", min == " + std::to_string(min) + ", max == " + std::to_string(max));
		if (min < max || min == max)
		{
			int yourPlace = 0;
			int yourNum = min;
			vector<string> MyNums = ReadFile(GetDir() + "/PlayerZero/Randoms/" + sName + ".txt");
			vector<int> NumList = {};

			if (MyNums.size() < 2)
				NumList = NewNums(min, max);
			else
			{
				bool newWrite = false;
				if (StringContains("min=", MyNums[0]))
				{
					if (StingNumbersInt(MyNums[0]) != min)
						newWrite = true;

					if (StringContains("max=", MyNums[1]))
					{
						if (StingNumbersInt(MyNums[1]) != max)
							newWrite = true;
					}
				}
				else
					newWrite = true;

				for (int i = 2; i < MyNums.size(); i++)
					NumList.push_back(StingNumbersInt(MyNums[i]));

				if (NumList.size() == 0 || newWrite)
					NumList = NewNums(min, max);
			}
			yourPlace = RandomInt(0, (int)NumList.size() - 1);
			yourNum = NumList[yourPlace];
			NumList.erase(NumList.begin() + yourPlace);

			vector<string> output = { "min=" + std::to_string(min) , "max=" + std::to_string(max) };

			for (int i = 0; i < NumList.size(); i++)
				output.push_back(std::to_string(NumList[i]));

			WriteFile(GetDir() + "/PlayerZero/Randoms/" + sName + ".txt", output);

			return yourNum;
		}
		else
			return min;
	}

	float GetAngle(Vector3 postion1, Vector3 postion2)
	{
		double ang = (postion1.x * postion2.x) + (postion1.y * postion2.y);
		double output = 0;
		if (ang != 0)
		{
			double ang1 = sqrt((postion1.x * 2) + (postion1.y * 2)) / ang;
			double ang2 = sqrt((postion2.x * 2) + (postion2.y * 2)) / ang;
			double output = cos(ang1 + ang2 / ang);
		}
		return (float)output;
	}

	float DistanceTo(Vector3 postion1, Vector3 postion2)
	{
		double num = postion2.x - postion1.x;
		double num2 = postion2.y - postion1.y;
		double num3 = postion2.z - postion1.z;
		return (float)sqrt(num * num + num2 * num2 + num3 * num3);
	}
	float DistanceTo(Entity entity1, Entity entity2)
	{
		Vector3 postion1 = ENTITY::GET_ENTITY_COORDS(entity1, false);
		Vector3 postion2 = ENTITY::GET_ENTITY_COORDS(entity2, false);

		return DistanceTo(postion1, postion2);
	}
	float DistanceTo(Entity entity1, Vector3 postion2)
	{
		Vector3 postion1 = ENTITY::GET_ENTITY_COORDS(entity1, false);

		return DistanceTo(postion1, postion2);
	}
	float DistanceTo(PZclass::Vector4 postion1, Vector3 postion2)
	{
		double num = postion2.x - postion1.X;
		double num2 = postion2.y - postion1.Y;
		double num3 = postion2.z - postion1.Z;
		return (float)sqrt(num * num + num2 * num2 + num3 * num3);
	}
	float DistanceTo(PZclass::Vector4 postion1, PZclass::Vector4 postion2)
	{
		double num = postion2.X - postion1.X;
		double num2 = postion2.Y - postion1.Y;
		double num3 = postion2.Z - postion1.Z;
		return (float)sqrt(num * num + num2 * num2 + num3 * num3);
	}

	Vector3 EntityPosition(Entity Object)
	{
		return  ENTITY::GET_ENTITY_COORDS(Object, 0);
	}
	Vector3 PlayerPosi()
	{
		return EntityPosition(PLAYER::PLAYER_PED_ID());
	}
	Vector3 MyWayPoint()
	{
		return UI::GET_BLIP_INFO_ID_COORD(UI::GET_FIRST_BLIP_INFO_ID(8));
	}
	Vector3 NewVector3(float X, float Y, float Z)
	{
		Vector3 NewVec = Vector3();
		NewVec.x = X;
		NewVec.y = Y;
		NewVec.z = Z;

		return NewVec;
	}
	Vector3 RightOfEntity(Entity Object)
	{
		Vector3 Pos = ENTITY::GET_ENTITY_ROTATION(Object, 00);
		const double PI = 3.14259;
		double num = cos((double)Pos.y * (PI / 180.0));
		double num2 = cos((double)(0 - Pos.z) * (PI / 180.0)) * num;
		double num3 = sin((double)Pos.z * (PI / 180.0)) * num;
		double num4 = sin((double)(0 - Pos.y) * (PI / 180.0));

		Pos.x = (float)num2;
		Pos.y = (float)num3;
		Pos.z = (float)num4;

		return Pos;
	}
	Vector3 FowardOf(Entity ent, float multi, bool inFront)
	{
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(ent, true);
		Vector3 FV = ENTITY::GET_ENTITY_FORWARD_VECTOR(ent);
		FV.x *= multi;
		FV.y *= multi;
		FV.z *= multi;

		if (inFront)
		{
			vec.x += FV.x;
			vec.y += FV.y;
			vec.z += FV.z;
		}
		else
		{
			vec.x -= FV.x;
			vec.y -= FV.y;
			vec.z -= FV.z;
		}

		return vec;
	}
	Vector3 RightOf(Entity ent, float multi, bool inFront)
	{
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(ent, true);
		Vector3 FV = RightOfEntity(ent);
		FV.x *= multi;
		FV.y *= multi;
		FV.z *= multi;

		if (inFront)
		{
			vec.x += FV.x;
			vec.y += FV.y;
			vec.z += FV.z;
		}
		else
		{
			vec.x -= FV.x;
			vec.y -= FV.y;
			vec.z -= FV.z;
		}

		return vec;
	}

	PZclass::Vector4 InAreaOf(PZclass::Vector4  area, float minDist, float maxDist)
	{
		float X = RandomFloat(maxDist * -1, maxDist);
		float Y = RandomFloat(maxDist * -1, maxDist);

		if (X > -1.0 && X < minDist)
			X = minDist;
		else if (X < 1.0 && X > minDist * -1)
			X = minDist * -1;

		if (Y > -1.0 && Y < minDist)
			Y = minDist;
		else if (Y < 1.0 && Y > minDist * -1)
			Y = minDist * -1;

		float x = area.X + X;
		float y = area.Y + Y;
		float z = area.Z;
		float r = area.R;

		return PZclass::Vector4(x, y, z, r);
	}
	PZclass::Vector4 InAreaOf(Vector3 area, float minDist, float maxDist)
	{
		return InAreaOf(PZclass::Vector4(area.x, area.y, area.z, 0.0), minDist, maxDist);
	}
	PZclass::Vector4 GetPosV4(Entity Object)
	{
		Vector3 Pos = EntityPosition(Object);
		return PZclass::Vector4(Pos.x, Pos.y, Pos.z, ENTITY::GET_ENTITY_HEADING(Object));
	}

	void GhostTown()
	{
		Vehicle PlayersVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

		const int VEH_ARR_SIZE = 1024;
		Vehicle vehicles[VEH_ARR_SIZE];
		int Vehcount = worldGetAllVehicles(vehicles, VEH_ARR_SIZE);

		for (int i = 0; i < Vehcount; i++)
		{
			Vehicle Vent = vehicles[i];
			if (Vent != NULL && ENTITY::DOES_ENTITY_EXIST(Vent))
			{
				if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(Vent) || Vent == PlayersVeh)
				{

				}
				else
				{
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vent, true, true);
					VEHICLE::DELETE_VEHICLE(&Vent);
				}
			}
		}

		const int PED_ARR_SIZE = 1024;
		Ped Pedds[PED_ARR_SIZE];
		int Pedcount = worldGetAllPeds(Pedds, PED_ARR_SIZE);

		for (int i = 0; i < Pedcount; i++)
		{
			Ped Pent = Pedds[i];
			if (Pent != NULL && ENTITY::DOES_ENTITY_EXIST(Pent))
			{
				if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(Pent))
				{

				}
				else
				{
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Pent, true, true);
					PED::DELETE_PED(&Pent);
				}
			}
		}
	}

	static std::vector<Vector3> LastDrop = {};
	bool NearThisList(PZclass::Vector4 postion1)
	{
		bool b = false;
		for (int i = 0; i < LastDrop.size(); i++)
		{
			if (15.0f > DistanceTo(postion1, LastDrop.at(i)))
				b = true;
		}
		return b;
	}
	bool NearThisList(Entity postion1)
	{
		return NearThisList(GetPosV4(postion1));
	}

	const std::vector<PZclass::Vector4> SanLoocIndex = {
		PZclass::Vector4(-812.1608f, -2201.21f, 16.7955f, 0.0f),
		PZclass::Vector4(211.7755f, -3045.787f, 5.089411f, 0.0f),
		PZclass::Vector4(990.5366f, -3114.517f, 5.426713f, 0.0f),
		PZclass::Vector4(-1230.986f, -1088.824f, 7.947563f, 0.0f),
		PZclass::Vector4(592.1391f, -1825.843f, 24.36324f, 0.0f),
		PZclass::Vector4(17.29186f, -805.5605f, 47.16209f, 0.0f),
		PZclass::Vector4(2042.241f, -2126.156f, 116.9319f, 0.0f),
		PZclass::Vector4(1602.962f, -550.4477f, 168.798f, 0.0f),
		PZclass::Vector4(-747.7878f, 200.8658f, 75.26504f, 0.0f),
		PZclass::Vector4(615.4108f, 266.308f, 102.7443f, 0.0f),
		PZclass::Vector4(-2812.112f, 637.9633f, 104.826f, 0.0f),
		PZclass::Vector4(-402.1792f, 2294.639f, 229.0287f, 0.0f),
		PZclass::Vector4(2260.562f, 1439.681f, 138.9774f, 0.0f),
		PZclass::Vector4(1622.083f, 4018.104f, 37.64009f, 0.0f),
		PZclass::Vector4(-1027.057f, 5541.436f, 20.38375f, 0.0f),
		PZclass::Vector4(1272.17f, 6244.464f, 115.8106f, 0.0f),
	};
	
	PZclass::Vector4 FindMySpot(const std::vector<PZclass::Vector4>* list, Vector3 local)
	{
		int iList = 0;
		float fDis = 4000.0f;
		PZclass::Vector4 YourVec = PZclass::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < list->size(); i++)
		{
			if (DistanceTo(list->at(i), local) < fDis && !NearThisList(list->at(i)))
			{
				fDis = DistanceTo(list->at(i), local);
				iList = i;
			}
		}
		return list->at(iList);
	}
	PZclass::Vector4 VehPlace(Vector3 local)
	{
		LoggerLight("VehPlace");
		int iList = -1;
		float fDis = 4000.0f;
		for (int i = 0; i < SanLoocIndex.size(); i++)
		{
			if (DistanceTo(SanLoocIndex[i], local) < fDis)
			{
				fDis = DistanceTo(SanLoocIndex[i], local);
				iList = i;
			}
		}
		
		if (iList == 0)
			return FindMySpot(&VehDrops01, local);
		else if (iList == 1)
			return FindMySpot(&VehDrops02, local);
		else if (iList == 2)
			return FindMySpot(&VehDrops03, local);
		else if (iList == 3)
			return FindMySpot(&VehDrops04, local);
		else if (iList == 4)
			return FindMySpot(&VehDrops05, local);
		else if (iList == 5)
			return FindMySpot(&VehDrops06, local);
		else if (iList == 6)
			return FindMySpot(&VehDrops07, local);
		else if (iList == 7)
			return FindMySpot(&VehDrops08, local);
		else if (iList == 8)
			return FindMySpot(&VehDrops09, local);
		else if (iList == 9)
			return FindMySpot(&VehDrops10, local);
		else if (iList == 10)
			return FindMySpot(&VehDrops11, local);
		else if (iList == 11)
			return FindMySpot(&VehDrops12, local);
		else if (iList == 12)
			return FindMySpot(&VehDrops13, local);
		else if (iList == 13)
			return FindMySpot(&VehDrops14, local);
		else if (iList == 14)
			return FindMySpot(&VehDrops15, local);
		else if (iList == 15)
			return FindMySpot(&VehDrops16, local);
		else
			return PZclass::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	PZclass::Vector4 PedPlace(Vector3 local)
	{
		LoggerLight("PedPlace");
		int iList = -1;
		float fDis = 4000.0f;
		for (int i = 0; i < SanLoocIndex.size(); i++)
		{
			if (DistanceTo(SanLoocIndex[i], local) < fDis)
			{
				fDis = DistanceTo(SanLoocIndex[i], local);
				iList = i;
			}
		}
		
		if (iList == 0)
			return FindMySpot(&PedDrops01, local);
		else if (iList == 1)
			return FindMySpot(&PedDrops02, local);
		else if (iList == 2)
			return FindMySpot(&PedDrops03, local);
		else if (iList == 3)
			return FindMySpot(&PedDrops04, local);
		else if (iList == 4)
			return FindMySpot(&PedDrops05, local);
		else if (iList == 5)
			return FindMySpot(&PedDrops06, local);
		else if (iList == 6)
			return FindMySpot(&PedDrops07, local);
		else if (iList == 7)
			return FindMySpot(&PedDrops08, local);
		else if (iList == 8)
			return FindMySpot(&PedDrops09, local);
		else if (iList == 9)
			return FindMySpot(&PedDrops10, local);
		else if (iList == 10)
			return FindMySpot(&PedDrops11, local);
		else if (iList == 11)
			return FindMySpot(&PedDrops12, local);
		else if (iList == 12)
			return FindMySpot(&PedDrops13, local);
		else if (iList == 13)
			return FindMySpot(&PedDrops14, local);
		else if (iList == 14)
			return FindMySpot(&PedDrops15, local);
		else if (iList == 15)
			return FindMySpot(&PedDrops16, local);
		else
			return PZclass::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	int FindAirSpots()
	{
		int iNear = -1;
		Vector3 PlayPos = PlayerPosi();
		float fIsh = 9999.0;
		for (int i = 0; i < (int)AirSpots.size(); i++)
		{
			float f = DistanceTo(AirSpots[i], PlayerPosi());
			if (f < fIsh)
			{
				iNear = i;
				fIsh = f;
			}
		}
		return iNear;
	}

	PZclass::Vector4 FindPedSpPoint(PZclass::Vector4 Pos)
	{
		float fAr = 4000.0f;
		int Near = -1;

		if (FileExists(ZeroYank))
		{
			for (int i = 0; i < (int)YankSpPoint.size(); i++)
			{
				if (DistanceTo(Pos, YankSpPoint[i]) < fAr && !NearThisList(YankSpPoint[i]))
				{
					fAr = DistanceTo(Pos, YankSpPoint[i]);
					Near = i;
				}
			}

			LastDrop.push_back(NewVector3(YankSpPoint[Near].X, YankSpPoint[Near].Y, YankSpPoint[Near].Z));
			if (LastDrop.size() > 15)
				LastDrop.erase(LastDrop.begin());

			return YankSpPoint[Near];
		}
		else if (FileExists(ZeroCayo))
		{
			for (int i = 0; i < (int)CayoSpPoint.size(); i++)
			{
				if (DistanceTo(Pos, CayoSpPoint[i]) < fAr && !NearThisList(CayoSpPoint[i]))
				{
					fAr = DistanceTo(Pos, CayoSpPoint[i]);
					Near = i;
				}
			}

			LastDrop.push_back(NewVector3(CayoSpPoint[Near].X, CayoSpPoint[Near].Y, CayoSpPoint[Near].Z));
			if (LastDrop.size() > 15)
				LastDrop.erase(LastDrop.begin());

			return CayoSpPoint[Near];
		}
		else
		{
			PZclass::Vector4 PlaceHere = PedPlace(PlayerPosi());

			LastDrop.push_back(NewVector3(PlaceHere.X, PlaceHere.Y, PlaceHere.Z));
			if (LastDrop.size() > 15)
				LastDrop.erase(LastDrop.begin());

			return PlaceHere;
		}
	}
	PZclass::Vector4 FindVehSpPoint(PZclass::Vector4 Pos)
	{
		float fAr = 4000.0f;
		int Near = -1;

		if (FileExists(ZeroYank))
		{
			if (MakeCarz[MakeCarz.size() - 1].Brains.IsPlane || MakeCarz[MakeCarz.size() - 1].Brains.IsHeli)
				return PZclass::Vector4(3280.166f, -4648.12f, 550.0f, 82.46803f);
			else
			{
				for (int i = 0; i < (int)YankVhPoint.size(); i++)
				{
					if (DistanceTo(Pos, YankVhPoint[i]) < fAr && !NearThisList(YankVhPoint[i]))
					{
						fAr = DistanceTo(Pos, YankVhPoint[i]);
						Near = i;
					}
				}
				LastDrop.push_back(NewVector3(YankVhPoint[Near].X, YankVhPoint[Near].Y, YankVhPoint[Near].Z));
				if (LastDrop.size() > 15)
					LastDrop.erase(LastDrop.begin());

				return YankVhPoint[Near];
			}
		}
		else if (FileExists(ZeroCayo))
		{
			if (MakeCarz[MakeCarz.size() - 1].Brains.IsPlane || MakeCarz[MakeCarz.size() - 1].Brains.IsHeli)
				return InAreaOf(AirSpots[FindAirSpots()], 35.0, 300.0);
			else
			{
				for (int i = 0; i < (int)CayoVhPoint.size(); i++)
				{
					if (DistanceTo(Pos, CayoVhPoint[i]) < fAr && !NearThisList(CayoVhPoint[i]))
					{
						fAr = DistanceTo(Pos, CayoVhPoint[i]);
						Near = i;
					}
				}
				LastDrop.push_back(NewVector3(CayoVhPoint[Near].X, CayoVhPoint[Near].Y, CayoVhPoint[Near].Z));
				if (LastDrop.size() > 15)
					LastDrop.erase(LastDrop.begin());

				return CayoVhPoint[Near];
			}
		}
		else
		{
			if (MakeCarz[MakeCarz.size() - 1].Brains.IsPlane || MakeCarz[MakeCarz.size() - 1].Brains.IsHeli)
				return InAreaOf(AirSpots[FindAirSpots()], 35.0, 300.0);
			else
			{
				PZclass::Vector4 PlaceHere = VehPlace(PlayerPosi());

				LastDrop.push_back(NewVector3(PlaceHere.X, PlaceHere.Y, PlaceHere.Z));
				if (LastDrop.size() > 15)
					LastDrop.erase(LastDrop.begin());

				return PlaceHere;
			}
		}
	}

	Hash MyHashKey(string name)
	{
		return GAMEPLAY::GET_HASH_KEY((LPSTR)name.c_str());
	}

	void AddMarker(Entity object)
	{
		PZclass::Vector4 Entpos = GetPosV4(object);
		GRAPHICS::DRAW_MARKER(2, Entpos.X, Entpos.Y, Entpos.Z + 1.0f, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f, Entpos.R, 0.2f, 0.2f, 0.2f, 236, 240, 41, 255, false, true, 2, true, 0, 0, false);
		/*
		void DRAW_MARKER(int type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int red, int green, int blue, int alpha, BOOL bobUpAndDown, BOOL faceCamera, int p19, BOOL rotate, char* textureDict, char* textureName, BOOL drawOnEnts) // 28477EC23D892089 48D84A02
			enum MarkerTypes
		{
			MarkerTypeUpsideDownCone = 0,
			MarkerTypeVerticalCylinder = 1,
			MarkerTypeThickChevronUp = 2,
			MarkerTypeThinChevronUp = 3,
			MarkerTypeCheckeredFlagRect = 4,
			MarkerTypeCheckeredFlagCircle = 5,
			MarkerTypeVerticleCircle = 6,
			MarkerTypePlaneModel = 7,
			MarkerTypeLostMCDark = 8,
			MarkerTypeLostMCLight = 9,
			MarkerTypeNumber0 = 10,
			MarkerTypeNumber1 = 11,
			MarkerTypeNumber2 = 12,
			MarkerTypeNumber3 = 13,
			MarkerTypeNumber4 = 14,
			MarkerTypeNumber5 = 15,
			MarkerTypeNumber6 = 16,
			MarkerTypeNumber7 = 17,
			MarkerTypeNumber8 = 18,
			MarkerTypeNumber9 = 19,
			MarkerTypeChevronUpx1 = 20,
			MarkerTypeChevronUpx2 = 21,
			MarkerTypeChevronUpx3 = 22,
			MarkerTypeHorizontalCircleFat = 23,
			MarkerTypeReplayIcon = 24,
			MarkerTypeHorizontalCircleSkinny = 25,
			MarkerTypeHorizontalCircleSkinny_Arrow = 26,
			MarkerTypeHorizontalSplitArrowCircle = 27,
			MarkerTypeDebugSphere = 28,
			MarkerTypeDallorSign = 29,
			MarkerTypeHorizontalBars = 30,
			MarkerTypeWolfHead = 31,
			MarkerTypeQuestionMark = 32,
			MarkerTypePlaneUpwards = 33,
			MarkerTypeHelicopter = 34,
			MarkerTypeBoat = 35,
			MarkerTypeCar = 36,
			MarkerTypeMotorcycle = 37,
			MarkerTypeBicycle = 38,
			MarkerTypeTruck = 39,
			MarkerTypeParachute = 40,
			MarkerTypeJetpack = 41,
			MarkerTypeTwirlingStar = 42
		};

		dirX / Y / Z represent a heading on each axis in which the marker should face, alternatively you can rotate each axis independently with rotX / Y / Z(and set dirX / Y / Z all to 0).

			faceCamera - Rotates only the y - axis(the heading) towards the camera

			p19 - no effect, default value in script is 2

			rotate - Rotates only on the y - axis(the heading)

			textureDict - Name of texture dictionary to load texture from(e.g. "GolfPutting")

			textureName - Name of texture inside dictionary to load(e.g. "PuttingMarker")

			drawOnEnts - Draws the marker onto any entities that intersect it

			basically what he said, except textureDict and textureName are totally not char*, or if so, then they are always set to 0 / NULL / nullptr in every script I checked, eg:

		bj.c : graphics::draw_marker(6, vParam0, 0f, 0f, 1f, 0f, 0f, 0f, 4f, 4f, 4f, 240, 200, 80, iVar1, 0, 0, 2, 0, 0, 0, false);

		his is what I used to draw an amber downward pointing chevron "V", has to be redrawn every frame.The 180 is for 180 degrees rotation around the Y axis, the 50 is alpha, assuming max is 100, but it will accept 255.
		*/
	}
	void menu_beep()
	{
		AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
	}
	void ButtonDisabler(int LButt)
	{
		//CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(0);
		CONTROLS::DISABLE_CONTROL_ACTION(1, LButt, true);
	}
	bool WhileButtonDown(int CButt, bool bDisable)
	{
		if (bDisable)
			ButtonDisabler(CButt); ;

		bool bButt = (bool)CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, CButt);

		if (bButt)
		{
			while (!(bool)CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(0, CButt))
				WAIT(1);
		}
		return bButt;
	}
	bool ButtonDown(int CButt, bool bDisable)
	{
		if (bDisable)
			ButtonDisabler(CButt);
		return (bool)CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, CButt);
	}
	int AddScreenText(int font, string caption, float textLeftScaled, float lineTopScaled, float lineHeightScaled, float text_scale, int text_col[4])
	{
		// text upper part
		UI::SET_TEXT_FONT(font);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
		UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

		// text lower part
		UI::SET_TEXT_FONT(font);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::_SET_TEXT_GXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
		return UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));
	}
	void AddSprite(string spriteLocation, string spriteName, float posX, float posY, float sizeX, float sizeY, float heading)
	{
		if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((LPSTR)spriteLocation.c_str()))
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((LPSTR)spriteLocation.c_str(), true);

		int red = 101;
		int blue = 185;
		int green = 231;
		int alpha = 120;

		int iWidth, iHeight;
		GRAPHICS::GET_SCREEN_RESOLUTION(&iWidth, &iHeight);

		float num = (float)iWidth / (float)iHeight;
		float num2 = (float)1080.0 * num;
		float width = posX / num2;
		float height = posY / 1080;
		float screenX = sizeX / num2 + width * 0.5f;
		float screenY = sizeY / 1080 + height * 0.5f;
		GRAPHICS::DRAW_SPRITE((LPSTR)spriteLocation.c_str(), (LPSTR)spriteName.c_str(), screenX, screenY, width, height, heading, red, green, blue, alpha);
	}
	void get_button_state(bool* a, bool* b, bool* up, bool* down, bool* l, bool* r, bool* shutDown)
	{
		if (a) *a = IsKeyDown(VK_RETURN);
		if (b) *b = IsKeyDown(VK_DELETE) || IsKeyDown(VK_BACK);
		if (up) *up = IsKeyDown(VK_UP);
		if (down) *down = IsKeyDown(VK_DOWN);
		if (l) *l = IsKeyDown(VK_LEFT);
		if (r) *r = IsKeyDown(VK_RIGHT);
		if (shutDown) *shutDown = IsKeyDown(VK_ESCAPE);
	}
	void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
	{
		GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
	}
	void DrawSessionList(std::string caption, std::string level, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, float textLeft2, bool active, bool title, int position)
	{
		// default values
		int text_col[4] = { 255, 255, 255, 255 },
			rect_col[4] = { 0, 0, 0, 120 };
		float text_scale = 0.50;
		int font = 4;

		// correcting values for active line
		if (title)
		{
			rect_col[0] = 0;
			rect_col[1] = 0;
			rect_col[2] = 0;
			rect_col[3] = 250;
		}

		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

		textLeft += lineLeft;
		textLeft2 += lineLeft;

		float lineWidthScaled = lineWidth / (float)screen_w; // line width
		float lineTopScaled = lineTop / (float)screen_h; // line top offset
		float textLeftScaled = textLeft / (float)screen_w; // text left offset
		float textLeftScaled2 = textLeft2 / (float)screen_w; // text left offset
		float lineHeightScaled = lineHeight / (float)screen_h; // line height

		float lineLeftScaled = lineLeft / (float)screen_w;

		// this is how it's done in original scripts

		// text upper part
		int num25 = AddScreenText(font, caption, textLeftScaled, lineTopScaled, lineHeightScaled, text_scale, text_col);
		int num26 = AddScreenText(font, level, textLeftScaled2, lineTopScaled, lineHeightScaled, text_scale, text_col);
		// rect
		draw_rect(lineLeftScaled, lineTopScaled + (0.00278f), lineWidthScaled, ((((float)(num25)*UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f), rect_col[0], rect_col[1], rect_col[2], rect_col[3]);

		if (active)
		{
			float PosX = 55.5;
			float PosY = 62.0;
			float sizeX = 352.0;
			float sizeY = 51.0;
			float Heading = 0.0;

			float iTimes1 = 1.0;
			float iTimes2 = 33.0;

			AddSprite("mpleaderboard", "leaderboard_globe_icon", PosX, PosY + (float)position * iTimes1, sizeX, sizeY + (float)position * iTimes2, 0.0);
		}
	}
	void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText)
	{
		// default values
		int text_col[4] = { 247, 245, 158, 255 },
			rect_col[4] = { 70, 95, 95, 255 };
		float text_scale = 0.35;
		int font = 0;

		// correcting values for active line
		if (active)
		{
			text_col[0] = 198;
			text_col[1] = 255;
			text_col[2] = 226;

			rect_col[0] = 100;
			rect_col[1] = 44;
			rect_col[2] = 101;

			if (rescaleText) text_scale = 0.40;
		}

		if (title)
		{
			rect_col[0] = 112;
			rect_col[1] = 44;
			rect_col[2] = 20;

			if (rescaleText) text_scale = 0.50;
			font = 1;
		}

		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

		textLeft += lineLeft;

		float lineWidthScaled = lineWidth / (float)screen_w; // line width
		float lineTopScaled = lineTop / (float)screen_h; // line top offset
		float textLeftScaled = textLeft / (float)screen_w; // text left offset
		float lineHeightScaled = lineHeight / (float)screen_h; // line height

		float lineLeftScaled = lineLeft / (float)screen_w;

		// this is how it's done in original scripts
		int num25 = AddScreenText(font, caption, textLeftScaled, lineTopScaled, lineHeightScaled, text_scale, text_col);

		// rect
		draw_rect(lineLeftScaled, lineTopScaled + (0.00278f), lineWidthScaled, ((((float)(num25)*UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
			rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
	}
	void CloseBaseHelpBar(int CloseMe)
	{
		LoggerLight("CloseBaseHelpBar");
		int SF = CloseMe;
		GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&SF);
	}
	int BottomRight(vector<int> iButtons, vector<string> sInstuctions)
	{
		LoggerLight("BottomRight");

		string CharCon = "instructional_buttons";
		int iScale = GRAPHICS::REQUEST_SCALEFORM_MOVIE((LPSTR)CharCon.c_str());

		while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(iScale))
			WAIT(1);

		CharCon = "CLEAR_ALL";
		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(iScale, (LPSTR)CharCon.c_str());
		CharCon = "TOGGLE_MOUSE_BUTTONS";
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iScale, (LPSTR)CharCon.c_str());
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(0);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		CharCon = "CREATE_CONTAINER";
		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(iScale, (LPSTR)CharCon.c_str());//Was---_CALL_SCALEFORM_MOVIE_FUNCTION_VOID

		int iAddOns = 0;

		for (int i = 0; i < (int)iButtons.size(); i++)
		{
			CharCon = "SET_DATA_SLOT";
			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iScale, (LPSTR)CharCon.c_str());
			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(iAddOns);
			GRAPHICS::_0xE83A3E3557A56640(CONTROLS::_GET_CONTROL_ACTION_NAME(2, iButtons[i], 1));
			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING((LPSTR)sInstuctions[i].c_str());
			GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
			iAddOns++;
		}
		CharCon = "DRAW_INSTRUCTIONAL_BUTTONS";
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iScale, (LPSTR)CharCon.c_str());
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(-1);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		return iScale;
	}
	int MyMenuSys(string caption, vector<string> MyMenu)
	{

		const float lineWidth = 450.0;
		int iPick = 0;
		int iLow = 0;
		int iHigh = 7;
		bool Scroling = false;

		if ((int)MyMenu.size() < 7)
			iHigh = (int)MyMenu.size();
		else
			Scroling = true;

		DWORD waitTime = 150;

		vector<string> sInstBut = { PZLang[84], PZLang[85] };
		vector<int> iInstBut = { 21, 45 };

		int iBc = BottomRight(iInstBut, sInstBut);

		while (true)
		{
			if (PlayDead)
			{
				iPick = -1;
				break;
			}
			// timed menu draw, used for pause after active line switch

			DWORD maxTickCount = GetTickCount() + waitTime;
			do
			{
				// draw menu
				draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
				for (int i = iLow; i < iHigh; i++)
					if (i != iPick)
						draw_menu_line(MyMenu[i], lineWidth, 9.0, 60.0 + (i - iLow) * 36.0, 0.0, 9.0, false, false);
				draw_menu_line(MyMenu[iPick], lineWidth + 1.0, 11.0, 56.0 + (iPick - iLow) * 36.0, 0.0, 7.0, true, false);

				GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(iBc, 255, 255, 255, 255, 0);
				WAIT(0);
			} while (GetTickCount() < maxTickCount);
			waitTime = 0;

			// process buttons
			bool bUp, bDown, bEnter, bBack, bShutDown;
			get_button_state(&bEnter, &bBack, &bUp, &bDown, NULL, NULL, &bShutDown);
			if (bShutDown || ButtonDown(199, false))
			{
				iMenuSys = 0;
				break;
			}
			else if (ButtonDown(21, true) || bEnter)
			{
				menu_beep();
				break;
			}
			else
			{
				if (ButtonDown(45, true) || bBack)
				{
					menu_beep();
					iPick = -1;
					break;
				}
				else
				{
					if (bUp || ButtonDown(27, true))
					{
						menu_beep();
						if (Scroling)
						{
							if (iLow > 0)
							{
								iLow--;
								iHigh--;
							}

							if (iPick == 0)
							{
								iPick = (int)MyMenu.size();
								if (iPick > 6)
								{
									iHigh = iPick;
									iLow = iHigh - 7;
								}
							}

							iPick--;
						}
						else
						{
							if (iPick == 0)
								iPick = (int)MyMenu.size();
							iPick--;
						}

						waitTime = 150;
					}
					else if (bDown || ButtonDown(19, true))
					{
						menu_beep();
						iPick++;
						if (Scroling)
						{
							if (iPick == (int)MyMenu.size())
							{
								iPick = 0;
								iHigh = 7;
								iLow = 0;
							}
							else if (iPick == iHigh && iHigh < (int)MyMenu.size())
							{
								iLow++;
								iHigh++;
							}
						}
						else
						{
							if (iPick == (int)MyMenu.size())
								iPick = 0;
						}
						waitTime = 150;
					}
				}
			}
		}

		CloseBaseHelpBar(iBc);

		return iPick;
	}
	vector<int> MyMenuSysPlus(string caption, vector<PZclass::MenuNumbers> MinMax)
	{

		const float lineWidth = 250.0;
		int iPick = 0;
		int iLow = 0;
		int iHigh = 7;
		bool Scroling = false;
		vector<int> MyPick = {};

		for (int i = 0; i < (int)MinMax.size(); i++)
			MyPick.push_back(MinMax[i].Current);

		if ((int)MinMax.size() < 7)
			iHigh = (int)MinMax.size();
		else
			Scroling = true;

		vector<string> sInstBut = { PZLang[84], PZLang[85] };
		vector<int> iInstBut = { 21, 45 };

		int iBc = BottomRight(iInstBut, sInstBut);
		int iHeld = 150;

		DWORD waitTime = 150;
		while (true)
		{
			if (PlayDead)
			{
				iPick = -1;
				break;
			}
			// timed menu draw, used for pause after active line switch
			DWORD maxTickCount = GetTickCount() + waitTime;
			do
			{
				// draw menu
				draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
				for (int i = iLow; i < iHigh; i++)
					if (i != iPick)
						draw_menu_line(MinMax[i].Title + std::to_string(MyPick[i]), lineWidth, 9.0, 60.0 + (i - iLow) * 36.0, 0.0, 9.0, false, false);
				draw_menu_line(MinMax[iPick].Title + std::to_string(MyPick[iPick]), lineWidth + 1.0, 11.0, 56.0 + (iPick - iLow) * 36.0, 0.0, 7.0, true, false);

				GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(iBc, 255, 255, 255, 255, 0);
				WAIT(0);
			} while (GetTickCount() < maxTickCount);
			waitTime = 0;

			// process buttons
			bool bUp, bDown, bLeft, bRight, bEnter, bBack, bShutDown;
			get_button_state(&bEnter, &bBack, &bUp, &bDown, &bLeft, &bRight, &bShutDown);
			if (bShutDown || ButtonDown(199, false))
			{
				iMenuSys = 0;
				break;
			}
			else if (ButtonDown(21, true) || bEnter)
			{
				menu_beep();
				break;
			}
			else
			{
				if (ButtonDown(45, true)|| bBack)
				{
					menu_beep();
					for (int i = 0; i < (int)MinMax.size(); i++)
						MyPick[i] = MinMax[i].Current;

					break;
				}
				else
				{
					if (bUp || ButtonDown(27, true))
					{
						if (Scroling)
						{
							if (iLow > 0)
							{
								iLow--;
								iHigh--;
							}

							if (iPick == 0)
							{
								iPick = (int)MinMax.size();
								if (iPick > 6)
								{
									iHigh = iPick;
									iLow = iHigh - 7;
								}
							}

							iPick--;
						}
						else
						{
							if (iPick == 0)
								iPick = (int)MinMax.size();
							iPick--;
						}

						waitTime = 150;
					}
					else if (bDown || ButtonDown(19, true))
					{
						iPick++;
						if (Scroling)
						{
							if (iPick == (int)MinMax.size())
							{
								iPick = 0;
								iHigh = 7;
								iLow = 0;
							}
							else if (iPick == iHigh && iHigh < (int)MinMax.size())
							{
								iLow++;
								iHigh++;
							}
						}
						else
						{
							if (iPick == (int)MinMax.size())
								iPick = 0;
						}

						waitTime = 150;
					}
					else if (bLeft || ButtonDown(47, true))
					{
						if (MyPick[iPick] > MinMax[iPick].Min)
							MyPick[iPick] -= 1;

						if (iHeld > 0)
							iHeld--;

						waitTime = iHeld;
					}
					else if (bRight || ButtonDown(46, true))
					{
						if (MyPick[iPick] < MinMax[iPick].Max)
							MyPick[iPick] += 1;

						if (iHeld > 0)
							iHeld--;

						waitTime = iHeld;
					}
					else
						iHeld = 150;
				}
			}
		}

		CloseBaseHelpBar(iBc);

		return MyPick;
	}

	int InGameTime()
	{
		return (int)GAMEPLAY::GET_GAME_TIMER();
	}
	bool IsIsSafe(int Key)
	{
		bool isSafe = false;
		if (!(bool)PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(PLAYER::PLAYER_PED_ID()))
		{
			while (ButtonDown(Key, false))
			{
				WAIT(1000);
				if (ButtonDown(Key, false))
					isSafe = true;
				break;
			}
		}
		return isSafe;
	}
	bool WHileKeyDown(int iKey)
	{
		bool k = false;
		while (IsKeyDown(KeyFind[iKey]))
		{
			if (!k)
				k = true;
			WAIT(1);
		}
		return k;
	}

	void showText(float x, float y, float scale, string text, int font, PZclass::RGBA rgba, bool outline)
	{
		UI::SET_TEXT_FONT(font);
		UI::SET_TEXT_SCALE(scale, scale);
		UI::SET_TEXT_COLOUR(rgba.R, rgba.G, rgba.B, rgba.A);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(0);
		if (outline)
			UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)text.c_str());
		UI::_DRAW_TEXT(x, y);
	}
	void showPlayerLabel(Vector3 location, float baseSize, const std::vector<std::string>& textLines)
	{
		Vector3 cameraPos = CAM::GET_GAMEPLAY_CAM_COORD();
		float distance = DistanceTo(cameraPos, location);
		float totalMult = baseSize / (distance * (CAM::GET_GAMEPLAY_CAM_FOV() / 60.0f));

		float height = 0.0125f * totalMult;
		PZclass::RGBA fontColor = PZclass::RGBA(255, 255, 255, 255);
		GRAPHICS::SET_DRAW_ORIGIN(location.x, location.y, location.z, 0);
		int i = 0;

		for (auto line : textLines) {

			showText(0.0f, 0.0f + height * i, 0.2f * totalMult, line, 0, fontColor, true);
			i++;
		}

		GRAPHICS::CLEAR_DRAW_ORIGIN();
	}
	void BigMessage(string message, string message2, int colour)
	{
		int iScale = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MIDSIZED_MESSAGE");

		WAIT(1500);
		while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(iScale))
			WAIT(1);

		GRAPHICS::_START_SCREEN_EFFECT("SuccessNeutral", 8500, false);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(iScale, "SHOW_SHARD_MIDSIZED_MESSAGE");
		GRAPHICS::_BEGIN_TEXT_COMPONENT("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)message.c_str());
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_BEGIN_TEXT_COMPONENT("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)message2.c_str());
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(colour);// color 0,1=white 2=black 3=grey 6,7,8=red 9,10,11=blue 12,13,14=yellow 15,16,17=orange 18,19,20=green 21,22,23=purple 
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		int iWait4 = InGameTime() + 8000;

		while (iWait4 > InGameTime())
		{
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(iScale, 255, 255, 255, 255, 2);
			WAIT(1);
		}

		GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&iScale);
	}
	void TopLeft(string sText)
	{
		UI::_SET_TEXT_COMPONENT_FORMAT("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)sText.c_str());
		UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, 1, 5000);
	}
	int BottomLeft(string sText)
	{
		if (!PZData::MySettings.NoNotify)
		{
			UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)sText.c_str());
			return UI::_DRAW_NOTIFICATION(1, 1);
		}
	}
	int BottomLeftIcon(string sText1, string sText2, string subject, string badge, string clanTag)
	{
		//badge = "CHAR_SOCIAL_CLUB";
		// clanTag = "__EXAMPLE"

		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)sText1.c_str());
		///char* picName1, char* picName2, BOOL flash, int iconType1, char* sender, char* subject, float duration, char* clanTag, int iconType2, int p9
		UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2((LPSTR)badge.c_str(), (LPSTR)badge.c_str(), 0, 7, (LPSTR)sText2.c_str(), (LPSTR)subject.c_str(), 3.0, (LPSTR)clanTag.c_str(), 7, 0);
		return UI::_DRAW_NOTIFICATION(1, 1);
	}

	void ClearYourFriend(bool Retry)
	{
		if (YouFriend.MyBrain != NULL)
		{
			YouFriend.MyBrain->WanBeFriends = false;

			if (Retry)
				YouFriend.MyBrain->ApprochPlayer = true;
		}

		YouFriend = PZclass::JoinMe("Empty");
	}
	vector<string> LastMessage = {};
	void RightSide(std::string caption, int iPos)
	{
		int text_col[4] = { 255, 255, 255, 255 },
			rect_col[4] = { 0, 0, 0, 120 };
		float text_scale = 0.5;
		int font = 4;

		float lineTop = (float)251.0 + (float)iPos * 22.5;
		float lineHeight = 49.5;
		float lineLeft = 1000.0;
		float textLeft = -10.0;

		// correcting values for active line
		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

		textLeft += lineLeft;

		float lineTopScaled = lineTop / (float)screen_h; // line top offset
		float textLeftScaled = textLeft / (float)screen_w; // text left offset
		float lineHeightScaled = lineHeight / (float)screen_h; // line height

		float lineLeftScaled = lineLeft / (float)screen_w;

		// this is how it's done in original scripts

		// text upper part
		int num25 = AddScreenText(font, caption, textLeftScaled, lineTopScaled, lineHeightScaled, text_scale, text_col);
	}

	int FindKeyBinds(bool Control)
	{
		WAIT(4000);
		int iReturn = -1;
		int TestCase = 0;

		while (true)
		{
			if (PlayDead)
				break;

			TopLeft(PZLang[89]);

			if (Control)
			{
				if (TestCase < (int)ControlSym.size())
				{
					if (ButtonDown(TestCase, false))
					{
						BottomLeft(PZLang[90]);
						iReturn = TestCase;
						break;
					}
					TestCase++;
				}
				else
					TestCase = 0;
			}
			else
			{
				if (TestCase < (int)KeyFind.size())
				{
					if (IsKeyDown(KeyFind[TestCase]))
					{
						BottomLeft(PZLang[90]);
						iReturn = TestCase;
						break;
					}
					TestCase++;
				}
				else
					TestCase = 0;
			}
			WAIT(1);
		}
		return iReturn;
	}

	vector<PZclass::PhoneContact> LoadContacts()
	{
		LoggerLight("LoadContacts");
		vector<PZclass::PhoneContact> PzCont = {};

		string OutputFolder = GetDir() + "/PlayerZero/Contacts";
		if (CreateDirectoryA((LPSTR)OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{

		}
		vector<string> Files = {};
		for (const auto& entry : std::filesystem::directory_iterator(OutputFolder))
			Files.push_back(entry.path().string());

		for (int i = 0; i < Files.size(); i++)
		{
			LoggerLight("LoadContacts == " + Files[i]);

			int intList = 0;

			int BlipC = 38, Level = 101; string Name = "Bob", Id = "Bob101"; int iNat = 1, Gun = -1, PreVeh = 0;

			string model = "mp_m_freemode_01";

			int shapeFirstID = 5, shapeSecondID = 5; float shapeMix = 0.0, skinMix = 0.92, thirdMix = 0.0;

			bool male = true; int hairColour = 7, hairStreaks = 7, eyeColour = 0;

			int comp = 3, text = 4; string HairTag = "H_FMM_3_4", HairName = ""; int overLib = -1, over = 1;

			string Cloths = ""; vector<int> ClothA = {}, ClothB = {}, ExtraA = {}, ExtraB = {};

			vector<PZclass::FreeOverLay> myOverlay = {};
			vector<PZclass::Tattoo> myTattoo = {};

			vector<string> MyColect = ReadFile(Files[i]);

			for (int i = 0; i < MyColect.size(); i++)
			{
				string line = MyColect[i];
				if (StringContains("National", line))
				{
					iNat = StingNumbersInt(line);
				}
				else if (StringContains("BlipColour", line))
				{
					BlipC = StingNumbersInt(line);
				}
				else if (StringContains("Level", line))
				{
					Level = StingNumbersInt(line);
				}
				else if (StringContains("GunSelect", line))
				{
					Gun = StingNumbersInt(line);
				}
				else if (StringContains("PrefredVehicle", line))
				{
					PreVeh = StingNumbersInt(line);
				}
				else if (StringContains("PlayersName", line))
				{
					Name = AfterEqual(line);
				}
				else if (StringContains("PlayersId", line))
				{
					Id = AfterEqual(line);
				}
				else if (StringContains("Model", line))
				{
					model = AfterEqual(line);
				}
				else if (StringContains("ShapeFirstID", line))
				{
					shapeFirstID = StingNumbersInt(line);
				}
				else if (StringContains("ShapeSecondID", line))
				{
					shapeSecondID = StingNumbersInt(line);
				}
				else if (StringContains("ShapeMix", line))
				{
					shapeMix = StingNumbersFloat(line);
				}
				else if (StringContains("SkinMix", line))
				{
					skinMix = StingNumbersFloat(line);
				}
				else if (StringContains("ThirdMix", line))
				{
					thirdMix = StingNumbersFloat(line);
				}
				else if (StringContains("Male", line))
				{
					male = StringBool(line);
				}
				else if (StringContains("HairColour", line))
				{
					hairColour = StingNumbersInt(line);
				}
				else if (StringContains("HairStreaks", line))
				{
					hairStreaks = StingNumbersInt(line);
				}
				else if (StringContains("EyeColour", line))
				{
					eyeColour = StingNumbersInt(line);
				}
				else if (StringContains("Comp", line))
				{
					comp = StingNumbersInt(line);
				}
				else if (StringContains("Text", line))
				{
					text = StingNumbersInt(line);
				}
				else if (StringContains("HairTag", line))
				{
					HairTag = AfterEqual(line);
				}
				else if (StringContains("HairName", line))
				{
					HairName = AfterEqual(line);
				}
				else if (StringContains("OverLib", line))
				{
					overLib = StingNumbersInt(line);
				}
				else if (StringContains("Over", line))
				{
					over = StingNumbersInt(line);
				}
				else if (StringContains("Title", line))
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
					if (StringContains("ClothA", line))
						ClothA.push_back(StingNumbersInt(line));
				}
				else if (intList == 2)
				{
					if (StringContains("ClothB", line))
						ClothB.push_back(StingNumbersInt(line));
				}
				else if (intList == 3)
				{
					if (StringContains("ExtraA", line))
						ExtraA.push_back(StingNumbersInt(line));
				}
				else if (intList == 4)
				{
					if (StringContains("ExtraB", line))
						ExtraB.push_back(StingNumbersInt(line));
				}
				else if (intList == 5)
				{
					if (StringContains("Overlay", line))
						myOverlay.push_back(PZclass::FreeOverLay(StingNumbersInt(line), StingNumbersInt(MyColect[i + 1]), StingNumbersFloat(MyColect[i + 2])));
				}
				else if (intList == 6)
				{
					if (StringContains("BaseName", line))
						myTattoo.push_back(PZclass::Tattoo(AfterEqual(line), AfterEqual(MyColect[i + 1]), AfterEqual(MyColect[i + 2])));
				}
			}

			bool bReBuild = false;

			if (Gun == -1)
			{
				Gun = RandomInt(1, 7);
				PreVeh = 0;
				bReBuild = true;
			}

			PZclass::FaceBank myFaces = PZclass::FaceBank(shapeFirstID, shapeSecondID, shapeMix, skinMix, thirdMix);
			PZclass::ClothX cothing = PZclass::ClothX(Cloths, ClothA, ClothB, ExtraA, ExtraB);
			PZclass::HairSets myHair = PZclass::HairSets(comp, text, HairTag, HairName, overLib, over);

			PZclass::PhoneContact ThisCon = PZclass::PhoneContact(Name, PZclass::PlayerBrain(Name, Id, PZclass::ClothBank(Name, model, cothing, myFaces, male, myHair, hairColour, hairStreaks, eyeColour, myOverlay, myTattoo), 0, Level, true, iNat, Gun, PreVeh));

			PzCont.push_back(ThisCon);

			PlayerNames.push_back(Name);

			if (bReBuild)
				SaveContacts(ThisCon);
		}

		return PzCont;
	}
	void SaveContacts(PZclass::PhoneContact contact)
	{
		LoggerLight("SaveContacts == " + contact.YourFriend.MyName);

		string OutputFolder = GetDir() + "/PlayerZero/Contacts";
		if (CreateDirectoryA((LPSTR)OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{

		}

		string sb = "false";

		if (contact.YourFriend.PFMySetting.Male)
			sb = "true";

		vector<string> MyCon = {
			"[PlayerBrain]",
			"National=" + std::to_string(contact.YourFriend.Nationality),
			"BlipColour=" + std::to_string(contact.YourFriend.BlipColour),
			"Level=" + std::to_string(contact.YourFriend.Level),
			"GunSelect=" + std::to_string(contact.YourFriend.GunSelect),
			"PrefredVehicle=" + std::to_string(contact.YourFriend.PrefredVehicle),
			"PlayersName=" + contact.YourFriend.MyName,
			"PlayersId=" + contact.YourFriend.MyIdentity,
			"[ClothBank]",
			"Model=" + contact.YourFriend.PFMySetting.Model,
			"[FaceBank]",
			"ShapeFirstID=" + std::to_string(contact.YourFriend.PFMySetting.MyFaces.ShapeFirstID),
			"ShapeSecondID=" + std::to_string(contact.YourFriend.PFMySetting.MyFaces.ShapeSecondID),
			"ShapeMix=" + std::to_string(contact.YourFriend.PFMySetting.MyFaces.ShapeMix),
			"SkinMix=" + std::to_string(contact.YourFriend.PFMySetting.MyFaces.SkinMix),
			"ThirdMix=" + std::to_string(contact.YourFriend.PFMySetting.MyFaces.ThirdMix),
			"Male=" + sb,
			"HairColour=" + std::to_string(contact.YourFriend.PFMySetting.HairColour),
			"HairStreaks=" + std::to_string(contact.YourFriend.PFMySetting.HairStreaks),
			"EyeColour=" + std::to_string(contact.YourFriend.PFMySetting.EyeColour),
			"[Hair]",
			"Comp=" + std::to_string(contact.YourFriend.PFMySetting.MyHair.Comp),
			"Text=" + std::to_string(contact.YourFriend.PFMySetting.MyHair.Text),
			"HairTag=" + contact.YourFriend.PFMySetting.MyHair.HandName,
			"HairName=" + contact.YourFriend.PFMySetting.MyHair.Name,
			"OverLib=" + std::to_string(contact.YourFriend.PFMySetting.MyHair.OverLib),
			"Over=" + std::to_string(contact.YourFriend.PFMySetting.MyHair.Over),
			"[ClothX]",
			"Title= " + contact.YourFriend.PFMySetting.Cothing.Title
		};

		MyCon.push_back("[ClothA]");

		for (int i = 0; i < contact.YourFriend.PFMySetting.Cothing.ClothA.size(); i++)
			MyCon.push_back("ClothA=" + std::to_string(contact.YourFriend.PFMySetting.Cothing.ClothA[i]));

		MyCon.push_back("[ClothB]");

		for (int i = 0; i < contact.YourFriend.PFMySetting.Cothing.ClothB.size(); i++)
			MyCon.push_back("ClothB=" + std::to_string(contact.YourFriend.PFMySetting.Cothing.ClothB[i]));

		MyCon.push_back("[ExtraA]");

		for (int i = 0; i < contact.YourFriend.PFMySetting.Cothing.ExtraA.size(); i++)
			MyCon.push_back("ExtraA=" + std::to_string(contact.YourFriend.PFMySetting.Cothing.ExtraA[i]));

		MyCon.push_back("[ExtraB]");

		for (int i = 0; i < contact.YourFriend.PFMySetting.Cothing.ExtraB.size(); i++)
			MyCon.push_back("ExtraB=" + std::to_string(contact.YourFriend.PFMySetting.Cothing.ExtraB[i]));

		MyCon.push_back("[FreeOverLay]");
		for (int i = 0; i < contact.YourFriend.PFMySetting.MyOverlay.size(); i++)
		{
			MyCon.push_back("[OverLay]");
			MyCon.push_back("Overlay=" + std::to_string(contact.YourFriend.PFMySetting.MyOverlay[i].Overlay));
			MyCon.push_back("OverCol=" + std::to_string(contact.YourFriend.PFMySetting.MyOverlay[i].OverCol));
			MyCon.push_back("OverOpc=" + std::to_string(contact.YourFriend.PFMySetting.MyOverlay[i].OverOpc));
		}
		
		MyCon.push_back("[Tattoo]");
		if ((int)contact.YourFriend.PFMySetting.MyTattoo.size() > 0)
		{
			for (int i = 0; i < contact.YourFriend.PFMySetting.MyTattoo.size(); i++)
			{
				MyCon.push_back("[Tat]");
				MyCon.push_back("BaseName=" + contact.YourFriend.PFMySetting.MyTattoo[i].BaseName);
				MyCon.push_back("TatsDesc=" + contact.YourFriend.PFMySetting.MyTattoo[i].Name);
				MyCon.push_back("TatName=" + contact.YourFriend.PFMySetting.MyTattoo[i].TatName);
			}
		}

		ofstream MyFile(GetDir() + "/PlayerZero/Contacts/" + contact.YourFriend.MyName + ".ini");

		for (int i = 0; i < MyCon.size(); i++)
		{
			MyFile << "\n";
			MyFile << MyCon[i];
		}

		MyFile.close();
	}

	void SaveOutfits(PZclass::ClothX cloths, bool male)
	{
		LoggerLight("Savecloths == " + cloths.Title);

		string OutputFolder;

		if (male)
			OutputFolder = GetDir() + "/Outfits/Male";
		else
			OutputFolder = GetDir() + "/Outfits/Female";

		vector<string> MyCon = {
			"[ClothX]",
			"Title= " + cloths.Title
		};

		MyCon.push_back("[ClothA]");

		for (int i = 0; i < cloths.ClothA.size(); i++)
			MyCon.push_back("ClothA=" + std::to_string(cloths.ClothA[i]));

		MyCon.push_back("[ClothB]");

		for (int i = 0; i < cloths.ClothB.size(); i++)
			MyCon.push_back("ClothB=" + std::to_string(cloths.ClothB[i]));

		MyCon.push_back("[ExtraA]");

		for (int i = 0; i < cloths.ExtraA.size(); i++)
			MyCon.push_back("ExtraA=" + std::to_string(cloths.ExtraA[i]));

		MyCon.push_back("[ExtraB]");

		for (int i = 0; i < cloths.ExtraB.size(); i++)
			MyCon.push_back("ExtraB=" + std::to_string(cloths.ExtraB[i]));

		ofstream MyFile(OutputFolder + "/" + cloths.Title + ".ini");

		for (int i = 0; i < MyCon.size(); i++)
		{
			MyFile << "\n";
			MyFile << MyCon[i];
		}

		MyFile.close();
	}
	void OutfitFolderTest()
	{
		const std::vector<std::string> ReadMe = {
			"This txt file is a marker that this folder has Outfits to load.",
			"If the any of the two folders are empty delete this txt file.",
			"If You wish to manualy alter any of the files or add your own,",
			"Title does not have to match file name and the mod searches the",
			"the assoosiated folder for any .ini.",
			"While i've got youur attention consider subing my youtube channel as i get a lot of views but no subs means no revenue.",
			"the mods i've made have taken years of unpaid work to complete and the few coins from youtube would help a lot.",
			"",
			"",
			"",
			"",
			"https://www.youtube.com/@adopcalipt"
		};
		const std::vector<PZclass::ClothX> MaleOut = {
			PZclass::ClothX("MaleBeachThe Bare Chest", { 0, 0, -1, 15, 16, 0, 1, 16, 15, 0, 0, 15 }, { 0, 0, 0, 0, 1, 0, 7, 2, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Dude", { 0, 0, -1, 5, 15, 0, 16, 0, 15, 0, 0, 17 }, { 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 4 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Heat", { 0, 0, -1, 5, 18, 0, 16, 0, 15, 0, 0, 5 }, { 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 7 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Paradise", { 0, 0, -1, 0, 18, 0, 1, 0, 15, 0, 0, 1 }, { 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Skimpy", { 0, 0, -1, 15, 18, 0, 5, 0, 15, 0, 0, 15 }, { 0, 0, 0, 0, 11, 0, 3, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Summer Rays", { 0, 0, -1, 5, 16, 0, 16, 17, 15, 0, 0, 17 }, { 0, 0, 0, 0, 2, 0, 8, 2, 0, 0, 0, 3 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Surfer", { 0, 0, -1, 5, 12, 0, 16, 0, 15, 0, 0, 17 }, { 0, 0, 0, 0, 4, 0, 6, 0, 0, 0, 0, 5 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBeachThe Tidal Wave", { 0, 0, -1, 0, 6, 0, 16, 0, 15, 0, 0, 44 }, { 0, 0, 0, 0, 10, 0, 6, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerAvarus", { 0, 0, -1, 19, 72, 0, 53, 0, 82, 0, 0, 157 }, { 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 }, { 83, 24, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerBlazer", { 0, 0, -1, 14, 62, 0, 7, 0, 23, 0, 0, 167 }, { 0, 0, 0, 0, 2, 0, 15, 0, 2, 0, 0, 3 }, { 76, 0, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerChimera", { 0, 0, -1, 130, 73, 0, 56, 0, 15, 0, 0, 162 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3 }, { 83, 8, -1, -1, -1 }, { 0, 6, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerFaggio", { 0, 0, 10, 6, 4, 0, 23, 0, 15, 0, 0, 184 }, { 0, 0, 0, 0, 4, 0, 9, 0, 0, 0, 0, 1 }, { 29, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerHakuchou", { 0, 0, -1, 6, 9, 0, 51, 0, 15, 0, 0, 165 }, { 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 1 }, { 83, 25, -1, -1, -1 }, { 5, 7, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerManchez", { 0, 0, -1, 0, 69, 0, 7, 0, 83, 0, 0, 180 }, { 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 2 }, { 77, 25, -1, -1, -1 }, { 18, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerNightblade", { 0, 0, -1, 116, 74, 0, 52, 0, 15, 0, 0, 173 }, { 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 2 }, { -1, 8, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerRat Bike", { 0, 0, -1, 30, 72, 0, 50, 0, 83, 0, 0, 158 }, { 0, 0, 0, 1, 4, 0, 0, 0, 8, 0, 0, 1 }, { -1, 24, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerWolfsbane", { 0, 0, -1, 24, 76, 0, 56, 0, 15, 0, 0, 174 }, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 }, { -1, 9, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBikerZombie", { 0, 0, -1, 22, 4, 0, 53, 0, 75, 0, 0, 181 }, { 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0 }, { -1, 0, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Alligator", { 0, 0, -1, 4, 23, 0, 20, 12, 10, 0, 0, 23 }, { 0, 0, 0, 0, 8, 0, 8, 2, 2, 0, 0, 0 }, { 5, 7, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Bulldog", { 0, 0, -1, 11, 10, 0, 11, 10, 15, 0, 0, 26 }, { 0, 0, 0, 0, 0, 0, 12, 2, 0, 0, 0, 9 }, { 12, 17, -1, -1, -1 }, { 0, 6, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Collector", { 0, 0, -1, 11, 23, 0, 21, 25, 6, 0, 0, 25 }, { 0, 0, 0, 0, 8, 0, 6, 13, 12, 0, 0, 9 }, { 12, 18, -1, -1, -1 }, { 4, 7, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Devil Chic", { 0, 0, -1, 4, 4, 0, 20, 25, 4, 0, 0, 24 }, { 0, 0, 0, 0, 0, 0, 7, 2, 0, 0, 0, 3 }, { 12, 18, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Eccentric", { 0, 0, -1, 11, 23, 0, 1, 22, 6, 0, 0, 25 }, { 0, 0, 0, 0, 2, 0, 11, 8, 7, 0, 0, 5 }, { 27, -1, -1, -1, -1 }, { 5, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe European", { 0, 0, -1, 11, 23, 0, 10, 12, 15, 0, 0, 26 }, { 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 5 }, { 26, 18, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Mogul", { 0, 0, -1, 11, 22, 0, 20, 25, 6, 0, 0, 25 }, { 0, 0, 0, 0, 7, 0, 2, 10, 11, 0, 0, 0 }, { -1, 18, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Mr. Frost", { 0, 0, -1, 4, 4, 0, 20, 25, 4, 0, 0, 24 }, { 0, 0, 0, 0, 2, 0, 5, 2, 0, 0, 0, 4 }, { 7, 18, -1, -1, -1 }, { 1, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Tally-ho", { 0, 0, -1, 4, 22, 0, 20, 25, 4, 0, 0, 24 }, { 0, 0, 0, 0, 11, 0, 6, 4, 1, 0, 0, 0 }, { 7, -1, -1, -1, -1 }, { 3, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness CasualThe Victorious", { 0, 0, -1, 14, 22, 0, 20, 21, 28, 0, 0, 27 }, { 0, 0, 0, 0, 1, 0, 11, 1, 8, 0, 0, 1 }, { 26, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Collegiate", { 0, 0, -1, 4, 23, 0, 20, 25, 26, 0, 0, 23 }, { 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 2 }, { 26, 17, -1, -1, -1 }, { 2, 4, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Country Club", { 0, 0, -1, 4, 23, 0, 21, 0, 25, 0, 0, 24 }, { 0, 0, 0, 0, 10, 0, 11, 0, 11, 0, 0, 10 }, { 26, -1, -1, -1, -1 }, { 12, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Discoteque", { 0, 0, -1, 4, 22, 0, 10, 25, 26, 0, 0, 23 }, { 0, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 3 }, { 12, 18, -1, -1, -1 }, { 1, 3, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Egotist", { 0, 0, -1, 4, 23, 0, 20, 25, 26, 0, 0, 24 }, { 0, 0, 0, 0, 3, 0, 3, 15, 3, 0, 0, 3 }, { -1, 13, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Gentry", { 0, 0, -1, 4, 22, 0, 20, 22, 26, 0, 0, 24 }, { 0, 0, 0, 0, 6, 0, 0, 10, 5, 0, 0, 6 }, { 27, -1, -1, -1, -1 }, { 8, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe High Roller", { 0, 0, -1, 14, 10, 0, 20, 22, 26, 0, 0, 27 }, { 0, 0, 0, 0, 0, 0, 7, 4, 3, 0, 0, 1 }, { 27, -1, -1, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Panther", { 0, 0, -1, 4, 10, 0, 10, 25, 4, 0, 0, 28 }, { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0 }, { -1, 17, -1, -1, -1 }, { 0, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Sophisticate", { 0, 0, -1, 4, 13, 0, 20, 21, 10, 0, 0, 24 }, { 0, 0, 0, 0, 0, 0, 8, 11, 10, 0, 0, 1 }, { 27, 18, -1, -1, -1 }, { 4, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Tycoon", { 0, 0, -1, 4, 10, 0, 10, 25, 4, 0, 0, 28 }, { 0, 0, 0, 0, 2, 0, 0, 14, 2, 0, 0, 2 }, { 26, 17, -1, -1, -1 }, { 2, 6, 0, 0, 0 }),
			PZclass::ClothX("MaleBusiness SmartThe Yeehaw", { 0, 0, -1, 1, 22, 0, 20, 0, 12, 0, 0, 24 }, { 0, 0, 0, 0, 6, 0, 2, 0, 5, 0, 0, 7 }, { 13, 18, -1, -1, -1 }, { 2, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCasualThe Denims", { 0, 0, -1, 8, 4, 0, 4, 0, 15, 0, 0, 38 }, { 0, 0, 0, 0, 4, 0, 1, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCasualThe Hangout", { 0, 0, -1, 0, 0, 0, 1, 17, 15, 0, 0, 33 }, { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCasualThe Mellow", { 0, 0, -1, 12, 1, 0, 1, 0, 15, 0, 0, 41 }, { 0, 0, 0, 0, 14, 0, 4, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCasualThe Plain White", { 0, 0, -1, 0, 0, 0, 0, 0, 15, 0, 0, 1 }, { 0, 0, 0, 0, 2, 0, 10, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCasualThe Simple", { 0, 0, -1, 0, 1, 0, 1, 0, 15, 0, 0, 22 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOAlpha", { 0, 0, -1, 4, 82, 0, 12, 15, 0, 0, 0, 192 }, { 0, 0, 0, 0, 6, 0, 3, 0, 1, 0, 0, 1 }, { -1, 8, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOAnonymous", { 0, 106, -1, 131, 87, 0, 62, 0, 15, 0, 0, 205 }, { 0, 25, 0, 0, 9, 0, 3, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOBroker", { 0, 0, -1, 4, 37, 0, 20, 38, 10, 0, 0, 142 }, { 0, 0, 0, 0, 2, 0, 5, 6, 2, 0, 0, 2 }, { 29, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOChief", { 0, 0, -1, 12, 37, 0, 20, 0, 73, 0, 0, 137 }, { 0, 0, 0, 0, 2, 0, 5, 0, 1, 0, 0, 0 }, { -1, 17, -1, -1, -1 }, { 0, 9, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOClown", { 0, 95, -1, 44, 39, 0, 27, 0, 15, 0, 0, 66 }, { 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOCommander", { 0, 0, -1, 22, 92, 0, 24, 0, 15, 0, 0, 228 }, { 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 14 }, { 114, 5, -1, -1, -1 }, { 10, 1, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOCreator", { 0, 0, -1, 12, 13, 0, 7, 0, 11, 0, 0, 35 }, { 0, 0, 0, 0, 0, 0, 15, 0, 2, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEODemon", { 0, 94, -1, 33, 79, 0, 52, 0, 15, 0, 0, 153 }, { 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 7 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEODirector", { 0, 0, -1, 4, 37, 0, 15, 28, 31, 0, 0, 140 }, { 0, 0, 0, 0, 0, 0, 10, 1, 0, 0, 0, 2 }, { -1, 17, -1, -1, -1 }, { 0, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOFat Cat", { 0, 0, -1, 4, 20, 0, 40, 11, 35, 0, 0, 27 }, { 0, 0, 0, 0, 2, 0, 9, 2, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOFounder", { 0, 0, -1, 0, 0, 0, 2, 0, 15, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 6, 0, 0, 0, 0, 1 }, { -1, -1, 0, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOGeneral", { 0, 0, -1, 4, 9, 0, 27, 0, 15, 0, 0, 138 }, { 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 2 }, { 6, 16, -1, -1, -1 }, { 5, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOGuerilla", { 0, 0, -1, 141, 87, 0, 60, 0, 15, 0, 0, 221 }, { 0, 0, 0, 16, 16, 0, 3, 0, 0, 0, 0, 9 }, { 108, 5, -1, -1, -1 }, { 16, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOGunrunner", { 0, 0, -1, 145, 87, 0, 62, 0, 15, 0, 0, 222 }, { 0, 0, 0, 19, 17, 0, 7, 0, 0, 0, 0, 23 }, { 107, 5, -1, -1, -1 }, { 17, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOHedonist", { 0, 0, -1, 14, 56, 0, 37, 0, 15, 0, 0, 114 }, { 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5 }, { 13, 8, -1, -1, -1 }, { 2, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOIcon", { 0, 0, -1, 6, 83, 0, 29, 89, 15, 0, 0, 190 }, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 }, { 96, -1, -1, -1, -1 }, { 6, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOInstigator", { 0, 107, -1, 41, 89, 0, 61, 0, 15, 0, 0, 208 }, { 0, 18, 0, 0, 20, 0, 0, 0, 0, 0, 0, 11 }, { 96, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOMastermind", { 0, 0, -1, 35, 79, 0, 43, 0, 15, 0, 0, 204 }, { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 }, { 96, 7, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOMoon Curser", { 0, 104, -1, 141, 87, 0, 60, 0, 15, 0, 0, 220 }, { 0, 25, 0, 3, 3, 0, 0, 0, 0, 0, 0, 20 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOPioneer", { 0, 0, -1, 4, 63, 0, 2, 0, 15, 0, 0, 139 }, { 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 3 }, { -1, 3, -1, -1, -1 }, { 0, 9, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOPlayboy", { 0, 0, -1, 4, 60, 0, 36, 0, 72, 0, 0, 108 }, { 0, 0, 0, 0, 2, 0, 3, 0, 3, 0, 0, 4 }, { -1, 13, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOPremier", { 0, 0, -1, 23, 43, 0, 57, 0, 15, 4, 0, 5 }, { 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 2 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOPresident", { 0, 0, -1, 6, 7, 0, 28, 0, 15, 0, 0, 75 }, { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 10 }, { 66, 18, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("MaleCEORecon Leader", { 0, 0, -1, 37, 33, 0, 27, 0, 15, 10, 0, 222 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 20 }, { 112, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEORingleader", { 0, 0, -1, 4, 78, 0, 57, 0, 0, 0, 0, 191 }, { 0, 0, 0, 0, 7, 0, 4, 0, 5, 0, 0, 0 }, { 55, 13, -1, -1, -1 }, { 19, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleCEORoller", { 0, 0, -1, 0, 43, 0, 57, 51, 81, 0, 0, 170 }, { 0, 0, 0, 0, 1, 0, 6, 0, 2, 0, 0, 3 }, { -1, 18, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOStanduot", { 0, 0, -1, 0, 82, 0, 57, 0, 15, 0, 0, 193 }, { 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 5 }, { 96, 7, -1, -1, -1 }, { 1, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOSurvivalist", { 0, 0, -1, 141, 87, 0, 60, 0, 101, 0, 0, 212 }, { 0, 0, 0, 19, 6, 0, 7, 0, 6, 0, 0, 18 }, { 105, 23, -1, -1, -1 }, { 23, 9, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOTop Dog", { 0, 0, -1, 0, 43, 0, 57, 15, 15, 0, 0, 193 }, { 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 22 }, { 55, 2, 32, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOTrailblazer", { 0, 0, -1, 4, 78, 0, 57, 123, 23, 0, 0, 191 }, { 0, 0, 0, 0, 6, 0, 9, 0, 0, 0, 0, 23 }, { 96, 7, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOVillain", { 0, 101, -1, 33, 78, 0, 57, 0, 71, 0, 0, 203 }, { 0, 13, 0, 0, 2, 0, 10, 0, 3, 0, 0, 25 }, { 102, 13, -1, -1, -1 }, { 7, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleCEOWarlord", { 0, 0, -1, 6, 59, 0, 27, 85, 5, 0, 0, 35 }, { 0, 0, 0, 0, 7, 0, 0, 0, 2, 0, 0, 1 }, { 55, 23, -1, -1, -1 }, { 1, 9, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Deluxe", { 0, 0, -1, 14, 4, 0, 29, 49, 65, 0, 0, 74 }, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }, { 29, 18, 20, -1, -1 }, { 1, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Exclusive", { 0, 0, -1, 4, 4, 0, 28, 0, 15, 0, 0, 75 }, { 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 10 }, { 6, 13, 17, -1, -1 }, { 1, 0, 2, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Flash", { 0, 0, -1, 14, 26, 0, 28, 0, 23, 0, 0, 74 }, { 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6 }, { 7, 8, 5, -1, -1 }, { 0, 0, 2, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Grand", { 0, 0, -1, 6, 26, 0, 28, 52, 5, 0, 0, 70 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 }, { -1, 13, 14, -1, -1 }, { 0, 2, 2, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Luxor", { 0, 0, -1, 14, 7, 0, 22, 50, 65, 0, 0, 74 }, { 0, 0, 0, 0, 4, 0, 11, 0, 3, 0, 0, 5 }, { -1, 18, 17, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Midas", { 0, 0, -1, 0, 37, 0, 29, 50, 15, 0, 0, 71 }, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, { 4, 18, 8, -1, -1 }, { 1, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Perseus", { 0, 0, -1, 0, 7, 0, 29, 0, 15, 0, 0, 73 }, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 5 }, { -1, 13, 14, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Pimp", { 0, 0, -1, 4, 26, 0, 28, 54, 10, 0, 0, 70 }, { 0, 0, 0, 0, 9, 0, 4, 0, 2, 0, 0, 1 }, { 12, 10, 14, -1, -1 }, { 4, 4, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Refined", { 0, 0, -1, 50, 37, 0, 12, 0, 23, 0, 0, 72 }, { 0, 0, 0, 0, 2, 0, 3, 0, 1, 0, 0, 1 }, { 7, -1, 20, -1, -1 }, { 2, 0, 4, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Sessanta Nove", { 0, 0, -1, 6, 4, 0, 29, 49, 5, 0, 0, 74 }, { 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3 }, { 7, 18, 11, -1, -1 }, { 2, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Talent", { 0, 0, -1, 1, 26, 0, 28, 53, 65, 0, 0, 62 }, { 0, 0, 0, 0, 11, 0, 3, 1, 10, 0, 0, 0 }, { 4, 7, 20, -1, -1 }, { 0, 0, 3, 0, 0 }),
			PZclass::ClothX("MaleDesignerThe Vogue", { 0, 0, -1, 6, 26, 0, 28, 55, 5, 0, 0, 72 }, { 0, 0, 0, 0, 6, 0, 5, 1, 0, 0, 0, 0 }, { -1, 10, 8, -1, -1 }, { 0, 1, 1, 0, 0 }),
			PZclass::ClothX("MaleFlashyThe Blues", { 0, 0, -1, 12, 25, 0, 10, 29, 31, 0, 0, 31 }, { 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 2 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleFlashyThe Musician", { 0, 0, -1, 6, 4, 0, 10, 0, 5, 0, 0, 4 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleFlashyThe Royal", { 0, 0, -1, 6, 26, 0, 20, 0, 5, 0, 0, 4 }, { 0, 0, 0, 0, 0, 0, 3, 0, 2, 0, 0, 14 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleFlashyThe V.I.P.", { 0, 0, -1, 12, 24, 0, 10, 27, 35, 0, 0, 30 }, { 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterActivist", { 0, 0, -1, 8, 26, 0, 23, 0, 15, 0, 0, 38 }, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1 }, { 28, 20, -1, -1, -1 }, { 5, 1, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterAficionado", { 0, 0, -1, 1, 26, 0, 22, 0, 38, 0, 0, 35 }, { 0, 0, 0, 0, 11, 0, 8, 0, 0, 0, 0, 0 }, { 28, 20, -1, -1, -1 }, { 2, 3, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterArtist", { 0, 0, -1, 11, 28, 0, 23, 0, 15, 0, 0, 43 }, { 0, 0, 0, 0, 12, 0, 4, 0, 0, 0, 0, 0 }, { 29, 20, -1, -1, -1 }, { 6, 0, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterEco", { 0, 0, -1, 11, 26, 0, 22, 11, 15, 0, 0, 42 }, { 0, 0, 0, 0, 3, 0, 7, 2, 0, 0, 0, 0 }, { 28, 20, -1, -1, -1 }, { 3, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterElitist", { 0, 0, -1, 1, 26, 0, 23, 0, 43, 0, 0, 35 }, { 0, 0, 0, 0, 7, 0, 6, 0, 3, 0, 0, 3 }, { -1, 19, -1, -1, -1 }, { 0, 6, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterFitness", { 0, 0, -1, 0, 27, 0, 22, 0, 15, 0, 0, 39 }, { 0, 0, 0, 0, 5, 0, 10, 0, 0, 0, 0, 0 }, { 29, 19, -1, -1, -1 }, { 7, 2, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterIronic", { 0, 0, -1, 11, 26, 0, 23, 11, 6, 0, 0, 40 }, { 0, 0, 0, 0, 9, 0, 3, 2, 12, 0, 0, 0 }, { 29, 20, -1, -1, -1 }, { 5, 6, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterNatural", { 0, 0, -1, 1, 26, 0, 22, 0, 38, 0, 0, 35 }, { 0, 0, 0, 0, 6, 0, 0, 0, 1, 0, 0, 2 }, { 29, 19, -1, -1, -1 }, { 2, 4, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterPop Up", { 0, 0, -1, 0, 28, 0, 23, 30, 15, 0, 0, 33 }, { 0, 0, 0, 0, 2, 0, 14, 4, 0, 0, 0, 0 }, { 28, 20, -1, -1, -1 }, { 1, 5, 0, 0, 0 }),
			PZclass::ClothX("MaleHipsterUrban Hippy", { 0, 0, -1, 8, 26, 0, 22, 30, 15, 0, 0, 38 }, { 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 4 }, { 28, 20, -1, -1, -1 }, { 0, 7, 0, 0, 0 })
		};
		const std::vector<PZclass::ClothX> FemaleOut = {
			PZclass::ClothX("FemaleBeachThe Bather", { 0, 0, -1, 11, 17, 0, 16, 11, 3, 0, 0, 36 }, { 0, 0, 0, 0, 7, 0, 8, 2, 0, 0, 0, 3 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Beach Babe", { 0, 0, -1, 15, 12, 0, 3, 11, 3, 0, 0, 18 }, { 0, 0, 0, 0, 14, 0, 13, 1, 0, 0, 0, 9 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Day Tripper", { 0, 0, -1, 5, 16, 0, 16, 10, 16, 0, 0, 31 }, { 0, 0, 0, 0, 10, 0, 7, 0, 4, 0, 0, 5 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Lifeguard", { 0, 0, -1, 11, 17, 0, 16, 3, 3, 0, 0, 11 }, { 0, 0, 0, 0, 4, 0, 7, 1, 0, 0, 0, 10 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Siesta", { 0, 0, -1, 15, 25, 0, 16, 1, 3, 0, 0, 18 }, { 0, 0, 0, 0, 2, 0, 9, 2, 0, 0, 0, 10 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Sun Kissed", { 0, 0, -1, 15, 16, 0, 16, 9, 3, 0, 0, 18 }, { 0, 0, 0, 0, 8, 0, 1, 0, 0, 0, 0, 6 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBeachThe Tropical", { 0, 0, -1, 9, 16, 0, 15, 14, 3, 0, 0, 17 }, { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Avarus", { 0, 0, -1, 31, 78, 0, 54, 0, 86, 0, 0, 181 }, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, { 82, 26, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Blazer", { 0, 0, -1, 3, 73, 0, 11, 0, 72, 0, 0, 164 }, { 0, 0, 0, 0, 3, 0, 3, 0, 2, 0, 0, 3 }, { 75, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Chimera", { 0, 0, -1, 147, 76, 0, 53, 0, 15, 0, 0, 159 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3 }, { 82, 11, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Faggio", { 0, 0, -1, 3, 0, 0, 13, 0, 3, 0, 15, 186 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Hakuchou", { 0, 0, -1, 3, 11, 0, 51, 0, 3, 0, 0, 162 }, { 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 1 }, { 82, 27, -1, -1, -1 }, { 5, 7, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Manchez", { 0, 0, -1, 4, 71, 0, 11, 0, 3, 0, 0, 171 }, { 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 1 }, { 76, 27, -1, -1, -1 }, { 6, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Nightblade", { 0, 0, -1, 133, 77, 0, 53, 0, 17, 0, 0, 175 }, { 0, 0, 0, 1, 2, 0, 0, 0, 3, 0, 0, 2 }, { -1, 11, -1, -1, -1 }, { 0, 4, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Rat Bike", { 0, 0, -1, 24, 76, 0, 51, 0, 3, 0, 0, 173 }, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, { -1, 26, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Wolfsbane", { 0, 0, -1, 27, 74, 0, 51, 0, 3, 0, 0, 176 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { -1, 10, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleBikerThe Zombie", { 0, 0, -1, 23, 0, 0, 56, 0, 77, 0, 0, 163 }, { 0, 0, 0, 0, 8, 0, 0, 0, 3, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Art Dealer", { 0, 0, -1, 5, 23, 0, 0, 2, 23, 0, 0, 24 }, { 0, 0, 0, 0, 1, 0, 2, 5, 3, 0, 0, 7 }, { 14, -1, -1, -1, -1 }, { 5, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Aviator", { 0, 0, -1, 0, 11, 0, 8, 13, 24, 0, 0, 28 }, { 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 0, 4 }, { 27, 2, -1, -1, -1 }, { 8, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Ball-buster", { 0, 0, -1, 5, 23, 0, 0, 0, 13, 0, 0, 24 }, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 2 }, { 26, 2, -1, -1, -1 }, { 3, 5, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Boardroom", { 0, 0, -1, 5, 6, 0, 20, 6, 23, 0, 0, 6 }, { 0, 0, 0, 0, 2, 0, 7, 4, 6, 0, 0, 2 }, { 26, 18, -1, -1, -1 }, { 2, 7, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Designer", { 0, 0, -1, 6, 23, 0, 19, 11, 23, 0, 0, 25 }, { 0, 0, 0, 0, 10, 0, 5, 1, 1, 0, 0, 10 }, { 27, 16, -1, -1, -1 }, { 9, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Director", { 0, 0, -1, 6, 6, 0, 19, 0, 0, 0, 0, 25 }, { 0, 0, 0, 0, 0, 0, 4, 0, 12, 0, 0, 1 }, { -1, 19, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Fashionista", { 0, 0, -1, 6, 23, 0, 6, 1, 23, 0, 0, 25 }, { 0, 0, 0, 0, 11, 0, 2, 2, 3, 0, 0, 7 }, { 26, 16, -1, -1, -1 }, { 9, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Liberty", { 0, 0, -1, 0, 23, 0, 19, 13, 2, 0, 0, 27 }, { 0, 0, 0, 0, 4, 0, 8, 2, 0, 0, 0, 4 }, { 8, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Networker", { 0, 0, -1, 5, 23, 0, 19, 1, 23, 0, 0, 24 }, { 0, 0, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0 }, { -1, 18, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness PantsThe Parisienne", { 0, 0, -1, 5, 23, 0, 20, 6, 0, 0, 0, 24 }, { 0, 0, 0, 0, 12, 0, 11, 0, 13, 0, 0, 9 }, { 13, -1, -1, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe 9-5", { 0, 0, -1, 0, 24, 0, 19, 13, 2, 0, 0, 27 }, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 }, { -1, 18, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Architect", { 0, 0, -1, 5, 24, 0, 19, 1, 13, 0, 0, 24 }, { 0, 0, 0, 0, 7, 0, 9, 1, 8, 0, 0, 3 }, { 13, 1, -1, -1, -1 }, { 1, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Creative", { 0, 0, -1, 5, 24, 0, 20, 6, 23, 0, 0, 24 }, { 0, 0, 0, 0, 0, 0, 9, 1, 3, 0, 0, 10 }, { -1, 10, -1, -1, -1 }, { 0, 5, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Extrovert", { 0, 0, -1, 5, 24, 0, 20, 10, 23, 0, 0, 24 }, { 0, 0, 0, 0, 10, 0, 8, 0, 12, 0, 0, 1 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Firm", { 0, 0, -1, 6, 8, 0, 0, 11, 23, 0, 0, 7 }, { 0, 0, 0, 0, 4, 0, 0, 3, 1, 0, 0, 1 }, { 26, -1, -1, -1, -1 }, { 13, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Fresco", { 0, 0, -1, 5, 24, 0, 20, 11, 0, 0, 0, 24 }, { 0, 0, 0, 0, 5, 0, 7, 3, 15, 0, 0, 8 }, { -1, 18, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Heritage", { 0, 0, -1, 0, 12, 0, 8, 13, 24, 0, 0, 28 }, { 0, 0, 0, 0, 2, 0, 0, 5, 2, 0, 0, 3 }, { 27, 4, -1, -1, -1 }, { 10, 4, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Leopard", { 0, 0, -1, 6, 24, 0, 0, 1, 13, 0, 0, 25 }, { 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 9 }, { -1, 19, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Purrfectionist", { 0, 0, -1, 4, 24, 0, 0, 2, 2, 0, 0, 13 }, { 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 15 }, { 26, -1, -1, -1, -1 }, { 8, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleBusiness SkirtsThe Vanquish", { 0, 0, -1, 5, 8, 0, 19, 6, 23, 0, 0, 24 }, { 0, 0, 0, 0, 0, 0, 4, 1, 1, 0, 0, 6 }, { 26, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Casual", { 0, 0, -1, 0, 16, 0, 2, 2, 3, 0, 0, 0 }, { 0, 0, 0, 0, 4, 0, 5, 1, 0, 0, 0, 11 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Comfort", { 0, 0, -1, 2, 2, 0, 2, 5, 3, 0, 0, 2 }, { 0, 0, 0, 0, 2, 0, 0, 4, 0, 0, 0, 6 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Daily", { 0, 0, -1, 9, 4, 0, 13, 1, 3, 0, 0, 9 }, { 0, 0, 0, 0, 9, 0, 12, 2, 0, 0, 0, 9 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Easy", { 0, 0, -1, 3, 2, 0, 16, 2, 3, 0, 0, 3 }, { 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 11 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Homebody", { 0, 0, -1, 2, 3, 0, 16, 1, 3, 0, 0, 2 }, { 0, 0, 0, 0, 7, 0, 11, 0, 0, 0, 0, 15 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCasualThe Laid Back", { 0, 0, -1, 3, 3, 0, 16, 1, 3, 0, 0, 3 }, { 0, 0, 0, 0, 11, 0, 1, 3, 0, 0, 0, 10 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsBlack & Red", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 13 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsBlack", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsBold Abstract", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 15 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsBrown", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsCobble", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 11 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsCrosshatch", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 12 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsGray Camo", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 8 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsGray", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsLight Woodland", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 9 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsOx Blood", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsPatterned", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 14 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsSplinter", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsTan", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 3, 0, 3, 0, 0, 0, 0, 3 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsTiger", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 16 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsWorn Brown", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 6, 0, 2, 0, 0, 0, 0, 6 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsWorn Gray", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCatsuitsYellow", { 21, 0, 0, 7, 102, 0, 77, 0, 14, 0, 0, 262 }, { 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 5 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOAlpha", { 0, 0, -1, 3, 84, 0, 30, 0, 51, 0, 0, 194 }, { 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 1 }, { -1, 11, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOAnonymous", { 0, 106, -1, 147, 90, 0, 65, 0, 15, 0, 0, 207 }, { 0, 25, 0, 0, 9, 0, 3, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOArms Dealer", { 0, 0, -1, 3, 41, 0, 29, 20, 39, 0, 0, 97 }, { 0, 0, 0, 0, 2, 0, 2, 5, 4, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOBroker", { 0, 0, -1, 7, 54, 0, 0, 22, 38, 0, 0, 139 }, { 0, 0, 0, 0, 2, 0, 1, 6, 2, 0, 0, 2 }, { 28, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOBusiness Oligarch", { 0, 0, -1, 36, 41, 0, 29, 0, 67, 0, 0, 107 }, { 0, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0 }, { -1, 10, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOChief", { 0, 0, -1, 3, 24, 0, 19, 0, 75, 0, 0, 134 }, { 0, 0, 0, 0, 3, 0, 9, 0, 1, 0, 0, 0 }, { -1, 14, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOClown", { 0, 95, -1, 49, 39, 0, 26, 0, 3, 0, 0, 60 }, { 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOCommander", { 0, 0, -1, 23, 95, 0, 24, 0, 14, 0, 0, 238 }, { 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 14 }, { 113, 11, -1, -1, -1 }, { 10, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOCreator", { 0, 0, -1, 7, 27, 0, 11, 0, 39, 0, 0, 66 }, { 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEODemon", { 0, 94, -1, 36, 81, 0, 53, 0, 3, 0, 0, 150 }, { 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 7 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEODirector", { 0, 0, -1, 7, 64, 0, 0, 0, 13, 0, 0, 137 }, { 0, 0, 0, 0, 3, 0, 0, 0, 6, 0, 0, 2 }, { -1, 18, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEODrug Baron", { 0, 0, -1, 9, 0, 0, 38, 0, 2, 0, 0, 96 }, { 0, 0, 0, 0, 10, 0, 2, 0, 0, 0, 0, 0 }, { -1, 11, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOEl Jefe", { 0, 0, -1, 3, 0, 0, 30, 0, 2, 18, 0, 103 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1 }, { 6, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOEntrepreneur", { 0, 0, -1, 3, 43, 0, 4, 84, 65, 0, 0, 100 }, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 55, -1, -1, -1, -1 }, { 24, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOFat Cat", { 0, 0, -1, 3, 64, 0, 6, 23, 41, 0, 0, 58 }, { 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOFounder", { 0, 0, -1, 2, 0, 0, 10, 0, 2, 0, 0, 2 }, { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1 }, { -1, -1, 0, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOGeneral", { 0, 0, -1, 3, 11, 0, 30, 0, 3, 0, 0, 135 }, { 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 2 }, { 14, 0, -1, -1, -1 }, { 0, 4, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOGuerrilla", { 0, 0, -1, 174, 90, 0, 63, 0, 3, 0, 0, 231 }, { 0, 0, 0, 16, 16, 0, 3, 0, 0, 0, 0, 9 }, { 107, 11, -1, -1, -1 }, { 16, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOGunrunner", { 0, 0, -1, 180, 90, 0, 65, 0, 14, 0, 0, 232 }, { 0, 0, 0, 19, 17, 0, 7, 0, 0, 0, 0, 23 }, { 106, 11, -1, -1, -1 }, { 17, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOHedonist", { 0, 0, -1, 0, 57, 0, 38, 0, 3, 0, 0, 105 }, { 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5 }, { 2, 11, -1, -1, -1 }, { 1, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOIcon", { 0, 0, -1, 0, 85, 0, 31, 67, 3, 0, 0, 192 }, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 }, { 95, -1, 13, -1, -1 }, { 6, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOInstigator", { 0, 107, -1, 55, 92, 0, 62, 0, 2, 0, 0, 226 }, { 0, 18, 0, 0, 20, 0, 20, 0, 0, 0, 0, 11 }, { 95, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOKingpin", { 0, 0, -1, 3, 50, 0, 37, 0, 66, 0, 0, 104 }, { 0, 0, 0, 0, 0, 0, 3, 0, 5, 0, 0, 0 }, { -1, 2, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOMastermind", { 0, 0, -1, 40, 85, 0, 7, 0, 3, 0, 0, 206 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 95, 24, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOMedia Mogul", { 0, 0, -1, 7, 0, 0, 3, 85, 55, 0, 0, 66 }, { 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 }, { 58, -1, -1, -1, -1 }, { 2, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOMob Boss", { 0, 0, -1, 3, 51, 0, 37, 0, 3, 0, 0, 102 }, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, { -1, 16, -1, -1, -1 }, { 0, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOMoon Curser", { 0, 104, -1, 174, 90, 0, 63, 0, 14, 0, 0, 230 }, { 0, 25, 0, 3, 3, 0, 0, 0, 0, 0, 0, 20 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOOil Tycoon", { 0, 0, -1, 36, 37, 0, 29, 0, 39, 0, 0, 65 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOOverseas Investor", { 0, 0, -1, 3, 37, 0, 29, 22, 38, 0, 0, 7 }, { 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOPioneer", { 0, 0, -1, 3, 1, 0, 10, 0, 2, 0, 0, 136 }, { 0, 0, 0, 0, 8, 0, 2, 0, 0, 0, 0, 3 }, { -1, 19, -1, -1, -1 }, { 0, 9, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOPlayboy", { 0, 0, -1, 3, 63, 0, 41, 0, 76, 0, 0, 99 }, { 0, 0, 0, 0, 2, 0, 2, 0, 3, 0, 0, 2 }, { -1, 2, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOPremier", { 0, 0, -1, 31, 0, 0, 60, 0, 3, 1, 0, 73 }, { 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 1 }, { -1, -1, 14, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOPresident", { 0, 0, -1, 7, 43, 0, 20, 0, 13, 0, 0, 69 }, { 0, 0, 0, 0, 4, 0, 0, 0, 6, 0, 0, 0 }, { -1, 11, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEORecon Leader", { 0, 0, -1, 41, 32, 0, 26, 0, 14, 12, 0, 232 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 20 }, { 111, -1, -1, -1, -1 }, { 4, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEORingleader", { 0, 0, -1, 3, 80, 0, 60, 0, 69, 0, 0, 193 }, { 0, 0, 0, 0, 7, 0, 4, 0, 2, 0, 0, 0 }, { 55, 11, -1, -1, -1 }, { 19, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEORoller", { 0, 0, -1, 139, 73, 0, 60, 38, 52, 0, 0, 167 }, { 0, 0, 0, 0, 1, 0, 10, 0, 1, 0, 0, 3 }, { -1, 11, 13, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOShipping Tycoon", { 0, 0, -1, 3, 54, 0, 30, 0, 3, 0, 0, 98 }, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2 }, { -1, 24, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOStandout", { 0, 0, -1, 4, 84, 0, 60, 0, 3, 0, 0, 195 }, { 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 5 }, { 95, -1, 16, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOSurvivalist", { 0, 0, -1, 174, 90, 0, 63, 0, 130, 0, 0, 216 }, { 0, 0, 0, 19, 6, 0, 7, 0, 6, 0, 0, 18 }, { 104, 25, -1, -1, -1 }, { 23, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOSyndicate Leader", { 0, 0, -1, 3, 55, 0, 29, 0, 38, 0, 0, 95 }, { 0, 0, 0, 0, 0, 0, 2, 0, 10, 0, 0, 0 }, { -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOTop Dog", { 0, 0, -1, 4, 83, 0, 60, 0, 3, 0, 0, 195 }, { 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 2 }, { 55, -1, 14, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOTrailblazer", { 0, 0, -1, 3, 80, 0, 60, 93, 59, 0, 0, 193 }, { 0, 0, 0, 0, 6, 0, 9, 0, 0, 0, 0, 23 }, { 95, 2, -1, -1, -1 }, { 0, 5, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOVillain", { 0, 101, -1, 23, 80, 0, 60, 0, 3, 0, 0, 205 }, { 0, 13, 0, 0, 2, 0, 10, 0, 0, 0, 0, 25 }, { 101, 3, -1, -1, -1 }, { 7, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleCEOWarlord", { 0, 0, -1, 7, 61, 0, 26, 53, 13, 0, 0, 34 }, { 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0 }, { 55, 25, -1, -1, -1 }, { 1, 9, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Accessory", { 0, 0, -1, 14, 41, 0, 3, 37, 3, 0, 0, 68 }, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3 }, { 4, 3, 12, -1, -1 }, { 0, 7, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Chic", { 0, 0, -1, 39, 43, 0, 30, 39, 45, 0, 0, 64 }, { 0, 0, 0, 0, 0, 0, 0, 1, 10, 0, 0, 3 }, { -1, 2, 3, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Class", { 0, 0, -1, 6, 43, 0, 8, 0, 45, 0, 0, 65 }, { 0, 0, 0, 0, 0, 0, 6, 0, 11, 0, 0, 5 }, { -1, 16, 4, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Elite", { 0, 0, -1, 7, 9, 0, 22, 0, 23, 0, 0, 65 }, { 0, 0, 0, 0, 7, 0, 0, 0, 1, 0, 0, 8 }, { -1, 16, 9, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Golden Girl", { 0, 0, -1, 2, 16, 0, 20, 42, 3, 0, 0, 67 }, { 0, 0, 0, 0, 8, 0, 11, 0, 0, 0, 0, 0 }, { -1, -1, 7, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Lavish", { 0, 0, -1, 6, 27, 0, 0, 0, 46, 0, 0, 66 }, { 0, 0, 0, 0, 9, 0, 2, 0, 2, 0, 0, 3 }, { -1, 4, 11, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Perseus", { 0, 0, -1, 14, 43, 0, 31, 41, 3, 0, 0, 68 }, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 5 }, { -1, 11, -1, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Puma", { 0, 0, -1, 6, 43, 0, 30, 29, 15, 0, 0, 66 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { -1, 7, 4, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Shine", { 0, 0, -1, 6, 9, 0, 31, 0, 49, 0, 0, 64 }, { 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0 }, { 13, 11, 5, -1, -1 }, { 0, 2, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Socialite", { 0, 0, -1, 3, 43, 0, 19, 0, 50, 0, 0, 65 }, { 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 0, 0 }, { -1, 7, 6, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Status", { 0, 0, -1, 7, 26, 0, 19, 33, 13, 0, 0, 66 }, { 0, 0, 0, 0, 0, 0, 2, 0, 15, 0, 0, 2 }, { 22, 16, 11, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleDesignerThe Wealth", { 0, 0, -1, 3, 43, 0, 30, 41, 45, 0, 0, 64 }, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 }, { -1, 2, 8, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe 90s", { 0, 0, -1, 5, 27, 0, 22, 14, 28, 0, 0, 35 }, { 0, 0, 0, 0, 13, 0, 3, 2, 8, 0, 0, 1 }, { -1, 20, -1, -1, -1 }, { 0, 7, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Bubblegum", { 0, 0, -1, 5, 27, 0, 22, 14, 28, 0, 0, 35 }, { 0, 0, 0, 0, 11, 0, 4, 3, 6, 0, 0, 3 }, { -1, 21, -1, -1, -1 }, { 0, 6, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Craze", { 0, 0, -1, 5, 27, 0, 22, 14, 28, 0, 0, 31 }, { 0, 0, 0, 0, 12, 0, 5, 2, 7, 0, 0, 2 }, { 29, 20, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Jungle", { 0, 0, -1, 6, 27, 0, 3, 0, 28, 0, 0, 34 }, { 0, 0, 0, 0, 2, 0, 10, 0, 3, 0, 0, 0 }, { -1, 21, -1, -1, -1 }, { 0, 3, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Palms", { 0, 0, -1, 5, 27, 0, 22, 14, 26, 0, 0, 31 }, { 0, 0, 0, 0, 10, 0, 10, 2, 1, 0, 0, 3 }, { 29, 21, -1, -1, -1 }, { 2, 7, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Roar", { 0, 0, -1, 4, 27, 0, 22, 11, 2, 0, 0, 33 }, { 0, 0, 0, 0, 8, 0, 7, 0, 0, 0, 0, 2 }, { -1, 21, -1, -1, -1 }, { 0, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Safari", { 0, 0, -1, 5, 25, 0, 21, 14, 4, 0, 0, 35 }, { 0, 0, 0, 0, 4, 0, 9, 0, 14, 0, 0, 9 }, { 28, 20, -1, -1, -1 }, { 1, 0, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Tropics", { 0, 0, -1, 4, 25, 0, 3, 15, 2, 0, 0, 33 }, { 0, 0, 0, 0, 12, 0, 13, 0, 0, 0, 0, 4 }, { -1, 20, -1, -1, -1 }, { 0, 4, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Vintage", { 0, 0, -1, 15, 21, 0, 22, 14, 2, 0, 0, 37 }, { 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4 }, { -1, 20, -1, -1, -1 }, { 0, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Youth", { 0, 0, -1, 4, 27, 0, 11, 14, 2, 0, 0, 33 }, { 0, 0, 0, 0, 7, 0, 3, 1, 0, 0, 0, 5 }, { 29, 21, -1, -1, -1 }, { 3, 1, 0, 0, 0 }),
			PZclass::ClothX("FemaleHipsterThe Yuppie", { 0, 0, -1, 5, 27, 0, 22, 14, 28, 0, 0, 31 }, { 0, 0, 0, 0, 6, 0, 5, 0, 0, 0, 0, 6 }, { 28, 20, -1, -1, -1 }, { 4, 1, 0, 0, 0 })
		};

		string OutputFolder = GetDir() + "/Outfits";
		string OutputFolderM = GetDir() + "/Outfits/Male";
		string OutputFolderF = GetDir() + "/Outfits/Female";
		if (CreateDirectoryA((LPSTR)OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			WriteFile(HasOutfits, ReadMe);
			if (CreateDirectoryA((LPSTR)OutputFolderM.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				for (int i = 0; i < (int)MaleOut.size(); i++)
					SaveOutfits(MaleOut[i], true);
			}

			if (CreateDirectoryA((LPSTR)OutputFolderF.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				for (int i = 0; i < (int)FemaleOut.size(); i++)
					SaveOutfits(FemaleOut[i], false);
			}
		}
	}
	void DisplayPlayers()
	{
		//const float lineWidth = 250.0;
		CAM::DO_SCREEN_FADE_IN(1);

		vector<string> PlayerZList1 = {};
		vector<string> PlayerZList2 = {};

		vector<string> PlayerZLevel1 = {};
		vector<string> PlayerZLevel2 = {};

		int Playerz = (int)PedList.size() + (int)AFKList.size();
		std::string Public = PZLang[87];
		if (PZData::MySettings.InviteOnly)
			Public = PZLang[88];
		std::string caption = "GTA Online (" + Public + ", " + std::to_string(Playerz) + ")";

		for (int i = 0; i < PedList.size(); i++)
		{
			if (PlayerZList1.size() > 16)
			{
				PlayerZList2.push_back(PedList[i].MyName);
				PlayerZLevel2.push_back(std::to_string(PedList[i].Level));
			}
			else
			{
				PlayerZList1.push_back(PedList[i].MyName);
				PlayerZLevel1.push_back(std::to_string(PedList[i].Level));
			}
		}

		for (int i = 0; i < AFKList.size(); i++)
		{
			if (PlayerZList1.size() > 16)
			{
				PlayerZList2.push_back(AFKList[i].MyName);
				PlayerZLevel2.push_back(std::to_string(AFKList[i].Level));
			}
			else
			{
				PlayerZList1.push_back(AFKList[i].MyName);
				PlayerZLevel1.push_back(std::to_string(AFKList[i].Level));
			}
		}

		int iWait = InGameTime() + 7000;

		vector<string> sInstBut = { PZLang[86] };
		vector<int> iInstBut = { PZData::MySettings.Control_Open };

		int iBc = BottomRight(iInstBut, sInstBut);

		float lineWidthA = 252.0;
		float lineHeightA = 5.0;
		float lineTopA = 11.5;
		float lineLeftA = 25.5;
		float textLeftA = 5.0;
		float textLeftA2 = 5.0;

		float lineWidthB = 252.0;
		float lineHeightB = -0.5;
		float lineTopB = 22.5;
		float lineLeftB = 25.5;
		float textLeftB = 9.0;
		float textLeftB2 = 219.5;

		bool bSwitch = true;
		DWORD waitTime = 0;

		while (true)
		{
			DWORD maxTickCount = GetTickCount() + waitTime;
			do
			{
				DrawSessionList(caption, "", lineWidthA, lineHeightA, lineTopA, lineLeftA, textLeftA, textLeftA2, false, true, 0);
				if (bSwitch || Playerz < 16)
				{
					for (int i = 0; i < PlayerZList1.size(); i++)
					{
						DrawSessionList(PlayerZList1[i], PlayerZLevel1[i], lineWidthB, lineHeightB, (float)45.0 + (float)i * lineTopB, lineLeftB, textLeftB, textLeftB2, true, false, i);
					}
				}
				else
				{
					for (int i = 0; i < PlayerZList2.size(); i++)
					{
						DrawSessionList(PlayerZList2[i], PlayerZLevel2[i], lineWidthB, lineHeightB, (float)45.0 + (float)i * lineTopB, lineLeftB, textLeftB, textLeftB2, true, false, i);
					}
				}

				GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(iBc, 255, 255, 255, 255, 0);

				WAIT(1);
			} while (GetTickCount() < maxTickCount);
			waitTime = 1000;

			if (ButtonDown(PZData::MySettings.Control_Players, true))
				bSwitch = !bSwitch;
			else if (ButtonDown(PZData::MySettings.Control_Open, true) || WHileKeyDown(PZData::MySettings.Keys_Open))
			{
				bTrigMenu = true;
				WAIT(1000);
				break;
			}

			if (iWait < InGameTime())
				break;
		}

		CloseBaseHelpBar(iBc);
	}

	vector<PZclass::PhoneContact*> YourFriends = {};
	int PhoneScales = -1;

	bool PlayerOnline(string id)
	{
		bool b = false;
		for (int i = 0; i < (int)PedList.size(); i++)
		{
			if (id == PedList[i].MyIdentity)
			{
				b = true;
				break;
			}
		}
		return b;
	}
	void ContactInSession(PZclass::PhoneContact* myCon)
	{
		Vector3 LandHere = FowardOf(PLAYER::PLAYER_PED_ID(), 5, true);
		myCon->InSession = true;
		myCon->YourFriend.Follower = true;
		myCon->YourFriend.WanBeFriends = false;
		myCon->YourFriend.BlipColour = 38;
		myCon->YourFriend.TimeOn = InGameTime() + PZSetMaxSession;
		if (myCon->YourFriend.PrefredVehicle != 0 && myCon->YourFriend.PrefredVehicle < 8)
		{
			if (myCon->YourFriend.PrefredVehicle == 5 || myCon->YourFriend.PrefredVehicle == 3)
				myCon->YourFriend.IsPlane = true;
			else if (myCon->YourFriend.PrefredVehicle == 4 || myCon->YourFriend.PrefredVehicle == 2)
				myCon->YourFriend.IsHeli = true;

			myCon->YourFriend.ApprochPlayer = true;
			myCon->YourFriend.Driver = true;
			PZclass::FindVeh myCar = PZclass::FindVeh(15.00f, 145.00f, true, false, myCon->YourFriend);
			MakeCarz.push_back(myCar);
		}
		else
		{
			PZclass::FindPed MyFinda = PZclass::FindPed(35.00f, 220.00f, myCon->YourFriend);
			MakeFrenz.push_back(MyFinda);
		}
	}
	int GetSelectedContact()
	{
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(PhoneScales, "GET_CURRENT_SELECTION");
		int num = GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION();
		while (!GRAPHICS::_0x768FF8961BA904D6(num))
			WAIT(1);
		int output = GRAPHICS::_0x2DE7EFA66B906036(num);

		LoggerLight("GetSelectedContact == " + std::to_string(output));

		return output;
	}
	void AnswerPhone(string ContactName)
	{
		LoggerLight("AnswerPhone, ContactName == " + ContactName);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(PhoneScales, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(4);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(3);
		GRAPHICS::_BEGIN_TEXT_COMPONENT("STRING");
		UI::_0x761B77454205A61D((LPSTR)ContactName.c_str(), -1);
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_BEGIN_TEXT_COMPONENT("CELL_2000");
		UI::_ADD_TEXT_COMPONENT_STRING("CELL_300");
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_BEGIN_TEXT_COMPONENT("STRING");
		UI::_0x761B77454205A61D(UI::_GET_LABEL_TEXT("CELL_211"), -1);
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(PhoneScales, "DISPLAY_VIEW");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(4);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		WAIT(4000);
		ClosePhone();
	}
	void DrawPhoneContact(int index, string name)
	{
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(PhoneScales, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(2);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(index);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		GRAPHICS::_BEGIN_TEXT_COMPONENT("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)name.c_str());
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_BEGIN_TEXT_COMPONENT("CELL_999");
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_BEGIN_TEXT_COMPONENT("CELL_2000");
		UI::_ADD_TEXT_COMPONENT_STRING("CHAR_BLANK_ENTRY");
		GRAPHICS::_END_TEXT_COMPONENT();
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
	}
	void ClosePhone()
	{

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(PhoneScales, "SHUTDOWN_MOVIE");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		WAIT(1);
		YourFriends.clear();
		PhoneScales = -1;
		MOBILE::DESTROY_MOBILE_PHONE();
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_flashhand");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");
		WAIT(1);
		StartScript("cellphone_flashhand", 1424);
		StartScript("cellphone_controller", 1424);
	}

	int ButtDelay = 0;
	int PhoneAdd = 30;

	void PhoneFreeking()
	{
		if (SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(MyHashKey("cellphone_flashhand")) > 0 && SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(MyHashKey("appcontacts")) > 0)
		{
			if (PhoneScales == -1)
			{
				ButtDelay = InGameTime() + 500;
				Hash MyKey = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());
				if (MyKey == GAMEPLAY::GET_HASH_KEY("player_zero"))
					PhoneScales = GRAPHICS::REQUEST_SCALEFORM_MOVIE("cellphone_ifruit");
				else if (MyKey == GAMEPLAY::GET_HASH_KEY("player_one"))
					PhoneScales = GRAPHICS::REQUEST_SCALEFORM_MOVIE("cellphone_badger");
				else if (MyKey == GAMEPLAY::GET_HASH_KEY("player_two"))
					PhoneScales = GRAPHICS::REQUEST_SCALEFORM_MOVIE("cellphone_facade");
				else
					PhoneScales = GRAPHICS::REQUEST_SCALEFORM_MOVIE("cellphone_ifruit");
				PhoneAdd = 35;
				while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(PhoneScales))
					WAIT(1);

				for (int i = 0; i < (int)PhoneContacts.size(); i++)
				{
					if (!PlayerOnline(PhoneContacts[i].YourFriend.MyIdentity))
						YourFriends.push_back(&PhoneContacts[i]);
				}

				while ((int)YourFriends.size() > 5)
				{
					int iRands = RandomInt(0, (int)YourFriends.size() - 1);
					YourFriends.erase(YourFriends.begin() + iRands);
					WAIT(1);
				}
			}

			for (int i = 0; i < (int)YourFriends.size(); i++)
				DrawPhoneContact(i + PhoneAdd, YourFriends[i]->Name);

			if (ButtonDown(176, true) && ButtDelay < InGameTime())
			{
				int iBe = GetSelectedContact() - PhoneAdd;
				if (iBe > -1 && iBe < (int)YourFriends.size())
				{
					AnswerPhone(YourFriends[iBe]->Name);
					ContactInSession(YourFriends[iBe]);
				}
				else
					ClosePhone();
			}
		}
		else if (PhoneScales != -1)
			ClosePhone();

	}

	void ReBuildIni(PZclass::PZSettings* PSet)
	{
		std::vector<std::string> AddIni = {
			"[Settings]",
			"Pz_Lang=" + std::to_string(PSet->Pz_Lang),
			"Aggression=" + std::to_string(PSet->Aggression),
			"MaxPlayers=" + std::to_string(PSet->MaxPlayers),
			"MinWait=" + std::to_string(PSet->MinWait),
			"MaxWait=" + std::to_string(PSet->MaxWait),
			"MinSession=" + std::to_string(PSet->MinSession),
			"MaxSession=" + std::to_string(PSet->MaxSession),
			"MinAccuracy=" + std::to_string(PSet->AccMin),
			"MaxAccuracy=" + std::to_string(PSet->AccMax),
			"AirVeh=" + std::to_string(PSet->AirVeh),
			"FriendlyFire=" + BoolToString(PSet->FriendlyFire),
			"SpaceWeaps=" + BoolToString(PSet->SpaceWeaps),
			"NameTags=" + BoolToString(PSet->NameTags),
			"PassiveMode=" + BoolToString(PSet->PassiveMode),
			"Debug=" + BoolToString(PSet->Debugger),
			"NoRadar=" + BoolToString(PSet->NoRadar),
			"NoNotify=" + BoolToString(PSet->NoNotify),
			"MobileData=" + BoolToString(PSet->MobileData),
			"InviteOnly=" + BoolToString(PSet->InviteOnly),
			"[Controls]",
			"Control_Players=" + std::to_string(PSet->Control_Players),
			"Control_Open=" + std::to_string(PSet->Control_Open),
			"Control_A_Clear=" + std::to_string(PSet->Control_A_Clear),
			"Control_B_Clear=" + std::to_string(PSet->Control_B_Clear),
			"Control_A_Invite=" + std::to_string(PSet->Control_A_Invite),
			"Control_B_Invite=" + std::to_string(PSet->Control_B_Invite),
			"Keys_Players=" + std::to_string(PSet->Keys_Players),
			"Keys_Open=" + std::to_string(PSet->Keys_Open),
			"Keys_Clear=" + std::to_string(PSet->Keys_Clear),
			"Keys_Invite=" + std::to_string(PSet->Keys_Players),
			"BackChat=" + BoolToString(PSet->BackChat)
		};

		ofstream MyFile(GetDir() + "/PlayerZero/PzSettings.ini");
		for (int i = 0; i < AddIni.size(); i++)
		{
			MyFile << "\n";
			MyFile << AddIni[i];
		}
		MyFile.close();
	}
	void FindSettings(PZclass::PZSettings* mySet)
	{
		LoggerLight("FindSettings");

		string sSettings = GetDir() + "/PlayerZero/PzSettings.ini";
		vector<string> MySettings = ReadFile(sSettings);

		if (MySettings.size() == 0)
		{
			string OutputFolder = GetDir() + "/PlayerZero";
			if (CreateDirectoryA((LPSTR)OutputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				LoggerLight("PlayerZero, Direct Working");
				FirstRun = true;
			}
			else
			{
				LoggerLight("PlayerZero, Direct failed");
			}
		}
		else
		{
			for (int i = 0; i < (int)MySettings.size(); i++)
			{
				if (StringContains("Pz_Lang", MySettings[i]))
				{
					mySet->Pz_Lang = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Aggression", MySettings[i]))
				{
					mySet->Aggression = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MaxPlayers", MySettings[i]))
				{
					mySet->MaxPlayers = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MinWait", MySettings[i]))
				{
					mySet->MinWait = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MaxWait", MySettings[i]))
				{
					mySet->MaxWait = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MinSession", MySettings[i]))
				{
					mySet->MinSession = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MaxSession", MySettings[i]))
				{
					mySet->MaxSession = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MinAccuracy", MySettings[i]))
				{
					mySet->AccMin = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("MaxAccuracy", MySettings[i]))
				{
					mySet->AccMax = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("AirVeh", MySettings[i]))
				{
					mySet->AirVeh = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("FriendlyFire", MySettings[i]))
				{
					mySet->FriendlyFire = StringBool(MySettings[i]);
				}
				else if (StringContains("SpaceWeaps", MySettings[i]))
				{
					mySet->SpaceWeaps = StringBool(MySettings[i]);
				}
				else if (StringContains("NameTags", MySettings[i]))
				{
					mySet->NameTags = StringBool(MySettings[i]);
				}
				else if (StringContains("PassiveMode", MySettings[i]))
				{
					mySet->PassiveMode = StringBool(MySettings[i]);
				}
				else if (StringContains("Debug", MySettings[i]))
				{
					mySet->Debugger = StringBool(MySettings[i]);
				}
				else if (StringContains("NoRadar", MySettings[i]))
				{
					mySet->NoRadar = StringBool(MySettings[i]);
				}
				else if (StringContains("NoNotify", MySettings[i]))
				{
					mySet->NoNotify = StringBool(MySettings[i]);
				}
				else if (StringContains("MobileData", MySettings[i]))
				{
					mySet->MobileData = StringBool(MySettings[i]);
				}
				else if (StringContains("InviteOnly", MySettings[i]))
				{
					mySet->InviteOnly = StringBool(MySettings[i]);
				}
				else if (StringContains("BackChat", MySettings[i]))
				{
					mySet->BackChat = StringBool(MySettings[i]);
				}
				else if (StringContains("Control_Players", MySettings[i]))
				{
					mySet->Control_Players = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Control_Open", MySettings[i]))
				{
					mySet->Control_Open = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Control_A_Clear", MySettings[i]))
				{
					mySet->Control_A_Clear = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Control_B_Clear", MySettings[i]))
				{
					mySet->Control_B_Clear = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Control_A_Invite", MySettings[i]))
				{
					mySet->Control_A_Invite = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Control_B_Invite", MySettings[i]))
				{
					mySet->Control_B_Invite = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Keys_Players", MySettings[i]))
				{
					mySet->Keys_Players = StingNumbersInt(MySettings[i]);
				}
				else if (StringContains("Keys_Open", MySettings[i]))
				{
					mySet->Keys_Open = StingNumbersInt(MySettings[i]);
					}
				else if (StringContains("Keys_Clear", MySettings[i]))
				{
					mySet->Keys_Clear = StingNumbersInt(MySettings[i]);
					}
				else if (StringContains("Keys_Invite", MySettings[i]))
				{
					mySet->Keys_Players = StingNumbersInt(MySettings[i]);
				}
			}
		}

		if (mySet->Control_Players >= (int)ControlSym.size() || mySet->Control_Players < 0)
			mySet->Control_Players = 19;

		if (mySet->Control_Open >= (int)ControlSym.size() || mySet->Control_Open < 0)
			mySet->Control_Open = 21;

		if (mySet->Control_A_Clear >= (int)ControlSym.size() || mySet->Control_A_Clear < 0)
			mySet->Control_A_Clear = 47;

		if (mySet->Control_B_Clear >= (int)ControlSym.size() || mySet->Control_B_Clear < 0)
			mySet->Control_B_Clear = 21;

		if (mySet->Control_A_Invite >= (int)ControlSym.size() || mySet->Control_A_Invite < 0)
			mySet->Control_A_Invite = 46;

		if (mySet->Control_B_Invite >= (int)ControlSym.size() || mySet->Control_B_Invite < 0)
			mySet->Control_B_Invite = 21;

		if (mySet->Keys_Players >= (int)KeyFind.size() || mySet->Keys_Players < 0)
			mySet->Keys_Players = 90;

		if (mySet->Keys_Open >= (int)KeyFind.size() || mySet->Keys_Open < 0)
			mySet->Keys_Open = 88;

		if (mySet->Keys_Clear >= (int)KeyFind.size() || mySet->Keys_Clear < 0)
			mySet->Keys_Clear = 67;

		if (mySet->Keys_Invite >= (int)KeyFind.size() || mySet->Keys_Invite < 0)
			mySet->Keys_Invite = 86;

		if (mySet->Aggression > 11)
			mySet->Aggression = 11;
		if (mySet->MinWait > mySet->MaxWait)
			mySet->MaxWait = mySet->MinWait + 1;
		if (mySet->MinSession > mySet->MaxSession)
			mySet->MaxSession = mySet->MinSession + 1;
		if (mySet->AccMin > mySet->AccMax)
			mySet->AccMax = mySet->AccMin + 1;

		ReBuildIni(mySet);
	}
	void LoadLang()
	{
		LoggerLight("LoadLang lang == " + std::to_string(PZData::MySettings.Pz_Lang));

		int MyLan = PZData::MySettings.Pz_Lang;
		std::vector<std::string> HoldMyLang = PZLang;

		if (MyLan == -1)
			MyLan = UNK::_GET_UI_LANGUAGE_ID(); // 2BDD44CC428A7EAE

		if (MyLan == 0)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/English.txt");
		else if (MyLan == 1)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/French.txt");
		else if (MyLan == 2)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/German.txt");
		else if (MyLan == 3)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Italian.txt");
		else if (MyLan == 4)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Spanish.txt");
		else if (MyLan == 5)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Portuguese.txt");
		else if (MyLan == 6)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Polish.txt");
		else if (MyLan == 7)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Russian.txt");
		else if (MyLan == 8)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Korean.txt");
		else if (MyLan == 9)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Chinese.txt");
		else if (MyLan == 10)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Japanese.txt");
		else if (MyLan == 11)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/Spanish.txt");
		else if (MyLan == 12)
			PZLang = ReadFile(GetDir() + "/PlayerZero/Translate/ChineseSimplify.txt");

		if ((int)PZLang.size() < 108)
			PZLang = HoldMyLang;

		MySettings.Pz_Lang = MyLan;
	}
}