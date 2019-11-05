#ifndef Hira_PosCali_h
#define Hira_PosCali_h 1

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

class Hira_PosCali : public TObject
{
public:
  Hira_PosCali();
  ~Hira_PosCali();
  
  void Read_Hira_PixelAngle(string FileName);
  double Theta_Lab[12][32][32]; //[HiraIndex][ X Index ][ Y Index ]
  double Phi_Lab[12][32][32]; //[HiraIndex][ X Index ][ Y Index ]
  
  double GetTheta(int iH, int iX, int iY){ return Theta_Lab[iH][iX][iY]; }
  double GetPhi(int iH, int iX, int iY){ return Phi_Lab[iH][iX][iY]; }
  
  ClassDef(Hira_PosCali,1)
};

#endif
