#define AnalyzePUEst_cxx
#include "AnalyzePUEst.h"

using namespace std;

void AnalyzePUEst::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L AnalyzePUEst.C
//      Root > AnalyzePUEst t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
       
//       if(lumi < 40)//instlumi < 200) <--use for 2012D runs
// 	{
// 	  continue;
// 	}      

      if(run == 198609 || run == 198603)
	continue;

      hXAvgPU->Fill(xAvgPU);
      hNVtx->Fill(nVtx);

      vector<unsigned> nPURegAtEta(22,0);
      vector<float> puEtAtEta(22,0.);

      for(unsigned ind = 0; ind < 22*18; ++ind)
	{
	  if(nVtx > 5.)
	    {
	      unsigned phi = ind % 18;
	      unsigned eta = (ind - phi) / 18;

	      hRegEtVsEta->Fill((float)eta,regEt->at(ind));

	      hTAvgPU->Fill(tAvgPU->at(ind));
	      hUICPU->Fill(uicPU->at(ind));

	      if(eta >= 5 && eta <= 16)
		{
		  hTAvgPUVsAvgLumiLoEta->Fill(lumiAvg->at(ind), 
					      tAvgPU->at(ind));
		  hTAvgPUVsNVtxLoEta->Fill(nVtx, tAvgPU->at(ind));
		  hTAvgPUSubEtVsNVtxLoEta->Fill(nVtx, tAvgPUSubEt->at(ind));
		}
	      else if(eta <= 2 || eta >= 19)
		{
		  hTAvgPUVsAvgLumiHiEta->Fill(lumiAvg->at(ind), 
					      tAvgPU->at(ind));
		  hTAvgPUVsNVtxHiEta->Fill(nVtx, tAvgPU->at(ind));
		  hTAvgPUSubEtVsNVtxHiEta->Fill(nVtx, tAvgPUSubEt->at(ind));
		}
	      else
		{
		  hTAvgPUVsAvgLumiMdEta->Fill(lumiAvg->at(ind), 
					      tAvgPU->at(ind));
		  hTAvgPUVsNVtxMdEta->Fill(nVtx, tAvgPU->at(ind));
		  hTAvgPUSubEtVsNVtxMdEta->Fill(nVtx, tAvgPUSubEt->at(ind));
		}
	      hTAvgPUVsAvgLumi->Fill(lumiAvg->at(ind), tAvgPU->at(ind));
	      hvTAvgPUVsAvgLumi.at(eta)->Fill(lumiAvg->at(ind), 
					      tAvgPU->at(ind));
	      hvTAvgPUVsNVtx.at(eta)->Fill(nVtx, 
					   tAvgPU->at(ind));
	      hvTAvgPUSubEtVsNVtx.at(eta)->Fill(nVtx, 
						tAvgPUSubEt->at(ind));
	      hvEstPUSubEtVsNVtx.at(eta)->Fill(nVtx, 
					       regEt->at(ind) 
					       - estPULvl->at(eta));
	      hTAvgPUVsEta->Fill((float)eta, tAvgPU->at(ind));
	      hUICPUVsEta->Fill((float)eta, uicPU->at(ind));
	      hTAvgPUVsPhi->Fill((float)phi, tAvgPU->at(ind));
	      hAvgLumi->Fill(lumiAvg->at(ind));
	      hTAvgPUSubEt->Fill(tAvgPUSubEt->at(ind));
	      hXAvgPUSubEt->Fill(xAvgPUSubEt->at(ind));
	      hUICPUSubEt->Fill(uicPUSubEt->at(ind));
	      hRegEt->Fill(regEt->at(ind));
	      hLumiStd->Fill(lumiStd->at(ind));
	      hTAvgPUSubEtVsEta->Fill((float)eta, tAvgPUSubEt->at(ind));
	      hEstPUSubEtVsEta->Fill((float)eta, 
				     regEt->at(ind) - estPULvl->at(eta));
	      hEstPUSubEt->Fill(regEt->at(ind) - estPULvl->at(eta));
	      h2TAvgPUSubEtVsEta->Fill((float)eta, tAvgPUSubEt->at(ind));
	      hXAvgPUSubEtVsEta->Fill((float)eta, xAvgPUSubEt->at(ind));
	      hUICPUSubEtVsEta->Fill((float)eta, uicPUSubEt->at(ind));

	      hTAvgPUSubEtVsRegEt->Fill(regEt->at(ind),tAvgPUSubEt->at(ind));
	      
	      ++nPURegAtEta.at(eta);
	      puEtAtEta.at(eta) += regEt->at(ind);

	      hvTAvgPUVsPhi.at(eta)->Fill(phi, tAvgPU->at(ind));
	      hvRegEtVsNVtx.at(eta)->Fill(nVtx, regEt->at(ind));

	      if(eta == 3)
		hRegEtVsPhi3->Fill(phi, regEt->at(ind));
	      if(eta == 4)
		hRegEtVsPhi4->Fill(phi, regEt->at(ind));
	    }
	}

      for(unsigned eta = 0; eta < nPURegAtEta.size(); ++eta)
	{
	  puEtAtEta.at(eta) /= (float)nPURegAtEta.at(eta);
	  hStripPU->Fill(puEtAtEta.at(eta));
	  hStripPUVsEta->Fill((float)eta,puEtAtEta.at(eta));

	  hComboPU->Fill(comboPU->at(eta));
	  hComboPUVsEta->Fill((float)eta,comboPU->at(eta));
	  
	  hvEstPUVsNVtx.at(eta)->Fill(nVtx, 
				      estPULvl->at(eta));

	  hEstPU->Fill(estPULvl->at(eta));

	  if(eta >= 5 && eta <= 16)
	    hEstPUVsNVtxLoEta->Fill(nVtx, estPULvl->at(eta));
	  else if(eta <= 2 || eta >= 19)
	    hEstPUVsNVtxHiEta->Fill(nVtx, estPULvl->at(eta));
	  else
	    hEstPUVsNVtxMdEta->Fill(nVtx, estPULvl->at(eta));

	  for(unsigned phi = 0; phi < 18; ++phi)
	    {
	      unsigned ind = eta * 18 + phi;

	      float stripPUSubEt = regEt->at(ind) - puEtAtEta.at(eta);
	      hStripPUSubEt->Fill(stripPUSubEt);
	      hStripPUSubEtVsEta->Fill((float)eta,stripPUSubEt);
	      h2StripPUSubEtVsEta->Fill((float)eta,stripPUSubEt);

	      float comboPUSubEt = regEt->at(ind) - comboPU->at(eta);
	      hComboPUSubEt->Fill(comboPUSubEt);
	      hComboPUSubEtVsEta->Fill((float)eta,comboPUSubEt);
	      hComboPUSubEtVsPhi->Fill((float)phi,comboPUSubEt);
	      h2ComboPUSubEtVsEta->Fill((float)eta,comboPUSubEt);

	      hStripPUSubEtVsRegEt->Fill(regEt->at(ind),stripPUSubEt);

	      if(eta >= 5 && eta <= 16)
		{
		  hEstPUSubEtVsNVtxLoEta->Fill(nVtx, 
					       regEt->at(ind) 
					       - estPULvl->at(eta));
		}
	      else if(eta <= 2 || eta >= 19)	  
		{
		  hEstPUSubEtVsNVtxHiEta->Fill(nVtx, 
					       regEt->at(ind) 
					       - estPULvl->at(eta));
		}
	      else
		{				  
		  hEstPUSubEtVsNVtxMdEta->Fill(nVtx, 
					       regEt->at(ind) 
					       - estPULvl->at(eta));
		}

	    }
	}
    }
}



