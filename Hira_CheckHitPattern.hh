#ifndef Hira_CheckHitPattern_h
#define Hira_CheckHitPattern_h 1

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
#include "TChain.h"

using namespace std;
using namespace TMath;

class Hira_CheckHitPattern : public TObject
{
public:
  Hira_CheckHitPattern(string SystemTagTem, string RunTagTem, string Hira_BadMap_VersionTem);
  ~Hira_CheckHitPattern();
  string SystemTag;
  string RunTag;
  string Hira_BadMap_Version;
  void SetAnaTag(string SystemTagTem, string RunTagTem, string Hira_BadMap_VersionTem);
  
  Hira_BadMap* Hira_BadMapper;
  bool IsActive_BadMap;
  void Set_IsActive_BadMap(bool tem) { IsActive_BadMap = tem; }
  void Initial_Hira_BadMapper(Hira_BadMap* tem) { Hira_BadMapper = tem; IsActive_BadMap = 1;}
  
  void ReadExpData(int FileNum, string ExpFileName[],string RootFilePathForStore);
  
  void InitialHisto();
  bool IsHistoInitialized;
  TH1D* h1_WholeHira_Multi_Dis;
  TH2D* h2_WholeHira_Theta_Phi_Lab[2];
  TH1D* h1_1Hira_Theta_HitCount[12][2];
  TH1D* h1_1Hira_Theta_HitCount_NormalizedWithPixelNum[12][2];
  TH2D* h2_1Hira_Theta_Phi_Lab[12][2];
  
  void GetNormalized_CountNum(TH1D* h1_Count,TH1D* h1_Count_Normalized,TH2D* h2_Pixel_Dis);
  void Draw_Info();
  
  ClassDef(Hira_CheckHitPattern,1)
};

#endif
