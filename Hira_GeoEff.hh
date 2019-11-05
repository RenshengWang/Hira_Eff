#ifndef Hira_GeoEff_h
#define Hira_GeoEff_h 1

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "iostream"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "fstream"
#include "TCutG.h"
#include "Hira_BadMap.hh"
#include "Hira_PosCali.hh"

#define NPTOOL 1

#ifdef NPTOOL
#include "THiraData.h"
#include "TInitialConditions.h"
#endif

using namespace std;
using namespace TMath;

class Hira_GeoEff : public TObject
{
public:
  Hira_GeoEff();
  ~Hira_GeoEff();
  
  Hira_BadMap* Hira_BadMapper;
  void Initial_Hira_BadMapper(Hira_BadMap* tem) { Hira_BadMapper = tem; }
  void Cal_GeoEff(TH1D* h1_Count,TH1D* h1_GeoEff,int TotalSimEvtNum);
  
  Hira_PosCali* HiraPos;
  void Initial_Hira_PosCali(Hira_PosCali* tem) { HiraPos = tem; }
  
  TFile* f1_Results;
  TH2D* h2_WholeHira_Theta_Phi_Lab;
  TH2D* h2_noBadMap_Theta_Phi_Lab;
  TH2D* h2_BadMap_Theta_Phi_Lab;
  TH1D* h1_BadMap_Theta_Lab_HitCount;
  TH1D* h1_BadMap_Theta_Lab_Eff;
  TH1D* h1_HiraHit_Multi;

  //the below is for producing the GeoEff from the NPTool simulation.
#ifdef NPTOOL
  void ReadSimData(string SimFileName,string RootFileNameForStore);
#endif
  void Draw_Info();
  //the below is for get the GeoEff from the Histogram.
  void ReadGeoEffHistogram(string RootFileName);
  double Get_GeoEff(double Theta_Lab); //unit: Deg.
  TH1D* Get_ThetaEff_Histogram() { return h1_BadMap_Theta_Lab_Eff; }
  
  ClassDef(Hira_GeoEff,1)
};

#endif
