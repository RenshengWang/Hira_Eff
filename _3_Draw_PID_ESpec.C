void _3_Draw_PID_ESpec()
{
  string DataPath = "/mnt/analysis/e15190/Brown/rootfiles";
  string Hira_BadMap_FilePath = "./GeoEff";
  int ImpactPar_Num = 2;
  double ImpactPars[20][2] = {{0,3.0},{3.0,5.0}};
  string ESpecStoreFilePath = "./ESpec";
  string ExpFileNameList[200];
  
  //the below is a DB for the map, you can increase as you need.
  int StartRunNo_Ca40Ni58E140[3] = {2571,2587,2848};
  int EndRunNo_Ca40Ni58E140[3] = {2586,2611,2870};
  string Hira_BadMap_Version_Ca40Ni58E140[3] = {"V2","V2.1","V2"};
  
  int StartRunNo_Ca48Ni64E140[2] = {4023,4082};
  int EndRunNo_Ca48Ni64E140[2] = {4032,4123};
  string Hira_BadMap_Version_Ca48Ni64E140[2] = {"V1","V1"};
  
  //the below is for setting the data file.
  int Index_Ca40Ni58E140 = 2;
  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = StartRunNo_Ca40Ni58E140[Index_Ca40Ni58E140];
  int EndRunNo = EndRunNo_Ca40Ni58E140[Index_Ca40Ni58E140];
  const int ExpFileNum = EndRunNo-StartRunNo+1;
  string RunTag = std::to_string(StartRunNo)+"-"+std::to_string(EndRunNo);
  string Hira_BadMap_Version = Hira_BadMap_Version_Ca40Ni58E140[Index_Ca40Ni58E140];

/*
  int Index_Ca48Ni64E140 = 0;
  string SystemTag = "Ca48Ni64E140";
  int StartRunNo = StartRunNo_Ca48Ni64E140[Index_Ca48Ni64E140];
  int EndRunNo = EndRunNo_Ca48Ni64E140[Index_Ca48Ni64E140];
  const int ExpFileNum = EndRunNo-StartRunNo+1;
  string RunTag = std::to_string(StartRunNo)+"-"+std::to_string(EndRunNo);
  string Hira_BadMap_Version = Hira_BadMap_Version_Ca48Ni64E140[Index_Ca48Ni64E140];
*/

  char NameTem[200];
  int FileIndex = 0;
  for(int i=StartRunNo;i<=EndRunNo;i++)
  {
    sprintf(NameTem,"%s/CalibratedData_%d.root",DataPath.c_str(),i);
    ExpFileNameList[FileIndex] = NameTem;
    FileIndex++;
  }
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Set_BadMapper_Version(Hira_BadMap_FilePath, Hira_BadMap_Version);
  
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  Hira_GeoEfficiency->ReadGeoEffHistogram(("./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root").c_str());
  
  Hira_PosCali* HiraPos = new Hira_PosCali();
  HiraPos->Read_Hira_PixelAngle("./Cal_PixelAngle/PixelAngle_BeamPos_0_0_0.dat");
  
  Hira_ESpec* Hira_ESpectrum = new Hira_ESpec(SystemTag,RunTag,Hira_BadMap_Version);
  Hira_ESpectrum->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_ESpectrum->Initial_Hira_GeoEff(Hira_GeoEfficiency);
  Hira_ESpectrum->Initial_Hira_PosCali(HiraPos);
  Hira_ESpectrum->Set_Is_HiraPos_Applied(1); //this option should be actived.
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