void AnalyzePUEst::plotAll()
{
  string sTAvgPU("tAvgPU");
  TCanvas cTAvgPU = TCanvas(sTAvgPU.c_str(), "Time-Averaged Pileup Plot", 
			    600, 600);
  hTAvgPU->SetFillStyle(1001);
  hTAvgPU->SetFillColor(kBlue);
  hTAvgPU->GetXaxis()->SetTitle("PU(time avg) (GeV)");
  hTAvgPU->GetYaxis()->SetTitle("4x4 Regions x Events");
  hTAvgPU->GetYaxis()->SetTitleOffset(1.8);
  hTAvgPU->GetYaxis()->SetTitleSize(.04);
  hTAvgPU->Draw();
  stringstream ssTAvgPU;
  ssTAvgPU << pathToPlots << sTAvgPU << ".png";
  cTAvgPU.Print(ssTAvgPU.str().c_str());

  string sEstPU("estPU");
  TCanvas cEstPU = TCanvas(sEstPU.c_str(), "Estimated Pileup Plot", 
			    600, 600);
  hEstPU->SetFillStyle(1001);
  hEstPU->SetFillColor(kBlue);
  hEstPU->GetXaxis()->SetTitle("PU(est) (GeV)");
  hEstPU->GetYaxis()->SetTitle("4x4 Regions x Eta Indices");
  hEstPU->GetYaxis()->SetTitleOffset(1.8);
  hEstPU->GetYaxis()->SetTitleSize(.04);
  hEstPU->Draw();
  stringstream ssEstPU;
  ssEstPU << pathToPlots << sEstPU << ".png";
  cEstPU.Print(ssEstPU.str().c_str());

  string sXAvgPU("xAvgPU");
  TCanvas cXAvgPU = TCanvas(sXAvgPU.c_str(), "Space-Averaged Pileup", 
			    600, 600);
  hXAvgPU->SetFillStyle(1001);
  hXAvgPU->SetFillColor(kBlue);
  hXAvgPU->GetXaxis()->SetTitle("PU(region avg) (GeV)");
  hXAvgPU->GetYaxis()->SetTitle("Events");
  hXAvgPU->GetYaxis()->SetTitleOffset(2.);
  hXAvgPU->GetYaxis()->SetTitleSize(.04);
  hXAvgPU->Draw();
  stringstream ssXAvgPU;
  ssXAvgPU << pathToPlots << sXAvgPU << ".png";
  cXAvgPU.Print(ssXAvgPU.str().c_str());


  string sStripPU("phiStripPU");
  TCanvas cStripPU = TCanvas(sStripPU.c_str(), "Phi-Averaged Pileup", 
			    600, 600);
  hStripPU->SetFillStyle(1001);
  hStripPU->SetFillColor(kBlue);
  hStripPU->GetXaxis()->SetTitle("PU(phi strip avg) (GeV)");
  hStripPU->GetYaxis()->SetTitle("Events x Eta Values");
  hStripPU->GetYaxis()->SetTitleOffset(1.9);
  hStripPU->GetYaxis()->SetTitleSize(.04);
  hStripPU->GetYaxis()->SetLabelSize(.04);
  hStripPU->Draw();
  stringstream ssStripPU;
  ssStripPU << pathToPlots << sStripPU << ".png";
  cStripPU.Print(ssStripPU.str().c_str());


  string sStripPUSubEt("phiStripAvgPUSubEt");
  TCanvas cStripPUSubEt = TCanvas(sStripPUSubEt.c_str(), 
				 "Phi-Averaged Pileup-Subtracted Et Plot", 
				 600, 600);
  hStripPUSubEt->SetFillStyle(1001);
  hStripPUSubEt->SetFillColor(kBlue);
  hStripPUSubEt->GetXaxis()->SetTitle("Et-PU(phi strip avg) (GeV)");
  hStripPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hStripPUSubEt->GetYaxis()->SetTitleOffset(1.3);
  hStripPUSubEt->GetYaxis()->SetTitleSize(0.05);
  hStripPUSubEt->Draw();
  stringstream ssStripPUSubEt;
  ssStripPUSubEt << pathToPlots << sStripPUSubEt << ".png";
  cStripPUSubEt.Print(ssStripPUSubEt.str().c_str());


  string sUICPU("uicPU");
  TCanvas cUICPU = TCanvas(sUICPU.c_str(), "UIC Pileup", 
			    600, 600);
  hUICPU->SetFillStyle(1001);
  hUICPU->SetFillColor(kBlue);
  hUICPU->GetXaxis()->SetTitle("PU(space avg) (GeV)");
  hUICPU->GetYaxis()->SetTitle("Events");
  hUICPU->GetYaxis()->SetTitleOffset(2.);
  hUICPU->GetYaxis()->SetTitleSize(.04);
  hUICPU->Draw();
  stringstream ssUICPU;
  ssUICPU << pathToPlots << sUICPU << ".png";
  cUICPU.Print(ssUICPU.str().c_str());


  string sAvgLumi("avgLumi");
  TCanvas cAvgLumi = TCanvas(sAvgLumi.c_str(), "Average Inst. Luminosity Plot",
			     600, 600);
  hAvgLumi->SetFillStyle(1001);
  hAvgLumi->SetFillColor(kBlue);
  hAvgLumi->GetXaxis()->SetTitle("Average Inst. Luminosity");
  hAvgLumi->GetYaxis()->SetTitle("4x4 Regions x Events");
  hAvgLumi->GetYaxis()->SetTitleOffset(1.2);
  hAvgLumi->Draw();
  stringstream ssAvgLumi;
  ssAvgLumi << pathToPlots << sAvgLumi << ".png";
  cAvgLumi.Print(ssAvgLumi.str().c_str());


  string sNVtx("nVtx");
  TCanvas cNVtx = TCanvas(sNVtx.c_str(), "Average # Primary Vertices",
			  600, 600);
  hNVtx->SetFillStyle(1001);
  hNVtx->SetFillColor(kBlue);
  hNVtx->GetXaxis()->SetTitle("Average # Primary Vertices");
  hNVtx->GetYaxis()->SetTitle("Events");
  hNVtx->GetYaxis()->SetTitleOffset(1.2);
  hNVtx->Draw();
  stringstream ssNVtx;
  ssNVtx << pathToPlots << sNVtx << ".png";
  cNVtx.Print(ssNVtx.str().c_str());


  string sLumiStd("lumiStd");
  TCanvas cLumiStd = TCanvas(sLumiStd.c_str(), "Inst. Luminosity Std Dev Plot",
			     600, 600);
  hLumiStd->SetFillStyle(1001);
  hLumiStd->SetFillColor(kBlue);
  hLumiStd->GetXaxis()->SetTitle("Std Dev Inst. Luminosity");
  hLumiStd->GetYaxis()->SetTitle("4x4 Regions x Events");
  hLumiStd->Draw();
  stringstream ssLumiStd;
  ssLumiStd << pathToPlots << sLumiStd << ".png";
  cLumiStd.Print(ssLumiStd.str().c_str());


  string sTAvgPUSubEt("timeAvgPUSubEt");
  TCanvas cTAvgPUSubEt = TCanvas(sTAvgPUSubEt.c_str(), 
				 "Time-Averaged Pileup-Subtracted Et Plot", 
				 600, 600);
  hTAvgPUSubEt->SetFillStyle(1001);
  hTAvgPUSubEt->SetFillColor(kBlue);
  hTAvgPUSubEt->GetXaxis()->SetTitle("Et-PU(time avg) (GeV)");
  hTAvgPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hTAvgPUSubEt->GetYaxis()->SetTitleOffset(1.2);
  hTAvgPUSubEt->Draw();
  stringstream ssTAvgPUSubEt;
  ssTAvgPUSubEt << pathToPlots << sTAvgPUSubEt << ".png";
  cTAvgPUSubEt.Print(ssTAvgPUSubEt.str().c_str());


  string sEstPUSubEt("EstPUSubEt");
  TCanvas cEstPUSubEt = TCanvas(sEstPUSubEt.c_str(), 
				"Estimated Pileup-Subtracted Et Plot", 
				600, 600);
  hEstPUSubEt->SetFillStyle(1001);
  hEstPUSubEt->SetFillColor(kBlue);
  hEstPUSubEt->GetXaxis()->SetTitle("Et-PU(est) (GeV)");
  hEstPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hEstPUSubEt->GetYaxis()->SetTitleOffset(1.2);
  hEstPUSubEt->Draw();
  stringstream ssEstPUSubEt;
  ssEstPUSubEt << pathToPlots << sEstPUSubEt << ".png";
  cEstPUSubEt.Print(ssEstPUSubEt.str().c_str());


  string sXAvgPUSubEt("spaceAvgPUSubEt");
  TCanvas cXAvgPUSubEt = TCanvas(sXAvgPUSubEt.c_str(), 
				 "Space-Averaged Pileup-Subtracted Et Plot", 
				 600, 600);
  hXAvgPUSubEt->SetFillStyle(1001);
  hXAvgPUSubEt->SetFillColor(kBlue);
  hXAvgPUSubEt->GetXaxis()->SetTitle("Et-PU(region avg) (GeV)");
  hXAvgPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hXAvgPUSubEt->GetYaxis()->SetTitleSize(.04);
  hXAvgPUSubEt->GetYaxis()->SetTitleOffset(1.8);
  hXAvgPUSubEt->Draw();
  stringstream ssXAvgPUSubEt;
  ssXAvgPUSubEt << pathToPlots << sXAvgPUSubEt << ".png";
  cXAvgPUSubEt.Print(ssXAvgPUSubEt.str().c_str());


  string sUICPUSubEt("uicPUSubEt");
  TCanvas cUICPUSubEt = TCanvas(sUICPUSubEt.c_str(), 
				 "UIC Pileup-Subtracted Et Plot", 
				 600, 600);
  hUICPUSubEt->SetFillStyle(1001);
  hUICPUSubEt->SetFillColor(kBlue);
  hUICPUSubEt->GetXaxis()->SetTitle("Et-PU(space avg) (GeV)");
  hUICPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hUICPUSubEt->GetYaxis()->SetTitleOffset(1.2);
  hUICPUSubEt->Draw();
  stringstream ssUICPUSubEt;
  ssUICPUSubEt << pathToPlots << sUICPUSubEt << ".png";
  cUICPUSubEt.Print(ssUICPUSubEt.str().c_str());


  string sRegEt("regionEt");
  TCanvas cRegEt = TCanvas(sRegEt.c_str(), "4x4 Region Et Plot", 600, 600);
  hRegEt->SetFillStyle(1001);
  hRegEt->SetFillColor(kBlue);
  hRegEt->GetXaxis()->SetTitle("Et (GeV)");
  hRegEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hRegEt->Draw();
  stringstream ssRegEt;
  ssRegEt << pathToPlots << sRegEt << ".png";
  cRegEt.Print(ssRegEt.str().c_str());


  string sComboPU("comboPU");
  TCanvas cComboPU = TCanvas(sComboPU.c_str(), "Combination Pileup", 
			    600, 600);
  hComboPU->SetFillStyle(1001);
  hComboPU->SetFillColor(kBlue);
  hComboPU->GetXaxis()->SetTitle("PU(Combination) (GeV)");
  hComboPU->GetYaxis()->SetTitle("Events");
  hComboPU->GetYaxis()->SetTitleOffset(2.);
  hComboPU->GetYaxis()->SetTitleSize(.04);
  hComboPU->Draw();
  stringstream ssComboPU;
  ssComboPU << pathToPlots << sComboPU << ".png";
  cComboPU.Print(ssComboPU.str().c_str());


  string sComboPUSubEt("comboPUSubEt");
  TCanvas cComboPUSubEt = TCanvas(sComboPUSubEt.c_str(), 
				 "Combination Pileup-Subtracted Et Plot", 
				 600, 600);
  hComboPUSubEt->SetFillStyle(1001);
  hComboPUSubEt->SetFillColor(kBlue);
  hComboPUSubEt->GetXaxis()->SetTitle("Et-PU(combination) (GeV)");
  hComboPUSubEt->GetYaxis()->SetTitle("4x4 Regions x Events");
  hComboPUSubEt->GetYaxis()->SetTitleOffset(1.2);
  hComboPUSubEt->Draw();
  stringstream ssComboPUSubEt;
  ssComboPUSubEt << pathToPlots << sComboPUSubEt << ".png";
  cComboPUSubEt.Print(ssComboPUSubEt.str().c_str());


  string sTAvgPUVsAvgLumi("tAvgPUVsAvgLumi");
  TCanvas cTAvgPUVsAvgLumi = TCanvas(sTAvgPUVsAvgLumi.c_str(), 
				     "Plot of Average Inst. Luminosity vs Time-Averaged Pileup", 
				     800, 600);
  hTAvgPUVsAvgLumi->GetXaxis()->SetTitle("Average Inst. Luminosity");
  hTAvgPUVsAvgLumi->GetYaxis()->SetTitle("Time Avg Pileup (GeV)");
  hTAvgPUVsAvgLumi->Draw("colz");
  stringstream ssTAvgPUVsAvgLumi;
  ssTAvgPUVsAvgLumi << pathToPlots << sTAvgPUVsAvgLumi << ".png";
  cTAvgPUVsAvgLumi.Print(ssTAvgPUVsAvgLumi.str().c_str());


  string sTAvgPUVsAvgLumiAllEta("tAvgPUVsAvgLumiAllEta");
  TCanvas cTAvgPUVsAvgLumiAllEta = TCanvas(sTAvgPUVsAvgLumiAllEta.c_str(), 
       "Plot of Average Inst. Luminosity vs Time-Averaged Pileup (all Eta)", 
					  800, 600);

  hTAvgPUVsAvgLumiMdEta->GetXaxis()->SetTitle("Average Inst. Luminosity");
  hTAvgPUVsAvgLumiMdEta->GetYaxis()->SetTitle("Time Avg Pileup (GeV)");
  hTAvgPUVsAvgLumiMdEta->SetTitle("Time-Avg Pileup vs. Avg Luminosity");
  hTAvgPUVsAvgLumiMdEta->SetMarkerColor(kRed);
  hTAvgPUVsAvgLumiMdEta->SetLineColor(kRed);
  hTAvgPUVsAvgLumiMdEta->Draw();

  hTAvgPUVsAvgLumiHiEta->GetXaxis()->SetTitle("Average Inst. Luminosity");
  hTAvgPUVsAvgLumiHiEta->GetYaxis()->SetTitle("Time Avg Pileup (GeV)");
  hTAvgPUVsAvgLumiHiEta->SetMarkerColor(kGreen);
  hTAvgPUVsAvgLumiHiEta->SetLineColor(kGreen);
  hTAvgPUVsAvgLumiHiEta->Draw("same");

  hTAvgPUVsAvgLumiLoEta->GetXaxis()->SetTitle("Average Inst. Luminosity");
  hTAvgPUVsAvgLumiLoEta->GetYaxis()->SetTitle("Time Avg Pileup (GeV)");
  hTAvgPUVsAvgLumiLoEta->SetMarkerColor(kBlue);
  hTAvgPUVsAvgLumiLoEta->SetLineColor(kBlue);
  hTAvgPUVsAvgLumiLoEta->Draw("same");

  TLegend* leg = new TLegend(.2, .65, .45, .85);
  leg->AddEntry(hTAvgPUVsAvgLumiLoEta, "4<|eta|<17");
  leg->AddEntry(hTAvgPUVsAvgLumiMdEta, "2<|eta|<5 & 16<|eta|<19");
  leg->AddEntry(hTAvgPUVsAvgLumiHiEta, "|eta|<3 & |eta|>18");
  leg->SetFillColor(kWhite);
  leg->Draw("same");

  stringstream ssTAvgPUVsAvgLumiAllEta;
  ssTAvgPUVsAvgLumiAllEta << pathToPlots << sTAvgPUVsAvgLumiAllEta << ".png";
  cTAvgPUVsAvgLumiAllEta.Print(ssTAvgPUVsAvgLumiAllEta.str().c_str());


  string sTAvgPUVsNVtxAllEta("tAvgPUVsNVtxAllEta");
  TCanvas cTAvgPUVsNVtxAllEta = TCanvas(sTAvgPUVsNVtxAllEta.c_str(), 
       "Plot of Average # Primary Vertices vs Time-Averaged Pileup (all Eta)", 
					  800, 600);

  hTAvgPUVsNVtxMdEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUVsNVtxMdEta->GetYaxis()->SetTitle("Time Avg Pileup Level(GeV)");
  hTAvgPUVsNVtxMdEta->SetTitle("Time-Avg Pileup vs. Avg # Primary Vertices");
  hTAvgPUVsNVtxMdEta->SetMarkerColor(kRed);
  hTAvgPUVsNVtxMdEta->SetLineColor(kRed);
  hTAvgPUVsNVtxMdEta->Draw();

  hTAvgPUVsNVtxHiEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUVsNVtxHiEta->GetYaxis()->SetTitle("Time Avg Pileup Level (GeV)");
  hTAvgPUVsNVtxHiEta->SetMarkerColor(kGreen);
  hTAvgPUVsNVtxHiEta->SetLineColor(kGreen);
  hTAvgPUVsNVtxHiEta->Draw("same");

  hTAvgPUVsNVtxLoEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUVsNVtxLoEta->GetYaxis()->SetTitle("Time Avg Pileup Level (GeV)");
  hTAvgPUVsNVtxLoEta->SetMarkerColor(kBlue);
  hTAvgPUVsNVtxLoEta->SetLineColor(kBlue);
  hTAvgPUVsNVtxLoEta->Draw("same");

  TLegend* leg2 = new TLegend(.2, .65, .45, .85);
  leg2->AddEntry(hTAvgPUVsNVtxLoEta, "4<|eta|<17");
  leg2->AddEntry(hTAvgPUVsNVtxMdEta, "2<|eta|<5 & 16<|eta|<19");
  leg2->AddEntry(hTAvgPUVsNVtxHiEta, "|eta|<3 & |eta|>18");
  leg2->SetFillColor(kWhite);
  leg2->Draw("same");

  stringstream ssTAvgPUVsNVtxAllEta;
  ssTAvgPUVsNVtxAllEta << pathToPlots << sTAvgPUVsNVtxAllEta << ".png";
  cTAvgPUVsNVtxAllEta.Print(ssTAvgPUVsNVtxAllEta.str().c_str());


  string sEstPUVsNVtxAllEta("estPUVsNVtxAllEta");
  TCanvas cEstPUVsNVtxAllEta = TCanvas(sEstPUVsNVtxAllEta.c_str(), 
       "Plot of Average # Primary Vertices vs Estimated Pileup (all Eta)", 
					  800, 600);

  hEstPUVsNVtxMdEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUVsNVtxMdEta->GetYaxis()->SetTitle("Estimated Pileup Level(GeV)");
  hEstPUVsNVtxMdEta->SetTitle("Estimated Pileup vs. Avg # Primary Vertices");
  hEstPUVsNVtxMdEta->SetMarkerColor(kRed);
  hEstPUVsNVtxMdEta->SetLineColor(kRed);
  hEstPUVsNVtxMdEta->Draw();

  hEstPUVsNVtxHiEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUVsNVtxHiEta->GetYaxis()->SetTitle("Estimated Pileup Level (GeV)");
  hEstPUVsNVtxHiEta->SetMarkerColor(kGreen);
  hEstPUVsNVtxHiEta->SetLineColor(kGreen);
  hEstPUVsNVtxHiEta->Draw("same");

  hEstPUVsNVtxLoEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUVsNVtxLoEta->GetYaxis()->SetTitle("Estimated Pileup Level (GeV)");
  hEstPUVsNVtxLoEta->SetMarkerColor(kBlue);
  hEstPUVsNVtxLoEta->SetLineColor(kBlue);
  hEstPUVsNVtxLoEta->Draw("same");

  TLegend* leg3 = new TLegend(.2, .65, .45, .85);
  leg3->AddEntry(hEstPUVsNVtxLoEta, "4<|eta|<17");
  leg3->AddEntry(hEstPUVsNVtxMdEta, "2<|eta|<5 & 16<|eta|<19");
  leg3->AddEntry(hEstPUVsNVtxHiEta, "|eta|<3 & |eta|>18");
  leg3->SetFillColor(kWhite);
  leg3->Draw("same");

  stringstream ssEstPUVsNVtxAllEta;
  ssEstPUVsNVtxAllEta << pathToPlots << sEstPUVsNVtxAllEta << ".png";
  cEstPUVsNVtxAllEta.Print(ssEstPUVsNVtxAllEta.str().c_str());


  string sTAvgPUSubEtVsNVtxAllEta("tAvgPUSubEtVsNVtxAllEta");
  TCanvas cTAvgPUSubEtVsNVtxAllEta = TCanvas(sTAvgPUSubEtVsNVtxAllEta.c_str(), 
       "Plot of Average # Primary Vertices vs Et - PU(time avg) (all Eta)", 
					  800, 600);

  hTAvgPUSubEtVsNVtxMdEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUSubEtVsNVtxMdEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level(GeV)");
  hTAvgPUSubEtVsNVtxMdEta->SetTitle("Et - PU(time avg) vs. # Primary Vertices");
  hTAvgPUSubEtVsNVtxMdEta->SetMarkerColor(kRed);
  hTAvgPUSubEtVsNVtxMdEta->SetLineColor(kRed);
  //  hTAvgPUSubEtVsNVtxMdEta->SetErrorOption("s");
  hTAvgPUSubEtVsNVtxMdEta->Draw();

  hTAvgPUSubEtVsNVtxHiEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUSubEtVsNVtxHiEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level (GeV)");
  hTAvgPUSubEtVsNVtxHiEta->SetMarkerColor(kGreen);
  hTAvgPUSubEtVsNVtxHiEta->SetLineColor(kGreen);
  //  hTAvgPUSubEtVsNVtxHiEta->SetErrorOption("s");
  hTAvgPUSubEtVsNVtxHiEta->Draw("same");

  hTAvgPUSubEtVsNVtxLoEta->GetXaxis()->SetTitle("Average nPVs");
  hTAvgPUSubEtVsNVtxLoEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level (GeV)");
  hTAvgPUSubEtVsNVtxLoEta->SetMarkerColor(kBlue);
  hTAvgPUSubEtVsNVtxLoEta->SetLineColor(kBlue);
  //  hTAvgPUSubEtVsNVtxLoEta->SetErrorOption("s");
  hTAvgPUSubEtVsNVtxLoEta->Draw("same");

  TLegend* leg4 = new TLegend(.2, .65, .45, .85);
  leg4->AddEntry(hTAvgPUSubEtVsNVtxLoEta, "4<|eta|<17");
  leg4->AddEntry(hTAvgPUSubEtVsNVtxMdEta, "2<|eta|<5 & 16<|eta|<19");
  leg4->AddEntry(hTAvgPUSubEtVsNVtxHiEta, "|eta|<3 & |eta|>18");
  leg4->SetFillColor(kWhite);
  leg4->Draw("same");

  stringstream ssTAvgPUSubEtVsNVtxAllEta;
  ssTAvgPUSubEtVsNVtxAllEta << pathToPlots << sTAvgPUSubEtVsNVtxAllEta << ".png";
  cTAvgPUSubEtVsNVtxAllEta.Print(ssTAvgPUSubEtVsNVtxAllEta.str().c_str());


  string sEstPUSubEtVsNVtxAllEta("estPUSubEtVsNVtxAllEta");
  TCanvas cEstPUSubEtVsNVtxAllEta = TCanvas(sEstPUSubEtVsNVtxAllEta.c_str(), 
       "Plot of Average # Primary Vertices vs Estimated Pileup (all Eta)", 
					  800, 600);

  hEstPUSubEtVsNVtxMdEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUSubEtVsNVtxMdEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level(GeV)");
  hEstPUSubEtVsNVtxMdEta->SetTitle("Et - PU(est) vs. # Primary Vertices");
  hEstPUSubEtVsNVtxMdEta->SetMarkerColor(kRed);
  hEstPUSubEtVsNVtxMdEta->SetLineColor(kRed);
  //  hEstPUSubEtVsNVtxMdEta->SetErrorOption("s");
  hEstPUSubEtVsNVtxMdEta->Draw();

  hEstPUSubEtVsNVtxHiEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUSubEtVsNVtxHiEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level (GeV)");
  hEstPUSubEtVsNVtxHiEta->SetMarkerColor(kGreen);
  hEstPUSubEtVsNVtxHiEta->SetLineColor(kGreen);
  //  hEstPUSubEtVsNVtxHiEta->SetErrorOption("s");
  hEstPUSubEtVsNVtxHiEta->Draw("same");

  hEstPUSubEtVsNVtxLoEta->GetXaxis()->SetTitle("Average nPVs");
  hEstPUSubEtVsNVtxLoEta->GetYaxis()->SetTitle("4x4 Region Et - Pileup Level (GeV)");
  hEstPUSubEtVsNVtxLoEta->SetMarkerColor(kBlue);
  hEstPUSubEtVsNVtxLoEta->SetLineColor(kBlue);
  //  hEstPUSubEtVsNVtxLoEta->SetErrorOption("s");
  hEstPUSubEtVsNVtxLoEta->Draw("same");

  TLegend* leg5 = new TLegend(.2, .65, .45, .85);
  leg5->AddEntry(hEstPUSubEtVsNVtxLoEta, "4<|eta|<17");
  leg5->AddEntry(hEstPUSubEtVsNVtxMdEta, "2<|eta|<5 & 16<|eta|<19");
  leg5->AddEntry(hEstPUSubEtVsNVtxHiEta, "|eta|<3 & |eta|>18");
  leg5->SetFillColor(kWhite);
  leg5->Draw("same");

  stringstream ssEstPUSubEtVsNVtxAllEta;
  ssEstPUSubEtVsNVtxAllEta << pathToPlots << sEstPUSubEtVsNVtxAllEta << ".png";
  cEstPUSubEtVsNVtxAllEta.Print(ssEstPUSubEtVsNVtxAllEta.str().c_str());


  string sTAvgPUVsEta("tAvgPUVsEta");
  TCanvas cTAvgPUVsEta = TCanvas(sTAvgPUVsEta.c_str(), 
				 "Plot of Eta vs Time Average Pileup", 
				 800, 600);
  hTAvgPUVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hTAvgPUVsEta->GetYaxis()->SetTitle("Time Avg Pileup (GeV)");
  hTAvgPUVsEta->Draw();
  stringstream ssTAvgPUVsEta;
  ssTAvgPUVsEta << pathToPlots << sTAvgPUVsEta << ".png";
  cTAvgPUVsEta.Print(ssTAvgPUVsEta.str().c_str());


  string sRegEtVsEta("regEtVsEta");
  TCanvas cRegEtVsEta = TCanvas(sRegEtVsEta.c_str(), 
				"Plot of Eta vs Region Et", 
				800, 600);
  hRegEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hRegEtVsEta->GetYaxis()->SetTitle("4x4 Region Et (GeV)");
  hRegEtVsEta->Draw("colz");
  stringstream ssRegEtVsEta;
  ssRegEtVsEta << pathToPlots << sRegEtVsEta << ".png";
  cRegEtVsEta.Print(ssRegEtVsEta.str().c_str());


  string sUICPUVsEta("uicPUVsEta");
  TCanvas cUICPUVsEta = TCanvas(sUICPUVsEta.c_str(), 
				 "Plot of Eta vs Space Average Pileup", 
				 800, 600);
  hUICPUVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hUICPUVsEta->GetYaxis()->SetTitle("Space Avg Pileup (GeV)");
  hUICPUVsEta->Draw();
  stringstream ssUICPUVsEta;
  ssUICPUVsEta << pathToPlots << sUICPUVsEta << ".png";
  cUICPUVsEta.Print(ssUICPUVsEta.str().c_str());


  string sStripPUVsEta("phiStripPUVsEta");
  TCanvas cStripPUVsEta = TCanvas(sStripPUVsEta.c_str(), 
				 "Plot of Eta vs Phi Strip Pileup", 
				 800, 600);
  hStripPUVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hStripPUVsEta->GetYaxis()->SetTitle("Phi Strip Pileup (GeV)");
  hStripPUVsEta->GetYaxis()->SetTitleOffset(0.9);
  hStripPUVsEta->Draw();
  stringstream ssStripPUVsEta;
  ssStripPUVsEta << pathToPlots << sStripPUVsEta << ".png";
  cStripPUVsEta.Print(ssStripPUVsEta.str().c_str());


  string sStripPUSubEtVsEta("phiStripPUSubEtVsEta");
  TCanvas cStripPUSubEtVsEta = TCanvas(sStripPUSubEtVsEta.c_str(), 
		   "Strip Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  hStripPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hStripPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(phi strip) (GeV)");
  hStripPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hStripPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.1);
  hStripPUSubEtVsEta->Draw("E");
  stringstream ssStripPUSubEtVsEta;
  ssStripPUSubEtVsEta << pathToPlots << sStripPUSubEtVsEta << ".png";
  cStripPUSubEtVsEta.Print(ssStripPUSubEtVsEta.str().c_str());


  string sTAvgPUVsPhi("tAvgPUVsPhi");
  TCanvas cTAvgPUVsPhi = TCanvas(sTAvgPUVsPhi.c_str(), 
				 "Time Average Pileup Vs Phi Plot", 800, 600);
  hTAvgPUVsPhi->GetXaxis()->SetTitle("GCT Phi Index");
  hTAvgPUVsPhi->GetYaxis()->SetTitle("Time-Avg Pileup");
  hTAvgPUVsPhi->Draw();
  stringstream ssTAvgPUVsPhi;
  ssTAvgPUVsPhi << pathToPlots << sTAvgPUVsPhi << ".png";
  cTAvgPUVsPhi.Print(ssTAvgPUVsPhi.str().c_str());


  string sTAvgPUSubEtVsEta("tAvgPUSubEtVsEta");
  TCanvas cTAvgPUSubEtVsEta = TCanvas(sTAvgPUSubEtVsEta.c_str(), 
		    "Time Average Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  hTAvgPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hTAvgPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(time avg) (GeV)");
  hTAvgPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hTAvgPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.4);
  hTAvgPUSubEtVsEta->Draw("E");
  stringstream ssTAvgPUSubEtVsEta;
  ssTAvgPUSubEtVsEta << pathToPlots << sTAvgPUSubEtVsEta << ".png";
  cTAvgPUSubEtVsEta.Print(ssTAvgPUSubEtVsEta.str().c_str());


  string sEstPUSubEtVsEta("estPUSubEtVsEta");
  TCanvas cEstPUSubEtVsEta = TCanvas(sEstPUSubEtVsEta.c_str(), 
		    "Estimated Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  hEstPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hEstPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(est) (GeV)");
  hEstPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hEstPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.4);
  hEstPUSubEtVsEta->Draw("E");
  stringstream ssEstPUSubEtVsEta;
  ssEstPUSubEtVsEta << pathToPlots << sEstPUSubEtVsEta << ".png";
  cEstPUSubEtVsEta.Print(ssEstPUSubEtVsEta.str().c_str());


  string sXAvgPUSubEtVsEta("xAvgPUSubEtVsEta");
  TCanvas cXAvgPUSubEtVsEta = TCanvas(sXAvgPUSubEtVsEta.c_str(), 
		   "Region Average Pileup-Subtracted Et Vs Eta Plot", 
				      800, 600);
  hXAvgPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hXAvgPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(region avg) (GeV)");
  hXAvgPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hXAvgPUSubEtVsEta->Draw("E");
  stringstream ssXAvgPUSubEtVsEta;
  ssXAvgPUSubEtVsEta << pathToPlots << sXAvgPUSubEtVsEta << ".png";
  cXAvgPUSubEtVsEta.Print(ssXAvgPUSubEtVsEta.str().c_str());


  string sUICPUSubEtVsEta("uicPUSubEtVsEta");
  TCanvas cUICPUSubEtVsEta = TCanvas(sUICPUSubEtVsEta.c_str(), 
		   "Space Average Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  hUICPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hUICPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(space avg) (GeV)");
  hUICPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hUICPUSubEtVsEta->Draw("E");
  stringstream ssUICPUSubEtVsEta;
  ssUICPUSubEtVsEta << pathToPlots << sUICPUSubEtVsEta << ".png";
  cUICPUSubEtVsEta.Print(ssUICPUSubEtVsEta.str().c_str());


  string s2TAvgPUSubEtVsEta("tAvgPUSubEtVsEtaColZ");
  TCanvas c2TAvgPUSubEtVsEta = TCanvas(s2TAvgPUSubEtVsEta.c_str(), 
		    "Time Average Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  h2TAvgPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  h2TAvgPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(time-avg) (GeV)");
  h2TAvgPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  h2TAvgPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.4);
  h2TAvgPUSubEtVsEta->Draw("colz");
  stringstream ss2TAvgPUSubEtVsEta;
  ss2TAvgPUSubEtVsEta << pathToPlots << s2TAvgPUSubEtVsEta << ".png";
  c2TAvgPUSubEtVsEta.Print(ss2TAvgPUSubEtVsEta.str().c_str());


  string s2StripPUSubEtVsEta("stripPUSubEtVsEtaColZ");
  TCanvas c2StripPUSubEtVsEta = TCanvas(s2StripPUSubEtVsEta.c_str(), 
	       "Phi Strip Average Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  h2StripPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  h2StripPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(phi strip avg) (GeV)");
  h2StripPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  h2StripPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.4);
  h2StripPUSubEtVsEta->Draw("colz");
  stringstream ss2StripPUSubEtVsEta;
  ss2StripPUSubEtVsEta << pathToPlots << s2StripPUSubEtVsEta << ".png";
  c2StripPUSubEtVsEta.Print(ss2StripPUSubEtVsEta.str().c_str());


  string s2ComboPUSubEtVsEta("comboPUSubEtVsEtaColZ");
  TCanvas c2ComboPUSubEtVsEta = TCanvas(s2ComboPUSubEtVsEta.c_str(), 
	       "Combination Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  h2ComboPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  h2ComboPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(combination) (GeV)");
  h2ComboPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  h2ComboPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.4);
  h2ComboPUSubEtVsEta->Draw("colz");
  stringstream ss2ComboPUSubEtVsEta;
  ss2ComboPUSubEtVsEta << pathToPlots << s2ComboPUSubEtVsEta << ".png";
  c2ComboPUSubEtVsEta.Print(ss2ComboPUSubEtVsEta.str().c_str());


  string sComboPUVsEta("comboPUVsEta");
  TCanvas cComboPUVsEta = TCanvas(sComboPUVsEta.c_str(), 
				 "Plot of Eta vs Combination Pileup", 
				 800, 600);
  hComboPUVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hComboPUVsEta->GetYaxis()->SetTitle("Combination Pileup (GeV)");
  hComboPUVsEta->GetYaxis()->SetTitleOffset(0.9);
  hComboPUVsEta->Draw();
  stringstream ssComboPUVsEta;
  ssComboPUVsEta << pathToPlots << sComboPUVsEta << ".png";
  cComboPUVsEta.Print(ssComboPUVsEta.str().c_str());


  string sComboPUSubEtVsEta("comboPUSubEtVsEta");
  TCanvas cComboPUSubEtVsEta = TCanvas(sComboPUSubEtVsEta.c_str(), 
		   "Combination Pileup-Subtracted Et Vs Eta Plot", 800, 600);
  hComboPUSubEtVsEta->GetXaxis()->SetTitle("GCT Eta Index");
  hComboPUSubEtVsEta->GetYaxis()->SetTitle("4x4 Region Et - PU(combination) (GeV)");
  hComboPUSubEtVsEta->GetYaxis()->SetTitleSize(0.04);
  hComboPUSubEtVsEta->GetYaxis()->SetTitleOffset(1.1);
  hComboPUSubEtVsEta->Draw("E");
  stringstream ssComboPUSubEtVsEta;
  ssComboPUSubEtVsEta << pathToPlots << sComboPUSubEtVsEta << ".png";
  cComboPUSubEtVsEta.Print(ssComboPUSubEtVsEta.str().c_str());


  string sComboPUSubEtVsPhi("comboPUSubEtVsPhi");
  TCanvas cComboPUSubEtVsPhi = TCanvas(sComboPUSubEtVsPhi.c_str(), 
		   "Combination Pileup-Subtracted Et Vs Phi Plot", 800, 600);
  hComboPUSubEtVsPhi->GetXaxis()->SetTitle("GCT Phi Index");
  hComboPUSubEtVsPhi->GetYaxis()->SetTitle("4x4 Region Et - PU(combination) (GeV)");
  hComboPUSubEtVsPhi->GetYaxis()->SetTitleSize(0.04);
  hComboPUSubEtVsPhi->GetYaxis()->SetTitleOffset(1.1);
  hComboPUSubEtVsPhi->Draw("E");
  stringstream ssComboPUSubEtVsPhi;
  ssComboPUSubEtVsPhi << pathToPlots << sComboPUSubEtVsPhi << ".png";
  cComboPUSubEtVsPhi.Print(ssComboPUSubEtVsPhi.str().c_str());


  string sTAvgPUSubEtVsRegEt("tAvgPUSubEtVsRegEt");
  TCanvas cTAvgPUSubEtVsRegEt = TCanvas(sTAvgPUSubEtVsRegEt.c_str(), 
		    "Time Average Pileup-Subtracted Et Vs 4x4 Region Et Plot", 
					800, 600);
  hTAvgPUSubEtVsRegEt->GetXaxis()->SetTitle("4x4 Region Et");
  hTAvgPUSubEtVsRegEt->GetYaxis()->SetTitle("4x4 Region Et - PU(time avg) (GeV)");
  hTAvgPUSubEtVsRegEt->GetYaxis()->SetTitleSize(0.04);
  hTAvgPUSubEtVsRegEt->GetYaxis()->SetTitleOffset(1.4);
  hTAvgPUSubEtVsRegEt->Draw("E");
  stringstream ssTAvgPUSubEtVsRegEt;
  ssTAvgPUSubEtVsRegEt << pathToPlots << sTAvgPUSubEtVsRegEt << ".png";
  cTAvgPUSubEtVsRegEt.Print(ssTAvgPUSubEtVsRegEt.str().c_str());


  string sStripPUSubEtVsRegEt("stripPUSubEtVsRegEt");
  TCanvas cStripPUSubEtVsRegEt = TCanvas(sStripPUSubEtVsRegEt.c_str(), 
	       "Phi Strip Average Pileup-Subtracted Et Vs 4x4 Region Et Plot", 
					  800, 600);
  hStripPUSubEtVsRegEt->GetXaxis()->SetTitle("4x4 Region Et");
  hStripPUSubEtVsRegEt->GetYaxis()->SetTitle("4x4 Region Et - PU(phi strip avg) (GeV)");
  hStripPUSubEtVsRegEt->GetYaxis()->SetTitleSize(0.04);
  hStripPUSubEtVsRegEt->GetYaxis()->SetTitleOffset(1.4);
  hStripPUSubEtVsRegEt->Draw("E");
  stringstream ssStripPUSubEtVsRegEt;
  ssStripPUSubEtVsRegEt << pathToPlots << sStripPUSubEtVsRegEt << ".png";
  cStripPUSubEtVsRegEt.Print(ssStripPUSubEtVsRegEt.str().c_str());


}


