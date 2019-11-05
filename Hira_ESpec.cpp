#include "Hira_ESpec.hh"
ClassImp(Hira_ESpec);

Hira_ESpec::Hira_ESpec()
{
  Hira_BadMapper = 0;
  Hira_GeoEfficiency = 0;
}

Hira_ESpec::~Hira_ESpec()
{;}

void Hira_ESpec::Initial_ESpecHisto()
{
  //checking the spectrum of lab for different particles.
  Num_Theta_Lab_ForChecking = 5;
  TH1D* h1_GeoEff_tem = Hira_GeoEfficiency->Get_ThetaEff_Histogram();
  double R1 = h1_GeoEff_tem->GetBinCenter( h1_GeoEff_tem->FindFirstBinAbove(GeoEff_Cut));
  double R2 = h1_GeoEff_tem->GetBinCenter( h1_GeoEff_tem->FindLastBinAbove(GeoEff_Cut));
  for(int i=0;i<Num_Theta_Lab_ForChecking;i++)
  {
    Theta_Lab_Range_ForChecking[i][0] = R1+i*(R2-R1)/Num_Theta_Lab_ForChecking;
    Theta_Lab_Range_ForChecking[i][1] = R1+(i+1)*(R2-R1)/Num_Theta_Lab_ForChecking;
  }
  
  ParticleNum = 6;
  string ParticleNameTem[]={"P","D","T","3He","4He","6He","6Li","7Li"};
  double Mass_1u = 931.49410242;
  double MassTem[] = {938.272,1876.179,2809.514,2809.496,3728.510,6.01888589*Mass_1u,6.0151228874*Mass_1u,7.016003437*Mass_1u}; //unit: MeV/c2
  int ZTem[] = {1,1,1,2,2,2,3,3};
  int ATem[] = {1,2,3,3,4,6,6,7};
  
  char NameTem[200];
  for(int i=0;i<ParticleNum;i++)
  {
    ParticleName[i] = ParticleNameTem[i];
    ParticleMass[i] = MassTem[i];
    ParticleZ[i] = ZTem[i];
    ParticleA[i] = ATem[i];
    
    for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
    {
      double R1 = Theta_Lab_Range_ForChecking[iTheta_Lab][0]; double R2 = Theta_Lab_Range_ForChecking[iTheta_Lab][1];
      sprintf(NameTem,"h1_%s_Hira_ESpec_Lab_GeoEff_Theta_%.1f_%.0f",ParticleName[i].c_str(),R1,R2);
      h1_Hira_ESpec_Lab_GeoEff[i][iTheta_Lab] = new TH1D(NameTem,";Ekin_{Lab}(MeV/u);Count",200,0,200);
      
      sprintf(NameTem,"h1_%s_Hira_ESpec_Lab_GeoReactionEff_Theta_%.1f_%.0f",ParticleName[i].c_str(),R1,R2);
      h1_Hira_ESpec_Lab_GeoReactionEff[i][iTheta_Lab] = new TH1D(NameTem,";Ekin_{Lab}(MeV/u);Count",200,0,200);
    }

    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoReactionEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoReactionEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV);#theta_{CM}(Deg.)",300,0,300,180,0,180);
    
    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV);#theta_{CM}(Deg.)",300,0,300,180,0,180);

    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_noEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_noEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV);#theta_{CM}(Deg.)",300,0,300,180,0,180);
  }
  
  h1_Hira_ThetaLab_Dis = new TH1D("h1_Hira_ThetaLab_Dis",";#theta_{Lab}(Deg.);Count",600,20,80);
  h2_PID_ZA = new TH2D("h2_PID_ZA",";ParticleZ;ParticleA",10,0,10,10,0,10);
}

