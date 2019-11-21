void Set_1D_HistoStyle(TH1D* h1);

void Cal_IsoScaling()
{
  int EffCor_Index = 0;
  int RebinNum = 5;
  string EffCor_Tag[] = { "_GeoReactionEff", "_GeoEff", "_noEff"};
  double ImpactPar_Range[2] = {0.0,3.0};
  double ThetaRange[2] = {80.0,90.0};
//  double ThetaRange[2] = {90.0,100.0};
  
  string SystemTag_1 = "Ca48Ni64E140";
  string RunTag_1 = "Merged";
  
  string SystemTag_2 = "Ca40Ni58E140";
  string RunTag_2 = "Merged";
  
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  double S1_Multi_perEvt[ParticleNum] = {0,0,0,0,0,0};
  double S2_Multi_perEvt[ParticleNum] = {0,0,0,0,0,0};
  double S21_MultiRatio_perEvt[ParticleNum] = {0,0,0,0,0,0};
  
  char NameTem[200];
  sprintf(NameTem,"./f1_ESpec%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag[EffCor_Index].c_str(),SystemTag_1.c_str(),RunTag_1.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
  TFile* f1_ESpec_1 = new TFile(NameTem,"update");
  
  sprintf(NameTem,"./f1_ESpec%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag[EffCor_Index].c_str(),SystemTag_2.c_str(),RunTag_2.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
  TFile* f1_ESpec_2 = new TFile(NameTem,"update");
  
  TH1D* h1_ESpec_1[ParticleNum];
  TH1D* h1_ESpec_2[ParticleNum];
  TH1D* h1_ESpec_Ratio[ParticleNum];
  
  TCanvas* c1_ESpec = new TCanvas("c1_ESpec","c1_ESpec",1);
  c1_ESpec->Divide(3,2);
  
  TCanvas* c1_ESpec_Ratio = new TCanvas("c1_ESpec_Ratio","c1_ESpec_Ratio",1);
  
  int PIDColor[]={1,1,1,2,2,2};
  int MarkerStyle[]={21,22,23,21,22,23};
  
  for(int iPID = 0;iPID<ParticleNum;iPID++)
  {
    sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag_1.c_str(),RunTag_1.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
    h1_ESpec_1[iPID] = (TH1D*) f1_ESpec_1->Get(NameTem);
    
    sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag_2.c_str(),RunTag_2.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
    h1_ESpec_2[iPID] = (TH1D*) f1_ESpec_2->Get(NameTem);
    
    //the integral should be ahead of the rebin.
    S1_Multi_perEvt[iPID] = h1_ESpec_1[iPID]->Integral(1,70);
    S2_Multi_perEvt[iPID] = h1_ESpec_2[iPID]->Integral(1,70);
    
    h1_ESpec_1[iPID]->Rebin(RebinNum);
    h1_ESpec_1[iPID]->Scale(1.0/RebinNum);
    h1_ESpec_2[iPID]->Rebin(RebinNum);
    h1_ESpec_2[iPID]->Scale(1.0/RebinNum);
    
    
    
    c1_ESpec->cd(iPID+1);
    h1_ESpec_1[iPID]->SetLineColor(1);
    h1_ESpec_1[iPID]->Draw("hist"); Set_1D_HistoStyle(h1_ESpec_1[iPID]);
    h1_ESpec_2[iPID]->Draw("histsame"); Set_1D_HistoStyle(h1_ESpec_2[iPID]);
    h1_ESpec_2[iPID]->SetLineColor(2);
    
    sprintf(NameTem,"h1_ESpec_Ratio_%s_%s_%s",SystemTag_1.c_str(),SystemTag_2.c_str(),ParticleName[iPID].c_str());
    h1_ESpec_Ratio[iPID] = (TH1D*) h1_ESpec_1[iPID]->Clone(NameTem);
    h1_ESpec_Ratio[iPID]->Divide(h1_ESpec_2[iPID]);
    c1_ESpec_Ratio->cd();
    if(iPID==0) 
    {
      h1_ESpec_Ratio[iPID]->Draw("EC");
      h1_ESpec_Ratio[iPID]->GetXaxis()->SetRangeUser(0,100);
      h1_ESpec_Ratio[iPID]->GetYaxis()->SetRangeUser(0.6,2.0);
    }
    else if(iPID!=5) { h1_ESpec_Ratio[iPID]->Draw("ECsame");}
    
    Set_1D_HistoStyle(h1_ESpec_Ratio[iPID]);
    h1_ESpec_Ratio[iPID]->SetLineColor(PIDColor[iPID]);
    h1_ESpec_Ratio[iPID]->SetMarkerColor(PIDColor[iPID]);
    h1_ESpec_Ratio[iPID]->SetMarkerStyle(MarkerStyle[iPID]);
    h1_ESpec_Ratio[iPID]->SetMarkerSize(1.8);
    h1_ESpec_Ratio[iPID]->SetLineWidth(2.0);
    h1_ESpec_Ratio[iPID]->GetYaxis()->SetTitle("R_{21}(N,Z)");
  }
  
  //the below is for the total yield ratio.
  
  for(int iP=0;iP<ParticleNum;iP++)
  {
    S21_MultiRatio_perEvt[iP] = S1_Multi_perEvt[iP]/S2_Multi_perEvt[iP];
  }
  int Num_H = 3;
  double N_H[3] = {0,1,2};
  double R21_H[3] = {S21_MultiRatio_perEvt[0],S21_MultiRatio_perEvt[1],S21_MultiRatio_perEvt[2]};
  int Num_He = 3;
  double N_He[3] = {1,2,4};
  double R21_He[3] = {S21_MultiRatio_perEvt[3],S21_MultiRatio_perEvt[4],S21_MultiRatio_perEvt[5]};
  
  TGraph* gr1_Ratio_H = new TGraph(Num_H,N_H,R21_H);
  TGraph* gr1_Ratio_He = new TGraph(Num_He,N_He,R21_He);
  
  TCanvas* c1_Ratio = new TCanvas("c1_Ratio","c1_Ratio",1);
  gr1_Ratio_H->Draw("AL*");
  gr1_Ratio_H->GetXaxis()->SetLimits(-0.1,4.5);
  gr1_Ratio_H->GetYaxis()->SetRangeUser(0.5,2);
  gr1_Ratio_He->Draw("Lsame*");
  TLegend* legend = new TLegend(0.6,0.6,0.8,0.8);
  legend->AddEntry(gr1_Ratio_H,"H", "lp");
  legend->AddEntry(gr1_Ratio_He,"He", "lp");
  legend->Draw();
}

void Set_1D_HistoStyle(TH1D* h1)
{
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetLabelSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.05);
}