// void AnalyzePUEst::fitPUToLumi()
// {
//   vector<float> coeffHiA;
//   vector<float> coeffHiB;
//   vector<float> coeffLoA;
//   vector<float> coeffLoB;

// //   cout << "Fits: <PU Level> = a+b*Lumi" << endl;

//   for(unsigned eta = 0; eta < hvTAvgPUVsAvgLumi.size(); ++eta)
//     {
// //       TF1* fLoExp = new TF1("fLoExp", "expo", 400., 2000.);
// //       TF1* fHiExp = new TF1("fHiExp", "expo", 2500., 3400.);
//       TF1* fLoLin = new TF1("fLoLin", "pol1", 400., 2000.);
//       TF1* fHiLin = new TF1("fHiLin", "pol1", 2500., 3400.);

//       hvTAvgPUVsAvgLumi.at(eta)->Fit("fLoLin","QR");
//       hvTAvgPUVsAvgLumi.at(eta)->Fit("fHiLin","QR");

//       coeffLoA.push_back(fLoLin->GetParameter(0));
//       coeffLoB.push_back(fLoLin->GetParameter(1));
//       coeffHiA.push_back(fHiLin->GetParameter(0));
//       coeffHiB.push_back(fHiLin->GetParameter(1));

// //       hvTAvgPUVsAvgLumi.at(eta)->Fit("fLoExp","QR");
// //       hvTAvgPUVsAvgLumi.at(eta)->Fit("fHiExp","QR");

