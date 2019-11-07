TH1D* Get_ThetaDis(int Index,double Ekin_Theta_CM_Range[][4],TH2D* h2_Theta_Ekin_CM);

void _4_Check_Theta_CM()
{
  string RunTag = "4100-4109";
  string SystemTag = "Ca48Ni64E140";//"Ca40Ni56E140";
  string ESpecStoreFileName = "./ESpec/f1_ESpec_"+RunTag+".root";
  
  TFile* f1_ESpec = new TFile(ESpecStoreFileName.c_str(),"update");
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  
  TH2D* h2_Theta_Ekin_CM_GeoReactionEff[20];
  TH2D* h2_Theta_Ekin_CM_GeoEff[20];
  TH2D* h2_Theta_Ekin_CM_noEff[20];
  
  char NameTem[200];
  for(int i=0;i<ParticleNum;i++)
  {
    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoReactionEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoReactionEff[i] = (TH2D*) f1_ESpec->Get(NameTem);
    
    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoEff[i] = (TH2D*) f1_ESpec->Get(NameTem);

    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_noEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_noEff[i] = (TH2D*) f1_ESpec->Get(NameTem);
  }
  
  string ParticleTag = ParticleName[0];
  
  Hira_EkinTheta* EkinTheta_Spec = new Hira_EkinTheta(SystemTag,RunTag,ParticleTag);
  const int CutNum = 2;
  string CutTag[CutNum] = {"P_LowE_Cut","P_HighE_Cut"};
  //EkinTheta_Spec->InitialCut("./ESpec/P_EkinTheta_Cut.root", CutNum, CutTag);
  
  /*
  int EkinNum = 5;
  double StartEkin = 9.9;
  double EkinBinSize = 20;
  EkinTheta_Spec->SetEkinConfig(EkinNum, StartEkin, EkinBinSize);
  */
  const int ThetaNum = 1;
  double StartTheta = 80;
  double ThetaBinSize = 25;
  TH1D* h1_ESpec_T[ThetaNum];
  TH1D* h1_ESpec_3He[ThetaNum];
  EkinTheta_Spec->SetThetaConfig(ThetaNum, StartTheta, ThetaBinSize);
  
  EkinTheta_Spec->SetParticleTag("T");
  EkinTheta_Spec->ProduceEkinSpectrum(h2_Theta_Ekin_CM_GeoReactionEff[2],h1_ESpec_T);
  EkinTheta_Spec->DrawInfo();
  
  EkinTheta_Spec->SetParticleTag("3He");
  EkinTheta_Spec->ProduceEkinSpectrum(h2_Theta_Ekin_CM_GeoReactionEff[3],h1_ESpec_3He);
  EkinTheta_Spec->DrawInfo();
  
  //store in the root file for further analysis.
  TFile* f1_ESpec_T_3He = new TFile(("./ESpec/f1_T_3He_"+SystemTag+"_"+RunTag+".root").c_str(),"update");
  for(int i=0;i<ThetaNum;i++)
  {
    h2_Theta_Ekin_CM_GeoReactionEff[2]->Write("",TObject::kOverwrite);
    h1_ESpec_T[i]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_CM_GeoReactionEff[3]->Write("",TObject::kOverwrite);
    h1_ESpec_3He[i]->Write("",TObject::kOverwrite);
  }
}
