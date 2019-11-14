void _2_Draw_HitPattern_Together()
{
/*
  string SystemTag = "Ca40Ni58E140";
  int StartRunNo = 2850;
  const int ExpFileNum = 20;
  string RunTag = "2850-2870";
  string Hira_BadMap_Version = "V2";
*/

  string SystemTag = "Ca48Ni64E140";
  int StartRunNo = 4100;
  const int ExpFileNum = 20;
  string RunTag = "4100-4119";
  string Hira_BadMap_Version = "V1";
  
  
  TFile* f1_Hira_HitPattern_Exp = new TFile(("./GeoEff/f1_HitPattern_"+SystemTag+"_"+RunTag+"_"+Hira_BadMap_Version+".root").c_str(),"read");
  TH2D* h2_WholeHira_Theta_Phi_Lab_Exp = (TH2D*) f1_Hira_HitPattern_Exp->Get("h2_WholeHira_Theta_Phi_LabwithBadMap");
  
  TFile* f1_Sim_Results = new TFile(("./GeoEff/f1_GeoEff_"+Hira_BadMap_Version+".root").c_str(),"read");
  TH2D* h2_Theta_Phi_BadStrip = (TH2D*) f1_Sim_Results->Get("h2_BadMap_Theta_Phi_Lab");
  
  //TFile* f1_Hira_AngleMap = new TFile("BeamPosition_PixelAngle/f1_HitPattern_Hira.root");
  //TH2D* h2_Hira_AngleMap = (TH2D*) f1_Hira_AngleMap->Get("h2_Angle_Pixel_140MeV");
  
  TCanvas* c1_HitPattern_Compare = new TCanvas("c1_HitPattern_Compare","c1_HitPattern_Compare",1);
  c1_HitPattern_Compare->cd();
  h2_Theta_Phi_BadStrip->Draw("colz");
  h2_WholeHira_Theta_Phi_Lab_Exp->Draw("same");
  //h2_Hira_AngleMap->Draw("same");
}
