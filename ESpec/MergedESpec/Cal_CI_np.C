void Set_1D_HistoStyle(TH1D* h1);

void Cal_CI_np()
{
  int RebinNum = 5;
  
  int EffCor_Index = 1;
  string EffCor_Tag[] = { "_GeoReactionEff", "_GeoEff", "_noEff"};
//  double ThetaRange[2] = {80.0,90.0};
  double ThetaRange[2] = {90.0,100.0};
  
  string SystemTag_1 = "Ca48Ni64E140";
  string RunTag_1 = "Merged";
  
  string SystemTag_2 = "Ca40Ni58E140";
  string RunTag_2 = "Merged";
  double ImpactPar_Range[2] = {0,3.0};
  
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  
  char NameTem[200];
  sprintf(NameTem,"./f1_ESpec%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag[EffCor_Index].c_str(),SystemTag_1.c_str(),RunTag_1.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
  TFile* f1_ESpec_1 = new TFile(NameTem,"update");
  
  sprintf(NameTem,"./f1_ESpec%s_%s_%s_b_%.1f_%.1f.root",EffCor_Tag[EffCor_Index].c_str(),SystemTag_2.c_str(),RunTag_2.c_str(),ImpactPar_Range[0],ImpactPar_Range[1]);
  TFile* f1_ESpec_2 = new TFile(NameTem,"update");
  
  TH1D* h1_ESpec_1[ParticleNum];
  TH1D* h1_ESpec_2[ParticleNum];
  TH1D* h1_ESpec_T_3He_R1; TH1D* h1_ESpec_T_3He_R2;
  
  TCanvas* c1_ESpec = new TCanvas("c1_ESpec","c1_ESpec",1);
  c1_ESpec->Divide(3,2);
  
  //read the ESpec of kinds of particle from the root file.
  for(int iPID = 0;iPID<ParticleNum;iPID++)
  {
    sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag_1.c_str(),RunTag_1.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
    h1_ESpec_1[iPID] = (TH1D*) f1_ESpec_1->Get(NameTem);
    sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag_2.c_str(),RunTag_2.c_str(),ParticleName[iPID].c_str(),ThetaRange[0],ThetaRange[1]);
    h1_ESpec_2[iPID] = (TH1D*) f1_ESpec_2->Get(NameTem);
    
    c1_ESpec->cd(iPID+1); h1_ESpec_1[iPID]->Draw("hist");
    h1_ESpec_2[iPID]->Draw("histsame"); 
    h1_ESpec_1[iPID]->SetLineColor(1);
    h1_ESpec_2[iPID]->SetLineColor(2);
    
    h1_ESpec_1[iPID]->Rebin(RebinNum);
    h1_ESpec_1[iPID]->Scale(1.0/RebinNum);
    h1_ESpec_2[iPID]->Rebin(RebinNum);
    h1_ESpec_2[iPID]->Scale(1.0/RebinNum);
    
    Set_1D_HistoStyle(h1_ESpec_2[iPID]);
    Set_1D_HistoStyle(h1_ESpec_1[iPID]);
  }
  
  h1_ESpec_T_3He_R1 = (TH1D*) h1_ESpec_1[2]->Clone("h1_ESpec_T_3He_R1");
  h1_ESpec_T_3He_R2 = (TH1D*) h1_ESpec_2[2]->Clone("h1_ESpec_T_3He_R2");
  h1_ESpec_T_3He_R1->GetYaxis()->SetTitle("R(^{3}He/T)");
  h1_ESpec_T_3He_R2->GetYaxis()->SetTitle("R(^{3}He/T)");
  h1_ESpec_T_3He_R1->Divide(h1_ESpec_1[3]);
  h1_ESpec_T_3He_R2->Divide(h1_ESpec_2[3]);
  
  TCanvas* c1_ESpec_T_3He = new TCanvas("c1_ESpec_T_3He","c1_ESpec_T_3He",1);
  h1_ESpec_T_3He_R1->Draw("hist"); Set_1D_HistoStyle(h1_ESpec_T_3He_R1);
  h1_ESpec_T_3He_R2->Draw("histsame"); Set_1D_HistoStyle(h1_ESpec_T_3He_R2);
  
  TH1D* h1_CI_p2n_1 = (TH1D*) h1_ESpec_T_3He_R1->Clone("h1_CI_p2n_1");
  h1_CI_p2n_1->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  h1_CI_p2n_1->Multiply(h1_ESpec_1[0]);
  TH1D* h1_CI_p2n_2 = (TH1D*) h1_ESpec_T_3He_R2->Clone("h1_CI_p2n_2");
  h1_CI_p2n_2->Multiply(h1_ESpec_2[0]);
  h1_CI_p2n_2->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  
  //the below is for the 1st system.
  TH1D* h1_CI_n_1 = (TH1D*) h1_ESpec_1[1]->Clone("h1_CI_n_1");
  h1_CI_n_1->Add(h1_CI_p2n_1,1);
  h1_CI_n_1->Add(h1_ESpec_1[2],2);
  h1_CI_n_1->Add(h1_ESpec_1[3],1);
  h1_CI_n_1->Add(h1_ESpec_1[4],2);
  h1_CI_n_1->Add(h1_ESpec_1[5],4);
  
  TH1D* h1_CI_p_1 = (TH1D*) h1_ESpec_1[1]->Clone("h1_CI_p_1");
  h1_CI_p_1->Add(h1_ESpec_1[0],1);
  h1_CI_p_1->Add(h1_ESpec_1[2],1);
  h1_CI_p_1->Add(h1_ESpec_1[3],2);
  h1_CI_p_1->Add(h1_ESpec_1[4],2);
  h1_CI_p_1->Add(h1_ESpec_1[5],2);
  
  TCanvas* c1_CI_np_ESpec = new TCanvas("c1_CI_np_ESpec","c1_CI_np_ESpec",1);
  c1_CI_np_ESpec->Divide(2,2);
  c1_CI_np_ESpec->cd(1)->SetLogy(1); 
  h1_CI_p2n_1->Draw("hist"); h1_CI_p2n_1->SetLineColor(1);
  h1_CI_p2n_1->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  TH1D* h1_P_ESpec_tem1 = (TH1D*) h1_ESpec_1[0]->Clone("h1_P_ESpec_tem1");
  h1_P_ESpec_tem1->Draw("histsame"); 
  h1_P_ESpec_tem1->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  h1_P_ESpec_tem1->SetLineColor(2);
  
  c1_CI_np_ESpec->cd(2)->SetLogy(1); 
  h1_CI_n_1->Draw("hist"); h1_CI_n_1->SetLineColor(1);
  h1_CI_n_1->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  Set_1D_HistoStyle(h1_CI_n_1);
  h1_CI_p_1->Draw("histsame"); h1_CI_p_1->SetLineColor(2);
  Set_1D_HistoStyle(h1_CI_p_1);
  h1_CI_p_1->GetYaxis()->SetTitle("d^{2}M/dEd#Omega(MeV^{-1}Sr^{-1})");
  
  
  //the below is for the 2nd system.
  TH1D* h1_CI_n_2 = (TH1D*) h1_ESpec_2[1]->Clone("h1_CI_n_2");
  h1_CI_n_2->Add(h1_CI_p2n_2,1);
  h1_CI_n_2->Add(h1_ESpec_2[2],2);
  h1_CI_n_2->Add(h1_ESpec_2[3],1);
  h1_CI_n_2->Add(h1_ESpec_2[4],2);
  h1_CI_n_2->Add(h1_ESpec_2[5],4);
  
  TH1D* h1_CI_p_2 = (TH1D*) h1_ESpec_2[1]->Clone("h1_CI_p_2");
  h1_CI_p_2->Add(h1_ESpec_2[0],1);
  h1_CI_p_2->Add(h1_ESpec_2[2],1);
  h1_CI_p_2->Add(h1_ESpec_2[3],2);
  h1_CI_p_2->Add(h1_ESpec_2[4],2);
  h1_CI_p_2->Add(h1_ESpec_2[5],2);
  
  c1_CI_np_ESpec->cd(3)->SetLogy(1); 
  h1_CI_p2n_2->Draw("hist"); h1_CI_p2n_2->SetLineColor(1);
  h1_ESpec_2[0]->Draw("histsame"); h1_ESpec_2[0]->SetLineColor(2);
  
  c1_CI_np_ESpec->cd(4)->SetLogy(1); 
  h1_CI_n_2->Draw("hist"); h1_CI_n_2->SetLineColor(1);
  Set_1D_HistoStyle(h1_CI_n_2);
  h1_CI_p_2->Draw("histsame"); h1_CI_p_2->SetLineColor(2);
  Set_1D_HistoStyle(h1_CI_p_2); 
  
  //The below is for the CI np ratio
  TH1D* h1_CI_np_Ratio_1 = (TH1D*) h1_CI_n_1->Clone("h1_CI_np_Ratio_1");
  h1_CI_np_Ratio_1->Divide(h1_CI_p_1);
  h1_CI_np_Ratio_1->GetYaxis()->SetTitle("Ratio(CI-n/CI-p)");
  TH1D* h1_CI_np_Ratio_2 = (TH1D*) h1_CI_n_2->Clone("h1_CI_np_Ratio_2");
  h1_CI_np_Ratio_2->Divide(h1_CI_p_2);
  h1_CI_np_Ratio_2->GetYaxis()->SetTitle("Ratio(CI-n/CI-p)");
  
//  h1_CI_np_Ratio_1->Rebin(10);
//  h1_CI_np_Ratio_2->Rebin(10);
  
  TCanvas* c1_CI_np_Ratio = new TCanvas("c1_CI_np_Ratio","c1_CI_np_Ratio",1);
  c1_CI_np_Ratio->cd();
  h1_CI_np_Ratio_1->Draw("hist"); h1_CI_np_Ratio_1->SetLineColor(1);
  h1_CI_np_Ratio_2->Draw("histsame"); h1_CI_np_Ratio_2->SetLineColor(2);
  
  TH1D* h1_CI_np_DR = (TH1D*) h1_CI_np_Ratio_1->Clone("h1_CI_np_DR");
  h1_CI_np_DR->Divide(h1_CI_np_Ratio_2);
  h1_CI_np_DR->GetYaxis()->SetTitle("DR(CI-n/CI-p)");
  TCanvas* c1_CI_np_DR = new TCanvas("c1_CI_np_DR","c1_CI_np_DR",1);
  c1_CI_np_DR->cd();
  h1_CI_np_DR->Draw("hist");
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
