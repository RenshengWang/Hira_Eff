/*
  Normally, not all the runs has the same bad map. This script is used to merged the 2D histogram.
  Merge the energy spectrum from different run group, and also normalized the 2D-Histogram.
*/
TH2D* Merge(int Num,TH2D** h2_ESpec_tem,int* EvtNum);
void Read_EvtNum(string FileName,int* TotalEvtNum, int* uBallCutEvtNum, int ibNum, int* iBEvtNum);

void Merge_ESpec()
{
  const int EffOpt = 3;
  string EffCor_Tag[] = {"_GeoEff", "_noEff", "_GeoReactionEff"};
  
  const int ImpactParaNum = 2;
  string ImpactParaTag[] = {"b_0.0_3.0","b_3.0_5.0"};
  
  const int ParticleNum = 6;
  string ParticleName[]={"P","D","T","3He","4He","6He","6Li","7Li"};

  string SystemTag = "Ca40Ni58E140";
  const int RunGroupNum = 3;
  string RunTag[] = {"2571-2586","2587-2611","2848-2870"};
  string Hira_BadMap_Version[] = {"V2","V2.1","V2"};

/*
  string SystemTag = "Ca48Ni64E140";
  const int RunGroupNum = 2;
  string RunTag[] = {"4023-4032","4082-4123"};
  string Hira_BadMap_Version[] = {"V1","V1"};
*/
  int Total_EvtNum[RunGroupNum];
  int uBall_MultiCutEvtNum[RunGroupNum];
  int ImpactPar_EvtNum[RunGroupNum][ImpactParaNum];
  
  char NameTem[200];
  cout<<endl<<" *** The event number information *** "<<endl;
  for(int iFile=0;iFile<RunGroupNum;iFile++)
  {
    sprintf(NameTem,"f1_ESpec_%s_%s_%s_EvtNum.data",SystemTag.c_str(),RunTag[iFile].c_str(),Hira_BadMap_Version[iFile].c_str());
    cout<<"Reading "<<NameTem<<endl;
    Read_EvtNum(NameTem,&(Total_EvtNum[iFile]),&(uBall_MultiCutEvtNum[iFile]),ImpactParaNum, ImpactPar_EvtNum[iFile]);
    cout<<"  Total_EvtNum: "<<Total_EvtNum[iFile]<<endl;
    cout<<"  uBall_MultiCutEvtNum: "<<uBall_MultiCutEvtNum[iFile]<<endl;
    for(int ib=0;ib<ImpactParaNum;ib++)
    {
      cout<<" EvtNum ( "<<ImpactParaTag[ib]<<" ) : "<<ImpactPar_EvtNum[iFile][ib]<<endl;
    }
  }
  
//  int uBall_MultiCutEvtNum[] = { 11112335, 133343388 };
//  int ImpactPar_EvtNum[RunGroupNum][ImpactParaNum] = {{2296028,4283500},{25334495,49344393}};
  
  //the below is for store the data.
  TH2D* h2_Theta_Ekin_CM_GeoEff[RunGroupNum][EffOpt][ParticleNum];
  TH2D* h2_Theta_Ekin_Lab_GeoEff[RunGroupNum][EffOpt][ParticleNum];
  TH2D* h2_Theta_Ekin_CM_GeoEff_ImpactPars[RunGroupNum][EffOpt][ParticleNum][ImpactParaNum];
  TH2D* h2_Theta_Ekin_Lab_GeoEff_ImpactPars[RunGroupNum][EffOpt][ParticleNum][ImpactParaNum];
  
  
  for(int iFile=0;iFile<RunGroupNum;iFile++)
  {
    sprintf(NameTem,"f1_ESpec_%s_%s_%s.root",SystemTag.c_str(),RunTag[iFile].c_str(),Hira_BadMap_Version[iFile].c_str());
    TFile* f1_ESpec = new TFile(NameTem,"read");
    
    for(int iEff=0;iEff<EffOpt;iEff++)
    {
      for(int iP=0;iP<ParticleNum;iP++)
      {
        sprintf(NameTem,"h2_%s_Theta_Ekin_CM%s",ParticleName[iP].c_str(),EffCor_Tag[iEff].c_str());
        h2_Theta_Ekin_CM_GeoEff[iFile][iEff][iP] = (TH2D*) f1_ESpec->Get(NameTem);
        
        sprintf(NameTem,"h2_%s_Theta_Ekin_Lab%s",ParticleName[iP].c_str(),EffCor_Tag[iEff].c_str());
        h2_Theta_Ekin_Lab_GeoEff[iFile][iEff][iP] = (TH2D*) f1_ESpec->Get(NameTem);
        
        for(int ib=0;ib<ImpactParaNum;ib++)
        {
          sprintf(NameTem,"h2_%s_%s_Theta_Ekin_CM%s_ImpactPars",ParticleName[iP].c_str(),ImpactParaTag[ib].c_str(),EffCor_Tag[iEff].c_str());
          h2_Theta_Ekin_CM_GeoEff_ImpactPars[iFile][iEff][iP][ib] = (TH2D*) f1_ESpec->Get(NameTem);
          sprintf(NameTem,"h2_%s_%s_Theta_Ekin_Lab%s_ImpactPars",ParticleName[iP].c_str(),ImpactParaTag[ib].c_str(),EffCor_Tag[iEff].c_str());
          h2_Theta_Ekin_Lab_GeoEff_ImpactPars[iFile][iEff][iP][ib] = (TH2D*) f1_ESpec->Get(NameTem);
        }
      }
    }
    //f1_ESpec->Close();
  }
  
  //merge the 2D histograms @ diff runs into 1 2D histogram.
  TH2D* h2_Theta_Ekin_CM_GeoEff_Merged[EffOpt][ParticleNum];
  TH2D* h2_Theta_Ekin_Lab_GeoEff_Merged[EffOpt][ParticleNum];
  TH2D* h2_Theta_Ekin_CM_GeoEff_ImpactPars_Merged[EffOpt][ParticleNum][ImpactParaNum];
  TH2D* h2_Theta_Ekin_Lab_GeoEff_ImpactPars_Merged[EffOpt][ParticleNum][ImpactParaNum];
  
  for(int iEff=0;iEff<EffOpt;iEff++)
  {
    for(int iP=0;iP<ParticleNum;iP++)
    {
      //for all the event.
      TH2D* h2_ESpec_CM_tem[RunGroupNum] = {0};
      TH2D* h2_ESpec_Lab_tem[RunGroupNum] = {0};
      for(int iRun=0;iRun<RunGroupNum;iRun++) 
      {
        h2_ESpec_CM_tem[iRun] = h2_Theta_Ekin_CM_GeoEff[iRun][iEff][iP];
        h2_ESpec_Lab_tem[iRun] = h2_Theta_Ekin_Lab_GeoEff[iRun][iEff][iP];
      }
      h2_Theta_Ekin_CM_GeoEff_Merged[iEff][iP] = Merge(RunGroupNum,h2_ESpec_CM_tem,uBall_MultiCutEvtNum);
      h2_Theta_Ekin_Lab_GeoEff_Merged[iEff][iP] = Merge(RunGroupNum,h2_ESpec_Lab_tem,uBall_MultiCutEvtNum);
      
      //for impact parameter selection.
      for(int ib=0;ib<ImpactParaNum;ib++)
      {
        TH2D* h2_ESpec_CM_ImpactPar_tem[RunGroupNum] = {0};
        TH2D* h2_ESpec_Lab_ImpactPar_tem[RunGroupNum] = {0};
        int EvtNumTem[RunGroupNum] = {0};
        for(int iRun=0;iRun<RunGroupNum;iRun++) 
        {
          h2_ESpec_CM_ImpactPar_tem[iRun] = h2_Theta_Ekin_CM_GeoEff_ImpactPars[iRun][iEff][iP][ib];
          h2_ESpec_Lab_ImpactPar_tem[iRun] = h2_Theta_Ekin_Lab_GeoEff_ImpactPars[iRun][iEff][iP][ib];
          EvtNumTem[iRun] = ImpactPar_EvtNum[iRun][ib];
        }
        h2_Theta_Ekin_CM_GeoEff_ImpactPars_Merged[iEff][iP][ib] = Merge(RunGroupNum,h2_ESpec_CM_ImpactPar_tem,EvtNumTem);;
        h2_Theta_Ekin_Lab_GeoEff_ImpactPars_Merged[iEff][iP][ib] = Merge(RunGroupNum,h2_ESpec_Lab_ImpactPar_tem,EvtNumTem);;
      }
      
    }
  }
  
  //for writing the merged histogram to a new file.
  sprintf(NameTem,"f1_MergedESpec_%s.root",SystemTag.c_str());
  TFile* f1_Merged_ESpec = new TFile(NameTem,"update");
  for(int iEff=0;iEff<EffOpt;iEff++)
  {
    for(int iP=0;iP<ParticleNum;iP++)
    {
      h2_Theta_Ekin_CM_GeoEff_Merged[iEff][iP]->Write("",TObject::kOverwrite);
      h2_Theta_Ekin_Lab_GeoEff_Merged[iEff][iP]->Write("",TObject::kOverwrite);
      for(int ib=0;ib<ImpactParaNum;ib++)
      {
        h2_Theta_Ekin_CM_GeoEff_ImpactPars_Merged[iEff][iP][ib]->Write("",TObject::kOverwrite);
        h2_Theta_Ekin_Lab_GeoEff_ImpactPars_Merged[iEff][iP][ib]->Write("",TObject::kOverwrite);
      }
    }
  }
  f1_Merged_ESpec->Close();


  //the below is just for test.
  int EffIndex = 0; int PIDIndex = 0; int bIndex = 0;
  TH2D* h2_Merged_tem = h2_Theta_Ekin_CM_GeoEff_ImpactPars_Merged[EffIndex][PIDIndex][bIndex];
  //TH2D* h2_Merged_tem = h2_Theta_Ekin_CM_GeoEff_Merged[EffIndex][PIDIndex];
  TH2D* h2_Seperated_tem[3];
  for(int i=0;i<RunGroupNum;i++)
  { 
    h2_Seperated_tem[i] = h2_Theta_Ekin_CM_GeoEff_ImpactPars[i][EffIndex][PIDIndex][bIndex]; 
//    h2_Seperated_tem[i] = h2_Theta_Ekin_CM_GeoEff[i][EffIndex][PIDIndex]; 
  }
  
  cout<<endl<<"*** the below is only for checking the merge procedure ***"<<endl;
  TCanvas* c1_ESpec_tem = new TCanvas("c1_ESpec_tem","c1_ESpec_tem",1);
  c1_ESpec_tem->Divide(2,2);
  int iBinX = 40; int iBinY = 80;
  for(int i=0;i<RunGroupNum;i++)
  {
    c1_ESpec_tem->cd(i+1);
    h2_Seperated_tem[i]->Draw("colz");
    double BinContent = h2_Seperated_tem[i]->GetBinContent(iBinX,iBinY);
    double BinError = h2_Seperated_tem[i]->GetBinError(iBinX,iBinY);
    cout<<i<<" BinContent : "<<BinContent<<"  "<<BinError<<" RelErr : "<<BinError/BinContent<<endl;
  }
  c1_ESpec_tem->cd(4); h2_Merged_tem->Draw("colz");
  cout<<" Merged BinContent : "<<h2_Merged_tem->GetBinContent(iBinX,iBinY)<<"  "<<h2_Merged_tem->GetBinError(iBinX,iBinY)
  <<"  "<<h2_Merged_tem->GetBinError(iBinX,iBinY)/h2_Merged_tem->GetBinContent(iBinX,iBinY)<<endl;
  
}

