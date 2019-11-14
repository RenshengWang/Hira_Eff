#include "Hira_EkinTheta.hh"
ClassImp(Hira_EkinTheta);

Hira_EkinTheta::Hira_EkinTheta(string SystemTagTem,string RunTagTem,string ParticleTagTem)
{
  SystemTag = SystemTagTem;
  RunTag = RunTagTem;
  ParticleTag = ParticleTagTem;
  
  CutNum = 0;
  Is_ThetaCM_Dis = 0;
  Is_EkinCM_Dis = 0;
}

Hira_EkinTheta::~Hira_EkinTheta()
{;}

void Hira_EkinTheta::SetEkinConfig(int Num, double Ekin, double BinSize)
{
  EkinNum = Num;
  StartEkin = Ekin;
  EkinBinSize = BinSize;
  char NameTem[200];
  
  for(int i=0;i<EkinNum;i++)
  {
    EkinRange[i][0] = StartEkin + i*EkinBinSize;
    EkinRange[i][1] = StartEkin + (i+1)*EkinBinSize;
  }
  Is_ThetaCM_Dis = 1;
}

void Hira_EkinTheta::SetThetaConfig(int Num, double Theta, double BinSize)
{
  ThetaNum = Num;
  StartTheta = Theta;
  ThetaBinSize = BinSize;
  char NameTem[200];
  
  for(int i=0;i<ThetaNum;i++)
  {
    ThetaRange[i][0] = StartTheta+i*ThetaBinSize;
    ThetaRange[i][1] = StartTheta+(i+1)*ThetaBinSize;
  }
  Is_EkinCM_Dis = 1;
}

void Hira_EkinTheta::InitialCut(string FileName, int Num, string* Tag)
{
  CutNum = Num;
  TFile* f1_Cut = new TFile(FileName.c_str(),"read");
  for(int i=0;i<CutNum;i++)
  {
    Cut_ThetaEkin[i] = (TCutG*) f1_Cut->Get(Tag[i].c_str());
    if(Cut_ThetaEkin[i]==0) { cout<<Tag[i]<<" is not found!"<<endl; }
  }
}

void Hira_EkinTheta::ProduceEkinSpectrum(TH2D* h2_Ekin_Theta,TH1D** h1_EkinCM_tem)
{
  //first clean the 2D histogram.
  int ENum = h2_Ekin_Theta->GetNbinsX();
  int TNum = h2_Ekin_Theta->GetNbinsY();
  char NameTem[200];
  
  if(CutNum>0)
  {
    for(int iE=1;iE<=ENum;iE++)
    {
      for(int iT=1;iT<=TNum;iT++)
      {
        double XBinCenter = h2_Ekin_Theta->GetXaxis()->GetBinCenter(iE);
        double YBinCenter = h2_Ekin_Theta->GetYaxis()->GetBinCenter(iT);
        for(int iCut=0;iCut<CutNum;iCut++)
        {
          if(Cut_ThetaEkin[iCut]->IsInside(XBinCenter,YBinCenter)) 
          { h2_Ekin_Theta->SetBinContent(iE,iT,0); }
        }
      }
    }
  }
  
  if(Is_EkinCM_Dis == 1)
  {
    h2_Ekin_Theta_forEkinDis = (TH2D*) h2_Ekin_Theta->Clone("h2_Ekin_Theta_forEkinDis");
    for(int iT=0;iT<ThetaNum;iT++)
    {
      int Bin1 = h2_Ekin_Theta->GetYaxis()->FindBin(ThetaRange[iT][0]);
      int Bin2 = h2_Ekin_Theta->GetYaxis()->FindBin(ThetaRange[iT][1]);
    
      for(int iEBin=1;iEBin<=ENum;iEBin++)
      {
        bool IsFullFill = 1;
        for(int iTBin=Bin1;iTBin<=Bin2;iTBin++)
        {
          if(h2_Ekin_Theta->GetBinContent(iEBin,iTBin)==0) { IsFullFill=0; break; }
        }
        if(IsFullFill==0 && iEBin<10)
        {
          for(int iTBin=Bin1;iTBin<=Bin2;iTBin++)
          { h2_Ekin_Theta_forEkinDis->SetBinContent(iEBin,iTBin,0); }
        }
      }
    
      sprintf(NameTem,"h1_%s_%s_%s_EkinCM_Dis_Theta_%.1f_%.1f",SystemTag.c_str(),RunTag.c_str(),ParticleTag.c_str(),ThetaRange[iT][0],ThetaRange[iT][1]);
      h1_EkinCM_Dis[iT] = h2_Ekin_Theta_forEkinDis->ProjectionX(NameTem,Bin1+1,Bin2-1);
      h1_EkinCM_tem[iT] = h1_EkinCM_Dis[iT];
    }
  }
}

