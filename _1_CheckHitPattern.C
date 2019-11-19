#include "TMath.h"
using namespace TMath;

void GetNormalized_CountNum(int X_Dir, TH1D* h1_Count,TH1D* h1_Count_Normalized,TH2D* h2_Pixel_Dis);

void _1_CheckHitPattern()
{
/*
  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = 2600;
  const int ExpFileNum = 1;
  string RunTag = "2600-2600";
  string Hira_BadMap_Version = "V2";
*/

  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = 2587;
  int EndRunNo = 2611;
  const int ExpFileNum = EndRunNo-StartRunNo+1;
  string RunTag = std::to_string(StartRunNo)+"-"+std::to_string(EndRunNo);
  string Hira_BadMap_Version = "V2.1";

/*
  string SystemTag = "Ca48Ni64E140";
  int StartRunNo = 4023;
  const int ExpFileNum = 10;
  string RunTag = "4023-4032";
  string Hira_BadMap_Version = "V1";
*/
  
  string DataPath = "/mnt/analysis/e15190/Brown/rootfiles";
  string Hira_BadMap_FilePath = "./GeoEff";
  string HitPattern_OutputPath = "./GeoEff";
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Set_BadMapper_Version(Hira_BadMap_FilePath, Hira_BadMap_Version); //this will read the map data automatically.
  
  Hira_PosCali* HiraPos = new Hira_PosCali();
  HiraPos->Read_Hira_PixelAngle("./Cal_PixelAngle/PixelAngle_BeamPos_0_0_0.dat");
  
  Hira_CheckHitPattern* Hira_HitPattern_Checker = new Hira_CheckHitPattern(SystemTag,RunTag,Hira_BadMap_Version);
  Hira_HitPattern_Checker->Set_IsActive_BadMap(1);
  Hira_HitPattern_Checker->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_HitPattern_Checker->Initial_Hira_PosCali(HiraPos);
  Hira_HitPattern_Checker->SetAnaTag(SystemTag,RunTag,Hira_BadMap_Version);
  
  //prepare the filenamelist
  string ExpFileNameList[ExpFileNum];
  char NameTem[200];
  for(int i=0;i<ExpFileNum;i++)
  {
    sprintf(NameTem,"%s/CalibratedData_%d.root",DataPath.c_str(),StartRunNo+i);
    ExpFileNameList[i] = NameTem;
  }
  
  Hira_HitPattern_Checker->ReadExpData(ExpFileNum,ExpFileNameList,HitPattern_OutputPath);
  Hira_HitPattern_Checker->Draw_Info();
}
