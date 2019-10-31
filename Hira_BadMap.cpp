#include "Hira_BadMap.hh"
ClassImp(Hira_BadMap);

Hira_BadMap::Hira_BadMap()
{
  IsShowInfo = 1;
}

Hira_BadMap::~Hira_BadMap()
{;}

void Hira_BadMap::Read_BadMap_Strip(string FileName)
{
  for(int i=0;i<12;i++)
  {
    for(int j=0;j<32;j++)
    {
      BadStripMap[i][j][0] = 0;
      BadStripMap[i][j][1] = 0;
    }
  }
  
  ifstream infile(FileName.c_str());
  infile.ignore(99,'\n'); //ignore the first comment line.
  cout<<"*** - Bad Hira Strip - ***"<<endl;
  while(infile)
  {
    int FrontBack = 0; int HiraIndex = 0; int StripIndex= 0; bool IsBad = 1;
    infile>>FrontBack>>HiraIndex>>StripIndex>>IsBad;
    if(IsShowInfo==1) { cout<<" Bad Hira Strip: "<<FrontBack<<"  "<<HiraIndex<<"  "<<StripIndex<<"  "<<IsBad<<endl; }
    BadStripMap[HiraIndex][StripIndex][FrontBack] = IsBad;

    infile.ignore(99,'\n');
    char first; infile.get(first);
    infile.putback(first);
  }
  cout<<"*** - Bad Hira Strip - ***"<<endl;
}

void Hira_BadMap::Read_BadMap_CsI(string FileName)
{
  ifstream infile(FileName.c_str());
  infile.ignore(99,'\n');//ignore the first comment line.
  
  //First Suppose all the CsI is good.
  for(int iHira=0;iHira<12;iHira++)
  {
    for(int iCsI = 0;iCsI<4;iCsI++) { BadCsIMap[iHira][iCsI] = 0; }
  }
  cout<<"*** - Bad Hira CsI - ***"<<endl;
  while(infile)
  {
    int HiraIndex = 0; int CsIIndex = 0; bool IsBad = 1;
    infile>>HiraIndex>>CsIIndex>>IsBad;
    if(IsShowInfo==1) { cout<<HiraIndex<<"  "<<CsIIndex<<"  "<<IsBad<<endl; }
    BadCsIMap[HiraIndex][CsIIndex] = IsBad;

    infile.ignore(99,'\n');
    char first; infile.get(first);
    infile.putback(first);
  }
  cout<<"*** - Bad Hira CsI - ***"<<endl;
}

void Hira_BadMap::Read_BadMap_Tele(string FileName)
{
  ifstream infile(FileName.c_str());
  infile.ignore(99,'\n'); //ignore the first comment line.
  
  //First Suppose all the Hira is good.
  for(int iHira=0;iHira<12;iHira++) { BadHiraMap[iHira] = 0; } 
  cout<<"*** - Bad Hira - ***"<<endl;
  while(infile)
  {
    int HiraIndex = 0; bool IsBad = 1;
    infile>>HiraIndex>>IsBad;
    if(IsShowInfo==1) { cout<<"Hira"<<HiraIndex<<"  "<<IsBad<<endl; }
    BadHiraMap[HiraIndex] = IsBad;

    infile.ignore(99,'\n');
    char first; infile.get(first);
    infile.putback(first);
  }
  cout<<"*** - Bad Hira - ***"<<endl;
}

bool Hira_BadMap::IsBad_StripX(int HiraIndex,int StripX_Index)
{
  if(BadStripMap[HiraIndex][StripX_Index][0]==1) { return 1; }
return 0;
}

bool Hira_BadMap::IsBad_StripY(int HiraIndex,int StripY_Index)
{
  if(BadStripMap[HiraIndex][StripY_Index][1]==1) { return 1; }
return 0;
}

bool Hira_BadMap::IsBad_CsI(int HiraIndex,int CsI_Index)
{
  if(BadCsIMap[HiraIndex][CsI_Index]==1) { return 1; }
return 0;
}

bool Hira_BadMap::IsBad_CsI(int HiraIndex,int X_StripIndex,int Y_StripIndex)
{
  int CsIIndex = 0;
  if(X_StripIndex<=15 && Y_StripIndex>=16) { CsIIndex = 0; }
  else if(X_StripIndex<=15 && Y_StripIndex<=15) { CsIIndex = 1; }
  else if(X_StripIndex>=16 && Y_StripIndex<=15) { CsIIndex = 2; }
  else if(X_StripIndex>=16 && Y_StripIndex>=16) { CsIIndex = 3; }
  
return IsBad_CsI(HiraIndex,CsIIndex);
}

bool Hira_BadMap::IsBad_Hira(int HiraIndex)
{
  if(BadHiraMap[HiraIndex]==1) {return 1;}
return 0;
}
