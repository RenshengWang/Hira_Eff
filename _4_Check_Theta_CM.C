TH1D* Get_ThetaDis(int Index,double Ekin_Theta_CM_Range[][4],TH2D* h2_Theta_Ekin_CM);

void _4_Check_Theta_CM()
{
  string DataPath = "./"; //"/mnt/analysis/e15190/Brown/rootfiles";
  string RunNo = "2860";
  string SystemTag = "Ca40Ni58E140";//"Ca40Ni56E140";
  string DataFileName = "CalibratedData_"+RunNo+".root";
  string ESpecStoreFileName = "f1_ESpec_"+RunNo+".root";
  
  TFile* f1_ESpec = new TFile((DataPath+RunNo+"/"+ESpecStoreFileName).c_str(),"update");
  const int ParticleNum = 5;
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
  
  //the below is for checking the hit count in the CM.
  //check Proton First.
  const int EkinCM_Num = 6;
  double Ekin_Theta_CM_Range[100][4] = {{20,40,60,120},
                                        {40,60,50,110},
                                        {60,80,50,110},
                                        {80,100,50,100},
                                        {100,120,60,100},
                                        {120,140,70,100}};
  
  TH1D* h1_ThetaCM_Dis[EkinCM_Num] = {0};
  int ParticleIndex = 0;
  
  TCanvas* c1_ThetaCM_Dis = new TCanvas("c1_ThetaCM_Dis","c1_ThetaCM_Dis",1);
  for(int i=0;i<EkinCM_Num;i++)
  {
    h1_ThetaCM_Dis[i] = Get_ThetaDis(i,Ekin_Theta_CM_Range,h2_Theta_Ekin_CM_GeoReactionEff[ParticleIndex]);
    if(i==0){ h1_ThetaCM_Dis[i]->Draw("hist");}
    else { h1_ThetaCM_Dis[i]->Draw("histsame"); }
  }
  
  TCanvas* c1_Ekin_Theta_CM_GeoReactionEff = new TCanvas("c1_Ekin_Theta_CM_GeoReactionEff","c1_Ekin_Theta_CM_GeoReactionEff",1);
  c1_Ekin_Theta_CM_GeoReactionEff->Divide(3,2);
  
  TCanvas* c1_Ekin_Theta_CM_GeoEff = new TCanvas("c1_Ekin_Theta_CM_GeoEff","c1_Ekin_Theta_CM_GeoEff",1);
  c1_Ekin_Theta_CM_GeoEff->Divide(3,2);
  
  TCanvas* c1_Ekin_Theta_CM_noEff = new TCanvas("c1_Ekin_Theta_CM_noEff","c1_Ekin_Theta_CM_noEff",1);
  c1_Ekin_Theta_CM_noEff->Divide(3,2);
  
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    c1_Ekin_Theta_CM_GeoReactionEff->cd(iPID+1)->SetGridx(1);
    c1_Ekin_Theta_CM_GeoReactionEff->cd(iPID+1)->SetGridy(1);
    h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Draw("colz");
    
    c1_Ekin_Theta_CM_GeoEff->cd(iPID+1)->SetGridx(1);
    c1_Ekin_Theta_CM_GeoEff->cd(iPID+1)->SetGridy(1);
    h2_Theta_Ekin_CM_GeoEff[iPID]->Draw("colz");
    
    c1_Ekin_Theta_CM_noEff->cd(iPID+1)->SetGridx(1);
    c1_Ekin_Theta_CM_noEff->cd(iPID+1)->SetGridy(1);
    h2_Theta_Ekin_CM_noEff[iPID]->Draw("colz");
  }
}

TH1D* Get_ThetaDis(int Index,double Ekin_Theta_CM_Range[][4],TH2D* h2_Theta_Ekin_CM)
{
  double Proj_R1 = Ekin_Theta_CM_Range[Index][0]; double Proj_R2 = Ekin_Theta_CM_Range[Index][1];
  double Cut_Theta1 = Ekin_Theta_CM_Range[Index][2]; double Cut_Theta2 = Ekin_Theta_CM_Range[Index][3];
  int Proj_R1_Bin = h2_Theta_Ekin_CM->GetXaxis()->FindBin(Proj_R1);
  int Proj_R2_Bin = h2_Theta_Ekin_CM->GetXaxis()->FindBin(Proj_R2);;
  int Cut_Theta1_Bin = h2_Theta_Ekin_CM->GetYaxis()->FindBin(Cut_Theta1); 
  int Cut_Theta2_Bin = h2_Theta_Ekin_CM->GetYaxis()->FindBin(Cut_Theta2);
  
  char NameTem[200];
  sprintf(NameTem,"h1_ThetaCM_%.0f_%.0f_Dis",Proj_R1,Proj_R2);
  TH1D* h1_ThetaCM_Dis = h2_Theta_Ekin_CM->ProjectionY(NameTem,Proj_R1_Bin,Proj_R2_Bin);
  for(int i=1;i<h1_ThetaCM_Dis->GetNbinsX();i++)
  {
    if(i<Cut_Theta1_Bin || i>Cut_Theta2_Bin) { h1_ThetaCM_Dis->SetBinContent(i,0); }
  }
  h1_ThetaCM_Dis->Rebin(4);
return h1_ThetaCM_Dis;
}
