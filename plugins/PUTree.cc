/*
 * ============================================================================
 *
 *       Filename:  PUTree.cc
 *
 *    Description:  Produce a tree for computing time-averaged pileup level
 *                  from min/zerobias data
 *
 *         Author:  N. Woods, S. Dasu, E. Friis, M. Cepeda
 *        Company:  UW Madison
 *
 * ============================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Scalers/interface/LumiScalers.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TTree.h"

#include "L1Trigger/UCT2015/interface/helpers.h"

using namespace std;
using namespace edm;

typedef vector<InputTag> VInputTag;

class PUTree : public EDAnalyzer 
{
public:
  PUTree(const ParameterSet& pset);
  virtual ~PUTree();
  void analyze(const Event& evt, const EventSetup& es);
private:
  void updateTAvgPU();
  void updateXAvgPU();
  void updateRegEt();
  void updateLumiStd();
  void updateMCTruth();

  inline unsigned getRegInd(unsigned eta, unsigned phi) const
  {
    return eta * 18 + phi;
  }

  //   VInputTag src_;
  TTree* tree;
  UInt_t run_;
  UInt_t lumi_;
  ULong64_t event_;
  InputTag scalerSrc_;
  Float_t instLumi_;
  float nVtx_; // data version (MC version is nPUVtx_, below)

  vector<float>* tAvgPU_;
  Float_t xAvgPU_;
  vector<float>* uicPU_;
  vector<float>* lumiAvg_;
  vector<list<float>> prevRegRanks_;
  vector<list<float>> prevLumi_;
  unsigned puAvgCount_;
  const double tAvgPUCut_;
  const double xAvgPUCut_;
  vector<float>* lumiStd_;
  vector<float>* tAvgPUSubEt_;
  vector<float>* xAvgPUSubEt_;
  vector<float>* regEt_;
  vector<float>* uicPUSubEt_;
  vector<float>* comboPU_;
  vector<list<float>> prevStrips_;


  // only used for MC
  Handle<vector<PileupSummaryInfo>> puInfo_; 
  int nPUVtx_; // MC version (data version is nVtx_, above)
  float sumPUPt_;

  Handle<L1CaloRegionCollection> newRegions_;

  double regionLSB_;

  const bool isMC_;
};

PUTree::PUTree(const ParameterSet& pset) :
  tAvgPUCut_(pset.getParameter<double>("tAvgPUCut")),
  xAvgPUCut_(pset.getParameter<double>("xAvgPUCut")),
  regionLSB_(pset.getParameter<double>("regionLSB")),
  isMC_(pset.getParameter<bool>("isMC"))
{
  tAvgPU_ = new vector<float>(18*22,0.);
  lumiAvg_ = new vector<float>(18*22,0.);
  prevRegRanks_ = vector<list<float>>(18*22,list<float>());
  prevLumi_ = vector<list<float>>(18*22,list<float>());
  puAvgCount_ = 0;
  lumiStd_ = new vector<float>(18*22,0.);
  tAvgPUSubEt_ = new vector<float>(18*22,0.);
  xAvgPUSubEt_ = new vector<float>(18*22,0.);
  regEt_ = new vector<float>(18*22,0.);
  uicPU_ = new vector<float>(18*22,0.);
  uicPUSubEt_ = new vector<float>(18*22,0.);
  comboPU_ = new vector<float>(22,0.);
  prevStrips_ = vector<list<float>>(22,list<float>());


  // Initialize the ntuple builder
  Service<TFileService> fs;
  tree = fs->make<TTree>("Ntuple", "Ntuple");

  tree->Branch("tAvgPU", "std::vector<float>", &tAvgPU_);
  tree->Branch("xAvgPU", &xAvgPU_, "xAvgPU/F");
  if(!isMC_)
    {
      tree->Branch("lumiAvg", "std::vector<float>", &lumiAvg_);
      tree->Branch("lumiStd", "std::vector<float>", &lumiStd_);
      tree->Branch("instlumi", &instLumi_, "instlumi/F");
    }
  tree->Branch("run", &run_, "run/i");
  tree->Branch("lumi", &lumi_, "lumi/i");
  tree->Branch("evt", &event_, "evt/l");
  tree->Branch("tAvgPUSubEt", "std::vector<float>", &tAvgPUSubEt_);
  tree->Branch("xAvgPUSubEt", "std::vector<float>", &xAvgPUSubEt_);
  tree->Branch("regEt", "std::vector<float>", &regEt_);
  tree->Branch("uicPU", "std::vector<float>", &uicPU_);
  tree->Branch("uicPUSubEt", "std::vector<float>", &uicPUSubEt_);
  tree->Branch("comboPU", "std::vector<float>", &comboPU_);
  if(!isMC_)
    tree->Branch("nVtx", &nVtx_, "nVtx/F");
  if(isMC_)
    {
      tree->Branch("nPUVtx", &nPUVtx_, "nPUVtx/i");
      tree->Branch("sumPUPt", &sumPUPt_, "nPUVtx/F");
    }

  //   src_ = pset.getParameter<VInputTag>("src");
  scalerSrc_ = pset.exists("scalerSrc") ?
    pset.getParameter<InputTag>("scalerSrc") : InputTag("scalersRawToDigi");
}

PUTree::~PUTree() 
{
  delete tAvgPU_;

  if(!isMC_)
    {
      delete lumiAvg_;
      delete lumiStd_;
    }
  delete tAvgPUSubEt_;
  delete xAvgPUSubEt_;
  delete regEt_;
  delete comboPU_;
}



void PUTree::analyze(const Event& evt, const EventSetup& es) 
{

  // Setup meta info
  run_ = evt.id().run();
  lumi_ = evt.id().luminosityBlock();
  event_ = evt.id().event();

  if(!isMC_)
    {
      // Get instantaneous lumi from the scalers
      // thx to Carlo Battilana
      Handle<LumiScalersCollection> lumiScalers;
      evt.getByLabel(scalerSrc_, lumiScalers);
      instLumi_ = -1;
      if (lumiScalers->size())
	{
	  instLumi_ = lumiScalers->begin()->instantLumi();
	  nVtx_ = lumiScalers->begin()->pileup();
	}
//       cout << "Lumi Scalers Version " 
// 	   << lumiScalers->begin()->version()
// 	   << ", size "
// 	   << lumiScalers->size()
// 	   << ", nVtx values: ";
//       for(unsigned q = 0; q < lumiScalers->size(); ++q)
// 	cout << lumiScalers->at(q).pileup() << " ";
//       cout << endl;
    }

  evt.getByLabel("uctDigis", newRegions_);

  if(newRegions_->size() == 18*22)
    {
      if(puAvgCount_ % 260 == 20)
	updateLumiStd();
      updateTAvgPU();
    }
  else
    throw cms::Exception("BadInput") << "WARNING: " << newRegions_->size() 
				     << " regions (396 expected) when event counter = " 
				     << puAvgCount_ << endl 
				     << "Event will not be included in average" << endl;

  if(puAvgCount_ > 20 && 
     puAvgCount_ % 260 == 20) // extra few to ensure completeness+independance
    {
      if(isMC_)
	{
	  edm::InputTag PileupSrc_("addPileupInfo");
	  evt.getByLabel(PileupSrc_, puInfo_);

	  updateMCTruth();
	}

      updateXAvgPU(); // only need to do this for event in output
      updateRegEt();
      tree->Fill(); // only put events in output when we have an indep. average
    }

//   if(puAvgCount_ == 20)
//     {
//       cout << "Region ETs for 20th record: " << endl;
//       for(unsigned q = 0; q < newRegions_->size(); ++q)
// 	cout << q << ": " << newRegions_->at(q).et() << endl;
//     }

}


void PUTree::updateTAvgPU()
{
  //   if(puAvgCount_ == 1)
  //     cout << "Region ETs:" << endl;

  vector<float> stripPU(22,0.);
  vector<unsigned> nRegInStrip(22,0);

  for(unsigned rgn = 0; rgn < 22*18; ++rgn)
    {
      unsigned eta = newRegions_->at(rgn).gctEta();
      unsigned phi = newRegions_->at(rgn).gctPhi();
      unsigned ind = getRegInd(eta, phi);
	  
      float newEt = newRegions_->at(rgn).et() * regionLSB_;

      //       if(puAvgCount_ == 1)
      // 	cout << newEt << endl;
	  
      if(prevRegRanks_.at(ind).size() == 0) // if fist item, PU=0
	{
	  tAvgPU_->at(ind) = 0;
	  if(!isMC_)
	    lumiAvg_->at(ind) = 0;
	}
      if(prevStrips_.at(eta).size() == 0)
	{
	  comboPU_->at(eta) = 0;
	}
      
      // Get the time-averaged pileup+lumi for each 4x4
      //// If this event doesn't pass the cut, the average doesn't change
      //// and we do nothing
      if(newEt < tAvgPUCut_)
	{
	  stripPU.at(eta) += newEt;
	  ++nRegInStrip.at(eta);

	  if(prevRegRanks_.at(ind).size() == 257)
	    {
	      float totalEt = tAvgPU_->at(ind) * 256.;
	      totalEt -= prevRegRanks_.at(ind).back();
	      prevRegRanks_.at(ind).pop_back();
	      totalEt += prevRegRanks_.at(ind).front();
	      tAvgPU_->at(ind) = totalEt / 256.;

	      if(!isMC_)
		{
		  float totalLumi = lumiAvg_->at(ind) * 256.;
		  totalLumi -= prevLumi_.at(ind).back();
		  prevLumi_.at(ind).pop_back();
		  totalLumi += prevLumi_.at(ind).front();
		  lumiAvg_->at(ind) = totalLumi / 256.;
		}
	    }
	  else if(prevRegRanks_.at(ind).size() != 0)
	    {
	      float totalEt = tAvgPU_->at(ind) * 
		(prevRegRanks_.at(ind).size() - 1);
	      totalEt += prevRegRanks_.at(ind).front();
	      tAvgPU_->at(ind) = totalEt / 
		prevRegRanks_.at(ind).size();

	      if(!isMC_)
		{
		  float totalLumi = lumiAvg_->at(ind) * 
		    (prevLumi_.at(ind).size() - 1);
		  totalLumi += prevLumi_.at(ind).front();
		  lumiAvg_->at(ind) = totalLumi /
		    prevLumi_.at(ind).size();
		}
	    }
	  prevRegRanks_.at(ind).push_front(newEt);
	  if(!isMC_)
	    prevLumi_.at(ind).push_front(instLumi_);
	}
    }

  for(unsigned eta = 0; eta < 22; ++eta)
    {
      float stripAvgPU = stripPU.at(eta) / (float)nRegInStrip.at(eta);

      if(prevStrips_.at(eta).size() == 257)
	{
	  float totalEt = comboPU_->at(eta) * 256.;
	  totalEt -= prevStrips_.at(eta).back();
	  prevStrips_.at(eta).pop_back();
	  totalEt += prevStrips_.at(eta).front();
	  comboPU_->at(eta) = totalEt / 256.;
	}
      else if(prevStrips_.at(eta).size() != 0)
	{
	  float totalEt = comboPU_->at(eta) * (prevStrips_.at(eta).size() - 1);
	  totalEt += prevStrips_.at(eta).front();
	  comboPU_->at(eta) = totalEt / prevStrips_.at(eta).size();
	}
      prevStrips_.at(eta).push_front(stripAvgPU);
    }

  ++puAvgCount_;
}



// Get the space-averaged pileup for the event
void PUTree::updateXAvgPU()
{
  float puAreaTotal = 0.;
  unsigned xPUCount = 0;
  float puEtTotal = 0.;
    
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned rgn = getRegInd(eta,phi);

	  float newEt = newRegions_->at(rgn).et() * regionLSB_;
	  
	  if(newEt < xAvgPUCut_)
	    {
	      puEtTotal += newEt;
	      ++xPUCount;
	      puAreaTotal += getRegionArea(eta);
	    }
	}
    }

  xAvgPU_ = puEtTotal / (float)xPUCount;
  float uicPUAvg_ = puEtTotal / puAreaTotal;

  for(unsigned eta = 0; eta < 22; ++eta)
    {
      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned rgn = getRegInd(eta,phi);
	  uicPU_->at(rgn) = uicPUAvg_ * getRegionArea(eta);
	}
    }
}



// Get the standard deviation of luminosity to check configuration
void PUTree::updateLumiStd()
{
  for(unsigned p = 0; p < 22*18; ++p)
    {
      float sDevSqr = 0;
      for(list<float>::const_iterator q = prevLumi_.at(p).cbegin(); 
	  q != prevLumi_.at(p).cend(); 
	  ++q)
	{
	  if(q == prevLumi_.at(p).cbegin())
	    continue;

	  sDevSqr += pow(*q - lumiAvg_->at(p),2);
	}
      sDevSqr /= prevLumi_.at(p).size();
      lumiStd_->at(p) = sqrt(sDevSqr);
    }
}



void PUTree::updateRegEt()
{
  for(unsigned rgn = 0; rgn < newRegions_->size(); ++rgn)
    {
      unsigned ind = getRegInd(newRegions_->at(rgn).gctEta(),
			       newRegions_->at(rgn).gctPhi());
      
      float newEt = newRegions_->at(rgn).et() * regionLSB_;
      
      regEt_->at(ind) = newEt;
      tAvgPUSubEt_->at(ind) = newEt - tAvgPU_->at(ind);
      xAvgPUSubEt_->at(ind) = newEt - xAvgPU_;
      uicPUSubEt_->at(ind) = newEt - uicPU_->at(ind);
    }
}



void PUTree::updateMCTruth()
{
  nPUVtx_ = 0;
  sumPUPt_ = 0;

  for(unsigned bx = 0; bx < puInfo_->size(); ++bx)
    {
      nPUVtx_ += puInfo_->at(bx).getPU_NumInteractions();
      
      vector<float> vtxPts = puInfo_->at(bx).getPU_sumpT_lowpT();
      for(unsigned vtx = 0; vtx < vtxPts.size(); ++vtx)
	sumPUPt_ += vtxPts.at(vtx);
    }
}



#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PUTree);
