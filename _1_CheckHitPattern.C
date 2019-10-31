#include "TMath.h"
using namespace TMath;

void GetNormalized_CountNum(int X_Dir, TH1D* h1_Count,TH1D* h1_Count_Normalized,TH2D* h2_Pixel_Dis);

void _1_CheckHitPattern()
{
  string DataPath = "./"; //"/mnt/analysis/e15190/Brown/rootfiles";
  string RunTag = "4100-4101";
  string DataFileName = "CalibratedData_"+RunTag+".root";
  
  string Hira_BadMap_Version = "V1";
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_BadMap_Strip("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Strip_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_CsI("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_CsI_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_Tele("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Tele_"+Hira_BadMap_Version+".dat");
  //Hira_BadMapper->Read_StripBad_Map("./GeoEff/HiRA_StripBad_E15190_run2860.dat");
  
  
  Hira_CheckHitPattern* Hira_HitPattern_Checker = new Hira_CheckHitPattern();
  Hira_HitPattern_Checker->Set_IsActive_BadMap(1);
  Hira_HitPattern_Checker->Initial_Hira_BadMapper(Hira_BadMapper);
  
  const int ExpFileNum = 2;
  int RunNoList[] = {4100,4101,4102,4103,4104,4105,4106,4107,4108,4109};
  string ExpFileNameList[ExpFileNum];
  char NameTem[200];
  for(int i=0;i<ExpFileNum;i++)
  {
    sprintf(NameTem,"/mnt/analysis/e15190/Brown/rootfiles/CalibratedData_%d.root",RunNoList[i]);
    ExpFileNameList[i] = NameTem;
  }
  
  Hira_HitPattern_Checker->ReadExpData(ExpFileNum,ExpFileNameList,"./GeoEff/f1_HitPattern"+RunTag+".root");
//  Hira_HitPattern_Checker->ReadExpData(ExpFileName,"./GeoEff/f1_HitPattern"+RunTag+".root");
  Hira_HitPattern_Checker->Draw_Info();
}
