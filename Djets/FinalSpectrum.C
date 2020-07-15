void FinalSpectrum()
{
//=========Macro generated from canvas: FinalSpectrum/FinalSpectrum
//=========  (Tue Jul 14 21:04:29 2020) by ROOT version 6.16/00
   TCanvas *FinalSpectrum = new TCanvas("FinalSpectrum", "FinalSpectrum",0,73,700,700);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   FinalSpectrum->SetHighLightColor(2);
   FinalSpectrum->Range(0,0,1,1);
   FinalSpectrum->SetFillColor(0);
   FinalSpectrum->SetBorderMode(0);
   FinalSpectrum->SetBorderSize(2);
   FinalSpectrum->SetTickx(1);
   FinalSpectrum->SetTicky(1);
   FinalSpectrum->SetLeftMargin(0.12);
   FinalSpectrum->SetRightMargin(0.04);
   FinalSpectrum->SetTopMargin(0.04);
   FinalSpectrum->SetBottomMargin(0.14);
   FinalSpectrum->SetFrameLineColor(0);
   FinalSpectrum->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: pad_top
   TPad *pad_top = new TPad("pad_top", "pad_top",0,0.35,1,1);
   pad_top->Draw();
   pad_top->cd();
   pad_top->Range(-5.384616,-5.69897,52.30769,1.776125);
   pad_top->SetFillColor(0);
   pad_top->SetBorderMode(0);
   pad_top->SetBorderSize(2);
   pad_top->SetLogy();
   pad_top->SetTickx(1);
   pad_top->SetTicky(1);
   pad_top->SetLeftMargin(0.18);
   pad_top->SetRightMargin(0.04);
   pad_top->SetTopMargin(0.04);
   pad_top->SetBottomMargin(0);
   pad_top->SetFrameLineColor(0);
   pad_top->SetFrameBorderMode(0);
   pad_top->SetFrameLineColor(0);
   pad_top->SetFrameBorderMode(0);
   Double_t xAxis1[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hEmpty_up__1 = new TH1D("hEmpty_up__1","Central Values",8, xAxis1);
   hEmpty_up__1->SetMinimum(2e-06);
   hEmpty_up__1->SetMaximum(30);
   hEmpty_up__1->SetStats(0);
   hEmpty_up__1->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   hEmpty_up__1->GetXaxis()->SetLabelFont(42);
   hEmpty_up__1->GetXaxis()->SetLabelSize(0.035);
   hEmpty_up__1->GetXaxis()->SetTitleSize(0.035);
   hEmpty_up__1->GetXaxis()->SetTitleOffset(0.8);
   hEmpty_up__1->GetXaxis()->SetTitleFont(42);
   hEmpty_up__1->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} mb (GeV/#it{c})^{-1}");
   hEmpty_up__1->GetYaxis()->SetLabelFont(43);
   hEmpty_up__1->GetYaxis()->SetLabelSize(22);
   hEmpty_up__1->GetYaxis()->SetTitleSize(26);
   hEmpty_up__1->GetYaxis()->SetTitleOffset(2);
   hEmpty_up__1->GetYaxis()->SetTitleFont(43);
   hEmpty_up__1->GetZaxis()->SetLabelFont(42);
   hEmpty_up__1->GetZaxis()->SetLabelSize(0.1);
   hEmpty_up__1->GetZaxis()->SetTitleSize(0.1);
   hEmpty_up__1->GetZaxis()->SetTitleOffset(1);
   hEmpty_up__1->GetZaxis()->SetTitleFont(42);
   hEmpty_up__1->Draw("axis");
   
   Double_t haeData_binned_syst_fx3001[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t haeData_binned_syst_fy3001[8] = {
   0.03490911,
   0.01871483,
   0.007935365,
   0.003577374,
   0.00186927,
   0.0007164286,
   0.0001191137,
   1.671235e-05};
   Double_t haeData_binned_syst_felx3001[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t haeData_binned_syst_fely3001[8] = {
   0.003490911,
   0.001684335,
   0.0008728901,
   0.0004292848,
   0.0002616978,
   0.0001146286,
   2.739615e-05,
   4.010964e-06};
   Double_t haeData_binned_syst_fehx3001[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t haeData_binned_syst_fehy3001[8] = {
   0.003141819,
   0.001497187,
   0.0007141828,
   0.0003577374,
   0.0002056197,
   9.313572e-05,
   1.786705e-05,
   3.175346e-06};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(8,haeData_binned_syst_fx3001,haeData_binned_syst_fy3001,haeData_binned_syst_felx3001,haeData_binned_syst_fehx3001,haeData_binned_syst_fely3001,haeData_binned_syst_fehy3001);
   grae->SetName("haeData_binned_syst");
   grae->SetTitle("Graph");
   grae->SetFillColor(17);
   grae->SetFillStyle(1000);
   grae->SetLineColor(17);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#990000");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.9);
   
   TH1F *Graph_haeData_binned_syst3001 = new TH1F("Graph_haeData_binned_syst3001","Graph",100,0.5,54.5);
   Graph_haeData_binned_syst3001->SetMinimum(1.143125e-05);
   Graph_haeData_binned_syst3001->SetMaximum(0.04185475);
   Graph_haeData_binned_syst3001->SetDirectory(0);
   Graph_haeData_binned_syst3001->SetStats(0);
   Graph_haeData_binned_syst3001->GetXaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst3001->GetXaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst3001->GetXaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst3001->GetXaxis()->SetTitleOffset(0.8);
   Graph_haeData_binned_syst3001->GetXaxis()->SetTitleFont(42);
   Graph_haeData_binned_syst3001->GetYaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst3001->GetYaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst3001->GetYaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst3001->GetYaxis()->SetTitleOffset(1.2);
   Graph_haeData_binned_syst3001->GetYaxis()->SetTitleFont(42);
   Graph_haeData_binned_syst3001->GetZaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst3001->GetZaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst3001->GetZaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst3001->GetZaxis()->SetTitleOffset(1);
   Graph_haeData_binned_syst3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_haeData_binned_syst3001);
   
   grae->Draw("2p");
   Double_t xAxis2[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hData_binned__2 = new TH1D("hData_binned__2","",8, xAxis2);
   hData_binned__2->SetBinContent(0,0.1500041);
   hData_binned__2->SetBinContent(1,0.03490911);
   hData_binned__2->SetBinContent(2,0.01871483);
   hData_binned__2->SetBinContent(3,0.007935365);
   hData_binned__2->SetBinContent(4,0.003577374);
   hData_binned__2->SetBinContent(5,0.00186927);
   hData_binned__2->SetBinContent(6,0.0007164286);
   hData_binned__2->SetBinContent(7,0.0001191137);
   hData_binned__2->SetBinContent(8,1.671235e-05);
   hData_binned__2->SetBinContent(9,1.248664e-06);
   hData_binned__2->SetBinError(0,0.003213618);
   hData_binned__2->SetBinError(1,0.001242406);
   hData_binned__2->SetBinError(2,0.0005652364);
   hData_binned__2->SetBinError(3,0.0003462556);
   hData_binned__2->SetBinError(4,0.0002299692);
   hData_binned__2->SetBinError(5,0.0001692877);
   hData_binned__2->SetBinError(6,4.783324e-05);
   hData_binned__2->SetBinError(7,1.54824e-05);
   hData_binned__2->SetBinError(8,3.519775e-06);
   hData_binned__2->SetBinError(9,2.944799e-07);
   hData_binned__2->SetMaximum(0.06981821);
   hData_binned__2->SetEntries(2227.693);
   hData_binned__2->SetStats(0);

   ci = TColor::GetColor("#990000");
   hData_binned__2->SetLineColor(ci);
   hData_binned__2->SetLineWidth(2);

   ci = TColor::GetColor("#990000");
   hData_binned__2->SetMarkerColor(ci);
   hData_binned__2->SetMarkerStyle(20);
   hData_binned__2->SetMarkerSize(0.9);
   hData_binned__2->GetXaxis()->SetTitle("p_{T,jet}^{ch} (GeV/#it{c})");
   hData_binned__2->GetXaxis()->SetRange(1,11);
   hData_binned__2->GetXaxis()->SetLabelFont(42);
   hData_binned__2->GetXaxis()->SetLabelSize(0.035);
   hData_binned__2->GetXaxis()->SetTitleSize(0.035);
   hData_binned__2->GetXaxis()->SetTitleOffset(1);
   hData_binned__2->GetXaxis()->SetTitleFont(42);
   hData_binned__2->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}d#it{#eta} (mb)");
   hData_binned__2->GetYaxis()->SetLabelFont(42);
   hData_binned__2->GetYaxis()->SetLabelSize(0.035);
   hData_binned__2->GetYaxis()->SetTitleSize(0.035);
   hData_binned__2->GetYaxis()->SetTitleFont(42);
   hData_binned__2->GetZaxis()->SetLabelFont(42);
   hData_binned__2->GetZaxis()->SetLabelSize(0.035);
   hData_binned__2->GetZaxis()->SetTitleSize(0.035);
   hData_binned__2->GetZaxis()->SetTitleOffset(1);
   hData_binned__2->GetZaxis()->SetTitleFont(42);
   hData_binned__2->Draw("same p  e0 x0");
   
   Double_t Graph0_fx3002[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph0_fy3002[8] = {
   0.01504664,
   0.008644216,
   0.004250185,
   0.002227078,
   0.001244754,
   0.0005025615,
   0.0001145212,
   1.798207e-05};
   Double_t Graph0_felx3002[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph0_fely3002[8] = {
   0.01059851,
   0.005781523,
   0.002652054,
   0.001316319,
   0.0007028084,
   0.0002668629,
   5.516328e-05,
   8.680574e-06};
   Double_t Graph0_fehx3002[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph0_fehy3002[8] = {
   0.01918039,
   0.01042546,
   0.004712846,
   0.002303941,
   0.001309958,
   0.0004802072,
   0.0001093375,
   1.554115e-05};
   grae = new TGraphAsymmErrors(8,Graph0_fx3002,Graph0_fy3002,Graph0_felx3002,Graph0_fehx3002,Graph0_fely3002,Graph0_fehy3002);
   grae->SetName("Graph0");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#000099");
   grae->SetLineColor(ci);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#000099");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(24);
   
   TH1F *Graph_Graph03002 = new TH1F("Graph_Graph03002","Graph",100,0.5,54.5);
   Graph_Graph03002->SetMinimum(8.371343e-06);
   Graph_Graph03002->SetMaximum(0.0376488);
   Graph_Graph03002->SetDirectory(0);
   Graph_Graph03002->SetStats(0);
   Graph_Graph03002->GetXaxis()->SetLabelFont(42);
   Graph_Graph03002->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph03002->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph03002->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph03002->GetXaxis()->SetTitleFont(42);
   Graph_Graph03002->GetYaxis()->SetLabelFont(42);
   Graph_Graph03002->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph03002->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph03002->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph03002->GetYaxis()->SetTitleFont(42);
   Graph_Graph03002->GetZaxis()->SetLabelFont(42);
   Graph_Graph03002->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph03002->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph03002->GetZaxis()->SetTitleOffset(1);
   Graph_Graph03002->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph03002);
   
   grae->Draw("2p");
   
   Double_t Graph1_fx3003[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph1_fy3003[8] = {
   0.0135802,
   0.007756128,
   0.003757127,
   0.001966035,
   0.001115195,
   0.0004447402,
   0.0001068695,
   1.692307e-05};
   Double_t Graph1_felx3003[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph1_fely3003[8] = {
   0.009544747,
   0.005163693,
   0.002327338,
   0.001138323,
   0.0006163302,
   0.0002207361,
   4.868444e-05,
   6.960559e-06};
   Double_t Graph1_fehx3003[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph1_fehy3003[8] = {
   0.0170058,
   0.008817268,
   0.003812228,
   0.00189747,
   0.001028204,
   0.000397071,
   9.157852e-05,
   1.538682e-05};
   grae = new TGraphAsymmErrors(8,Graph1_fx3003,Graph1_fy3003,Graph1_felx3003,Graph1_fehx3003,Graph1_fely3003,Graph1_fehy3003);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#990011");
   grae->SetLineColor(ci);
   grae->SetLineStyle(2);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#990011");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(25);
   
   TH1F *Graph_Graph13003 = new TH1F("Graph_Graph13003","Graph",100,0.5,54.5);
   Graph_Graph13003->SetMinimum(8.966263e-06);
   Graph_Graph13003->SetMaximum(0.0336436);
   Graph_Graph13003->SetDirectory(0);
   Graph_Graph13003->SetStats(0);
   Graph_Graph13003->GetXaxis()->SetLabelFont(42);
   Graph_Graph13003->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph13003->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph13003->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph13003->GetXaxis()->SetTitleFont(42);
   Graph_Graph13003->GetYaxis()->SetLabelFont(42);
   Graph_Graph13003->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph13003->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph13003->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph13003->GetYaxis()->SetTitleFont(42);
   Graph_Graph13003->GetZaxis()->SetLabelFont(42);
   Graph_Graph13003->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph13003->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph13003->GetZaxis()->SetTitleOffset(1);
   Graph_Graph13003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph13003);
   
   grae->Draw("2p");
   
   Double_t Graph2_fx3004[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph2_fy3004[8] = {
   0.0492732,
   0.02775853,
   0.01262641,
   0.006168103,
   0.003201371,
   0.001138999,
   0.0002244394,
   2.803281e-05};
   Double_t Graph2_felx3004[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph2_fely3004[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph2_fehx3004[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph2_fehy3004[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph2_fx3004,Graph2_fy3004,Graph2_felx3004,Graph2_fehx3004,Graph2_fely3004,Graph2_fehy3004);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#009933");
   grae->SetLineColor(ci);
   grae->SetLineStyle(3);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#009933");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(26);
   
   TH1F *Graph_Graph23004 = new TH1F("Graph_Graph23004","Graph",100,0.5,54.5);
   Graph_Graph23004->SetMinimum(2.522953e-05);
   Graph_Graph23004->SetMaximum(0.05419771);
   Graph_Graph23004->SetDirectory(0);
   Graph_Graph23004->SetStats(0);
   Graph_Graph23004->GetXaxis()->SetLabelFont(42);
   Graph_Graph23004->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph23004->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph23004->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph23004->GetXaxis()->SetTitleFont(42);
   Graph_Graph23004->GetYaxis()->SetLabelFont(42);
   Graph_Graph23004->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph23004->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph23004->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph23004->GetYaxis()->SetTitleFont(42);
   Graph_Graph23004->GetZaxis()->SetLabelFont(42);
   Graph_Graph23004->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph23004->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph23004->GetZaxis()->SetTitleOffset(1);
   Graph_Graph23004->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph23004);
   
   grae->Draw("2p");
   
   Double_t Graph3_fx3005[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph3_fy3005[8] = {
   0.04011591,
   0.02320663,
   0.01100189,
   0.005444101,
   0.002853349,
   0.001082766,
   0.0002165444,
   3.02158e-05};
   Double_t Graph3_felx3005[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph3_fely3005[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph3_fehx3005[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph3_fehy3005[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph3_fx3005,Graph3_fy3005,Graph3_felx3005,Graph3_fehx3005,Graph3_fely3005,Graph3_fehy3005);
   grae->SetName("Graph3");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#6600cc");
   grae->SetLineColor(ci);
   grae->SetLineStyle(4);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#6600cc");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(27);
   
   TH1F *Graph_Graph33005 = new TH1F("Graph_Graph33005","Graph",100,0.5,54.5);
   Graph_Graph33005->SetMinimum(2.719422e-05);
   Graph_Graph33005->SetMaximum(0.04412448);
   Graph_Graph33005->SetDirectory(0);
   Graph_Graph33005->SetStats(0);
   Graph_Graph33005->GetXaxis()->SetLabelFont(42);
   Graph_Graph33005->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph33005->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph33005->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph33005->GetXaxis()->SetTitleFont(42);
   Graph_Graph33005->GetYaxis()->SetLabelFont(42);
   Graph_Graph33005->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph33005->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph33005->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph33005->GetYaxis()->SetTitleFont(42);
   Graph_Graph33005->GetZaxis()->SetLabelFont(42);
   Graph_Graph33005->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph33005->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph33005->GetZaxis()->SetTitleOffset(1);
   Graph_Graph33005->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph33005);
   
   grae->Draw("2p");
   
   Double_t Graph4_fx3006[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph4_fy3006[8] = {
   0.02285313,
   0.01326108,
   0.006475532,
   0.003299667,
   0.001804841,
   0.0006947205,
   0.0001491756,
   1.94399e-05};
   Double_t Graph4_felx3006[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph4_fely3006[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph4_fehx3006[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph4_fehy3006[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph4_fx3006,Graph4_fy3006,Graph4_felx3006,Graph4_fehx3006,Graph4_fely3006,Graph4_fehy3006);
   grae->SetName("Graph4");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#cc6600");
   grae->SetLineColor(ci);
   grae->SetLineStyle(5);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#cc6600");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(28);
   
   TH1F *Graph_Graph43006 = new TH1F("Graph_Graph43006","Graph",100,0.5,54.5);
   Graph_Graph43006->SetMinimum(1.749591e-05);
   Graph_Graph43006->SetMaximum(0.0251365);
   Graph_Graph43006->SetDirectory(0);
   Graph_Graph43006->SetStats(0);
   Graph_Graph43006->GetXaxis()->SetLabelFont(42);
   Graph_Graph43006->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph43006->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph43006->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph43006->GetXaxis()->SetTitleFont(42);
   Graph_Graph43006->GetYaxis()->SetLabelFont(42);
   Graph_Graph43006->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph43006->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph43006->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph43006->GetYaxis()->SetTitleFont(42);
   Graph_Graph43006->GetZaxis()->SetLabelFont(42);
   Graph_Graph43006->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph43006->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph43006->GetZaxis()->SetTitleOffset(1);
   Graph_Graph43006->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph43006);
   
   grae->Draw("2p");
   
   TLegend *leg = new TLegend(0.5,0.4,0.8,0.65,NULL,"NB NDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(43);
   leg->SetTextSize(23);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("haeData_binned_syst","Data","fp");
   entry->SetFillColor(17);
   entry->SetFillStyle(1000);
   entry->SetLineColor(17);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990000");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(0.9);
   entry->SetTextFont(43);
   entry=leg->AddEntry("Graph0","POWHEG hvq + PYTHIA 6","pf");
   entry->SetFillColor(1);

   ci = TColor::GetColor("#000099");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#000099");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(24);
   entry->SetMarkerSize(1);
   entry->SetTextFont(43);
   entry=leg->AddEntry("Graph1","POWHEG hvq + PYTHIA 8","pf");
   entry->SetFillColor(1);

   ci = TColor::GetColor("#990011");
   entry->SetLineColor(ci);
   entry->SetLineStyle(2);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#990011");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(25);
   entry->SetMarkerSize(1);
   entry->SetTextFont(43);
   entry=leg->AddEntry("Graph2","PYTHIA 6 Perugia 2011","pf");
   entry->SetFillColor(1);

   ci = TColor::GetColor("#009933");
   entry->SetLineColor(ci);
   entry->SetLineStyle(3);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#009933");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(26);
   entry->SetMarkerSize(1);
   entry->SetTextFont(43);
   entry=leg->AddEntry("Graph3","PYTHIA 8 Monash 2013","pf");
   entry->SetFillColor(1);

   ci = TColor::GetColor("#6600cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(4);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#6600cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(27);
   entry->SetMarkerSize(1);
   entry->SetTextFont(43);
   entry=leg->AddEntry("Graph4","PYTHIA 8 Monash 2013 Soft mode 2","pf");
   entry->SetFillColor(1);

   ci = TColor::GetColor("#cc6600");
   entry->SetLineColor(ci);
   entry->SetLineStyle(5);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#cc6600");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(28);
   entry->SetMarkerSize(1);
   entry->SetTextFont(43);
   leg->Draw();
   
   TPaveText *pt = new TPaveText(0.2,0.85,0.85,0.95,"NB NDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextAlign(13);
   pt->SetTextFont(43);
   pt->SetTextSize(22);
   TText *pt_LaTex = pt->AddText("ALICE Preliminary");
   pt_LaTex = pt->AddText("pp, #sqrt{#it{s}} = 13 TeV");
   pt->Draw();
   
   pt = new TPaveText(0.2,0.7,0.85,0.85,"NB NDC");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextAlign(13);
   pt->SetTextFont(43);
   pt->SetTextSize(22);
   pt_LaTex = pt->AddText("charged jets, anti-#it{k}_{T}, #it{R} = 0.4, |#it{#eta}_{lab}^{jet}| < 0.5");
   pt_LaTex = pt->AddText("with D^{0}, 2 < #it{p}_{T,D^{0}} < 36 GeV/#it{c}");
   pt->Draw();
   Double_t xAxis3[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hEmpty_up__3 = new TH1D("hEmpty_up__3","Central Values",8, xAxis3);
   hEmpty_up__3->SetMinimum(2e-06);
   hEmpty_up__3->SetMaximum(30);
   hEmpty_up__3->SetStats(0);
   hEmpty_up__3->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   hEmpty_up__3->GetXaxis()->SetLabelFont(42);
   hEmpty_up__3->GetXaxis()->SetLabelSize(0.035);
   hEmpty_up__3->GetXaxis()->SetTitleSize(0.035);
   hEmpty_up__3->GetXaxis()->SetTitleOffset(0.8);
   hEmpty_up__3->GetXaxis()->SetTitleFont(42);
   hEmpty_up__3->GetYaxis()->SetTitle("#frac{d^{2}#sigma}{d#it{p}_{T}d#it{#eta}} mb (GeV/#it{c})^{-1}");
   hEmpty_up__3->GetYaxis()->SetLabelFont(43);
   hEmpty_up__3->GetYaxis()->SetLabelSize(22);
   hEmpty_up__3->GetYaxis()->SetTitleSize(26);
   hEmpty_up__3->GetYaxis()->SetTitleOffset(2);
   hEmpty_up__3->GetYaxis()->SetTitleFont(43);
   hEmpty_up__3->GetZaxis()->SetLabelFont(42);
   hEmpty_up__3->GetZaxis()->SetLabelSize(0.1);
   hEmpty_up__3->GetZaxis()->SetTitleSize(0.1);
   hEmpty_up__3->GetZaxis()->SetTitleOffset(1);
   hEmpty_up__3->GetZaxis()->SetTitleFont(42);
   hEmpty_up__3->Draw("sameaxis");
   pad_top->Modified();
   FinalSpectrum->cd();
  
// ------------>Primitives in pad: pad_bottom
   TPad *pad_bottom = new TPad("pad_bottom", "pad_bottom",0,0,1,0.35);
   pad_bottom->Draw();
   pad_bottom->cd();
   pad_bottom->Range(-5.384616,-0.680871,52.30769,2.307714);
   pad_bottom->SetFillColor(0);
   pad_bottom->SetBorderMode(0);
   pad_bottom->SetBorderSize(2);
   pad_bottom->SetGridy();
   pad_bottom->SetTickx(1);
   pad_bottom->SetTicky(1);
   pad_bottom->SetLeftMargin(0.18);
   pad_bottom->SetRightMargin(0.04);
   pad_bottom->SetTopMargin(0);
   pad_bottom->SetBottomMargin(0.27);
   pad_bottom->SetFrameLineColor(0);
   pad_bottom->SetFrameBorderMode(0);
   pad_bottom->SetFrameLineColor(0);
   pad_bottom->SetFrameBorderMode(0);
   Double_t xAxis4[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hEmpty_bottom__4 = new TH1D("hEmpty_bottom__4","Central Values",8, xAxis4);
   hEmpty_bottom__4->SetMinimum(0.1260469);
   hEmpty_bottom__4->SetMaximum(2.307714);
   hEmpty_bottom__4->SetStats(0);
   hEmpty_bottom__4->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   hEmpty_bottom__4->GetXaxis()->SetLabelFont(43);
   hEmpty_bottom__4->GetXaxis()->SetLabelSize(22);
   hEmpty_bottom__4->GetXaxis()->SetTitleSize(26);
   hEmpty_bottom__4->GetXaxis()->SetTitleOffset(3);
   hEmpty_bottom__4->GetXaxis()->SetTitleFont(43);
   hEmpty_bottom__4->GetYaxis()->SetTitle("theory / data");
   hEmpty_bottom__4->GetYaxis()->CenterTitle(true);
   hEmpty_bottom__4->GetYaxis()->SetDecimals();
   hEmpty_bottom__4->GetYaxis()->SetNdivisions(509);
   hEmpty_bottom__4->GetYaxis()->SetLabelFont(43);
   hEmpty_bottom__4->GetYaxis()->SetLabelOffset(0.007);
   hEmpty_bottom__4->GetYaxis()->SetLabelSize(22);
   hEmpty_bottom__4->GetYaxis()->SetTitleSize(26);
   hEmpty_bottom__4->GetYaxis()->SetTitleOffset(2);
   hEmpty_bottom__4->GetYaxis()->SetTitleFont(43);
   hEmpty_bottom__4->GetZaxis()->SetLabelFont(42);
   hEmpty_bottom__4->GetZaxis()->SetLabelSize(0.1);
   hEmpty_bottom__4->GetZaxis()->SetTitleSize(0.1);
   hEmpty_bottom__4->GetZaxis()->SetTitleOffset(1);
   hEmpty_bottom__4->GetZaxis()->SetTitleFont(42);
   hEmpty_bottom__4->Draw("axis");
   
   Double_t haeData_binned_syst_ratio_fx3007[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t haeData_binned_syst_ratio_fy3007[8] = {
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1};
   Double_t haeData_binned_syst_ratio_felx3007[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t haeData_binned_syst_ratio_fely3007[8] = {
   0.1,
   0.09,
   0.11,
   0.12,
   0.14,
   0.16,
   0.23,
   0.24};
   Double_t haeData_binned_syst_ratio_fehx3007[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t haeData_binned_syst_ratio_fehy3007[8] = {
   0.09,
   0.08,
   0.09,
   0.1,
   0.11,
   0.13,
   0.15,
   0.19};
   grae = new TGraphAsymmErrors(8,haeData_binned_syst_ratio_fx3007,haeData_binned_syst_ratio_fy3007,haeData_binned_syst_ratio_felx3007,haeData_binned_syst_ratio_fehx3007,haeData_binned_syst_ratio_fely3007,haeData_binned_syst_ratio_fehy3007);
   grae->SetName("haeData_binned_syst_ratio");
   grae->SetTitle("Graph");
   grae->SetFillColor(17);
   grae->SetFillStyle(1000);
   grae->SetLineColor(17);

   ci = TColor::GetColor("#990000");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.9);
   
   TH1F *Graph_haeData_binned_syst_ratio3007 = new TH1F("Graph_haeData_binned_syst_ratio3007","Graph",100,0.5,54.5);
   Graph_haeData_binned_syst_ratio3007->SetMinimum(0.717);
   Graph_haeData_binned_syst_ratio3007->SetMaximum(1.233);
   Graph_haeData_binned_syst_ratio3007->SetDirectory(0);
   Graph_haeData_binned_syst_ratio3007->SetStats(0);
   Graph_haeData_binned_syst_ratio3007->GetXaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst_ratio3007->GetXaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetXaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetXaxis()->SetTitleOffset(0.8);
   Graph_haeData_binned_syst_ratio3007->GetXaxis()->SetTitleFont(42);
   Graph_haeData_binned_syst_ratio3007->GetYaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst_ratio3007->GetYaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetYaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetYaxis()->SetTitleOffset(1.2);
   Graph_haeData_binned_syst_ratio3007->GetYaxis()->SetTitleFont(42);
   Graph_haeData_binned_syst_ratio3007->GetZaxis()->SetLabelFont(42);
   Graph_haeData_binned_syst_ratio3007->GetZaxis()->SetLabelSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetZaxis()->SetTitleSize(0.1);
   Graph_haeData_binned_syst_ratio3007->GetZaxis()->SetTitleOffset(1);
   Graph_haeData_binned_syst_ratio3007->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_haeData_binned_syst_ratio3007);
   
   grae->Draw("2p");
   Double_t xAxis5[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hData_binned_ratio__5 = new TH1D("hData_binned_ratio__5","",8, xAxis5);
   hData_binned_ratio__5->SetBinContent(0,0.1500041);
   hData_binned_ratio__5->SetBinContent(1,1);
   hData_binned_ratio__5->SetBinContent(2,1);
   hData_binned_ratio__5->SetBinContent(3,1);
   hData_binned_ratio__5->SetBinContent(4,1);
   hData_binned_ratio__5->SetBinContent(5,1);
   hData_binned_ratio__5->SetBinContent(6,1);
   hData_binned_ratio__5->SetBinContent(7,1);
   hData_binned_ratio__5->SetBinContent(8,1);
   hData_binned_ratio__5->SetBinContent(9,1.248664e-06);
   hData_binned_ratio__5->SetBinError(0,0.003213618);
   hData_binned_ratio__5->SetBinError(1,0.03558974);
   hData_binned_ratio__5->SetBinError(2,0.03020259);
   hData_binned_ratio__5->SetBinError(3,0.0436345);
   hData_binned_ratio__5->SetBinError(4,0.06428438);
   hData_binned_ratio__5->SetBinError(5,0.09056354);
   hData_binned_ratio__5->SetBinError(6,0.06676623);
   hData_binned_ratio__5->SetBinError(7,0.12998);
   hData_binned_ratio__5->SetBinError(8,0.2106092);
   hData_binned_ratio__5->SetBinError(9,2.944799e-07);
   hData_binned_ratio__5->SetMinimum(0.1260469);
   hData_binned_ratio__5->SetMaximum(2.307714);
   hData_binned_ratio__5->SetEntries(2235.693);
   hData_binned_ratio__5->SetStats(0);

   ci = TColor::GetColor("#990000");
   hData_binned_ratio__5->SetLineColor(ci);
   hData_binned_ratio__5->SetLineWidth(2);

   ci = TColor::GetColor("#990000");
   hData_binned_ratio__5->SetMarkerColor(ci);
   hData_binned_ratio__5->SetMarkerStyle(20);
   hData_binned_ratio__5->SetMarkerSize(0.9);
   hData_binned_ratio__5->GetXaxis()->SetTitle("p_{T,jet}^{ch} (GeV/#it{c})");
   hData_binned_ratio__5->GetXaxis()->SetRange(1,11);
   hData_binned_ratio__5->GetXaxis()->SetLabelFont(42);
   hData_binned_ratio__5->GetXaxis()->SetLabelSize(0.035);
   hData_binned_ratio__5->GetXaxis()->SetTitleSize(0.035);
   hData_binned_ratio__5->GetXaxis()->SetTitleOffset(1);
   hData_binned_ratio__5->GetXaxis()->SetTitleFont(42);
   hData_binned_ratio__5->GetYaxis()->SetTitle("d^{2}#sigma/dp_{T}d#it{#eta} (mb)");
   hData_binned_ratio__5->GetYaxis()->SetLabelFont(42);
   hData_binned_ratio__5->GetYaxis()->SetLabelSize(0.035);
   hData_binned_ratio__5->GetYaxis()->SetTitleSize(0.035);
   hData_binned_ratio__5->GetYaxis()->SetTitleFont(42);
   hData_binned_ratio__5->GetZaxis()->SetLabelFont(42);
   hData_binned_ratio__5->GetZaxis()->SetLabelSize(0.035);
   hData_binned_ratio__5->GetZaxis()->SetTitleSize(0.035);
   hData_binned_ratio__5->GetZaxis()->SetTitleOffset(1);
   hData_binned_ratio__5->GetZaxis()->SetTitleFont(42);
   hData_binned_ratio__5->Draw("same p  e0 x0");
   
   Double_t Graph0_fx3008[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph0_fy3008[8] = {
   0.4310234,
   0.4618912,
   0.5356005,
   0.6225456,
   0.6659041,
   0.7014816,
   0.9614443,
   1.075975};
   Double_t Graph0_felx3008[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph0_fely3008[8] = {
   0.3036032,
   0.3089273,
   0.334207,
   0.3679568,
   0.3759802,
   0.3724906,
   0.4631146,
   0.5194108};
   Double_t Graph0_fehx3008[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph0_fehy3008[8] = {
   0.549438,
   0.5570692,
   0.5939041,
   0.6440314,
   0.700786,
   0.6702793,
   0.9179256,
   0.9299202};
   grae = new TGraphAsymmErrors(8,Graph0_fx3008,Graph0_fy3008,Graph0_felx3008,Graph0_fehx3008,Graph0_fely3008,Graph0_fehy3008);
   grae->SetName("Graph0");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#000099");
   grae->SetLineColor(ci);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#000099");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(24);
   
   TH1F *Graph_Graph03008 = new TH1F("Graph_Graph03008","Graph",100,0.5,54.5);
   Graph_Graph03008->SetMinimum(0.1146783);
   Graph_Graph03008->SetMaximum(2.193742);
   Graph_Graph03008->SetDirectory(0);
   Graph_Graph03008->SetStats(0);
   Graph_Graph03008->GetXaxis()->SetLabelFont(42);
   Graph_Graph03008->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph03008->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph03008->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph03008->GetXaxis()->SetTitleFont(42);
   Graph_Graph03008->GetYaxis()->SetLabelFont(42);
   Graph_Graph03008->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph03008->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph03008->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph03008->GetYaxis()->SetTitleFont(42);
   Graph_Graph03008->GetZaxis()->SetLabelFont(42);
   Graph_Graph03008->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph03008->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph03008->GetZaxis()->SetTitleOffset(1);
   Graph_Graph03008->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph03008);
   
   grae->Draw("2p");
   
   Double_t Graph1_fx3009[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph1_fy3009[8] = {
   0.389016,
   0.4144375,
   0.4734662,
   0.549575,
   0.5965938,
   0.620774,
   0.8972056,
   1.012609};
   Double_t Graph1_felx3009[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph1_fely3009[8] = {
   0.2734171,
   0.2759145,
   0.2932868,
   0.3182008,
   0.3297171,
   0.3081062,
   0.4087225,
   0.4164919};
   Double_t Graph1_fehx3009[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph1_fehy3009[8] = {
   0.487145,
   0.4711379,
   0.4804099,
   0.5304088,
   0.5500563,
   0.5542367,
   0.7688329,
   0.9206855};
   grae = new TGraphAsymmErrors(8,Graph1_fx3009,Graph1_fy3009,Graph1_felx3009,Graph1_fehx3009,Graph1_fely3009,Graph1_fehy3009);
   grae->SetName("Graph1");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#990011");
   grae->SetLineColor(ci);
   grae->SetLineStyle(2);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#990011");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(25);
   
   TH1F *Graph_Graph13009 = new TH1F("Graph_Graph13009","Graph",100,0.5,54.5);
   Graph_Graph13009->SetMinimum(0.1040389);
   Graph_Graph13009->SetMaximum(2.115064);
   Graph_Graph13009->SetDirectory(0);
   Graph_Graph13009->SetStats(0);
   Graph_Graph13009->GetXaxis()->SetLabelFont(42);
   Graph_Graph13009->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph13009->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph13009->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph13009->GetXaxis()->SetTitleFont(42);
   Graph_Graph13009->GetYaxis()->SetLabelFont(42);
   Graph_Graph13009->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph13009->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph13009->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph13009->GetYaxis()->SetTitleFont(42);
   Graph_Graph13009->GetZaxis()->SetLabelFont(42);
   Graph_Graph13009->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph13009->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph13009->GetZaxis()->SetTitleOffset(1);
   Graph_Graph13009->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph13009);
   
   grae->Draw("2p");
   
   Double_t Graph2_fx3010[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph2_fy3010[8] = {
   1.411471,
   1.483237,
   1.591157,
   1.724199,
   1.712632,
   1.589829,
   1.884245,
   1.677371};
   Double_t Graph2_felx3010[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph2_fely3010[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph2_fehx3010[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph2_fehy3010[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph2_fx3010,Graph2_fy3010,Graph2_felx3010,Graph2_fehx3010,Graph2_fely3010,Graph2_fehy3010);
   grae->SetName("Graph2");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#009933");
   grae->SetLineColor(ci);
   grae->SetLineStyle(3);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#009933");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(26);
   
   TH1F *Graph_Graph23010 = new TH1F("Graph_Graph23010","Graph",100,0.5,54.5);
   Graph_Graph23010->SetMinimum(1.364194);
   Graph_Graph23010->SetMaximum(1.931522);
   Graph_Graph23010->SetDirectory(0);
   Graph_Graph23010->SetStats(0);
   Graph_Graph23010->GetXaxis()->SetLabelFont(42);
   Graph_Graph23010->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph23010->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph23010->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph23010->GetXaxis()->SetTitleFont(42);
   Graph_Graph23010->GetYaxis()->SetLabelFont(42);
   Graph_Graph23010->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph23010->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph23010->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph23010->GetYaxis()->SetTitleFont(42);
   Graph_Graph23010->GetZaxis()->SetLabelFont(42);
   Graph_Graph23010->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph23010->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph23010->GetZaxis()->SetTitleOffset(1);
   Graph_Graph23010->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph23010);
   
   grae->Draw("2p");
   
   Double_t Graph3_fx3011[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph3_fy3011[8] = {
   1.149153,
   1.240013,
   1.386438,
   1.521815,
   1.526451,
   1.511338,
   1.817964,
   1.807992};
   Double_t Graph3_felx3011[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph3_fely3011[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph3_fehx3011[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph3_fehy3011[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph3_fx3011,Graph3_fy3011,Graph3_felx3011,Graph3_fehx3011,Graph3_fely3011,Graph3_fehy3011);
   grae->SetName("Graph3");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#6600cc");
   grae->SetLineColor(ci);
   grae->SetLineStyle(4);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#6600cc");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(27);
   
   TH1F *Graph_Graph33011 = new TH1F("Graph_Graph33011","Graph",100,0.5,54.5);
   Graph_Graph33011->SetMinimum(1.082272);
   Graph_Graph33011->SetMaximum(1.884845);
   Graph_Graph33011->SetDirectory(0);
   Graph_Graph33011->SetStats(0);
   Graph_Graph33011->GetXaxis()->SetLabelFont(42);
   Graph_Graph33011->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph33011->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph33011->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph33011->GetXaxis()->SetTitleFont(42);
   Graph_Graph33011->GetYaxis()->SetLabelFont(42);
   Graph_Graph33011->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph33011->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph33011->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph33011->GetYaxis()->SetTitleFont(42);
   Graph_Graph33011->GetZaxis()->SetLabelFont(42);
   Graph_Graph33011->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph33011->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph33011->GetZaxis()->SetTitleOffset(1);
   Graph_Graph33011->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph33011);
   
   grae->Draw("2p");
   
   Double_t Graph4_fx3012[8] = {
   5.5,
   7,
   9,
   11,
   13,
   17,
   25,
   40};
   Double_t Graph4_fy3012[8] = {
   0.6546468,
   0.7085866,
   0.8160346,
   0.9223713,
   0.9655327,
   0.9696996,
   1.25238,
   1.163206};
   Double_t Graph4_felx3012[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph4_fely3012[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph4_fehx3012[8] = {
   0.5,
   1,
   1,
   1,
   1,
   3,
   5,
   10};
   Double_t Graph4_fehy3012[8] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   grae = new TGraphAsymmErrors(8,Graph4_fx3012,Graph4_fy3012,Graph4_felx3012,Graph4_fehx3012,Graph4_fely3012,Graph4_fehy3012);
   grae->SetName("Graph4");
   grae->SetTitle("Graph");
   grae->SetFillColor(1);
   grae->SetFillStyle(0);

   ci = TColor::GetColor("#cc6600");
   grae->SetLineColor(ci);
   grae->SetLineStyle(5);
   grae->SetLineWidth(2);

   ci = TColor::GetColor("#cc6600");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(28);
   
   TH1F *Graph_Graph43012 = new TH1F("Graph_Graph43012","Graph",100,0.5,54.5);
   Graph_Graph43012->SetMinimum(0.5948734);
   Graph_Graph43012->SetMaximum(1.312154);
   Graph_Graph43012->SetDirectory(0);
   Graph_Graph43012->SetStats(0);
   Graph_Graph43012->GetXaxis()->SetLabelFont(42);
   Graph_Graph43012->GetXaxis()->SetLabelSize(0.1);
   Graph_Graph43012->GetXaxis()->SetTitleSize(0.1);
   Graph_Graph43012->GetXaxis()->SetTitleOffset(0.8);
   Graph_Graph43012->GetXaxis()->SetTitleFont(42);
   Graph_Graph43012->GetYaxis()->SetLabelFont(42);
   Graph_Graph43012->GetYaxis()->SetLabelSize(0.1);
   Graph_Graph43012->GetYaxis()->SetTitleSize(0.1);
   Graph_Graph43012->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph43012->GetYaxis()->SetTitleFont(42);
   Graph_Graph43012->GetZaxis()->SetLabelFont(42);
   Graph_Graph43012->GetZaxis()->SetLabelSize(0.1);
   Graph_Graph43012->GetZaxis()->SetTitleSize(0.1);
   Graph_Graph43012->GetZaxis()->SetTitleOffset(1);
   Graph_Graph43012->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph43012);
   
   grae->Draw("2p");
   Double_t xAxis6[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hEmpty_bottom__6 = new TH1D("hEmpty_bottom__6","Central Values",8, xAxis6);
   hEmpty_bottom__6->SetMinimum(0.1260469);
   hEmpty_bottom__6->SetMaximum(2.307714);
   hEmpty_bottom__6->SetStats(0);
   hEmpty_bottom__6->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   hEmpty_bottom__6->GetXaxis()->SetLabelFont(43);
   hEmpty_bottom__6->GetXaxis()->SetLabelSize(22);
   hEmpty_bottom__6->GetXaxis()->SetTitleSize(26);
   hEmpty_bottom__6->GetXaxis()->SetTitleOffset(3);
   hEmpty_bottom__6->GetXaxis()->SetTitleFont(43);
   hEmpty_bottom__6->GetYaxis()->SetTitle("theory / data");
   hEmpty_bottom__6->GetYaxis()->CenterTitle(true);
   hEmpty_bottom__6->GetYaxis()->SetDecimals();
   hEmpty_bottom__6->GetYaxis()->SetNdivisions(509);
   hEmpty_bottom__6->GetYaxis()->SetLabelFont(43);
   hEmpty_bottom__6->GetYaxis()->SetLabelOffset(0.007);
   hEmpty_bottom__6->GetYaxis()->SetLabelSize(22);
   hEmpty_bottom__6->GetYaxis()->SetTitleSize(26);
   hEmpty_bottom__6->GetYaxis()->SetTitleOffset(2);
   hEmpty_bottom__6->GetYaxis()->SetTitleFont(43);
   hEmpty_bottom__6->GetZaxis()->SetLabelFont(42);
   hEmpty_bottom__6->GetZaxis()->SetLabelSize(0.1);
   hEmpty_bottom__6->GetZaxis()->SetTitleSize(0.1);
   hEmpty_bottom__6->GetZaxis()->SetTitleOffset(1);
   hEmpty_bottom__6->GetZaxis()->SetTitleFont(42);
   hEmpty_bottom__6->Draw("sameaxis");
   Double_t xAxis7[9] = {5, 6, 8, 10, 12, 14, 20, 30, 50}; 
   
   TH1D *hEmpty_bottom__7 = new TH1D("hEmpty_bottom__7","Central Values",8, xAxis7);
   hEmpty_bottom__7->SetMinimum(0.1260469);
   hEmpty_bottom__7->SetMaximum(2.307714);
   hEmpty_bottom__7->SetStats(0);
   hEmpty_bottom__7->GetXaxis()->SetTitle("#it{p}_{T,ch jet} (GeV/#it{c})");
   hEmpty_bottom__7->GetXaxis()->SetLabelFont(43);
   hEmpty_bottom__7->GetXaxis()->SetLabelSize(22);
   hEmpty_bottom__7->GetXaxis()->SetTitleSize(26);
   hEmpty_bottom__7->GetXaxis()->SetTitleOffset(3);
   hEmpty_bottom__7->GetXaxis()->SetTitleFont(43);
   hEmpty_bottom__7->GetYaxis()->SetTitle("theory / data");
   hEmpty_bottom__7->GetYaxis()->CenterTitle(true);
   hEmpty_bottom__7->GetYaxis()->SetDecimals();
   hEmpty_bottom__7->GetYaxis()->SetNdivisions(509);
   hEmpty_bottom__7->GetYaxis()->SetLabelFont(43);
   hEmpty_bottom__7->GetYaxis()->SetLabelOffset(0.007);
   hEmpty_bottom__7->GetYaxis()->SetLabelSize(22);
   hEmpty_bottom__7->GetYaxis()->SetTitleSize(26);
   hEmpty_bottom__7->GetYaxis()->SetTitleOffset(2);
   hEmpty_bottom__7->GetYaxis()->SetTitleFont(43);
   hEmpty_bottom__7->GetZaxis()->SetLabelFont(42);
   hEmpty_bottom__7->GetZaxis()->SetLabelSize(0.1);
   hEmpty_bottom__7->GetZaxis()->SetTitleSize(0.1);
   hEmpty_bottom__7->GetZaxis()->SetTitleOffset(1);
   hEmpty_bottom__7->GetZaxis()->SetTitleFont(42);
   hEmpty_bottom__7->Draw("sameaxig");
   pad_bottom->Modified();
   FinalSpectrum->cd();
   FinalSpectrum->Modified();
   FinalSpectrum->cd();
   FinalSpectrum->SetSelected(FinalSpectrum);
}
