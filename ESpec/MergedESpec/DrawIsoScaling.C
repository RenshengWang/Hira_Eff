void DrawIsoScaling()
{
  const int PointNum1 = 3;
  double H_N[PointNum1] = {0,1,2};
  //double R21_H[PointNum1] = {0.85, 1.12, 1.6};
  double R21_H[PointNum1] = {0.82, 1.15, 1.52};
  
  const int PointNum2 = 2;
  double He_N[PointNum1] = {1,2};
  //double R21_He[PointNum2] = { 0.9,1.3 };
  double R21_He[PointNum2] = { 0.9,1.25 };

  TGraph* gr1_H = new TGraph(PointNum1,H_N,R21_H);
  TGraph* gr1_He = new TGraph(PointNum2,He_N,R21_He);
  
  gr1_H->Draw("AL*");
  gr1_He->Draw("LSame*");
}
