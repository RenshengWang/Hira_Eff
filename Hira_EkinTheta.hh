#ifndef Hira_EkinTheta_h
#define Hira_EkinTheta_h 1

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

using namespace std;
using namespace TMath;

class Hira_EkinTheta : public TObject
{
public:
  Hira_EkinTheta(string SystemTagTem,string RunTagTem,string ParticleTagTem);
  string SystemTag;
  string RunTag;
  string ParticleTag;
  void SetParticleTag(string Tem) { ParticleTag = Tem; }
  ~Hira_EkinTheta();
  
  int EkinNum;
  double StartEkin;
  double EkinBinSize;
  void SetEkinConfig(int Num, double Ekin, double BinSize);
  bool Is_ThetaCM_Dis;
  double EkinRange[100][2];
  TH1D* h1_ThetaCM_Dis[100];
  
  int ThetaNum;
  double StartTheta;
  double ThetaBinSize;
  void SetThetaConfig(int Num, double Theta, double BinSize);
  bool Is_EkinCM_Dis;
  double ThetaRange[100][2];
  TH1D* h1_EkinCM_Dis[100];
  
  int CutNum;
  TCutG* Cut_ThetaEkin[100];
  void InitialCut(string FileName, int Num, string* Tag);
  
  void ProduceEkinSpectrum(TH2D* h2_Ekin_Theta,TH1D** h1_ESpec);
  void ProduceThetaDis(TH2D* h2_Ekin_Theta,TH1D** h1_ESpec);
  
  void DrawInfo();
  TH2D* h2_Ekin_Theta_forEkinDis;
  TH2D* h2_Ekin_Theta_forThetaDis;
  
  ClassDef(Hira_EkinTheta,1)
};

#endif