void Hira_ESpec::ReadExpData(int ExpFileNum,string ExpDataFile[],string FileForStore)
{
  if(Hira_BadMapper==0) { cout<<"Hira_BadMapper==0"<<endl; return; }
  if(Hira_GeoEfficiency==0) { cout<<"Hira_GeoEfficiency==0"<<endl; return; }
  
  Initial_ESpecHisto();
  
  TChain* t1_Data = new TChain("E15190");
  for(int i=0;i<ExpFileNum;i++)
  {
    t1_Data->AddFile(ExpDataFile[i].c_str());
  }
  
  Int_t fmulti;
  double fTheta[200]; double fPhi[200]; 
  int fnumtel[200]; int fnumstripf[200]; int fnumstripb[200];
  double fEnergySifCal[200]; double fEnergySibCal[200]; 
  double fEnergycsifCal[200];
  double fKinEnergy[200];
  int fZId[200]; int fAId[200];
  
  t1_Data->SetMakeClass(1);
  t1_Data->SetBranchStatus("*",0);
  t1_Data->SetBranchAddress("HiRA.fmulti",&fmulti);
  t1_Data->SetBranchAddress("HiRA.fnumtel",fnumtel);
  t1_Data->SetBranchAddress("HiRA.fnumstripf",fnumstripf);
  t1_Data->SetBranchAddress("HiRA.fnumstripb",fnumstripb);
  t1_Data->SetBranchAddress("HiRA.fTheta",fTheta);
  t1_Data->SetBranchAddress("HiRA.fPhi",fPhi);
  
  t1_Data->SetBranchAddress("HiRA.fKinEnergy",fKinEnergy);
  t1_Data->SetBranchAddress("HiRA.fZId",fZId);
  t1_Data->SetBranchAddress("HiRA.fAId",fAId);
  
  int EvtNum = t1_Data->GetEntries();
  cout<<" EvtNum : "<<EvtNum<<endl;
  
  for(int iEvt = 0;iEvt<EvtNum;iEvt++)
  {
    if(iEvt%100000==0) { cout<<"iEvt: "<<iEvt<<endl; }
    t1_Data->GetEntry(iEvt);
    
    if(fmulti<=0) { continue; }
    for(int iP=0;iP<fmulti;iP++)
    {
      bool IsBad_Hira = Hira_BadMapper->IsBad_Hira(fnumtel[iP]);
      bool IsBad_CsI = Hira_BadMapper->IsBad_CsI(fnumtel[iP],fnumstripf[iP],fnumstripb[iP]);
      bool IsBad_StripX = Hira_BadMapper->IsBad_StripX(fnumtel[iP],fnumstripf[iP]);
      bool IsBad_StripY = Hira_BadMapper->IsBad_StripY(fnumtel[iP],fnumstripb[iP]);
      if(IsBad_Hira==0 && IsBad_CsI==0 && IsBad_StripX==0 && IsBad_StripY==0)
      {
        h2_PID_ZA->Fill(fZId[iP],fAId[iP]);
        for(int iPID=0;iPID<ParticleNum;iPID++)
        {
          if(fAId[iP]==ParticleA[iPID] && fZId[iP]==ParticleZ[iPID])
          {
            double P_Mag_Lab = Sqrt(fKinEnergy[iP]*fKinEnergy[iP]+2*fKinEnergy[iP]*ParticleMass[iPID]);
            TVector3 P_3D_Lab(0,0,P_Mag_Lab);
            
            P_3D_Lab.SetTheta(fTheta[iP]);
            P_3D_Lab.SetPhi(fPhi[iP]);
            
            /*
            P_3D_Lab.SetTheta(HiraPos->GetTheta(fnumtel[iP],fnumstripf[iP],fnumstripb[iP])*DegToRad());
            P_3D_Lab.SetPhi(HiraPos->GetPhi(fnumtel[iP],fnumstripf[iP],fnumstripb[iP])*DegToRad());
            */
            double Theta_Lab = P_3D_Lab.Theta()*RadToDeg();
            
            TLorentzVector P_4D_CM(P_3D_Lab,ParticleMass[iPID]+fKinEnergy[iP]);
            P_4D_CM.Boost(BetaVector_LabToCM[CurrentSystemIndex]);
            TVector3 P_3D_CM(P_4D_CM(0),P_4D_CM(1),P_4D_CM(2));
            double Theta_CM = P_3D_CM.Theta()*RadToDeg();
            double Phi_CM = P_3D_CM.Phi()*RadToDeg();
            double Ekin_CM = Sqrt(P_3D_CM.Mag()*P_3D_CM.Mag()+ParticleMass[iPID]*ParticleMass[iPID])-ParticleMass[iPID];
            
            double EffGeo = Hira_GeoEfficiency->Get_GeoEff(P_3D_Lab.Theta()*RadToDeg());
            double ReactionEff = Get_ReactionLost_CorEff(ParticleZ[iPID], ParticleA[iPID], fKinEnergy[iP]);
            
            h1_Hira_ThetaLab_Dis->Fill(Theta_Lab);
            int ThetaLab_Index = -1;
            for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
            {
              if(Theta_Lab_Range_ForChecking[iTheta_Lab][0]<Theta_Lab && Theta_Lab_Range_ForChecking[iTheta_Lab][1]>Theta_Lab)
              { ThetaLab_Index = iTheta_Lab; break; }
            }
            
            if(EffGeo>=GeoEff_Cut)
            {
              h2_Theta_Ekin_CM_GeoEff[iPID]->Fill(Ekin_CM,Theta_CM,1.0/EffGeo);
              if(ReactionEff>0.01) { h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Fill(Ekin_CM,Theta_CM,1.0/(EffGeo*ReactionEff)); }
              h2_Theta_Ekin_CM_noEff[iPID]->Fill(Ekin_CM,Theta_CM);
              if(ThetaLab_Index!=-1)
              {
                h1_Hira_ESpec_Lab_GeoEff[iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/EffGeo);
                if(ReactionEff>0.01) { h1_Hira_ESpec_Lab_GeoReactionEff[iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/(EffGeo*ReactionEff)); }
              }
              break;
            }
          }
        }
      }
    }
  }
  
  TFile* f1_ESpec = new TFile(FileForStore.c_str(),"update");
  f1_ESpec->cd();
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    h2_Theta_Ekin_CM_GeoEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_CM_noEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Write("",TObject::kOverwrite);
    
    for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
    { 
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->Write("",TObject::kOverwrite); 
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->Write("",TObject::kOverwrite); 
    }
  }
  h1_Hira_ThetaLab_Dis->Write("",TObject::kOverwrite);
  f1_ESpec->Close();
}

