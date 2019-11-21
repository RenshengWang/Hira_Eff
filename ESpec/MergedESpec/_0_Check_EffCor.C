#include "TMath.h"
using namespace TMath;
void Cal_Err(TH1D* h1_ESpec_Err);


void _0_Check_EffCor()
{
//  string SystemTag = "Ca48Ni64E140";
  string SystemTag = "Ca40Ni58E140";
  string RunTag = "Merged";
  double ImpactPar_Range[2] = {0,3.0};
  double ThetaRange[2] = {80.0,90.0};
//  double ThetaRange[2] = {90.0,100.0};
  
  const int EffNum = 3;
  string EffCor_Tag[] = { "_GeoReactionEff", "_GeoEff", "_noEff"};
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  
  TH1D* h1_ESpec[EffNum][ParticleNum];
  TH1D* h1_ESpec_Err[EffNum][ParticleNum];
  TH1D* h1_ESpec_Err_Ratio[ParticleNum];
  
  char NameTem[200];
  
  TCanvas* c1_ESpec = new TCanvas("c1_ESpec","c1_ESpec",1);
  c1_ESpec->Divide(3,2);
  TCanvas* c1_ESpec_Err = new TCanvas("c1_ESpec_Err","c1_ESpec_Err",1);
  c1_ESpec_Err->Divide(3,2);
  
  int LineColor[EffNum] = {2,4,1};
  
  for(int i=0;i<EffNum;i++)
  {
    sprintf(NameTem,"./f1_ESpec%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag[i].c_str(),SystemTag.c_str(),RunTag.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
    TFile* f1_ESpec_ForIsoScaling = new TFile(NameTem,"read");
    
    for(int iPID=0;iPID<ParticleNum;iPID++)
    {
      sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag.c_str(),RunTag.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
      h1_ESpec[i][iPID] = (TH1D*) f1_ESpec_ForIsoScaling->Get(NameTem);
      h1_ESpec[i][iPID]->SetLineColor(LineColor[i]);
      
      sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Err_Dis_Theta_%.1f_%.1f",SystemTag.c_str(),RunTag.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
      h1_ESpec_Err[i][iPID] = (TH1D*)h1_ESpec[i][iPID]->Clone(NameTem);
      Cal_Err(h1_ESpec_Err[i][iPID]);
      h1_ESpec_Err[i][iPID]->SetLineColor(LineColor[i]);
      c1_ESpec->cd(iPID+1)->SetLogy(1);
      if(i==0) { h1_ESpec[i][iPID]->Draw("hist"); }
      else { h1_ESpec[i][iPID]->Draw("histsame"); }
      
      c1_ESpec_Err->cd(iPID+1)->SetLogy(1);
      if(i==0) { h1_ESpec_Err[i][iPID]->Draw("hist"); }
      else { h1_ESpec_Err[i][iPID]->Draw("histsame"); }
    }
  }
  
  TCanvas* c1_ESpec_Err_Ratio = new TCanvas("c1_ESpec_Err_Ratio","c1_ESpec_Err_Ratio",1);
  c1_ESpec_Err_Ratio->Divide(3,2);
  
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    h1_ESpec_Err_Ratio[iPID] = (TH1D*) h1_ESpec_Err[1][iPID]->Clone();
    h1_ESpec_Err_Ratio[iPID]->Divide(h1_ESpec_Err[2][iPID]);
    c1_ESpec_Err_Ratio->cd(iPID+1);
    h1_ESpec_Err_Ratio[iPID]->Draw("hist");
  }
  
}

void Cal_Err(TH1D* h1_ESpec_Err)
{
  int XBinNum = h1_ESpec_Err->GetNbinsX();
  for(int i=1;i<XBinNum;i++)
  {
    double BinContent = h1_ESpec_Err->GetBinContent(i);
    double BinContentErr = h1_ESpec_Err->GetBinError(i);
    if(BinContent!=0)
    h1_ESpec_Err->SetBinContent(i,BinContentErr/BinContent);
  }
}
