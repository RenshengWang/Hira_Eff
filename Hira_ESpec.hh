#ifndef Hira_ESpec_h
#define Hira_ESpec_h 1

#include "TObject.h"
#include "TFile.h"
#include "TChain.h"
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
#include "TVector3.h"
#include "TLorentzVector.h"
#include "Hira_GeoEff.hh"
#include "TLegend.h"

using namespace std;
using namespace TMath;

class Hira_ESpec : public TObject
{
public:
  Hira_ESpec();
  ~Hira_ESpec();
  
  Hira_BadMap* Hira_BadMapper;
  void Initial_Hira_BadMapper(Hira_BadMap* tem) { Hira_BadMapper = tem; }
  Hira_GeoEff* Hira_GeoEfficiency;
  void Initial_Hira_GeoEff(Hira_GeoEff* tem) { Hira_GeoEfficiency = tem; }
  
  double GeoEff_Cut;
  void Set_GeoEff_Cut(double tem) { GeoEff_Cut = tem; }
  
  void Initial_ESpecHisto();
  int ParticleNum;
  string ParticleName[20]; double ParticleMass[20];
  int ParticleZ[20]; int ParticleA[20];
  int Num_Theta_Lab_ForChecking;
  void Set_Num_Theta_Lab_ForChecking(int NumTem) { Num_Theta_Lab_ForChecking = NumTem; }
  double Theta_Lab_Range_ForChecking[5][2];
  TH1D* h1_Hira_ThetaLab_Dis;
  TH1D* h1_Hira_ESpec_Lab_GeoEff[20][5]; //[PID][Theta_Lab]
  TH1D* h1_Hira_ESpec_Lab_GeoReactionEff[20][5]; //[PID][Theta_Lab]
  TH2D* h2_Theta_Ekin_CM_GeoReactionEff[20];
  TH2D* h2_Theta_Ekin_CM_GeoEff[20];
  TH2D* h2_Theta_Ekin_CM_noEff[20];
  void ReadExpData(string ExpDataFile,string FileForStore);
  TH2D* h2_PID_ZA;
  
  //The below is for transfer from Lab to CM.
  int SystemNum;
  int CurrentSystemIndex;
  string SystemTag[20];
  double BetaZ_LabToCM[20];
  void Initial_LabToCM(string SystemTagTem);
  TVector3 BetaVector_LabToCM[20];
  double GetBetaZ_LabToCM(double BeamMassTem,double TargetMassTem, double BeamEnergyTem);
  
  TF1* f1_ReactionLost_CorEff[20];
  int ReactionLost_Cor_ParticleNum;
  string ReactionLost_Cor_ParticleName[20];
  int ReactionLost_Cor_Z[20]; int ReactionLost_Cor_A[20];
  void Initial_ReactionLost_CorEff();
  double Get_ReactionLost_CorEff(int Z, int A, double Ekin_Lab);
  
  void Draw_ESpec_Info();
  void Draw_ReactionEff();
  ClassDef(Hira_ESpec,1)
};

#endif
