#include "TMath.h"
using namespace TMath;

void _0_Produce_GeoEff()
{
  /*
  string SimData_Path = "/home/zhong/Packages/nptool/Outputs/Simulation/Hira10";
  string SimConfigTag = "ThetaUniform";
  string RunTag = "2860"; //"run4761", "run2916"
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_StripBad_Map("./GeoEff/HiRA_StripBad_E15190_run2860.dat");
  */
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  string RootFileNameForStore = "./GeoEff/f1_GeoEff_run2860.root";
  {
    Hira_GeoEfficiency->ReadGeoEffHistogram(RootFileNameForStore);
    Hira_GeoEfficiency->Draw_Info(); //Drw the histograms that this class got.
    double GeoEff = Hira_GeoEfficiency->Get_GeoEff(60); //unit: Deg.
    cout<<"GeoEff : "<<GeoEff<<endl;
  }
  /*
  {//the below is for producing the GeoEff
    Hira_GeoEfficiency->Initial_Hira_BadMapper(Hira_BadMapper);
    string SimFileName = SimData_Path+"/myResult_"+SimConfigTag+".root";
    Hira_GeoEfficiency->ReadSimData(SimFileName,RootFileNameForStore);
  }
  */
}

