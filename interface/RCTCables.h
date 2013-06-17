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

  //// Determines which cards should take a given digi. 
  // 0th entry is index of cards that owns digi, other entries are any cards
  // that have a copy of the digi as padding.
  vector<unsigned> getCTPIndex(int iEta, unsigned iPhi) const;

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

 private:
  // Bounds are the MAXIMUM index handled by a given card:
  // if iEtaBound[0] == 4, then tower 4 is handled by card 0, 5 is not.
  // Does not check for towers that span multiple indices.
  vector<int> iEtaBounds;
  const unsigned nCardsEta;
  vector<unsigned> iPhiBounds;
  const unsigned nCardsPhi;
  vector<CTPCard*> cards;
  const double ecalLSB_;

  /**** Actually, let's not do this. Let's let the cards sort the digis.****/
  //// SortedCollections put themselves into a stupid order when placed in an 
  // event. These sort by eta index, and within each eta sort by phi index.
  // (These only work on SortedCollections sorted automatically. Used on  
  // collections arranged in any other order this will give garbage). 
  //   EcalTrigPrimDigiCollection 
  //     resortCollection(const EcalTrigPrimDigiCollection& digis) const;
  //   HcalTrigPrimDigiCollection 
  //     resortCollection(const HcalTrigPrimDigiCollection& digis) const;

  // Helper to find a given digi in the incoming whole collection
  // (assumes digis are ordered as the Sorted collection does automatically).
  unsigned getDigiIndex(int iEta,unsigned iPhi) const;
};
#endif /*end of include guard: RCTCables */
