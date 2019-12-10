#include "Hira_ESpec.hh"
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

ClassImp(Hira_ESpec);

Hira_ESpec::Hira_ESpec(string SystemTagTem,string RunTagTem,string Hira_BadMap_VersionTem)
{
  SystemTag = SystemTagTem;
  RunTag = RunTagTem;
  Hira_BadMap_Version = Hira_BadMap_VersionTem;
  Initial_LabToCM(SystemTag);
  
  Hira_BadMapper = 0;
  Hira_GeoEfficiency = 0;
  Is_HiraPos_Applied = 1;
  IsSelect_ImpactPar = 0;
  ImpactNum = 0;
  uBall_MultiCut = 5;
  
  ThetaLabCutNum = 0; //the default value is no theta cut.
}

Hira_ESpec::~Hira_ESpec()
{;}

void Hira_ESpec::AddThetaLab_Cut(double Theta1, double Theta2)
{
  ThetaLab_Cut[ThetaLabCutNum][0] = Theta1;
  ThetaLab_Cut[ThetaLabCutNum][1] = Theta2;
  cout<<"The "<<ThetaLabCutNum<<" select cut : "<<ThetaLab_Cut[ThetaLabCutNum][0]<<"  "<<ThetaLab_Cut[ThetaLabCutNum][1]<<endl;
  
  ThetaLabCutNum++;
}

bool Hira_ESpec::IsInThetaLab_Cut(double tem)
{
  if(ThetaLabCutNum==0) { return 1; } //it means there is no ThetaCut, just choose all the theta.
  for(int i=0;i<ThetaLabCutNum;i++)
  {
    if(tem>=ThetaLab_Cut[i][0] && tem<ThetaLab_Cut[i][1]) { return 1; break;}
  }
return 0;
}

void Hira_ESpec::SetAnaTag(string SystemTagTem, string RunTagTem, string Hira_BadMap_VersionTem)
{
  SystemTag = SystemTagTem;
  RunTag = RunTagTem;
  Hira_BadMap_Version = Hira_BadMap_VersionTem;
  Initial_LabToCM(SystemTag);
}

void Hira_ESpec::Initial_LabToCM(string SystemTagTem)
{
  SystemTag = SystemTagTem;
  
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
        SystemTag_All[SystemIndex] = NameTem;
        BetaZ_LabToCM[SystemIndex] = GetBetaZ_LabToCM(BeamMass[iBeam],TargetMass[iTarget],BeamA[iBeam]*BeamEnergy[iEnergy]);
        Rapidity_Beam_Lab[SystemIndex] = Get_BeamRapidity_Lab(BeamMass[iBeam],BeamA[iBeam]*BeamEnergy[iEnergy]);
        cout<<"SystemIndex : "<<SystemIndex<<"  "
            <<SystemTag_All[iBeam*TargetNum*BeamEnergyNum+iTarget*BeamEnergyNum+iEnergy]
            <<" BetaZ_LabToCM: "<<BetaZ_LabToCM[SystemIndex]
            <<" BeamRapidity_Lab: "<<Rapidity_Beam_Lab[SystemIndex]<<endl;
        if(SystemTagTem==SystemTag_All[SystemIndex]) { CurrentSystemIndex = SystemIndex; }
        BetaVector_LabToCM[SystemIndex].SetXYZ(0,0,-BetaZ_LabToCM[SystemIndex]);
      }
    }
  }
  cout<<SystemTagTem<<"  CurrentSystemIndex : "<<CurrentSystemIndex
      <<" BetaZ_LabToCM : "<<BetaZ_LabToCM[CurrentSystemIndex]
      <<" BeamRapidity_Lab : "<< Rapidity_Beam_Lab[CurrentSystemIndex] <<endl;
  if(CurrentSystemIndex==-1) { cout<<"***"<<SystemTagTem<<" is not found in this class***"<<endl; }
}

