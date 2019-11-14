/*
  Energy spectrum always include plenty of physical information, and are analyzed repeatly. In this class, a 2-D histogram, Ekin~theta are produced, so the spectrum of Ekin and theta can be projected out.
  
*/
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
#include "Hira_PosCali.hh"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "Hira_GeoEff.hh"
#include "TLegend.h"

using namespace std;
using namespace TMath;

class Hira_ESpec : public TObject
{
public:
  Hira_ESpec(string SystemTagTem,string RunTagTem,string Hira_BadMap_VersionTem);
  ~Hira_ESpec();
  string SystemTag;
  string RunTag;
  string Hira_BadMap_Version;
  void SetAnaTag(string SystemTagTem, string RunTagTem, string Hira_BadMap_VersionTem);
  
  //for impact parameters.
  TH2D* h2_Impact_uBallMulti;
  bool IsSelect_ImpactPar;
  int ImpactNum;
  int uBall_MultiCut;
  void Set_uBall_MultiCut(int tem) { uBall_MultiCut = tem; }
  double ImpactPar_Range[20][2];
  void Set_ImpactPar_Range(int Num, double Pars[20][2]);
  TH2D* h2_Theta_Ekin_CM_GeoReactionEff_ImpactPars[10][20];//[impactpar][PID]
  TH2D* h2_Theta_Ekin_CM_GeoEff_ImpactPars[10][20];//[impactpar][PID]
  TH2D* h2_Theta_Ekin_CM_noEff_ImpactPars[10][20];//[impactpar][PID]
  
  TH1D* h1_Hira_ESpec_Lab_GeoEff_ImpactPars[10][20][10]; //[impactpar][PID][Theta_Lab]
  TH1D* h1_Hira_ESpec_Lab_GeoReactionEff_ImpactPars[10][20][10]; //[impactpar][PID][Theta_Lab]
  TH2D* h2_Theta_Ekin_Lab_GeoReactionEff_ImpactPars[10][20];//[impactpar][PID]
  TH2D* h2_Theta_Ekin_Lab_GeoEff_ImpactPars[10][20];//[impactpar][PID]
  TH2D* h2_Theta_Ekin_Lab_noEff_ImpactPars[10][20];//[impactpar][PID]
  
  // for setting the Bad map.
  Hira_BadMap* Hira_BadMapper;
  void Initial_Hira_BadMapper(Hira_BadMap* tem) { Hira_BadMapper = tem; }
  Hira_GeoEff* Hira_GeoEfficiency;
  void Initial_Hira_GeoEff(Hira_GeoEff* tem) { Hira_GeoEfficiency = tem; }
  
  // apply the self-calculated Hira position( theta, phi )
  bool Is_HiraPos_Applied;
  void Set_Is_HiraPos_Applied(bool tem) { Is_HiraPos_Applied = tem; }
  Hira_PosCali* HiraPos;
  void Initial_Hira_PosCali(Hira_PosCali* tem) { HiraPos = tem; Is_HiraPos_Applied = 1;}
  
  double GeoEff_Cut;
  void Set_GeoEff_Cut(double tem) { GeoEff_Cut = tem; }
  
  void Initial_ESpecHisto();
  int ParticleNum;
  string ParticleName[20]; double ParticleMass[20];
  int ParticleZ[20]; int ParticleA[20];
  
  //checking the spectrum in the lab.
  int Num_Theta_Lab_ForChecking;
  void Set_Num_Theta_Lab_ForChecking(int NumTem) { Num_Theta_Lab_ForChecking = NumTem; }
  double Theta_Lab_Range_ForChecking[5][2];
  TH1D* h1_Hira_ThetaLab_Dis;
  TH1D* h1_Hira_ESpec_Lab_GeoEff[20][5]; //[PID][Theta_Lab]
  TH1D* h1_Hira_ESpec_Lab_GeoReactionEff[20][5]; //[PID][Theta_Lab]
  TH2D* h2_Theta_Ekin_Lab_GeoReactionEff[20];
  TH2D* h2_Theta_Ekin_Lab_GeoEff[20];
  TH2D* h2_Theta_Ekin_Lab_noEff[20];
  
  //check the spectrum in the lab.
  TH2D* h2_Theta_Ekin_CM_GeoReactionEff[20];
  TH2D* h2_Theta_Ekin_CM_GeoEff[20];
  TH2D* h2_Theta_Ekin_CM_noEff[20];
  void ReadExpData(int ExpFileNum,string ExpDataFile[],string FileForStore);
  TH2D* h2_PID_ZA;
  TH1D* h1_ImpactPar;
  TH1D* h1_ImpactPar_WithHiraCount;
  
  //The below is for transfer from Lab to CM.
  int SystemNum;
  int CurrentSystemIndex;
  string SystemTag_All[20];
  double BetaZ_LabToCM[20];
  void Initial_LabToCM(string SystemTagTem);
  TVector3 BetaVector_LabToCM[20];
  double GetBetaZ_LabToCM(double BeamMassTem,double TargetMassTem, double BeamEnergyTem);
  
  //the below is for setting the correction of Reaction lost.
  TF1* f1_ReactionLost_CorEff[20];
  int ReactionLost_Cor_ParticleNum;
  string ReactionLost_Cor_ParticleName[20];
  int ReactionLost_Cor_Z[20]; int ReactionLost_Cor_A[20];
  void Initial_ReactionLost_CorEff();
  double Get_ReactionLost_CorEff(int Z, int A, double Ekin_Lab);
  
  //before next analysis step, these spectrum are necessary to check.
  void Draw_ESpec_Info();
  void Draw_ReactionEff();
  ClassDef(Hira_ESpec,1)
};

#endif
