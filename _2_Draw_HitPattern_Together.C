void _2_Draw_HitPattern_Together()
{
  string RunTag = "4100-4101";
  string Hira_BadMap_Version = "V1";
  string ConfigTag = "ThetaUniform";
  
  TFile* f1_Hira_HitPattern_Exp = new TFile(("./GeoEff/f1_HitPattern"+RunTag+".root").c_str(),"update");
  TH2D* h2_WholeHira_Theta_Phi_Lab_Exp = (TH2D*) f1_Hira_HitPattern_Exp->Get("h2_WholeHira_Theta_Phi_LabwithBadMap");
  
  TFile* f1_Sim_Results = new TFile(("./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root").c_str(),"update");
  TH2D* h2_Theta_Phi_BadStrip = (TH2D*) f1_Sim_Results->Get("h2_BadMap_Theta_Phi_Lab");
  
  TCanvas* c1_HitPattern_Compare = new TCanvas("c1_HitPattern_Compare","c1_HitPattern_Compare",1);
  c1_HitPattern_Compare->cd();
  h2_Theta_Phi_BadStrip->Draw("colz");
  h2_WholeHira_Theta_Phi_Lab_Exp->Draw("same");
  
}