void Hira_ESpec::Set_ImpactPar_Range(int Num, double Pars[20][2])
{
  ImpactNum = Num;
  for(int i=0;i<ImpactNum;i++)
  {
    ImpactPar_Range[i][0] = Pars[i][0]; 
    ImpactPar_Range[i][1] = Pars[i][1];
  }
  IsSelect_ImpactPar = 1; 
}

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

    //the 2-D ESpec @ CM
    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoReactionEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoReactionEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);
    
    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_GeoEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_GeoEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);

    sprintf(NameTem,"h2_%s_Theta_Ekin_CM_noEff",ParticleName[i].c_str());
    h2_Theta_Ekin_CM_noEff[i] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);
    
    //the 2-D ESpec @ Lab
    sprintf(NameTem,"h2_%s_Theta_Ekin_Lab_GeoReactionEff",ParticleName[i].c_str());
    h2_Theta_Ekin_Lab_GeoReactionEff[i] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);
    
    sprintf(NameTem,"h2_%s_Theta_Ekin_Lab_GeoEff",ParticleName[i].c_str());
    h2_Theta_Ekin_Lab_GeoEff[i] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);

    sprintf(NameTem,"h2_%s_Theta_Ekin_Lab_noEff",ParticleName[i].c_str());
    h2_Theta_Ekin_Lab_noEff[i] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);
  }
  
  h1_Hira_ThetaLab_Dis = new TH1D("h1_Hira_ThetaLab_Dis",";#theta_{Lab}(Deg.);Count",600,20,80);
  h2_PID_ZA = new TH2D("h2_PID_ZA",";ParticleZ;ParticleA",10,0,10,10,0,10);
  
  //the below is for seting the impact parameters.
  h1_ImpactPar = new TH1D("h1_ImpactPar",";b(fm);Count",100,0,10);
  h1_ImpactPar_WithHiraCount = new TH1D("h1_ImpactPar_WithHiraCount",";b(fm);Count",100,0,10);
  h2_Impact_uBallMulti = new TH2D("h2_Impact_uBallMulti",";uBall_Multi;b(fm)",100,-10,90,200,-10,10);
   
  for(int ii=0;ii<ImpactNum;ii++)
  {
    for(int iP=0;iP<ParticleNum;iP++)
    {
      //the below is for the 2-D histogram in the CM.
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_CM_GeoReactionEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_CM_GeoReactionEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_CM_GeoEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_CM_GeoEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_CM_noEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_CM_noEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{CM}(MeV/A);#theta_{CM}(Deg.)",200,0,200,180,0,180);
      
      //the below is for the 2-D histogram in the lab.
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_Lab_GeoReactionEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_Lab_GeoReactionEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_Lab_GeoEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_Lab_GeoEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_Theta_Ekin_Lab_noEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_Theta_Ekin_Lab_noEff_ImpactPars[ii][iP] = new TH2D(NameTem,";Ekin_{Lab}(MeV/A);#theta_{Lab}(Deg.)",200,0,200,180,0,180);
      
      //the below is for the 2-D histogram ( y/ybeam ~ Pt ) in the lab
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_y_PtA_Lab_GeoReactionEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_y_PtA_Lab_GeoReactionEff_ImpactPars[ii][iP] = new TH2D(NameTem,";y/y{beam-Lab};#P_{t}(MeV/u)",100,0,1.0,600,0,600);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_y_PtA_Lab_GeoEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_y_PtA_Lab_GeoEff_ImpactPars[ii][iP] = new TH2D(NameTem,";y/y{beam-Lab};#P_{t}(MeV/u)",100,0,1.0,600,0,600);
      
      sprintf(NameTem,"h2_%s_b_%.1f_%.1f_y_PtA_Lab_noEff_ImpactPars",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1]);
      h2_y_PtA_Lab_noEff_ImpactPars[ii][iP] = new TH2D(NameTem,";y/y{beam-Lab};#P_{t}(MeV/u)",100,0,1.0,600,0,600);
      
      
      //the below is for defining the 1-D ESpec@Lab.
      for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
      {
         double R1 = Theta_Lab_Range_ForChecking[iTheta_Lab][0]; 
         double R2 = Theta_Lab_Range_ForChecking[iTheta_Lab][1];
         sprintf(NameTem,"h1_%s_b_%.1f_%.1f_Hira_ESpec_Lab_GeoEff_Theta_%.1f_%.0f",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1],R1,R2);
         h1_Hira_ESpec_Lab_GeoEff_ImpactPars[ii][iP][iTheta_Lab] = new TH1D(NameTem,";Ekin_{Lab}(MeV/u);Count",200,0,200);
      
         sprintf(NameTem,"h1_%s_b_%.1f_%.1f_Hira_ESpec_Lab_GeoReactionEff_Theta_%.1f_%.0f",ParticleName[iP].c_str(),ImpactPar_Range[ii][0],ImpactPar_Range[ii][1],R1,R2);
         h1_Hira_ESpec_Lab_GeoReactionEff_ImpactPars[ii][iP][iTheta_Lab] = new TH1D(NameTem,";Ekin_{Lab}(MeV/u);Count",200,0,200);
      }
    }
  }
}

