//
// Package:    EstimatedPUSubtractor.cc
//
// Original Author:  Nate Woods (Wisconsin)
//         Created:  Aug. 19 2013
//


// system include files
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <TFile.h>
#include <TProfile.h>
#include <string>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

using namespace std;
using namespace edm;


class EstimatedPUSubtractor : public edm::EDProducer {
public:

  explicit EstimatedPUSubtractor(const edm::ParameterSet&);

private:
  // Data Members
  auto_ptr<L1CaloRegionCollection> newRegions;

  const InputTag regionInputTag;
  const InputTag pvSrc;

  unsigned nVtx;

  // Set by setParams to be 1 TProfile for each eta index, 
  //// where x-axis is <nVtx> (as determined by lumi scalers), y-axis is 
  //// estimated PU level
  // Everything assumes you give it a set of 22 TProfiles with evenly sized 
  //// bins, all with the same name but the eta index as the last 1-2 chars
  //// (e.g. profName0, profName1, ...)
  vector<TProfile*> PUProfile;
  unsigned nBins;
  float binSize;
  float binStart;


  // Function Members
  virtual void produce(edm::Event&, const edm::EventSetup&);

  // Replaces region rank with (region rank - region PU) for all regions
  void subtractPU();

  void setParams(TFile* f, string profName);

  inline unsigned getBinInd(float nVtx) const
  {
    // bins are 1-indexed because root is stupid
    return unsigned((nVtx - binStart) / binSize) + 1;
  }

  inline unsigned getRegInd(unsigned eta, unsigned phi) const
  {
    return eta * 18 + phi;
  }

  unsigned getPULevel(unsigned eta, float nVtx) const;
};

//
// constructors and destructor
//
EstimatedPUSubtractor::EstimatedPUSubtractor(const ParameterSet& iConfig) :
  regionInputTag(iConfig.getParameter<InputTag>("regionSrc")),
  pvSrc(iConfig.exists("pvSrc") ? 
	iConfig.getParameter<edm::InputTag>("pvSrc") : 
	InputTag("offlinePrimaryVertices"))
  
{
  string paramFileName = iConfig.getParameter<string>("PUParamSrc");
  TFile* f = new TFile(paramFileName.c_str());
  string profName = iConfig.getParameter<string>("PUProfileName");
  setParams(f,profName);
  f->Close();
  delete f;

  produces<L1CaloRegionCollection>();
}


// ------------ method called for each event  ------------
void
EstimatedPUSubtractor::produce(Event& iEvent, const EventSetup& iSetup)
{

  // Get regions
  Handle<L1CaloRegionCollection> regionHandle;
  iEvent.getByLabel(regionInputTag, regionHandle);

  if(regionHandle->size() != 18*22)
    throw cms::Exception("BadRegions") << "incoming regions had "
				       << regionHandle->size()
				       << " regions!" << endl;

  newRegions = auto_ptr<L1CaloRegionCollection>(new L1CaloRegionCollection);

  newRegions->resize(22*18);

  for(unsigned q = 0; q < regionHandle->size(); ++q)
    {
      unsigned eta = regionHandle->at(q).gctEta();
      unsigned phi = regionHandle->at(q).gctPhi();

      unsigned ind = getRegInd(eta,phi);

      newRegions->at(ind) = regionHandle->at(q);
    }
  
  // Get PV collection
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(pvSrc, vertices);

  nVtx = vertices->size();

  subtractPU();

  iEvent.put(newRegions);
}


void EstimatedPUSubtractor::subtractPU()
{
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      unsigned puLevel = getPULevel(eta, nVtx);

      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned ind = getRegInd(eta,phi);

	  unsigned newEt = newRegions->at(ind).et();

	  if(newEt > puLevel)
	    newEt -= puLevel;
	  else
	    newEt = 0;

	  uint16_t regRawData = newRegions->at(ind).raw();
	  // Last 10(Barrel/Endcap) or 8(HF) bits are Et. 
	  // Remove + replace with PU subtracted Et.
	  if(newRegions->at(ind).isHf())
	    regRawData = (regRawData & 0xFF00) + newEt;
	  else
	    regRawData = (regRawData & 0xFC00) + newEt;
	  newRegions->at(ind).setRawData((uint32_t)regRawData);
	}
    }
}



