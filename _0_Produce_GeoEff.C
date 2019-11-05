#include "TMath.h"
using namespace TMath;

void _0_Produce_GeoEff()
{
  bool Is_ProduceGeoEff = 1;
  string SimData_Path = "/home/zhong/Packages/nptool/Outputs/Simulation/Hira10";
  string SimConfigTag = "ThetaUniform";
  string Hira_BadMap_Version = "V1";
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_BadMap_Strip("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Strip_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_CsI("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_CsI_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_Tele("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Tele_"+Hira_BadMap_Version+".dat");
  
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  string RootFileNameForStore = "./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root";
  if(Is_ProduceGeoEff==0)
  { //this is only for test the Efficiency of the Hira.
    Hira_GeoEfficiency->ReadGeoEffHistogram(RootFileNameForStore);
    Hira_GeoEfficiency->Draw_Info(); //Drw the histograms that this class got.
    double GeoEff = Hira_GeoEfficiency->Get_GeoEff(60); //unit: Deg.
    cout<<"GeoEff : "<<GeoEff<<endl;
  }
  else if(Is_ProduceGeoEff==1)
  {//the below is for producing the GeoEff
    Hira_GeoEfficiency->Initial_Hira_BadMapper(Hira_BadMapper);
    string SimFileName = SimData_Path+"/myResult_"+SimConfigTag+".root";
    Hira_GeoEfficiency->ReadSimData(SimFileName,RootFileNameForStore);
  }
  
}