void Hira_ESpec::ReadExpData(int ExpFileNum,string ExpDataFile[],string FilePathForStore)
{

  if(Hira_BadMapper==0) { cout<<"Hira_BadMapper==0"<<endl; return; }
  if(Hira_GeoEfficiency==0) { cout<<"Hira_GeoEfficiency==0"<<endl; return; }
  
  Initial_ESpecHisto();
  
  TChain* t1_Data = new TChain("E15190");
  int FileNum_Existed = 0;
  for(int i=0;i<ExpFileNum;i++)
  {
    cout<<"Reading "<<ExpDataFile[i]<<" : ";
    ifstream infile(ExpDataFile[i]);
    if(!infile.good()) { cout<< " Not Existed! "<<endl; continue; }
    else { cout<< " Existed! "<<endl; infile.close(); }
    t1_Data->AddFile(ExpDataFile[i].c_str());
    
    FileNum_Existed++;
  }
  cout<<" ---> Existed File Num: "<<FileNum_Existed<<endl;
  
  Int_t Hira_fmulti;
  double uBall_fb; int uBall_fmulti;
  double fTheta[200]; double fPhi[200]; 
  int fnumtel[200]; int fnumstripf[200]; int fnumstripb[200];
  double fEnergySifCal[200]; double fEnergySibCal[200]; 
  double fEnergycsifCal[200];
  double fKinEnergy[200];
  int fZId[200]; int fAId[200];
  
  t1_Data->SetMakeClass(1);
  t1_Data->SetBranchStatus("*",0);
  t1_Data->SetBranchAddress("uBall.fb",&uBall_fb);
  t1_Data->SetBranchAddress("uBall.fmulti",&uBall_fmulti);
  t1_Data->SetBranchAddress("HiRA.fmulti",&Hira_fmulti);
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
  
  //before counting the event number, set it to 0;
  EvtNum_Within_uBall_MultiCut = 0; 
  for(int iI=0;iI<ImpactNum;iI++) { EvtNum_ForDiffImpact[iI] = 0; }
  
  for(int iEvt = 0;iEvt<EvtNum;iEvt++)
  {
    if(iEvt%100000==0) { printProgress (1.0*iEvt/EvtNum); }
    t1_Data->GetEntry(iEvt);
    h2_Impact_uBallMulti->Fill(uBall_fmulti,uBall_fb);
    
    if(uBall_fmulti<uBall_MultiCut) { continue; } //make a 1st level cut for the uBall.
    h1_ImpactPar->Fill(uBall_fb);
    EvtNum_Within_uBall_MultiCut++;
    
    if(Hira_fmulti>0) { h1_ImpactPar_WithHiraCount->Fill(uBall_fb); }
    
    int Index_ImpactPar = -1;
    for(int iI=0;iI<ImpactNum;iI++)
    {
      if(uBall_fb>ImpactPar_Range[iI][0] && uBall_fb<ImpactPar_Range[iI][1])
      { Index_ImpactPar=iI; EvtNum_ForDiffImpact[iI]++; break; }
    }
    if(IsSelect_ImpactPar==0) { Index_ImpactPar = -1; } //this means, we didn't select the parameters.
    
    for(int iP=0;iP<Hira_fmulti;iP++)
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
            
            if(Is_HiraPos_Applied==0)
            {
              P_3D_Lab.SetTheta(fTheta[iP]);
              P_3D_Lab.SetPhi(fPhi[iP]);
            }
            
            if(Is_HiraPos_Applied==1)
            {
              P_3D_Lab.SetTheta(HiraPos->GetTheta(fnumtel[iP],fnumstripf[iP],fnumstripb[iP])*DegToRad());
              P_3D_Lab.SetPhi(HiraPos->GetPhi(fnumtel[iP],fnumstripf[iP],fnumstripb[iP])*DegToRad());
            }
            double Theta_Lab = P_3D_Lab.Theta()*RadToDeg();
            double Ekin_Lab = fKinEnergy[iP];
            
            if(ThetaLabCutNum!=0 && IsInThetaLab_Cut(Theta_Lab)==0) { continue; } //ignore the particles out of the theta range.
            
            double Pt_Lab = P_3D_Lab.Pt();//get the transverse momentum.
            double Pt_A_Lab = Pt_Lab/fAId[iP];
            
            TLorentzVector P_4D_CM(P_3D_Lab,ParticleMass[iPID]+fKinEnergy[iP]);
            //Attention, the below rapidity must be get before the boost.
            //the Rapidity_Lab already normalized to the beam rapidity.
            double Rapidity_Lab = P_4D_CM.Rapidity()/Rapidity_Beam_Lab[CurrentSystemIndex];
            
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
              h2_Theta_Ekin_CM_GeoEff[iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM,1.0/EffGeo);
              h2_Theta_Ekin_Lab_GeoEff[iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab,1.0/EffGeo);
              if(Index_ImpactPar!=-1)
              {
                h2_Theta_Ekin_CM_GeoEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM,1.0/EffGeo);
                h2_Theta_Ekin_Lab_GeoEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab,1.0/EffGeo);
                h2_y_PtA_Lab_GeoEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Rapidity_Lab,Pt_A_Lab,1.0/EffGeo);
              }
              
              if(ReactionEff>0.01) 
              {
                h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM,1.0/(EffGeo*ReactionEff));
                h2_Theta_Ekin_Lab_GeoReactionEff[iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab,1.0/(EffGeo*ReactionEff));
                if(Index_ImpactPar!=-1)
                {
                  h2_Theta_Ekin_CM_GeoReactionEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM,1.0/(EffGeo*ReactionEff));
                  h2_Theta_Ekin_Lab_GeoReactionEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab,1.0/(EffGeo*ReactionEff));
                  h2_y_PtA_Lab_GeoReactionEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Rapidity_Lab,Pt_A_Lab,1.0/(EffGeo*ReactionEff));
                }
              }
              h2_Theta_Ekin_CM_noEff[iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM);
              h2_Theta_Ekin_Lab_noEff[iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab);
              if(Index_ImpactPar!=-1)
              {
                h2_Theta_Ekin_CM_noEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_CM/ParticleA[iPID],Theta_CM);
                h2_Theta_Ekin_Lab_noEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Ekin_Lab/ParticleA[iPID],Theta_Lab);
                h2_y_PtA_Lab_noEff_ImpactPars[Index_ImpactPar][iPID]->Fill(Rapidity_Lab,Pt_A_Lab);
              }
              
              if(ThetaLab_Index!=-1)
              {
                h1_Hira_ESpec_Lab_GeoEff[iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/EffGeo);
                if(Index_ImpactPar!=-1)
                {
                  h1_Hira_ESpec_Lab_GeoEff_ImpactPars[Index_ImpactPar][iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/EffGeo);
                } 
                if(ReactionEff>0.01) 
                { 
                  h1_Hira_ESpec_Lab_GeoReactionEff[iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/(EffGeo*ReactionEff));
                  if(Index_ImpactPar!=-1)
                  {
                    h1_Hira_ESpec_Lab_GeoReactionEff_ImpactPars[Index_ImpactPar][iPID][ThetaLab_Index]->Fill(fKinEnergy[iP]/ParticleA[iPID],1.0/(EffGeo*ReactionEff));
                  }
                }
              }
              break;
            }
          }
        }
      }
    }
  }

  TFile* f1_ESpec = new TFile((FilePathForStore+"/f1_ESpec_"+SystemTag+"_"+RunTag+"_"+Hira_BadMap_Version+".root").c_str(),"update");
  f1_ESpec->cd();
  
  //the below is for showing and store event number information.
  ofstream outfile((FilePathForStore+"/f1_ESpec_"+SystemTag+"_"+RunTag+"_"+Hira_BadMap_Version+"_EvtNum.data").c_str());
  outfile<<"TotalEvtNum    "<<EvtNum<<endl;
  outfile<<"uBall_MultiCutEvtNum    "<<EvtNum_Within_uBall_MultiCut<<endl;
  for(int iI=0;iI<ImpactNum;iI++)
  {
    outfile<<"ib"<<iI<<"    "<<EvtNum_ForDiffImpact[iI]<<endl;
  }
  outfile<<endl<<endl;
  
  
  cout<<"---***---"<<endl;
  outfile<<"---***---"<<endl;
  cout<<"  Total event number: "<<EvtNum<<endl;
  outfile<<"  Total event number: "<<EvtNum<<endl;
  cout<<"  EvtNum_Within_uBall_MultiCut (uBall_MultiCut>="<<uBall_MultiCut<<") : "<<EvtNum_Within_uBall_MultiCut<<endl;
  outfile<<"  EvtNum_Within_uBall_MultiCut (uBall_MultiCut>="<<uBall_MultiCut<<") : "<<EvtNum_Within_uBall_MultiCut<<endl;
  for(int iI=0;iI<ImpactNum;iI++)
  {
    cout<<"  Impact parameter ("<< ImpactPar_Range[iI][0]<<","<< ImpactPar_Range[iI][1]<<") : "<<EvtNum_ForDiffImpact[iI]<<endl;
    outfile<<"  Impact parameter ("<< ImpactPar_Range[iI][0]<<","<< ImpactPar_Range[iI][1]<<") : "<<EvtNum_ForDiffImpact[iI]<<endl;
  }
  cout<<"---***---"<<endl;
  outfile<<"---***---"<<endl;
  
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    h2_Theta_Ekin_CM_GeoEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_CM_noEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Write("",TObject::kOverwrite);
    
    h2_Theta_Ekin_Lab_GeoEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_Lab_noEff[iPID]->Write("",TObject::kOverwrite);
    h2_Theta_Ekin_Lab_GeoReactionEff[iPID]->Write("",TObject::kOverwrite);
    
    for(int iI=0;iI<ImpactNum;iI++)
    {
      h2_Theta_Ekin_CM_GeoEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_Theta_Ekin_CM_noEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_Theta_Ekin_CM_GeoReactionEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      
      h2_Theta_Ekin_Lab_GeoEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_Theta_Ekin_Lab_noEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_Theta_Ekin_Lab_GeoReactionEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      
      h2_y_PtA_Lab_GeoEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_y_PtA_Lab_noEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
      h2_y_PtA_Lab_GeoReactionEff_ImpactPars[iI][iPID]->Write("",TObject::kOverwrite);
    }
    
    for(int iTheta_Lab=0;iTheta_Lab<Num_Theta_Lab_ForChecking;iTheta_Lab++)
    {
      double SolidAngle_Lab = 2*Pi()*(Cos(DegToRad()*Theta_Lab_Range_ForChecking[iTheta_Lab][0])-Cos(DegToRad()*Theta_Lab_Range_ForChecking[iTheta_Lab][1]));
      
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->Write("",TObject::kOverwrite); 
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->Write("",TObject::kOverwrite);
      
      h1_Hira_ESpec_Lab_GeoEff[iPID][iTheta_Lab]->GetYaxis()->SetTitle("Count");
      h1_Hira_ESpec_Lab_GeoReactionEff[iPID][iTheta_Lab]->GetYaxis()->SetTitle("Count");
      
      for(int iI=0;iI<ImpactNum;iI++)
      {
        h1_Hira_ESpec_Lab_GeoEff_ImpactPars[iI][iPID][iTheta_Lab]->Write("",TObject::kOverwrite); 
        h1_Hira_ESpec_Lab_GeoReactionEff_ImpactPars[iI][iPID][iTheta_Lab]->Write("",TObject::kOverwrite);
        
        h1_Hira_ESpec_Lab_GeoEff_ImpactPars[iI][iPID][iTheta_Lab]->GetYaxis()->SetTitle("Count");
        h1_Hira_ESpec_Lab_GeoReactionEff_ImpactPars[iI][iPID][iTheta_Lab]->GetYaxis()->SetTitle("Count");
      }
    }
  }
  h1_Hira_ThetaLab_Dis->Write("",TObject::kOverwrite);
  h1_ImpactPar->Write("",TObject::kOverwrite);
  h2_Impact_uBallMulti->Write("",TObject::kOverwrite);
  h1_ImpactPar_WithHiraCount->Write("",TObject::kOverwrite);
  f1_ESpec->Close();

}

