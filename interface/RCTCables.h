#ifndef RCTCABLES_h
#define RCTCABLES_h

/*
 * ============================================================     
 *    
 *       Filename:     RCTCables.h
 *    
 *    Description:     An emulator for the cabling system for the
 *                     upgrade calorimeter trigger cabling. Sends
 *                     digis to the correct card and selects the
 *                     digis for a given card from a collection.
 *    
 *         Author:     Nate Woods, nwoods@hep.wisc.edu
 *    Institution:     UW Madison
 *    
 * ============================================================    
 */    

// System include files
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;

// User include files
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveSample.h"
#include "DataFormats/Common/interface/SortedCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/UCT2015/interface/CTPCard.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"

using namespace std;
using namespace edm;

#ifndef CTPOUTPUT // Probably already defined in CTPCard.h
struct CTPOutput 
{
  unsigned et;
  int ieta;
  unsigned iphi;
#define CTPOUTPUT // So we don't define this struct again
};  
#endif // ifndef CTPOUTPUT

class RCTCables
{
 public:
  explicit RCTCables(const ParameterSet& config);
  ~RCTCables();

  // Tower-level algorithm methods

  // Must use both of these (correctly) before most CTP Card methods will
  // work
  void setEcalDigis(const EcalTrigPrimDigiCollection& digisIn);
  void setHcalDigis(const HcalTrigPrimDigiCollection& digisIn);

  // Total Et sum (ecal+hcal)
  double globalEtSum() const;

  inline unsigned getNCards() const {return nCardsEta * nCardsPhi;}
  const CTPCard& getCard(unsigned ind) const {return *(cards.at(ind));}
  
  // Get top n digis from each card
  vector<vector<CTPOutput> > topNEcalCands(int n) const;
  vector<vector<CTPOutput> > topNHcalCands(int n) const;

  // Report centers and total energies of 3x3 tower E clusters
  vector<vector<CTPOutput> > eTowerClusters(const unsigned eClusterSeed) const;


  //Stage 1/region-level algorithm methods
  void setRegions(const L1CaloRegionCollection& regions);
  void setEGTCands(const L1CaloEmCollection& cands);
  vector<vector<CTPOutput> > s1Jets();
  vector<vector<CTPOutput> > s1RlxTau();
  vector<vector<CTPOutput> > s1IsoTau();
  vector<vector<CTPOutput> > s1RlxEG();
  vector<vector<CTPOutput> > s1IsoEG();
  vector<unsigned> s1PULevel() const;
  vector<unsigned> s1Et() const;
  vector<unsigned> s1Met() const;
  vector<unsigned> s1Ht();
  vector<unsigned> s1Mht();

  inline unsigned getNCardsS1() const {return nCardsEtaS1 * nCardsPhiS1;}
  CTPCard getCardS1(unsigned ind) const {return *(cardsS1.at(ind));}


 private:
  // Tower-level algo data

  // Bounds are the MAXIMUM index handled by a given card:
  // if iEtaBound[0] == 4, then tower 4 is handled by card 0, 5 is not.
  // Does not check for towers that span multiple indices.
  // These are card boundaries. Some cards may be HF with no ecal; these 
  // bounds don't know that.
  vector<int> iEtaBounds;
  const unsigned nCardsEta;
  vector<unsigned> iPhiBounds;
  const unsigned nCardsPhi;
  vector<CTPCard*> cards;
  const double ecalLSB_;

  // Tower-level helper algos

  //// Determines which cards should take a given digi. 
  // 0th entry is index of cards that owns digi, other entries are any cards
  // that have a copy of the digi as padding.
  vector<unsigned> getTowCTPIndex(int iEta, unsigned iPhi) const;



  // Stage 1/region-level algorithm data
  vector<unsigned> regEtaBounds;
  const unsigned nCardsEtaS1;
  vector<unsigned> regPhiBounds;
  const unsigned nCardsPhiS1;
  vector<CTPCard*> cardsS1;

  // Stage 1/region-level algorithm helper methods

  vector<unsigned> getRegCTPIndex(unsigned regEta, unsigned regPhi) const;

};
#endif /*end of include guard: RCTCables */
