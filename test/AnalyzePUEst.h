//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 17 15:31:48 2013 by ROOT version 5.32/00
// from TTree Ntuple/Ntuple
// found on file: pu_tree_numEvent555.root
//////////////////////////////////////////////////////////

#ifndef AnalyzePUEst_h
#define AnalyzePUEst_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TF1.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <TGraph.h>
#include <TVector.h>
#include <TLine.h>

using namespace std;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class AnalyzePUEst {
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  vector<float>   *tAvgPU;
  Float_t         xAvgPU;
  vector<float>   *lumiAvg;
  UInt_t          run;
  UInt_t          lumi;
  ULong64_t       evt;
  Float_t         instlumi;
  vector<float>   *tAvgPUSubEt;
  vector<float>   *xAvgPUSubEt;
  vector<float>   *regEt;
  vector<float>   *lumiStd;
  vector<float>   *uicPU;
  vector<float>   *uicPUSubEt;
  vector<float>   *comboPU;
  vector<float>   *estPULvl;
  float           nVtx;
  unsigned        nBx;

  // List of branches
  TBranch        *b_tAvgPU;   //!
  TBranch        *b_xAvgPU;   //!
  TBranch        *b_lumiAvg;   //!
  TBranch        *b_run;   //!
  TBranch        *b_lumi;   //!
  TBranch        *b_evt;   //!
  TBranch        *b_instlumi;   //!
  TBranch        *b_tAvgPUSubEt;   //!
  TBranch        *b_xAvgPUSubEt;   //!
  TBranch        *b_regEt;   //!
  TBranch        *b_lumiStd;   //!
  TBranch        *b_uicPU;   //!
  TBranch        *b_uicPUSubEt;   //!
  TBranch        *b_comboPU;
  TBranch        *b_nVtx;
  TBranch        *b_estPULvl;
  TBranch        *b_nBx;

  AnalyzePUEst(TTree *tree=0);
  virtual ~AnalyzePUEst();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  void plotAll();
  //  void fitPUToLumi();
  void fitPUToNVtx();
  void getVarGraph(TProfile* PU, TGraph& gr);
  void saveParams();

  TH1F* hTAvgPU;
  TH1F* hXAvgPU;
  TH1F* hUICPU;
  TH1F* hAvgLumi;
  TH1F* hTAvgPUSubEt;
  TH1F* hXAvgPUSubEt;
  TH1F* hUICPUSubEt;
  TH1F* hRegEt;
  TH1F* hLumiStd;
  TH1F* hStripPU;
  TH1F* hStripPUSubEt;
  TH1F* hComboPU;
  TH1F* hComboPUSubEt;
  TH1F* hNVtx;
  TH1F* hEstPU;
  TH1F* hEstPUSubEt;

  TH2F* hTAvgPUVsAvgLumi;
  TH2F* h2TAvgPUVsAvgLumiLoEta;
  TH2F* h2TAvgPUVsAvgLumiMdEta;
  TH2F* h2TAvgPUVsAvgLumiHiEta;
  TH2F* h2TAvgPUSubEtVsEta;
  TH2F* h2StripPUSubEtVsEta;
  TH2F* hRegEtVsEta;
  TH2F* h2ComboPUSubEtVsEta;
/*   TH2F* hTAvgPUVsEta;	    */
/*   TH2F* hTAvgPUVsPhi;	    */
/*   TH2F* hTAvgPUSubEtVsEta; */
/*   TH2F* hXAvgPUSubEtVsEta; */

  TProfile* hTAvgPUVsAvgLumiLoEta;
  TProfile* hTAvgPUVsAvgLumiMdEta;
  TProfile* hTAvgPUVsAvgLumiHiEta;
  TProfile* hTAvgPUVsEta;
  TProfile* hEstPUVsEta;
  TProfile* hTAvgPUVsPhi;
  TProfile* hUICPUVsEta;
  TProfile* hTAvgPUSubEtVsEta;
  TProfile* hXAvgPUSubEtVsEta;
  TProfile* hUICPUSubEtVsEta;
  TProfile* hEstPUSubEtVsEta;
  TProfile* hStripPUVsEta;
  TProfile* hStripPUSubEtVsEta;
  TProfile* hComboPUVsEta;
  TProfile* hComboPUSubEtVsEta;
  TProfile* hComboPUSubEtVsPhi;
  TProfile* hTAvgPUSubEtVsRegEt;
  TProfile* hStripPUSubEtVsRegEt;
  TProfile* hTAvgPUVsNVtxLoEta;
  TProfile* hTAvgPUVsNVtxMdEta;
  TProfile* hTAvgPUVsNVtxHiEta;
  TProfile* hEstPUVsNVtxLoEta;
  TProfile* hEstPUVsNVtxMdEta;
  TProfile* hEstPUVsNVtxHiEta;
  TProfile* hTAvgPUVsNBx;
  TProfile* hTAvgPUSubEtVsNBx;
  TProfile* hXAvgPUVsNBx;

  TProfile* hEstPUSubEtVsNVtxLoEta;
  TProfile* hEstPUSubEtVsNVtxMdEta;
  TProfile* hEstPUSubEtVsNVtxHiEta;
  TProfile* hTAvgPUSubEtVsNVtxLoEta;
  TProfile* hTAvgPUSubEtVsNVtxMdEta;
  TProfile* hTAvgPUSubEtVsNVtxHiEta;
  vector<TProfile*> hvTAvgPUSubEtVsNVtx;
  vector<TProfile*> hvEstPUSubEtVsNVtx;

  vector<TProfile*> hvTAvgPUVsAvgLumi;
  vector<TProfile*> hvTAvgPUVsNVtx;
  vector<TProfile*> hvEstPUVsNVtx;
  vector<TProfile*> hvRegEtVsNVtx;
  vector<TProfile*> hvTAvgPUVsPhi;

  TProfile* hRegEtVsPhi3;
  TProfile* hRegEtVsPhi4;

  unsigned int nAll0;

  const string pathToPlots;

  vector<TGraph*> var;
};