// void EstimatedPUSubtractor::setCoeffs()
// {
//   coeffA[0] = -0.398557;
//   coeffA[1] = -0.386599;
//   coeffA[2] = -0.346655;
//   coeffA[3] = -0.201807;
//   coeffA[4] = 0; // must be treated separately
//   coeffA[5] = -0.0514639;
//   coeffA[6] = -0.0304773;
//   coeffA[7] = -0.035895;
//   coeffA[8] = -0.0423884;
//   coeffA[9] = -0.0397331;
//   coeffA[10] = -0.00760061;
//   coeffA[11] = -0.0181333;
//   coeffA[12] = -0.0349415;
//   coeffA[13] = -0.0416009;
//   coeffA[14] = -0.0281608;
//   coeffA[15] = -0.0335604;
//   coeffA[16] = -0.0517975;
//   coeffA[17] = 0; // must be treated separately
//   coeffA[18] = -0.191178;
//   coeffA[19] = -0.345969;
//   coeffA[20] = -0.386697;
//   coeffA[21] = -0.413654;

//   coeffB[0] = 0.045127;
//   coeffB[1] = 0.0536806;
//   coeffB[2] = 0.0603077;
//   coeffB[3] = 0.0314865;
//   coeffB[4] = 0;// must be treated separately
//   coeffB[5] = 0.00776713;
//   coeffB[6] = 0.00797786;
//   coeffB[7] = 0.00931965;
//   coeffB[8] = 0.0101424;
//   coeffB[9] = 0.0117935;
//   coeffB[10] = 0.0128345;
//   coeffB[11] = 0.0127174;
//   coeffB[12] = 0.0116347;
//   coeffB[13] = 0.00993312;
//   coeffB[14] = 0.00919783;
//   coeffB[15] = 0.00813148;
//   coeffB[16] = 0.00745618;
//   coeffB[17] = 0; // must be treated separately
//   coeffB[18] = 0.0299054;
//   coeffB[19] = 0.060129;
//   coeffB[20] = 0.0545827;
//   coeffB[21] = 0.047247;

//   var[0] = 0.0674131;
//   var[1] = 0.125199 ;
//   var[2] = 0.165922 ;
//   var[3] = 0.0766748;
//   var[4] = 0; // must be treated separately
//   var[5] = 0.0141715;
//   var[6] = 0.0247016;
//   var[7] = 0.0281513;
//   var[8] = 0.0285719;
//   var[9] = 0.0362951;
//   var[10] = 0.0655516;
//   var[11] = 0.0499496;
//   var[12] = 0.0376025;
//   var[13] = 0.0290704;
//   var[14] = 0.0308533;
//   var[15] = 0.0234916;
//   var[16] = 0.0147458;
//   var[17] = 0; // must be treated separately
//   var[18] = 0.0737861;
//   var[19] = 0.164002 ;
//   var[20] = 0.130799;
//   var[21] = 0.0881101;

//   loCoeffA4 = -0.09311282;
//   loCoeffB4 = 0.01277999;
//   hiCoeffA4 = -0.72091003;
//   hiCoeffB4 = 0.03855960;

//   loCoeffA17 = -0.07951813;
//   loCoeffB17 = 0.01138087;
//   hiCoeffA17 = -0.70540653;
//   hiCoeffB17 = 0.03737020;

//   // Cheating on variance for weird etas for now; fix later
//   varLo4 = .6;

// }


void EstimatedPUSubtractor::setParams(TFile* f, string profName)
{
  cout << "Profile name: contents of 1st bin" << endl;
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      stringstream nameStream;
      nameStream << profName << eta;

      PUProfile.push_back((TProfile*)f->Get(nameStream.str().c_str()));
    }
  nBins = PUProfile.at(0)->GetNbinsX();
  binSize = PUProfile.at(0)->GetBinWidth(1);
  binStart = PUProfile.at(0)->GetBinLowEdge(1);
}


unsigned EstimatedPUSubtractor::getPULevel(unsigned eta, float nVtx) const
{
  return (unsigned) std::round(PUProfile.at(eta)->
			       GetBinContent(getBinInd(nVtx)));
}

//define this as a plug-in
DEFINE_FWK_MODULE(EstimatedPUSubtractor);