void Hira_ESpec::Draw_ESpec_Info()
{
  TCanvas* c1_Ekin_Theta_CM_GeoReactionEff = new TCanvas("c1_Ekin_Theta_CM_GeoReactionEff","c1_Ekin_Theta_CM_GeoReactionEff",1);
  c1_Ekin_Theta_CM_GeoReactionEff->Divide(3,2);
  
  TCanvas* c1_Ekin_Theta_CM_GeoEff = new TCanvas("c1_Ekin_Theta_CM_GeoEff","c1_Ekin_Theta_CM_GeoEff",1);
  c1_Ekin_Theta_CM_GeoEff->Divide(3,2);
  
  TCanvas* c1_Ekin_Theta_CM_noEff = new TCanvas("c1_Ekin_Theta_CM_noEff","c1_Ekin_Theta_CM_noEff",1);
  c1_Ekin_Theta_CM_noEff->Divide(3,2);
  
  TCanvas* c1_Ekin_DiffTheta_Lab_GeoEff = new TCanvas("c1_Ekin_DiffTheta_Lab_GeoEff","c1_Ekin_DiffTheta_Lab_GeoEff",1);
  c1_Ekin_DiffTheta_Lab_GeoEff->Divide(3,2);
  int LineColor_DiffTheta[] = {1,2,3,4,6,7,9};
  TLegend* legend_DiffTheta_Lab = new TLegend(0.6,0.6,0.8,0.8);
  
  char NameTem[200];
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
    
    
    c1_Ekin_DiffTheta_Lab_GeoEff->cd(iPID+1)->SetLogy(1);
    for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
    {
      if(iTheta_Lab==0) { h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->Draw("hist"); }
      else { h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->Draw("histsame"); }
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->SetLineColor(LineColor_DiffTheta[iTheta_Lab]);
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->SetLineWidth(1);
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->SetLineStyle(1);
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->Draw("histsame");
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->SetLineColor(LineColor_DiffTheta[iTheta_Lab]);
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->SetLineWidth(3);
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->SetLineStyle(9);
      
      sprintf(NameTem,"#theta_{Lab}: %.0f - %.0f",Theta_Lab_Range_ForChecking[iTheta_Lab][0],Theta_Lab_Range_ForChecking[iTheta_Lab][1]);
      if(iPID==0) { legend_DiffTheta_Lab->AddEntry(h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab],NameTem,"l"); }
    }
  }
  c1_Ekin_DiffTheta_Lab_GeoEff->cd(ParticleNum);
  legend_DiffTheta_Lab->Draw();
  
  TCanvas* c1_PID_ZA = new TCanvas("c1_PID_ZA","c1_PID_ZA",1);
  h2_PID_ZA->Draw("colz");