void Hira_EkinTheta::ProduceThetaDis(TH2D* h2_Ekin_Theta,TH1D** h1_ThetaDis_tem)
{
  //first clean the 2D histogram.
  int ENum = h2_Ekin_Theta->GetNbinsX();
  int TNum = h2_Ekin_Theta->GetNbinsY();
  char NameTem[200];
  
  if(CutNum>0)
  {
    for(int iE=1;iE<=ENum;iE++)
    {
      for(int iT=1;iT<=TNum;iT++)
      {
        double XBinCenter = h2_Ekin_Theta->GetXaxis()->GetBinCenter(iE);
        double YBinCenter = h2_Ekin_Theta->GetYaxis()->GetBinCenter(iT);
        for(int iCut=0;iCut<CutNum;iCut++)
        {
          if(Cut_ThetaEkin[iCut]->IsInside(XBinCenter,YBinCenter)) 
          { h2_Ekin_Theta->SetBinContent(iE,iT,0); }
        }
      }
    }
  }
  
  //the below is for calculating the projection along Theta.
  if(Is_ThetaCM_Dis == 1)
  {
    h2_Ekin_Theta_forThetaDis = (TH2D*) h2_Ekin_Theta->Clone("h2_Ekin_Theta_forThetaDis");
    for(int iE=0;iE<EkinNum;iE++)
    {
      int Bin1 = h2_Ekin_Theta->GetXaxis()->FindBin(EkinRange[iE][0]);
      int Bin2 = h2_Ekin_Theta->GetXaxis()->FindBin(EkinRange[iE][1]);
    
      for(int iTBin=1;iTBin<=TNum;iTBin++)
      {
        bool IsFullFill = 1;
        for(int iEBin=Bin1;iEBin<=Bin2;iEBin++)
        {
          if(h2_Ekin_Theta->GetBinContent(iEBin,iTBin)==0) { IsFullFill=0; break; }
        }
        if(IsFullFill==0)
        {
          for(int iEBin=Bin1;iEBin<=Bin2;iEBin++)
          { h2_Ekin_Theta_forThetaDis->SetBinContent(iEBin,iTBin,0); }
        }
      }
      sprintf(NameTem,"h1_%s_%s_%s_ThetaCM_Dis_Ekin_%.1f_%.1f",SystemTag.c_str(),RunTag.c_str(),ParticleTag.c_str(),EkinRange[iE][0],EkinRange[iE][1]);
      h1_ThetaCM_Dis[iE] = h2_Ekin_Theta_forThetaDis->ProjectionY(NameTem,Bin1+1,Bin2-1);
      h1_ThetaDis_tem[iE] = h1_ThetaCM_Dis[iE];
    }
  }
}

void Hira_EkinTheta::DrawInfo()
{
  int LineColor[]={1,2,3,4,6,7,8,9,12,13,14,15,16,17,18,19,20};
  char NameTem[200];
  if(Is_EkinCM_Dis == 1)
  {
    sprintf(NameTem,"c1_%s_%s_%s_EkinCM_DiffThetaCM",SystemTag.c_str(),RunTag.c_str(),ParticleTag.c_str());
    TCanvas* c1_EkinCM_DiffThetaCM = new TCanvas(NameTem,NameTem,1);
    c1_EkinCM_DiffThetaCM->Divide(2,1);
    c1_EkinCM_DiffThetaCM->cd(1);
    for(int i=0;i<ThetaNum;i++)
    {
      if(i==0) { h1_EkinCM_Dis[i]->Draw("hist"); }
      else{ h1_EkinCM_Dis[i]->Draw("histsame"); }
      h1_EkinCM_Dis[i]->SetLineWidth(2);
      h1_EkinCM_Dis[i]->SetLineColor(LineColor[i]);
    }
    c1_EkinCM_DiffThetaCM->cd(2);
    h2_Ekin_Theta_forEkinDis->Draw("colz");
  }
  
  if(Is_ThetaCM_Dis == 1)
  {
    sprintf(NameTem,"c1_%s_%s_%s_ThetaCM_DiffEkinCM",SystemTag.c_str(),RunTag.c_str(),ParticleTag.c_str());
    TCanvas* c1_ThetaCM_DiffEkinCM = new TCanvas(NameTem,NameTem,1);
    c1_ThetaCM_DiffEkinCM->Divide(2,1);
    c1_ThetaCM_DiffEkinCM->cd(1);
    for(int i=0;i<EkinNum;i++)
    {
      if(i==0) { h1_ThetaCM_Dis[i]->Draw("hist"); }
      else{ h1_ThetaCM_Dis[i]->Draw("histsame"); }
      h1_ThetaCM_Dis[i]->SetLineWidth(2);
      h1_ThetaCM_Dis[i]->SetLineColor(LineColor[i]);
    }
    c1_ThetaCM_DiffEkinCM->cd(2);
    h2_Ekin_Theta_forThetaDis->Draw("colz");
  }
}