//Merge the histograms into 1 histogram.
//during this function, the ESpec will be normalized according to the EvtNum.
TH2D* Merge(int Num,TH2D** h2_ESpec_tem,int* EvtNum)
{
  string NameTem = h2_ESpec_tem[0]->GetTitle();
  TH2D* h2_ESpec_Merged = (TH2D*) h2_ESpec_tem[0]->Clone(NameTem.c_str());
  for(int i=1;i<Num;i++)
  {
    h2_ESpec_Merged -> Add(h2_ESpec_tem[i]);
  }
  int TotalNum = 0;
  for(int i=0;i<Num;i++) { TotalNum += EvtNum[i]; }
  if(TotalNum!=0) { h2_ESpec_Merged->Scale(1.0/TotalNum); }
  
return h2_ESpec_Merged;
}

//this histogram will be read the event number.
void Read_EvtNum(string FileName,int* TotalEvtNum, int* uBallCutEvtNum, int ibNum, int* iBEvtNum)
{
  ifstream infile(FileName.c_str());
  string Tag; int EvtNum;
  infile>>Tag>>EvtNum; 
  if( Tag=="TotalEvtNum" ) { (*TotalEvtNum) = EvtNum; }
  else { cout<<"EvtNum file format is wrong! --> TotalEvtNum"<<endl; }
  
  infile>>Tag>>EvtNum; 
  if( Tag=="uBall_MultiCutEvtNum" ) { (*uBallCutEvtNum) = EvtNum; }
  else { cout<<"EvtNum file format is wrong! --> uBall_MultiCutEvtNum"<<endl; }
  
  char NameTem[200];
  for(int ib=0;ib<ibNum;ib++)
  {
    sprintf(NameTem,"ib%d",ib+1);
    infile>>Tag>>EvtNum; 
    if(Tag==NameTem) { iBEvtNum[ib] = EvtNum; }
    else { cout<<"EvtNum file format is wrong! --> "<<NameTem<<endl; }
  }
}
