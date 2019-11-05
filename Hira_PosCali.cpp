#include "Hira_PosCali.hh"
ClassImp(Hira_PosCali);

Hira_PosCali::Hira_PosCali()
{
  
}

Hira_PosCali::~Hira_PosCali()
{;}

void Hira_PosCali::Read_Hira_PixelAngle(string FileName)
{
  ifstream infile(FileName.c_str());
  infile.ignore(99,'\n');
  
  int HiraIndex = 0; int StripXIndex = 0; int StripYIndex = 0;
  double Theta = 0; double Phi = 0;
  while(infile)
  {
    infile>>HiraIndex>>StripXIndex>>StripYIndex>>Theta>>Phi;
    Theta_Lab[HiraIndex][StripXIndex][StripYIndex] = Theta;
    Phi_Lab[HiraIndex][StripXIndex][StripYIndex] = Phi;
    
    infile.ignore(99,'\n');
    char first;
    infile.get(first);
    infile.putback(first);
  }
}
