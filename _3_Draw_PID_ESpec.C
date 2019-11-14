void _3_Draw_PID_ESpec()
{
/*
  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = 2850;
  const int ExpFileNum = 20;
  string RunTag = "2850-2870";
  string Hira_BadMap_Version = "V2";
*/

  string SystemTag = "Ca48Ni64E140";
  int StartRunNo = 4023;
  const int ExpFileNum = 10;
  string RunTag = "4023-4032";
  string Hira_BadMap_Version = "V1";

  int ImpactPar_Num = 2;
  double ImpactPars[20][2] = {{0,3.0},{3.0,5.0}};
  string ESpecStoreFilePath = "./ESpec";
  
  string ExpFileNameList[ExpFileNum];
  char NameTem[200];
  for(int i=0;i<ExpFileNum;i++)
  {
    sprintf(NameTem,"/mnt/analysis/e15190/Brown/rootfiles/CalibratedData_%d.root",StartRunNo+i);
    ExpFileNameList[i] = NameTem;
  }
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_BadMap_Strip("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Strip_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_CsI("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_CsI_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_Tele("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Tele_"+Hira_BadMap_Version+".dat");
  
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  Hira_GeoEfficiency->ReadGeoEffHistogram(("./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root").c_str());
  
  Hira_PosCali* HiraPos = new Hira_PosCali();
  HiraPos->Read_Hira_PixelAngle("./Cal_PixelAngle/PixelAngle_BeamPos_0_0_0.dat");
  
  Hira_ESpec* Hira_ESpectrum = new Hira_ESpec(SystemTag,RunTag,Hira_BadMap_Version);
  Hira_ESpectrum->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_ESpectrum->Initial_Hira_GeoEff(Hira_GeoEfficiency);
  Hira_ESpectrum->Initial_Hira_PosCali(HiraPos);
  Hira_ESpectrum->Set_Is_HiraPos_Applied(1);
  Hira_ESpectrum->SetAnaTag(SystemTag,RunTag,Hira_BadMap_Version);
  Hira_ESpectrum->Set_ImpactPar_Range(ImpactPar_Num,ImpactPars);
  
  Hira_ESpectrum->Set_GeoEff_Cut(0.001);
  Hira_ESpectrum->Set_uBall_MultiCut(5); //if the uBall's multi<5, then the impact par is not "credible".
  Hira_ESpectrum->Initial_ReactionLost_CorEff();
  Hira_ESpectrum->Set_Num_Theta_Lab_ForChecking(5); //this parameter will divide the theta into N parts, and draw the energy in the lab.
  
  Hira_ESpectrum->ReadExpData(ExpFileNum,ExpFileNameList,ESpecStoreFilePath);
  
  Hira_ESpectrum->Draw_ESpec_Info();
  Hira_ESpectrum->Draw_ReactionEff();
  
}