#endif

#ifdef AnalyzePUEst_cxx
AnalyzePUEst::AnalyzePUEst(TTree *tree) : 
  fChain(0),
  pathToPlots("/home/nwoods/www/L1_PU_nBx/")
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/scratch/nwoods/puTree_nBx0.root");
    if (!f || !f->IsOpen()) {
      f = new TFile("/scratch/nwoods/puTree_nBx0.root");
    }
    TDirectory * dir = (TDirectory*)f->Get("/scratch/nwoods/puTree_nBx0.root:/makePUTree");
    dir->GetObject("Ntuple",tree);
  }
  Init(tree);


  hTAvgPU = new TH1F("tAvgPU", "Time Average PU (all regions)", 
		     20, 0., 4.5);
  hEstPU = new TH1F("estPU", "Estimated PU (all eta)", 
		     20, 0., 4.5);
  hUICPU = new TH1F("uicPU", "Space Average PU (all regions)", 
		    20, 0., 4.5);
  hAvgLumi = new TH1F("avgLumi", "Time Average Luminosity (all regions)", 
		      20, 18.5, 21.5);
  hXAvgPU = new TH1F("xAvgPU", "Region Average PU", 20, 0., 4.5);
  hTAvgPUSubEt = new TH1F("tAvgPUSubEt", "Et-PU(time avg)", 27, -3., 6.);
  hEstPUSubEt = new TH1F("estPUSubEt", "Et-PU(est)", 27, -3., 6.);
  hXAvgPUSubEt = new TH1F("xAvgPUSubEt", "Et-PU(region avg)", 27, -3., 6.);
  hUICPUSubEt = new TH1F("uicPUSubEt", "Et-PU(space avg)", 27, -3., 6.);
  hRegEt = new TH1F("regEt", "4x4 Region Et", 10, 0., 5.);
  hLumiStd = new TH1F("lumiStd", "Inst Lumi Standard Deviation", 100, 0., 1.);
  hStripPU = new TH1F("stripPU", "Phi Strip Average PU", 20, 0., 4.5);
  hStripPUSubEt = new TH1F("stripPUSubEt", "Et-PU(phi strip avg)", 27, -3., 6.);
  hComboPU = new TH1F("comboPU", "Combination PU", 20, 0., 4.5);
  hComboPUSubEt = new TH1F("comboPUSubEt", "Et-PU(combination)", 27, -3., 6.);
  hNVtx = new TH1F("nVtx", "# Primary Vertices", 30, 8., 68.);

  hTAvgPUVsAvgLumi = new TH2F("tAvgPUVsAvgLumi", 
			      "Time-Avg PU Level vs Avg Luminosity (All Eta)", 
			      20, 18.5, 21.5,
			      30, 0., 1.0);
  h2TAvgPUSubEtVsEta = new TH2F("tAvgPUSubEtVsEtaColZ", 
				"Et - PU(time avg) vs GCT Eta Index", 
				22, -0.5, 21.5,
				40, -2., 3.);
  hRegEtVsEta = new TH2F("regEtVsEta", 
			 "4x4 Region Et vs GCT Eta Index", 
			 22, -0.5, 21.5,
			 10, 0., 5.);
  h2StripPUSubEtVsEta = new TH2F("stripPUSubEtVsEtaColZ", 
				 "Et - PU(phi strip avg) vs GCT Eta Index", 
				 22, -0.5, 21.5,
				 40, -2., 3.);
  h2ComboPUSubEtVsEta = new TH2F("comboPUSubEtVsEtaColZ", 
				 "Et - PU(combination) vs GCT Eta Index", 
				 22, -0.5, 21.5,
				 40, -2., 3.);

  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "tAvgPUVsAvgLumi" << eta;
      stringstream sTitle;
      sTitle << "Time Avg PU Level Vs Avg Luminosity (Eta=" << eta << ")";
      hvTAvgPUVsAvgLumi.push_back(new TProfile(sName.str().c_str(),
					       sTitle.str().c_str(),
					       100, 200., 3400.,
					       0., 10.));
      hvTAvgPUVsAvgLumi.at(eta)->SetMinimum(0.);
      hvTAvgPUVsAvgLumi.at(eta)->SetMaximum(6.);
    }
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "estPUVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Estimated PU Level Vs # Primary Vertices (Eta=" << eta << ")";
      hvEstPUVsNVtx.push_back(new TProfile(sName.str().c_str(),
					       sTitle.str().c_str(),
					       60, 8., 68.,
					       0., 10.));
      hvEstPUVsNVtx.at(eta)->SetMinimum(0.);
      hvEstPUVsNVtx.at(eta)->SetMaximum(6.);
    }
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "tAvgPUVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Time Avg PU Level Vs Avg # Primary Vertices (Eta=" 
	     << eta << ")";
      hvTAvgPUVsNVtx.push_back(new TProfile(sName.str().c_str(),
					    sTitle.str().c_str(),
					    32, 8., 40.,
					    0., 10.));
      hvTAvgPUVsNVtx.at(eta)->SetMinimum(0.);
      hvTAvgPUVsNVtx.at(eta)->SetMaximum(4.);
    }

  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "estPUSubEtVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Et - PU(est) Vs # Primary Vertices (Eta=" << eta << ")";
      hvEstPUSubEtVsNVtx.push_back(new TProfile(sName.str().c_str(),
						sTitle.str().c_str(),
						32, 8., 40.,
						-10., 10.));
      hvEstPUSubEtVsNVtx.at(eta)->SetMinimum(-1.);
      hvEstPUSubEtVsNVtx.at(eta)->SetMaximum(1.);
    }
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "tAvgPUSubEtVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Et - PU(time avg) Vs # Primary Vertices (Eta=" 
	     << eta << ")";
      hvTAvgPUSubEtVsNVtx.push_back(new TProfile(sName.str().c_str(),
						 sTitle.str().c_str(),
						 32, 8., 40.,
						 -10., 10.));
      hvTAvgPUSubEtVsNVtx.at(eta)->SetMinimum(-1.);
      hvTAvgPUSubEtVsNVtx.at(eta)->SetMaximum(1.);
    }

  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "tAvgPUVsPhi" << eta;
      stringstream sTitle;
      sTitle << "Time Avg PU Level Vs phi (Eta=" 
	     << eta << ")";
      hvTAvgPUVsPhi.push_back(new TProfile(sName.str().c_str(),
					   sTitle.str().c_str(),
					   18, -0.5, 17.5,
					   0., 10.));
      hvTAvgPUVsPhi.at(eta)->SetMinimum(0.);
      hvTAvgPUVsPhi.at(eta)->SetMaximum(4.);
      hvTAvgPUVsPhi.at(eta)->SetErrorOption("s");
    }
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream sName;
      sName << "regEtVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Region Et Vs Avg # Primary Vertices (Eta=" 
	     << eta << ")";
      hvRegEtVsNVtx.push_back(new TProfile(sName.str().c_str(),
					   sTitle.str().c_str(),
					   30, 8., 68.,
					   0., 10.));
      hvRegEtVsNVtx.at(eta)->SetMinimum(0.);
      hvRegEtVsNVtx.at(eta)->SetMaximum(4.);
    }

  hRegEtVsPhi3 = new TProfile("regEtVsPhi3",
			      "Region Et Vs Phi (Eta=3)",
			      18, -0.5, 17.5,
			      0.,10.);
  hRegEtVsPhi3->SetMinimum(0.);
  hRegEtVsPhi3->SetMaximum(2.);
  hRegEtVsPhi4 = new TProfile("regEtVsPhi4",
			      "Region Et Vs Phi (Eta=4)",
			      18, -0.5, 17.5,
			      0.,10.);
  hRegEtVsPhi4->SetMinimum(0.);
  hRegEtVsPhi4->SetMaximum(2.);
  hTAvgPUVsAvgLumiLoEta = new TProfile("tAvgPUVsAvgLumiLoEta", 
			  "Time Avg PU Level vs Avg Luminosity (4<Eta<17)", 
				       20, 18.5, 21.5,
				       0., 10.);
  hTAvgPUVsAvgLumiLoEta->SetMinimum(0.);
  hTAvgPUVsAvgLumiLoEta->SetMaximum(.75);
  hTAvgPUVsAvgLumiMdEta = new TProfile("tAvgPUVsAvgLumiMdEta", 
		  "Time Avg PU Level vs Avg Luminosity (2<Eta<5 & 16<Eta<19)", 
				       20, 18.5, 21.5,
				       0., 10.);
  hTAvgPUVsAvgLumiMdEta->SetMinimum(0.);
  hTAvgPUVsAvgLumiMdEta->SetMaximum(.75);
  hTAvgPUVsAvgLumiHiEta = new TProfile("tAvgPUVsAvgLumiHiEta", 
		       "Time Avg PU Level vs Avg Luminosity (Eta<3 & Eta>18)", 
				       20, 18.5, 21.5,
				       0., 10.);
  hTAvgPUVsAvgLumiHiEta->SetMinimum(0.);
  hTAvgPUVsAvgLumiHiEta->SetMaximum(.75);

  hTAvgPUVsNVtxLoEta = new TProfile("tAvgPUVsNVtxLoEta", 
         	      "Time Avg PU Level vs Avg # Primary Vertices (4<Eta<17)",
				       32, 8., 40.,
				       0., 10.);
  hTAvgPUVsNVtxLoEta->SetMinimum(0.);
  hTAvgPUVsNVtxLoEta->SetMaximum(2.);
  hTAvgPUVsNVtxMdEta = new TProfile("tAvgPUVsNVtxMdEta", 
	   "Time Avg PU Level vs Avg # Primary Vertices (2<Eta<5 & 16<Eta<19)",
				    32, 8., 40.,
				    0., 10.);
  hTAvgPUVsNVtxMdEta->SetMinimum(0.);
  hTAvgPUVsNVtxMdEta->SetMaximum(2.);
  hTAvgPUVsNVtxHiEta = new TProfile("tAvgPUVsNVtxHiEta", 
		       "Time Avg PU Level vs Avg Luminosity (Eta<3 & Eta>18)", 
				    32, 8., 40.,
				    0., 10.);
  hTAvgPUVsNVtxHiEta->SetMinimum(0.);
  hTAvgPUVsNVtxHiEta->SetMaximum(2.);

  hEstPUVsNVtxLoEta = new TProfile("estPUVsNVtxLoEta", 
         	     "Estimated PU Level vs # Primary Vertices (4<Eta<17)",
				   32, 8., 40.,
				   0., 10.);
  hEstPUVsNVtxLoEta->SetMinimum(0.);
  hEstPUVsNVtxLoEta->SetMaximum(2.);
  hEstPUVsNVtxMdEta = new TProfile("estPUVsNVtxMdEta", 
	  "Estimated PU Level vs Avg # Primary Vertices (2<Eta<5 & 16<Eta<19)",
				   32, 8., 40.,
				   0., 10.);
  hEstPUVsNVtxMdEta->SetMinimum(0.);
  hEstPUVsNVtxMdEta->SetMaximum(2.);
  hEstPUVsNVtxHiEta = new TProfile("estPUVsNVtxHiEta", 
		      "Estimated PU Level vs Avg Luminosity (Eta<3 & Eta>18)", 
				   32, 8., 40.,
				   0., 10.);
  hEstPUVsNVtxHiEta->SetMinimum(0.);
  hEstPUVsNVtxHiEta->SetMaximum(2.);

  hTAvgPUSubEtVsNVtxLoEta = new TProfile("tAvgPUSubEtVsNVtxLoEta", 
         	      "Et - PU(time avg) vs # Primary Vertices (4<Eta<17)",
					 32, 8., 40.,
					 -10., 10.);
  hTAvgPUSubEtVsNVtxLoEta->SetMinimum(-1.);
  hTAvgPUSubEtVsNVtxLoEta->SetMaximum(1.);
  hTAvgPUSubEtVsNVtxMdEta = new TProfile("tAvgPUSubEtVsNVtxMdEta", 
	   "Et - PU(time avg) vs # Primary Vertices (2<Eta<5 & 16<Eta<19)",
					 32, 8., 40.,
					 -10., 10.);
  hTAvgPUSubEtVsNVtxMdEta->SetMinimum(-1.);
  hTAvgPUSubEtVsNVtxMdEta->SetMaximum(1.);
  hTAvgPUSubEtVsNVtxHiEta = new TProfile("tAvgPUSubEtVsNVtxHiEta", 
	       "Et - PU(time avg) vs # Primary Vertices (Eta<3 & Eta>18)", 
					 32, 8., 40.,
					 -10., 10.);
  hTAvgPUSubEtVsNVtxHiEta->SetMinimum(-1.);
  hTAvgPUSubEtVsNVtxHiEta->SetMaximum(1.);

  hEstPUSubEtVsNVtxLoEta = new TProfile("estPUSubEtVsNVtxLoEta", 
         	     "Et - PU(est) vs # Primary Vertices (4<Eta<17)",
					32, 8., 40.,
					-10., 10.);
  hEstPUSubEtVsNVtxLoEta->SetMinimum(-1.);
  hEstPUSubEtVsNVtxLoEta->SetMaximum(1.);
  hEstPUSubEtVsNVtxMdEta = new TProfile("estPUSubEtVsNVtxMdEta", 
	  "Et - PU(est) vs # Primary Vertices (2<Eta<5 & 16<Eta<19)",
					32, 8., 40.,
					-10., 10.);
  hEstPUSubEtVsNVtxMdEta->SetMinimum(-1.);
  hEstPUSubEtVsNVtxMdEta->SetMaximum(1.);
  hEstPUSubEtVsNVtxHiEta = new TProfile("estPUSubEtVsNVtxHiEta", 
		      "Et - PU(est) vs # Primary Vertices (Eta<3 & Eta>18)", 
					32, 8., 40.,
					-10., 10.);
  hEstPUSubEtVsNVtxHiEta->SetMinimum(-1.);
  hEstPUSubEtVsNVtxHiEta->SetMaximum(1.);

  hTAvgPUVsEta = new TProfile("tAvgPUVsEta", 
			  "Time Avg PU Level vs GCT Eta Index", 
			  22, -0.5, 21.5,
			  0., 10);
  hTAvgPUVsEta->SetMinimum(0.);
  hTAvgPUVsEta->SetMaximum(4.5);
  hUICPUVsEta = new TProfile("uicPUVsEta", 
			  "Space Avg PU Level vs GCT Eta Index", 
			  22, -0.5, 21.5,
			  0., 10);
  hUICPUVsEta->SetMinimum(0.);
  hUICPUVsEta->SetMaximum(4.5);
  hTAvgPUVsPhi = new TProfile("tAvgPUVsPhi", 
			  "Time Avg PU Level vs GCT Phi Index",
			  18, -0.5, 17.5,
			  0., 10.);
  hTAvgPUVsPhi->SetMinimum(0.);
  hTAvgPUVsPhi->SetMaximum(4.5);
  hTAvgPUSubEtVsEta = new TProfile("tAvgPUSubEtVsEta", 
			  "4x4 Et - PU(time avg) vs GCT Eta Index",
			  22, -0.5, 21.5,
			  -2., 5.);
  hTAvgPUSubEtVsEta->SetMinimum(-2.5);
  hTAvgPUSubEtVsEta->SetMaximum(2.5);

  hEstPUSubEtVsEta = new TProfile("estPUSubEtVsEta", 
			  "4x4 Et - PU(est) vs GCT Eta Index",
			  22, -0.5, 21.5,
			  -2., 5.);
  hEstPUSubEtVsEta->SetMinimum(-2.5);
  hEstPUSubEtVsEta->SetMaximum(2.5);

  hXAvgPUSubEtVsEta = new TProfile("xAvgPUSubEtVsEta", 
			  "4x4 Et - PU(region avg) vs GCT Eta Index",
			  22, -0.5, 21.5,
			  -2., 5.);
  hXAvgPUSubEtVsEta->SetMinimum(-2.5);
  hXAvgPUSubEtVsEta->SetMaximum(2.5);
  hUICPUSubEtVsEta = new TProfile("uicPUSubEtVsEta", 
			  "4x4 Et - PU(space avg) vs GCT Eta Index",
			  22, -0.5, 21.5,
			  -2., 5.);
  hUICPUSubEtVsEta->SetMinimum(-2.5);
  hUICPUSubEtVsEta->SetMaximum(2.5);
  hStripPUVsEta = new TProfile("stripPUVsEta", 
			       "Phi Strip PU Level vs GCT Eta Index", 
			       22, -0.5, 21.5,
			       0., 10.);
  hStripPUVsEta->SetMinimum(0.);
  hStripPUVsEta->SetMaximum(4.5);
  hStripPUSubEtVsEta = new TProfile("stripPUSubEtVsEta", 
				    "4x4 Et - PU(phi strip) vs GCT Eta Index",
				    22, -0.5, 21.5,
				    -2., 5.);
  hStripPUSubEtVsEta->SetMinimum(-2.5);
  hStripPUSubEtVsEta->SetMaximum(2.5);
  hComboPUVsEta = new TProfile("comboPUVsEta", 
			       "Combination PU Level vs GCT Eta Index", 
			       22, -0.5, 21.5,
			       0., 10.);
  hComboPUVsEta->SetMinimum(0.);
  hComboPUVsEta->SetMaximum(4.5);
  hComboPUSubEtVsEta = new TProfile("comboPUSubEtVsEta", 
				   "4x4 Et - PU(combination) vs GCT Eta Index",
				    22, -0.5, 21.5,
				    -2., 5.);
  hComboPUSubEtVsEta->SetMinimum(-2.5);
  hComboPUSubEtVsEta->SetMaximum(2.5);
  hComboPUSubEtVsPhi = new TProfile("comboPUSubEtVsPhi", 
				   "4x4 Et - PU(combination) vs GCT Phi Index",
				    18, -0.5, 17.5,
				    -2., 5.);
  hComboPUSubEtVsPhi->SetMinimum(-2.5);
  hComboPUSubEtVsPhi->SetMaximum(2.5);
  hTAvgPUSubEtVsRegEt = new TProfile("tAvgPUSubEtVsRegEt", 
				     "4x4 Et - PU(time avg) vs 4x4 Et",
				     10, 0., 5.,
				     -8., 8.);
  hTAvgPUSubEtVsRegEt->SetMinimum(-2.5);
  hTAvgPUSubEtVsRegEt->SetMaximum(4.);
  hStripPUSubEtVsRegEt = new TProfile("stripPUSubEtVsRegEt", 
				      "4x4 Et - PU(phi strip) vs 4x4 Et",
				      10, 0., 5.,
				      -8., 8.);
  hStripPUSubEtVsRegEt->SetMinimum(-2.5);
  hStripPUSubEtVsRegEt->SetMaximum(4.);
  hTAvgPUVsNBx = new TProfile("tAvgPUVsNBx", 
			      "Time Avg Pileup Level vs Bunch Number",
			      140, 0., 3500.,
			      0., 8.);
  hTAvgPUVsNBx->SetMinimum(0);
  hTAvgPUVsNBx->SetMaximum(0.6);
  hTAvgPUSubEtVsNBx = new TProfile("tAvgPUSubEtVsNBx",
				   "4x4 Et - PU (time avg) vs Bunch Number",
				   140, 0., 3500.,
				   -10. ,10.);
  hTAvgPUSubEtVsNBx->SetMinimum(-2.5);
  hTAvgPUSubEtVsNBx->SetMaximum(2.5);
  hXAvgPUVsNBx = new TProfile("xAvgPUVsNBx",
			      "4x4 Et - PU (space avg) vs Bunch Number",
			      140, 0., 3500.,
			      -10. ,10.);
  hXAvgPUVsNBx->SetMinimum(0.);
  hXAvgPUVsNBx->SetMaximum(0.6);
}

