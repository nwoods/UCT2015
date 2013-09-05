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

#include "DataFormats/Scalers/interface/LumiScalers.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

using namespace std;
using namespace edm;


class EstimatedPUSubtractor : public edm::EDProducer {
public:

  explicit EstimatedPUSubtractor(const edm::ParameterSet&);

private:
  // Data Members
  auto_ptr<L1CaloRegionCollection> newRegions;
  auto_ptr<vector<float>> estimatedPULevel;

  const InputTag regionInputTag;

  vector<float> coeffA;
  vector<float> coeffB;

  float nVtx;

  InputTag scalerSrc_;

  // Set by setParams to be 1 TProfile for each eta index, 
  //// where x-axis is <nVtx> (as determined by lumi scalers), y-axis is 
  //// estimated PU level
  // Everything assumes you give it a set of 22 TProfiles with evenly sized 
  //// bins, all with the same name but the eta index as the last 1-2 chars
  //// (e.g. profName0, profName1, ...)
//   vector<TProfile*> PUProfile;
//   unsigned nBins;
//   float binSize;
//   float binStart;


  // Function Members
  virtual void produce(edm::Event&, const edm::EventSetup&);

  // Replaces region rank with (region rank - region PU) for all regions
  void subtractPU();

  //  void setParams(TFile* f, string profName);

  void setCoeffs();

//   inline unsigned getBinInd(float nVtx) const
//   {
//     // bins are 1-indexed because root is stupid
//     return unsigned((nVtx - binStart) / binSize) + 1;
//   }

  inline unsigned getRegInd(unsigned eta, unsigned phi) const
  {
    return eta * 18 + phi;
  }

  float getPULevel(unsigned eta, float nVtx) const;
};

//
// constructors and destructor
//
EstimatedPUSubtractor::EstimatedPUSubtractor(const ParameterSet& iConfig) :
  regionInputTag(iConfig.getParameter<InputTag>("regionSrc"))  
{
//   string paramFileName = iConfig.getParameter<string>("PUParamSrc");
//   TFile* f = new TFile(paramFileName.c_str());
//   string profName = iConfig.getParameter<string>("PUProfileName");
//   setParams(f,profName);
//   f->Close();
//   delete f;

  coeffA.resize(22);
  coeffB.resize(22);
  setCoeffs();

  scalerSrc_ = iConfig.exists("scalerSrc") ?
    iConfig.getParameter<InputTag>("scalerSrc") :
    InputTag("scalersRawToDigi");
   
  produces<L1CaloRegionCollection>();
  produces<vector<float>>();
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
  estimatedPULevel = auto_ptr<vector<float>>(new vector<float>);

  newRegions->resize(22*18);
  estimatedPULevel->resize(22);

  for(unsigned q = 0; q < regionHandle->size(); ++q)
    {
      unsigned eta = regionHandle->at(q).gctEta();
      unsigned phi = regionHandle->at(q).gctPhi();

      unsigned ind = getRegInd(eta,phi);

      newRegions->at(ind) = regionHandle->at(q);
    }
  
  Handle<LumiScalersCollection> lumiScalers;
  iEvent.getByLabel(scalerSrc_, lumiScalers);
  if (lumiScalers->size())
    {
      nVtx = lumiScalers->begin()->pileup();
    }
   
  subtractPU();

  iEvent.put(newRegions);
  iEvent.put(estimatedPULevel);
}


