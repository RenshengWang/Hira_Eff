#ifndef Hira_BadMap_h
#define Hira_BadMap_h 1

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

class Hira_BadMap : public TObject
{
public:
  Hira_BadMap();
  ~Hira_BadMap();
  
  bool IsShowInfo;
  void Set_IsShowInfo(bool tem) { IsShowInfo = tem; }
  
  bool BadHiraMap[12];
  bool BadCsIMap[12][4];
  bool BadStripMap[12][32][2]; //[HiraIndex][StripIndex][Front,Back]
  
  void Read_BadMap_Strip(string FileName);
  void Read_BadMap_CsI(string FileName);
  void Read_BadMap_Tele(string FileName);
  
  bool IsBad_StripX(int HiraIndex,int StripX_Index);
  bool IsBad_StripY(int HiraIndex,int StripY_Index); 
  bool IsBad_CsI(int HiraIndex,int CsI_Index);
  bool IsBad_CsI(int HiraIndex,int StripX_Index,int StripY_Index);
  bool IsBad_Hira(int HiraIndex);
  
  ClassDef(Hira_BadMap,1)
};

#endif