AnalyzePUEst::~AnalyzePUEst()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t AnalyzePUEst::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t AnalyzePUEst::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void AnalyzePUEst::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  tAvgPU = 0;
  lumiAvg = 0;
  tAvgPUSubEt = 0;
  xAvgPUSubEt = 0;
  regEt = 0;
  lumiStd = 0;
  uicPU = 0;
  uicPUSubEt = 0;
  comboPU = 0;
  nVtx = 0;
  estPULvl = 0;

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("tAvgPU", &tAvgPU, &b_tAvgPU);
  fChain->SetBranchAddress("xAvgPU", &xAvgPU, &b_xAvgPU);
  fChain->SetBranchAddress("uicPU", &uicPU, &b_uicPU);
  fChain->SetBranchAddress("lumiAvg", &lumiAvg, &b_lumiAvg);
  fChain->SetBranchAddress("run", &run, &b_run);
  fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
  fChain->SetBranchAddress("evt", &evt, &b_evt);
  fChain->SetBranchAddress("instlumi", &instlumi, &b_instlumi);
  fChain->SetBranchAddress("tAvgPUSubEt", &tAvgPUSubEt, &b_tAvgPUSubEt);
  fChain->SetBranchAddress("xAvgPUSubEt", &xAvgPUSubEt, &b_xAvgPUSubEt);
  fChain->SetBranchAddress("uicPUSubEt", &uicPUSubEt, &b_uicPUSubEt);
  fChain->SetBranchAddress("regEt", &regEt, &b_regEt);
  fChain->SetBranchAddress("lumiStd", &lumiStd, &b_lumiStd);
  fChain->SetBranchAddress("comboPU", &comboPU, &b_comboPU);
  fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
  fChain->SetBranchAddress("estimatedPULevel", &estPULvl, &b_estPULvl);
  fChain->SetBranchAddress("nBx", &nBx, &b_nBx);

  Notify();
}

Bool_t AnalyzePUEst::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void AnalyzePUEst::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t AnalyzePUEst::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}



#endif // #ifdef AnalyzePUEst_cxx
