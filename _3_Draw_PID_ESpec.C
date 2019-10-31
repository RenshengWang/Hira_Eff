void _3_Draw_PID_ESpec()
{
  string DataPath = "./"; //"/mnt/analysis/e15190/Brown/rootfiles";
  string RunNo = "2860";
  string SystemTag = "Ca40Ni58E140";//"Ca40Ni56E140";
  string DataFileName = "CalibratedData_"+RunNo+".root";
  string ESpecStoreFileName = "f1_ESpec_"+RunNo+".root";
  
  Hira_BadMap* Hira_BadMapper = new Hira_BadMap();
  Hira_BadMapper->Read_StripBad_Map("HiRA_StripBad_E15190_run2860.dat");
  
  Hira_GeoEff* Hira_GeoEfficiency = new Hira_GeoEff();
  string RootFileNameForStore = RunNo+"/f1_GeoEff_run"+RunNo+".root";
  Hira_GeoEfficiency->ReadGeoEffHistogram(RootFileNameForStore);
  //Hira_GeoEfficiency->Draw_Info();
  //double GeoEff = Hira_GeoEfficiency->Get_GeoEff(60); //unit: Deg.
  //cout<<"GeoEff : "<<GeoEff<<endl;
  
  Hira_ESpec* Hira_ESpectrum = new Hira_ESpec();
  Hira_ESpectrum->Initial_Hira_BadMapper(Hira_BadMapper);
  Hira_ESpectrum->Initial_Hira_GeoEff(Hira_GeoEfficiency);
  Hira_ESpectrum->Initial_LabToCM(SystemTag);
  Hira_ESpectrum->Set_GeoEff_Cut(0.01);
  Hira_ESpectrum->Initial_ReactionLost_CorEff();
  Hira_ESpectrum->Set_Num_Theta_Lab_ForChecking(5); //this parameter will divide the theta into N parts, and draw the energy in the lab.
  
  Hira_ESpectrum->ReadExpData((DataPath+DataFileName).c_str(),(DataPath+RunNo+"/"+ESpecStoreFileName).c_str());
  Hira_ESpectrum->Draw_ESpec_Info();
  Hira_ESpectrum->Draw_ReactionEff();
  
}