//  TCanvas* c1_Hira_ThetaLab_Dis = new TCanvas("c1_Hira_ThetaLab_Dis","c1_Hira_ThetaLab_Dis",1);
//  h1_Hira_ThetaLab_Dis->Draw("hist");
  
}

void Hira_ESpec::Initial_LabToCM(string SystemTagTem)
{
  CurrentSystemIndex = -1;
  SystemNum = 16;
  double Mass_1u = 931.49410242;
  
  const int BeamNum = 2; 
  string BeamTag[BeamNum] = {"Ca40","Ca48"};
  double BeamMass[BeamNum] = {39.962590866*Mass_1u,47.95252290*Mass_1u};
  int BeamA[BeamNum] = {40,48};
  
  const int TargetNum = 4; 
  string TargetTag[TargetNum] = {"Ni58","Ni64","Sn112","Sn124"};
  double TargetMass[TargetNum] = {57.935343*Mass_1u,63.9279660*Mass_1u,111.904818*Mass_1u,123.9052739*Mass_1u};
  
  const int BeamEnergyNum = 2; 
  double BeamEnergy[BeamEnergyNum] = {56,140}; //MeV/A
  
  char NameTem[200];
  for(int iBeam=0;iBeam<BeamNum;iBeam++)
  {
    for(int iTarget=0;iTarget<TargetNum;iTarget++)
    {
      for(int iEnergy=0;iEnergy<BeamEnergyNum;iEnergy++)
      {
        int SystemIndex = iBeam*TargetNum*BeamEnergyNum+iTarget*BeamEnergyNum+iEnergy;
        
        sprintf(NameTem,"%s%sE%.0f",BeamTag[iBeam].c_str(),TargetTag[iTarget].c_str(),BeamEnergy[iEnergy]);
        SystemTag[SystemIndex] = NameTem;
        BetaZ_LabToCM[SystemIndex] = GetBetaZ_LabToCM(BeamMass[iBeam],TargetMass[iTarget],BeamA[iBeam]*BeamEnergy[iEnergy]);
        cout<<"SystemIndex : "<<SystemIndex<<"  "<<SystemTag[iBeam*TargetNum*BeamEnergyNum+iTarget*BeamEnergyNum+iEnergy]<<"  "<<BetaZ_LabToCM[SystemIndex]<<endl;
        if(SystemTagTem==SystemTag[SystemIndex]) { CurrentSystemIndex = SystemIndex; }
        BetaVector_LabToCM[SystemIndex].SetXYZ(0,0,-BetaZ_LabToCM[SystemIndex]);
      }
    }
  }
  cout<<SystemTagTem<<"  CurrentSystemIndex : "<<CurrentSystemIndex<<" BetaZ_LabToCM : "<<BetaZ_LabToCM[CurrentSystemIndex]<<endl;
  if(CurrentSystemIndex==-1) { cout<<"***"<<SystemTagTem<<" is not found in this class***"<<endl; }
}

