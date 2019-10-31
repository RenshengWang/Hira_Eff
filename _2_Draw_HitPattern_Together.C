void _2_Draw_HitPattern_Together()
{
  string RunNo = "2860";
  string ConfigTag = "ThetaUniform";
  
  TFile* f1_Hira_HitPattern_Exp = new TFile((RunNo+"/f1_"+RunNo+"_Hira_HitPattern_Exp.root").c_str(),"update");
  TH2D* h2_WholeHira_Theta_Phi_Lab_Exp = (TH2D*) f1_Hira_HitPattern_Exp->Get(("h2_WholeHira_Theta_Phi_Lab_run"+RunNo).c_str());
  
  TFile* f1_Sim_Results = new TFile((RunNo+"/f1_"+RunNo+"_SimGeo.root").c_str(),"update");
  TH2D* h2_Theta_Phi_BadStrip = (TH2D*) f1_Sim_Results->Get(("h2_Theta_Phi_"+ConfigTag+"_BadStrip").c_str());
  
  TCanvas* c1_HitPattern_Compare = new TCanvas("c1_HitPattern_Compare","c1_HitPattern_Compare",1);
  c1_HitPattern_Compare->cd();
  h2_Theta_Phi_BadStrip->Draw("colz");
  h2_WholeHira_Theta_Phi_Lab_Exp->Draw("same");
  
}
