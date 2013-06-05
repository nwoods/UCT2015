/*
 * =====================================================================================
 *
 *    Description:  Define dataformats for the UCT.
 *
 *         Author:  Evan Friis, evan.friis@cern.ch
 *        Company:  UW Madison
 *
 * =====================================================================================
 */

#include "L1Trigger/UCT2015/interface/L1RecoMatch.h"
#include "L1Trigger/UCT2015/interface/UCTCandidate.h"
#include "L1Trigger/UCT2015/src/L1GObject.h"
#include "L1Trigger/UCT2015/interface/CTPCard.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/Common/interface/SortedCollection.h"


namespace {

  L1RecoMatch dummyMatch;
  L1GObject dummyL1G;
  std::vector<L1GObject> dummyL1GCollection;
  edm::Wrapper<std::vector<L1GObject> > dummyL1GWrapper;

  UCTCandidate dummytDictC;
  std::vector<UCTCandidate> dummyUCTCandCollection;
  edm::Wrapper<std::vector<UCTCandidate> > dummyUCTCandCollectionWrapper;

  UCTRegion dummytRegion;
  std::vector<UCTRegion> dummyUCTRegionCollection;
  RegionDiscriminantInfo dummyDiscInfo;
  std::vector<RegionDiscriminantInfo> dummyDiscInfoVector;

  std::map<std::string, float> dummyMap;

  CTPOutput dummyCTPOutput;
  std::vector<CTPOutput> dummyCTPOutputVector;
  std::vector<std::vector<CTPOutput> > dummyCTPOutputVectorVector;
  edm::Wrapper<std::vector<std::vector<CTPOutput> > > 
    dummyCTPOutputVectorVectorWrapper;

  EcalTriggerPrimitiveDigi dummyEcalDigi;
  HcalTriggerPrimitiveDigi dummyHcalDigi;

  edm::SortedCollection<EcalTriggerPrimitiveDigi,
    edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> > 
    dummyEcalDigiCollection;
  edm::SortedCollection<HcalTriggerPrimitiveDigi,
    edm::StrictWeakOrdering<HcalTriggerPrimitiveDigi> > 
    dummyHcalDigiCollection;

  std::vector<edm::SortedCollection<EcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> > > 
    dummyEcalDigiCollectionVector;
  std::vector<edm::SortedCollection<HcalTriggerPrimitiveDigi,
    edm::StrictWeakOrdering<HcalTriggerPrimitiveDigi> > >
    dummyHcalDigiCollectionVector;

  edm::Wrapper<edm::SortedCollection<EcalTriggerPrimitiveDigi,
    edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> > >
    dummyEcalDigiCollectionWrapper;
  edm::Wrapper<edm::SortedCollection<HcalTriggerPrimitiveDigi,
    edm::StrictWeakOrdering<HcalTriggerPrimitiveDigi> > >
    dummyHcalDigiCollectionWrapper;
}
