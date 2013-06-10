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

class CTPCard; // Forward declaration to make these things work

struct CTPOutput 
{
  unsigned et;
  int ieta;
  unsigned iphi;
};  

struct detRegions
{
  unsigned etaReg;
  unsigned phiReg;
};

enum direction_t {NORTH, NEAST, EAST, SEAST, SOUTH, SWEST, WEST, NWEST};

class RCTCables
{
 public:
  explicit RCTCables(const ParameterSet& config);
  ~RCTCables();
  unsigned getCTPIndex(int iEta, unsigned iPhi) const;

  void setEcalDigis(const EcalTrigPrimDigiCollection& digisIn);
  void setHcalDigis(const HcalTrigPrimDigiCollection& digisIn);

  double globalEtSum() const;

  inline unsigned getNCards() const {return nEtaRegions * nPhiRegions;}
  const CTPCard& getCard(unsigned ind) const {return *(cards.at(ind));}
  
/*   friend const CTPCard& CTPCard::getAdjacentCard(direction_t dir) const; */

  // get top n digis from each card
  vector<vector<CTPOutput> > topNEcalCands(int n) const;
  vector<vector<CTPOutput> > topNHcalCands(int n) const;


 private:
  // Bounds are the MAXIMUM index handled by a given card:
  // if iEtaBound[0] == 4, then tower 4 is handled by card 0, 5 is not.
  // Does not check for towers that span multiple indices.
  vector<int> iEtaBounds;
  vector<unsigned> iPhiBounds;
  const unsigned nEtaRegions;
  const unsigned nPhiRegions;
  vector<CTPCard*> cards;
  const double ecalLSB_;

  // Returns doublet [etaRegion, phiRegion]
  // Refers to internal organization, not region in actual detector
  inline detRegions recoverRegions(unsigned ind) const
    {
      detRegions output;
      output.phiReg = ind % nPhiRegions; // Phi region
      output.etaReg = (ind - output.phiReg) / nPhiRegions; // Eta region
      return output;
    }

};
#endif /*end of include guard: RCTCables */