//this function is used to show the data on time, so you can glimpse it to make sure there are no mistake.
void Hira_ESpec::Draw_ESpec_Info()
{
  TCanvas* c1_Ekin_Theta_CM = new TCanvas("c1_Ekin_Theta_CM","c1_Ekin_Theta_CM",1);
  c1_Ekin_Theta_CM->Divide(6,3);
  
  TCanvas* c1_Ekin_DiffTheta_Lab_GeoEff = new TCanvas("c1_Ekin_DiffTheta_Lab_GeoEff","c1_Ekin_DiffTheta_Lab_GeoEff",1);
  c1_Ekin_DiffTheta_Lab_GeoEff->Divide(3,2);
  int LineColor_DiffTheta[] = {1,2,3,4,6,7,9};
  TLegend* legend_DiffTheta_Lab = new TLegend(0.6,0.6,0.8,0.8);
  
  char NameTem[200];
  for(int iPID=0;iPID<ParticleNum;iPID++)
  {
    c1_Ekin_Theta_CM->cd(iPID+1)->SetGridx(1);
    c1_Ekin_Theta_CM->cd(iPID+1)->SetGridy(1);
    h2_Theta_Ekin_CM_noEff[iPID]->Draw("colz");
    
    c1_Ekin_Theta_CM->cd(iPID+6+1)->SetGridx(1);
    c1_Ekin_Theta_CM->cd(iPID+6+1)->SetGridy(1);
    h2_Theta_Ekin_CM_GeoEff[iPID]->Draw("colz");
    
    c1_Ekin_Theta_CM->cd(iPID+6+6+1)->SetGridx(1);
    c1_Ekin_Theta_CM->cd(iPID+6+6+1)->SetGridy(1);
    h2_Theta_Ekin_CM_GeoReactionEff[iPID]->Draw("colz");
    
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
  
  //the below is for setting the y~Pt_A.
  if(IsSelect_ImpactPar==1)
  {
    for(int iI=0;iI<ImpactNum;iI++)
    {
      sprintf(NameTem,"c1_y_PtA_Lab_b_%.1f_%.1f",ImpactPar_Range[iI][0],ImpactPar_Range[iI][1]);
      TCanvas* c1_y_PtA_Lab = new TCanvas(NameTem,NameTem,1);
      c1_y_PtA_Lab->Divide(6,3);
      for(int iPID=0;iPID<ParticleNum;iPID++)
      {
        c1_y_PtA_Lab->cd(iPID+1); h2_y_PtA_Lab_noEff_ImpactPars[iI][iPID]->Draw("colz");
        c1_y_PtA_Lab->cd(iPID+1+6); h2_y_PtA_Lab_GeoEff_ImpactPars[iI][iPID]->Draw("colz");
        c1_y_PtA_Lab->cd(iPID+1+6+6); h2_y_PtA_Lab_GeoReactionEff_ImpactPars[iI][iPID]->Draw("colz");
      }
    }
  }
  
  
  TCanvas* c1_PID_ZA = new TCanvas("c1_PID_ZA","c1_PID_ZA",1);
  h2_PID_ZA->Draw("colz");
  
  TCanvas* c1_Impact = new TCanvas("c1_Impact","c1_Impact",1);
  c1_Impact->Divide(3,1);
  c1_Impact->cd(1); h2_Impact_uBallMulti->Draw("colz");
  c1_Impact->cd(2); h1_ImpactPar->Draw("hist");
  c1_Impact->cd(3); h1_ImpactPar_WithHiraCount->Draw("hist");
  
  TCanvas* c1_Hira_ThetaLab_Dis = new TCanvas("c1_Hira_ThetaLab_Dis","c1_Hira_ThetaLab_Dis",1);
  h1_Hira_ThetaLab_Dis->Draw("hist");
  
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
  else { return f1_ReactionLost_CorEff[ReactionLost_Cor_ParticleNum-1]->Eval(Ekin_Total_Lab); } //the others are set to same with the last particle( always is alpha ).
}

double Hira_ESpec::GetBetaZ_LabToCM(double BMass,double TMass, double BEnergy)
{
  double BeamMomentum = Sqrt(BEnergy*BEnergy+2*BEnergy*BMass);
  double BeamGamma = (BEnergy+BMass)/BMass;
  double Beta = BeamMomentum/(BeamGamma*BMass+TMass);
return Beta;
}

double Hira_ESpec::Get_BeamRapidity_Lab(double BMass,double BEnergy)
{
  double BeamMomentum = Sqrt(BEnergy*BEnergy+2*BEnergy*BMass);
  TLorentzVector BeamLabTem(0,0,BeamMomentum,BMass+BEnergy);
  double BeamRapidityTem = BeamLabTem.Rapidity();

return BeamRapidityTem;
}

void Hira_ESpec::printProgress (double percentage)
{
  int val = (int) (percentage * 100);
  int lpad = (int) (percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush (stdout);
}
