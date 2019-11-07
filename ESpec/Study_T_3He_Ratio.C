#include "TMath.h"
using namespace TMath;

TH1D* Get_Ratio(int RebinNum, TH1D* h1_T, TH1D* h1_3He);
TH1D* Get_ExtrapolatedESpec(double* Range,TH1D* h1_ESpec, TF1* f1);

void Study_T_3He_Ratio()
{
  string RunTag = "4100-4109";
  string SystemTag = "Ca48Ni64E140";//"Ca40Ni56E140";
  TFile* f1_ESpec_T_3He = new TFile(("f1_T_3He_"+SystemTag+"_"+RunTag+".root").c_str(),"update");
  
  TH1D* h1_ESpec_T = (TH1D*) f1_ESpec_T_3He->Get("h1_Ca48Ni64E140_4100-4109_T_EkinCM_Dis_Theta_80.0_105.0");
  TH1D* h1_ESpec_3He = (TH1D*) f1_ESpec_T_3He->Get("h1_Ca48Ni64E140_4100-4109_3He_EkinCM_Dis_Theta_80.0_105.0");
  
  TH1D* h1_Ratio_T_3He_Rebin = Get_Ratio(2, h1_ESpec_T, h1_ESpec_3He);
  
  TCanvas* c1_ESpec_T_3He = new TCanvas("c1_ESpec_T_3He","c1_ESpec_T_3He",1);
  c1_ESpec_T_3He->Divide(2,1);
  c1_ESpec_T_3He->cd(1); 
  h1_ESpec_T->Draw("hist"); h1_ESpec_T->SetLineColor(1);
  h1_ESpec_T->GetYaxis()->SetTitle("Prob.(Arb.)");
  h1_ESpec_3He->Draw("histsame"); h1_ESpec_3He->SetLineColor(2);
  h1_ESpec_3He->GetYaxis()->SetTitle("Prob.(Arb.)");
  
  c1_ESpec_T_3He->cd(2);
  h1_Ratio_T_3He_Rebin->Draw("hist");
  h1_Ratio_T_3He_Rebin->GetYaxis()->SetTitle("Ratio(T/3He)");
  
  //do the extrapolation
  c1_ESpec_T_3He->cd(1);
  double FitRange[2] = {80,150};
  TF1* func1_T_ExpTail = new TF1("func1_T_ExpTail","[0]*TMath::Exp((x-[1])/[2])",FitRange[0],300);
  double Paras[3] = {1000000,10,-30};
  
  func1_T_ExpTail->SetParameters(Paras);
  h1_ESpec_T->Fit("func1_T_ExpTail","","",FitRange[0],FitRange[1]);
  TF1* f1_tem = h1_ESpec_T->GetFunction("func1_T_ExpTail");
  f1_tem->GetParameters(Paras);
  func1_T_ExpTail->SetParameters(Paras);
  func1_T_ExpTail->SetLineColor(4);
  func1_T_ExpTail->Draw("same");
  
  TF1* func1_3He_ExpTail = new TF1("func1_3He_ExpTail","[0]*TMath::Exp((x-[1])/[2])",FitRange[0],300);
  func1_3He_ExpTail->SetParameters(Paras);
  h1_ESpec_3He->Fit("func1_3He_ExpTail","","",FitRange[0],FitRange[1]);
  f1_tem = h1_ESpec_3He->GetFunction("func1_3He_ExpTail");
  f1_tem->GetParameters(Paras);
  func1_3He_ExpTail->SetParameters(Paras);
  
  func1_3He_ExpTail->Draw("same");
  func1_3He_ExpTail->SetLineColor(6);
  
  //the below is for drawing the extra-histo
  double ExtraRange[2] = {150,299};
  TH1D*  h1_ESpec_T_Extra = Get_ExtrapolatedESpec(ExtraRange,h1_ESpec_T, func1_T_ExpTail);
  TH1D*  h1_ESpec_3He_Extra = Get_ExtrapolatedESpec(ExtraRange,h1_ESpec_3He, func1_3He_ExpTail);
  
  TCanvas* c1_ESpec_T_3He_Extra = new TCanvas("c1_ESpec_T_3He_Extra","c1_ESpec_T_3He_Extra",1);
  c1_ESpec_T_3He_Extra->Divide(2,1);
  c1_ESpec_T_3He_Extra->cd(1); 
  h1_ESpec_T_Extra->Draw("hist"); 
  h1_ESpec_3He_Extra->Draw("histsame");
  
  TH1D* h1_Ratio_T_3He_Rebin_Extra = Get_Ratio(2, h1_ESpec_T_Extra, h1_ESpec_3He_Extra);
  h1_Ratio_T_3He_Rebin_Extra->GetYaxis()->SetTitle("Ratio(T/3He)");
  c1_ESpec_T_3He_Extra->cd(2);
  h1_Ratio_T_3He_Rebin_Extra->Draw("hist");
}

TH1D* Get_Ratio(int RebinNum, TH1D* h1_ESpec_T, TH1D* h1_ESpec_3He)
{
  TH1D* h1_ESpec_T_Rebin = (TH1D*) h1_ESpec_T->Rebin(RebinNum,"h1_ESpec_T_Rebin");
  TH1D* h1_ESpec_3He_Rebin = (TH1D*) h1_ESpec_3He->Rebin(RebinNum,"h1_ESpec_3He_Rebin");
  
  //get the ratio
  TH1D* h1_Ratio_T_3He_Rebin = (TH1D*) h1_ESpec_T_Rebin->Clone("h1_Ratio_T_3He_Rebin");
  h1_Ratio_T_3He_Rebin->Reset();
  int XBinNum = h1_ESpec_T_Rebin->GetNbinsX();
  for(int iX=1;iX<=XBinNum;iX++)
  {
    double C1 = h1_ESpec_T_Rebin->GetBinContent(iX);
    double C2 = h1_ESpec_3He_Rebin->GetBinContent(iX);
    if(C1!=0 && C2!=0)
    {
      double Ratio = C1/C2;
      h1_Ratio_T_3He_Rebin->SetBinContent(iX,Ratio);
    }
  }
return h1_Ratio_T_3He_Rebin;
}

TH1D* Get_ExtrapolatedESpec(double* Range,TH1D* h1_ESpec, TF1* f1)
{
  string HistoName = h1_ESpec->GetName();
  TH1D* h1_ESpec_Extra = (TH1D*) h1_ESpec->Clone((HistoName+"_Extra").c_str());
  int Bin1 = h1_ESpec->FindBin(Range[0]);
  int Bin2 = h1_ESpec->FindBin(Range[1]);
  
  cout<<Bin1<<"  "<<Bin2<<endl;
  
  for(int i=Bin1;i<=Bin2;i++)
  {
    double BinCenter = h1_ESpec->GetBinCenter(i);
    double ExtraValue = f1->Eval(BinCenter);
    
    h1_ESpec_Extra->SetBinContent(i,ExtraValue);
  }
return h1_ESpec_Extra;
}
