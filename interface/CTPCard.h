#ifndef CTPCARD
#define CTPCARD

/*
 * ============================================================     
 *    
 *       Filename:     CTPCard.h
 *    
 *    Description:     An emulator for the calorimeter trigger
 *                     processor card which can take edm handles
 *                     to hcal and ecal digi collections and return
 *                     run various algorithms
 *    
 *         Author:     Nate Woods, nwoods@hep.wisc.edu
 *    Institution:     UW Madison
 *    
 * ============================================================    
 */    

// System include files
#include <iostream>

// User include files
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"
#include "L1Trigger/UCT2015/interface/RCTCables.h"

class RCTCables; //Forward declaration

using namespace std;
using namespace edm;
using std::vector;

#ifndef RCTCABLES_h // make sure we don't define these twice
enum direction_t {NORTH, NEAST, EAST, SEAST, SOUTH, SWEST, WEST, NWEST};
#endif // ifndef RCTCABLES_h

struct CTPOutput;
/* { */
/*   unsigned et; */
/*   int ieta; */
/*   unsigned iphi; */
/* }; */


class CTPCard
{
 public:
  explicit CTPCard(double ecalLSB, const RCTCables* cables,
		   int iEtaMin, int iEtaMax, 
		   unsigned iPhiMin, unsigned iPhiMax);
  ~CTPCard();
  double sumEt() const;
  // outputs top n digis sorted highest to lowest et
  vector<CTPOutput> topNEcalCands(unsigned n) const;
  vector<CTPOutput> topNHcalCands(unsigned n) const;

/*   friend unsigned CTPCard::getNeighborTowerEt(int iEta, unsigned iPhi) const; */
/*   friend unsigned CTPCard::getNeighborRegionEt(int iEta, unsigned iPhi) const; */

  void setEcalDigis(EcalTrigPrimDigiCollection digis) {ecalDigis = digis;}
  void setHcalDigis(HcalTrigPrimDigiCollection digis) {hcalDigis = digis;}

/*   vector<CTPOutput> eTowerClusters(unsigned eClusterSeed, */
/* 				   const EcalTrigPrimDigiCollection&  */
/* 				   digis) const; */

 private:
  const double ecalLSB;
  const int iEtaMin;
  const int iEtaMax;
  const unsigned iPhiMin;
  const unsigned iPhiMax;
  const unsigned nTowEta;
  const unsigned nTowPhi;

  const RCTCables* cables;

  EcalTrigPrimDigiCollection ecalDigis;
  HcalTrigPrimDigiCollection hcalDigis;

/*   unsigned getNeighborTowerEt(int iEta, unsigned iPhi) const; */
/*   unsigned getNeighborRegionEt(int iEta, unsigned iPhi) const; */
/*   const CTPCard& getAdjacentCard(direction_t dir) const; */

};


#endif /*end of include guard: CTPCARD */