void EstimatedPUSubtractor::subtractPU()
{
  for(unsigned eta = 0; eta < 22; ++eta)
    {
      float puLevel = getPULevel(eta, nVtx);
      estimatedPULevel->at(eta) = puLevel;

      for(unsigned phi = 0; phi < 18; ++phi)
	{
	  unsigned ind = getRegInd(eta,phi);

	  unsigned newEt = newRegions->at(ind).et();

	  if(newEt > puLevel)
	    newEt -= (unsigned) std::round(puLevel);
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



void EstimatedPUSubtractor::setCoeffs()
{
  coeffA.at(0) = -0.294659;                 //-0.398557;
  coeffA.at(1) = -0.384448;                 //-0.386599;
  coeffA.at(2) = -0.448682;                 //-0.346655;
  coeffA.at(3) = -0.21784;                  //-0.201807;
  coeffA.at(4) = -0.0643482;                //0; // must be treated separately
  coeffA.at(5) = -0.0316119;                //-0.0514639;
  coeffA.at(6) = -0.0328163;                //-0.0304773;
  coeffA.at(7) = -0.0399358;                //-0.035895;
  coeffA.at(8) = -0.041178;                 //-0.0423884;
  coeffA.at(9) = -0.0401939;                //-0.0397331;
  coeffA.at(10) = -0.0211886;                //-0.00760061;
  coeffA.at(11) = -0.0242173;                //-0.0181333;
  coeffA.at(12) = -0.0342751;                //-0.0349415;
  coeffA.at(13) = -0.0395816;                //-0.0416009;
  coeffA.at(14) = -0.0312357;                //-0.0281608;
  coeffA.at(15) = -0.0356038;                //-0.0335604;
  coeffA.at(16) = -0.0315476;                //-0.0517975;
  coeffA.at(17) = -0.0625101;                //0; // must be treated separately
  coeffA.at(18) = -0.208595;                 //-0.191178;
  coeffA.at(19) = -0.444225;                 //-0.345969;
  coeffA.at(20) = -0.389511;                 //-0.386697;
  coeffA.at(21) = -0.311709;                 //-0.413654;
	
  coeffB.at(0) = 0.0373227;           //0.045127;
  coeffB.at(1) = 0.0528847;           //0.0536806;
  coeffB.at(2) = 0.0656358;           //0.0603077;
  coeffB.at(3) = 0.0320404;           //0.0314865;
  coeffB.at(4) = 0.0101106;           //0;// must be treated separately
  coeffB.at(5) = 0.00614193;          //0.00776713;
  coeffB.at(6) = 0.00802572;          //0.00797786;
  coeffB.at(7) = 0.00947565;          //0.00931965;
  coeffB.at(8) = 0.0098865;           //0.0101424;
  coeffB.at(9) = 0.0116322;           //0.0117935;
  coeffB.at(10) = 0.013636;            //0.0128345;
  coeffB.at(11) = 0.0129647;           //0.0127174;
  coeffB.at(12) = 0.011409;            //0.0116347;
  coeffB.at(13) = 0.00964048;          //0.00993312;
  coeffB.at(14) = 0.00930953;          //0.00919783;
  coeffB.at(15) = 0.00815619;          //0.00813148;
  coeffB.at(16) = 0.00581184;          //0.00745618;
  coeffB.at(17) = 0.00970106;          //0; // must be treated separately
  coeffB.at(18) = 0.0305973;           //0.0299054;
  coeffB.at(19) = 0.0652842;           //0.060129;
  coeffB.at(20) = 0.0540518;           //0.0545827;
  coeffB.at(21) = 0.0395865;           //0.047247;
}
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


// void EstimatedPUSubtractor::setParams(TFile* f, string profName)
// {
//   for(unsigned eta = 0; eta < 22; ++eta)
//     {
//       stringstream nameStream;
//       nameStream << profName << eta;

//       PUProfile.push_back((TProfile*)f->Get(nameStream.str().c_str()));
//     }
//   nBins = PUProfile.at(0)->GetNbinsX();
//   binSize = PUProfile.at(0)->GetBinWidth(1);
//   binStart = PUProfile.at(0)->GetBinLowEdge(1);
// }


float EstimatedPUSubtractor::getPULevel(unsigned eta, float nVtx) const
{
  return coeffA.at(eta) + nVtx * coeffB.at(eta);

  //  return PUProfile.at(eta)->GetBinContent(getBinInd(nVtx));
}

//define this as a plug-in
DEFINE_FWK_MODULE(EstimatedPUSubtractor);
