#include "TMath.h"
using namespace TMath;

void _0_Produce_GeoEff()
{
  bool Is_ProduceGeoEff = 1;
  bool Is_ApplyPixelAngle = 1;
  string SimData_Path = "../NPTool/Outputs/Simulation/Hira10";
  string SimConfigTag = "ThetaUniform";
  string Hira_BadMap_Version = "V1";
  
  string Hira_BadMap_FilePath = "./GeoEff";
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Set_BadMapper_Version(Hira_BadMap_FilePath, Hira_BadMap_Version); //this will read the map data automatically.

  
  Hira_PosCali* HiraPos = new Hira_PosCali();
  HiraPos->Read_Hira_PixelAngle("./Cal_PixelAngle/PixelAngle_BeamPos_0_0_0.dat");
  
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
    Hira_GeoEfficiency->Initial_Hira_PosCali(HiraPos);
    Hira_GeoEfficiency->Set_IsApplyPixelAngle(Is_ApplyPixelAngle);
    
    string SimFileName = SimData_Path+"/myResult_"+SimConfigTag+".root";
    Hira_GeoEfficiency->ReadSimData(SimFileName,RootFileNameForStore);
  }
  
}

