#include "TMath.h"
using namespace TMath;

void GetNormalized_CountNum(int X_Dir, TH1D* h1_Count,TH1D* h1_Count_Normalized,TH2D* h2_Pixel_Dis);

void _1_CheckHitPattern()
{
  string DataPath = "./"; //"/mnt/analysis/e15190/Brown/rootfiles";
  string SystemTag = "Ca40Ni58E140";
  string RunTag = "2850-2870";
  
  string Hira_BadMap_Version = "V1";
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_BadMap_Strip("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Strip_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_CsI("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_CsI_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_Tele("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Tele_"+Hira_BadMap_Version+".dat");
  //Hira_BadMapper->Read_StripBad_Map("./GeoEff/HiRA_StripBad_E15190_run2860.dat");
  
  Hira_CheckHitPattern* Hira_HitPattern_Checker = new Hira_CheckHitPattern(SystemTag,RunTag,Hira_BadMap_Version);
  Hira_HitPattern_Checker->Set_IsActive_BadMap(1);
  Hira_HitPattern_Checker->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_HitPattern_Checker->SetAnaTag(SystemTag,RunTag,Hira_BadMap_Version);
  
  const int ExpFileNum = 1;
  string ExpFileNameList[ExpFileNum];
  char NameTem[200];
  for(int i=0;i<ExpFileNum;i++)
  {
    sprintf(NameTem,"/mnt/analysis/e15190/Brown/rootfiles/CalibratedData_%d.root",2850+i);
    ExpFileNameList[i] = NameTem;
  }
  Hira_HitPattern_Checker->ReadExpData(ExpFileNum,ExpFileNameList,"./GeoEff");
  Hira_HitPattern_Checker->Draw_Info();
}
