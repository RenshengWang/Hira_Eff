TH1D* Get_ThetaDis(int Index,double Ekin_Theta_CM_Range[][4],TH2D* h2_Theta_Ekin_CM);

void _5_Produce_ESpec_CM_forIsoScaling()
{
/*
  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = 2850;
  const int ExpFileNum = 20;
  string RunTag = "2850-2870";
  string Hira_BadMap_Version = "V2";
  double ImpactPar_Range[2] = {0,3.0};
*/

  string SystemTag = "Ca48Ni64E140";
  int StartRunNo = 4082;
  const int ExpFileNum = 42;
  string RunTag = "4082-4123";
  string Hira_BadMap_Version = "V1";
  double ImpactPar_Range[2] = {0,3.0};
  string EffCor_Tag = "_GeoEff"; //"_GeoEff", "_noEff", "_GeoReactionEff"

  string ESpecStoreFileName = "./ESpec/f1_ESpec_"+SystemTag+"_"+RunTag+"_"+Hira_BadMap_Version+".root";
  
  TFile* f1_ESpec = new TFile(ESpecStoreFileName.c_str(),"update");
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  
  TH2D* h2_Theta_Ekin_CM[20];
  
  char NameTem[200];
  for(int i=0;i<ParticleNum;i++)
  {
    sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_CM%s_ImpactPars",ParticleName[i].c_str(),ImpactPar_Range[0],ImpactPar_Range[1],EffCor_Tag.c_str());
    h2_Theta_Ekin_CM[i] = (TH2D*) f1_ESpec->Get(NameTem);
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
  double ThetaBinSize = 20;
  TH1D* h1_ESpec_PID[ParticleNum][ThetaNum];
  EkinTheta_Spec->SetThetaConfig(ThetaNum, StartTheta, ThetaBinSize);
  
  for(int i=0;i<ParticleNum;i++)
  {
    EkinTheta_Spec->SetParticleTag(ParticleName[i].c_str());
    EkinTheta_Spec->ProduceEkinSpectrum(h2_Theta_Ekin_CM[i],h1_ESpec_PID[i]);
    EkinTheta_Spec->DrawInfo();
  }

  //store in the root file for further analysis.
  sprintf(NameTem,"./IsoScaling/f1_ESpec%s_%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag.c_str(),SystemTag.c_str(),RunTag.c_str(),Hira_BadMap_Version.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
  TFile* f1_ESpec_ForIsoScaling = new TFile(NameTem,"update");
  
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    h2_Theta_Ekin_CM[iPID]->Write("",TObject::kOverwrite);
    for(int i=0;i<ThetaNum;i++)
    {
       h1_ESpec_PID[iPID][i]->Write("",TObject::kOverwrite);
     }
  }

}
