//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 17 15:31:48 2013 by ROOT version 5.32/00
// from TTree Ntuple/Ntuple
// found on file: pu_tree_numEvent555.root
//////////////////////////////////////////////////////////

#ifndef AnalyzePUNVtx_h
#define AnalyzePUNVtx_h

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

using namespace std;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class AnalyzePUNVtx {
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
  float           nVtx;

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

  AnalyzePUNVtx(TTree *tree=0);
  virtual ~AnalyzePUNVtx();
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
  TProfile* hTAvgPUVsPhi;
  TProfile* hUICPUVsEta;
  TProfile* hTAvgPUSubEtVsEta;
  TProfile* hXAvgPUSubEtVsEta;
  TProfile* hUICPUSubEtVsEta;
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
  vector<TProfile*> hvTAvgPUVsAvgLumi;
  vector<TProfile*> hvTAvgPUVsNVtx;

  unsigned int nAll0;

  const string pathToPlots;

  vector<TGraph*> var;
};

#endif

#ifdef AnalyzePUNVtx_cxx
AnalyzePUNVtx::AnalyzePUNVtx(TTree *tree) : 
  fChain(0),
  pathToPlots("/home/nwoods/www/L1_PU_nVtx/")
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/scratch/nwoods/puComp_all_nVtx_0.root");
    if (!f || !f->IsOpen()) {
      f = new TFile("/scratch/nwoods/puComp_all_nVtx_0.root");
    }
    TDirectory * dir = (TDirectory*)f->Get("/scratch/nwoods/puComp_all_nVtx_0.root:/makePUTree");
    dir->GetObject("Ntuple",tree);
  }
  Init(tree);


  hTAvgPU = new TH1F("tAvgPU", "Time Average PU (all regions)", 
		     20, 0., 4.5);
  hUICPU = new TH1F("uicPU", "Space Average PU (all regions)", 
		    20, 0., 4.5);
  hAvgLumi = new TH1F("avgLumi", "Time Average Luminosity (all regions)", 
		      20, 18.5, 21.5);
  hXAvgPU = new TH1F("xAvgPU", "Region Average PU", 20, 0., 4.5);
  hTAvgPUSubEt = new TH1F("tAvgPUSubEt", "Et-PU(time avg)", 27, -3., 6.);
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
      sName << "tAvgPUVsNVtx" << eta;
      stringstream sTitle;
      sTitle << "Time Avg PU Level Vs Avg # Primary Vertices (Eta=" 
	     << eta << ")";
      hvTAvgPUVsNVtx.push_back(new TProfile(sName.str().c_str(),
					    sTitle.str().c_str(),
					    30, 8., 68.,
					    0., 10.));
      hvTAvgPUVsNVtx.at(eta)->SetMinimum(0.);
      hvTAvgPUVsNVtx.at(eta)->SetMaximum(4.);
    }
  hTAvgPUVsAvgLumiLoEta = new TProfile("tAvgPUVsAvgLumiLoEta", 
			  "Time Avg PU Level vs Avg Luminosity (4<Eta<17)", 
				       0, 18.5, 21.5,
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
				       30, 8., 68.,
				       0., 10.);
  hTAvgPUVsNVtxLoEta->SetMinimum(0.);
  hTAvgPUVsNVtxLoEta->SetMaximum(3.);
  hTAvgPUVsNVtxMdEta = new TProfile("tAvgPUVsNVtxMdEta", 
	   "Time Avg PU Level vs Avg # Primary Vertices (2<Eta<5 & 16<Eta<19)",
				       30, 8., 68.,
				       0., 10.);
  hTAvgPUVsNVtxMdEta->SetMinimum(0.);
  hTAvgPUVsNVtxMdEta->SetMaximum(3.);
  hTAvgPUVsNVtxHiEta = new TProfile("tAvgPUVsNVtxHiEta", 
		       "Time Avg PU Level vs Avg Luminosity (Eta<3 & Eta>18)", 
				       30, 8., 68.,
				       0., 10.);
  hTAvgPUVsNVtxHiEta->SetMinimum(0.);
  hTAvgPUVsNVtxHiEta->SetMaximum(3.);
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
}

AnalyzePUNVtx::~AnalyzePUNVtx()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t AnalyzePUNVtx::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t AnalyzePUNVtx::LoadTree(Long64_t entry)
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

void AnalyzePUNVtx::Init(TTree *tree)
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
  Notify();
}

Bool_t AnalyzePUNVtx::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void AnalyzePUNVtx::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t AnalyzePUNVtx::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}



#endif // #ifdef AnalyzePUNVtx_cxx
