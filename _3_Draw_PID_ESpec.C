void _3_Draw_PID_ESpec()
{
  string Hira_BadMap_Version = "V1";
  const int ExpFileNum = 10;
  int RunNoList[] = {4100,4101,4102,4103,4104,4105,4106,4107,4108,4109};
  string ExpFileNameList[ExpFileNum];
  char NameTem[200];
  for(int i=0;i<ExpFileNum;i++)
  {
    sprintf(NameTem,"/mnt/analysis/e15190/Brown/rootfiles/CalibratedData_%d.root",RunNoList[i]);
    ExpFileNameList[i] = NameTem;
  }
  string RunTag =  std::to_string(RunNoList[0])+"-"+std::to_string(RunNoList[ExpFileNum-1]);
  string SystemTag = "Ca48Ni64E140";
  string ESpecStoreFileName = "./ESpec/f1_ESpec_"+RunTag+".root";
  
  cout<<" -> RunTag : " <<RunTag<<endl;
  cout<<" -> SystemTag : " <<SystemTag<<endl;
  cout<<" -> ESpecStoreFileName : " <<ESpecStoreFileName<<endl;
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_BadMap_Strip("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Strip_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_CsI("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_CsI_"+Hira_BadMap_Version+".dat");
  Hira_BadMapper->Read_BadMap_Tele("./GeoEff/BadMap_"+Hira_BadMap_Version+"/Hira_BadMap_Tele_"+Hira_BadMap_Version+".dat");
  
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  string RootFileNameForStore = "./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root";
  {
    Hira_GeoEfficiency->ReadGeoEffHistogram(RootFileNameForStore);
    Hira_GeoEfficiency->Draw_Info(); //Drw the histograms that this class got.
    double GeoEff = Hira_GeoEfficiency->Get_GeoEff(60); //unit: Deg.
    cout<<"GeoEff : "<<GeoEff<<endl;
  }
  
  Hira_PosCali* HiraPos = new Hira_PosCali();
  HiraPos->Read_Hira_PixelAngle("./Cal_PixelAngle/PixelAngle_BeamPos_0_0_0.dat");
  
  Hira_ESpec* Hira_ESpectrum = new Hira_ESpec();
  Hira_ESpectrum->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_ESpectrum->Initial_Hira_GeoEff(Hira_GeoEfficiency);
  Hira_ESpectrum->Initial_Hira_PosCali(HiraPos);
  
  Hira_ESpectrum->Initial_LabToCM(SystemTag);
  Hira_ESpectrum->Set_GeoEff_Cut(0.001);
  Hira_ESpectrum->Initial_ReactionLost_CorEff();
  Hira_ESpectrum->Set_Num_Theta_Lab_ForChecking(5); //this parameter will divide the theta into N parts, and draw the energy in the lab.
  
  Hira_ESpectrum->ReadExpData(ExpFileNum,ExpFileNameList,ESpecStoreFileName);
  
  Hira_ESpectrum->Draw_ESpec_Info();
  Hira_ESpectrum->Draw_ReactionEff();
  
}