void Hira_ESpec::Initial_ReactionLost_CorEff()
{
  ReactionLost_Cor_ParticleNum = 5;
  int ZTem[] = {1,1,1,2,2}; int ATem[] = {1,2,3,3,4};
  double A[] = {-1.772780E-4,-3.124610E-4,-2.480730E-4,-9.824110E-5,-8.952670E-5};
  double B[] = {-1.120190E-5,-6.187160E-6,-4.943390E-6,-7.446480E-7,-5.984310E-7};
  
  string ParticleName[] = {"P","D","T","3He","4He"};
  char NameTem[200];
  int LineColor_DiffPID[] = {1,2,3,4,6,7,8,9};
  
  for(int iPID = 0;iPID<ReactionLost_Cor_ParticleNum;iPID++)
  {
    ReactionLost_Cor_ParticleName[iPID] = ParticleName[iPID];
    ReactionLost_Cor_Z[iPID] = ZTem[iPID];
    ReactionLost_Cor_A[iPID] = ATem[iPID];
    sprintf(NameTem,"f1_ReactionLost_CorEff_%s",ParticleName[iPID].c_str());
    f1_ReactionLost_CorEff[iPID] = new TF1(NameTem,"TMath::Exp([0]*x*x+[1]*x)",0,500);
    f1_ReactionLost_CorEff[iPID]->SetParameter(0,B[iPID]);
    f1_ReactionLost_CorEff[iPID]->SetParameter(1,A[iPID]);
    f1_ReactionLost_CorEff[iPID]->SetLineColor(LineColor_DiffPID[iPID]);
    f1_ReactionLost_CorEff[iPID]->SetLineWidth(2);
  }
}

void Hira_ESpec::Draw_ReactionEff()
{
  TCanvas* c1_ReactionEff = new TCanvas("c1_ReactionEff","c1_ReactionEff",1);
  c1_ReactionEff->cd();
  TLegend* legend_ReactionEff = new TLegend(0.6,0.6,0.8,0.8);
  
  for(int iPID = 0;iPID<ReactionLost_Cor_ParticleNum;iPID++)
  {
    if(iPID!=0) { f1_ReactionLost_CorEff[iPID] -> Draw("same"); }
    else { f1_ReactionLost_CorEff[iPID] ->Draw(); }
    legend_ReactionEff->AddEntry(f1_ReactionLost_CorEff[iPID],ReactionLost_Cor_ParticleName[iPID].c_str(),"l");
    f1_ReactionLost_CorEff[iPID]->GetXaxis()->SetTitle("Ekin(MeV)");
    f1_ReactionLost_CorEff[iPID]->GetYaxis()->SetTitle("Eff_{Reaction}");
  }
  legend_ReactionEff->Draw();
  
}

double Hira_ESpec::Get_ReactionLost_CorEff(int Z, int A, double Ekin_Total_Lab)
{
  int Index = -1;
  for(int iPID=0;iPID<ReactionLost_Cor_ParticleNum;iPID++)
  {
    if(Z==ReactionLost_Cor_Z[iPID] && A==ReactionLost_Cor_A[iPID])
    {
      Index = iPID;
      break;
    }
  }
  if(Index!=-1) { return f1_ReactionLost_CorEff[Index]->Eval(Ekin_Total_Lab); }
  else { return 1; }
}

double Hira_ESpec::GetBetaZ_LabToCM(double BMass,double TMass, double BEnergy)
{
  double BeamMomentum = Sqrt(BEnergy*BEnergy+2*BEnergy*BMass);
  double BeamGamma = (BEnergy+BMass)/BMass;
  double Beta = BeamMomentum/(BeamGamma*BMass+TMass);
return Beta;
}