// //       cout << eta << " Low: a="
// // 	   << fLoLin->GetParameter(0) << " b=" 
// // 	   << fLoLin->GetParameter(1) << " X^2=" 
// // 	   << fLoLin->GetChisquare() << " " 
// // 	   << endl;

// //       cout << eta << " High: a="
// // 	   << fHiLin->GetParameter(0) << " b=" 
// // 	   << fHiLin->GetParameter(1) << " X^2="
// // 	   << fHiLin->GetChisquare() 
// // 	   << endl;
//     }

//   cout << endl << "Low A:" << endl;
//   for(unsigned p = 0; p < 22; ++p)
//     cout << coeffLoA.at(p) << endl;

//   cout << "Low B:" << endl;
//   for(unsigned p = 0; p < 22; ++p)
//     cout << coeffLoB.at(p) << endl;

//   cout << "High A:" << endl;
//   for(unsigned p = 0; p < 22; ++p)
//     cout << coeffHiA.at(p) << endl;

//   cout << "High B:" << endl;
//   for(unsigned p = 0; p < 22; ++p)
//     cout << coeffHiB.at(p) << endl;
// }


void AnalyzePUEst::fitPUToNVtx()
{
  vector<float> avgCoeffA;
  vector<float> avgCoeffB;
  vector<float> varCoeffA;
  vector<float> varCoeffB;

  for(unsigned eta = 0; eta < hvTAvgPUVsNVtx.size(); ++eta)
    {
      // Fit mean of PU algorithm result
      TF1* fLin = new TF1("fLin", "pol1", 8., 35.);

      hvTAvgPUVsNVtx.at(eta)->Fit("fLin","QR");

      avgCoeffA.push_back(fLin->GetParameter(0));
      avgCoeffB.push_back(fLin->GetParameter(1));

      // Fit variance of PU algo result
      int nBins = hvTAvgPUVsNVtx.at(eta)->GetNbinsX();
      vector<double> ecks;
      vector<double> wye;
      hvTAvgPUVsNVtx.at(eta)->SetErrorOption("s");
      for(int bin = 1; bin <= nBins; ++bin)
	{
	  // very small numbers are probably 0
	  if(hvTAvgPUVsNVtx.at(eta)->GetBinError(bin) > 0.0000001)
	    {
	      ecks.push_back(hvTAvgPUVsNVtx.at(eta)->GetBinCenter(bin));
	      wye.push_back(hvTAvgPUVsNVtx.at(eta)->GetBinError(bin));
	    }
	}

      TVectorD ex(ecks.size(),&ecks[0]);
      TVectorD wy(wye.size(),&wye[0]);

      var.push_back(new TGraph(ex, wy));

      TF1* fLin2 = new TF1("fLin2", "pol1", 8., 40.);
      var.at(eta)->Fit("fLin2", "QR");

      varCoeffA.push_back(fLin2->GetParameter(0));
      varCoeffB.push_back(fLin2->GetParameter(1));

      delete fLin;
      delete fLin2;
    }


  cout << "Mean A:" << endl;
  for(unsigned p = 0; p < 22; ++p)
    cout << avgCoeffA.at(p) << endl;

  cout << "Mean B:" << endl;
  for(unsigned p = 0; p < 22; ++p)
    cout << avgCoeffB.at(p) << endl;

  cout << "Var A:" << endl;
  for(unsigned p = 0; p < 22; ++p)
    cout << varCoeffA.at(p) << endl;

  cout << "Var B:" << endl;
  for(unsigned p = 0; p < 22; ++p)
    cout << varCoeffB.at(p) << endl;

}


void AnalyzePUEst::getVarGraph(TProfile* PU, TGraph& gr)
{
  int nBins = PU->GetNbinsX();
  vector<double> ecks;
  vector<double> wye;
  PU->SetErrorOption("s");
  for(int bin = 1; bin <= nBins; ++bin)
    {
      // very small numbers are probably 0
      if(PU->GetBinError(bin) > 0.0000001)
	{
	  ecks.push_back(PU->GetBinCenter(bin));
	  wye.push_back(PU->GetBinError(bin));
	}
    }

  TVectorD ex(ecks.size(),&ecks[0]);
  TVectorD wy(wye.size(),&wye[0]);

  gr = TGraph(ex,wy);
}


void AnalyzePUEst::saveParams()
{
  TFile f("TAvgPUParams_2012D_v2.root", "new");

  for(unsigned i = 0; i < hvTAvgPUVsNVtx.size(); ++i)
    {
      hvTAvgPUVsNVtx.at(i)->Write();
    }

  f.Close();
}
