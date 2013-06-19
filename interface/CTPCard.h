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

struct CTPOutput
{
  unsigned et;
  int ieta;
  unsigned iphi;
#define CTPOUTPUT // So we don't define this struct again
};  

using namespace std;
using namespace edm;
using std::vector;



class CTPCard
{
 public:
  // In the constructor, iEtaMin, iEtaMax, iPhiMin, iPhiMax refer only to data
  // "owned" by this card and do not include "padding" regions on each side.
  // See long comments below for details.
  explicit CTPCard(double ecalLSB,
		   int iEtaMin, int iEtaMax, 
		   unsigned iPhiMin, unsigned iPhiMax);
  ~CTPCard();

  // Returns total et of region (Ecal+Hcal). Ignores padding.
  double sumEt() const;

  // Return top n digis sorted highest to lowest et. Ignores padding.
  vector<CTPOutput> topNEcalCands(unsigned n) const;
  vector<CTPOutput> topNHcalCands(unsigned n) const;

  // Sets or resets digi collections to digis. Both setEcalDigis and 
  // setHcalDigis must be used (correctly) at least once before most methods
  // will work. Everything assumes digis are in eta order, and within each
  // eta, are in phi order. Must include padding.
  void setEcalDigis(EcalTrigPrimDigiCollection digis);
  void setHcalDigis(HcalTrigPrimDigiCollection digis);

  // Finds 3x3 tower ecal clusters where the center has raw input 
  // et > eClusterSeed and the center has the most et (or tied for most,
  // depending on geometry--see implementation). Returns vector where each item
  // is et of all 9 towers and location of center tower.
  vector<CTPOutput> eTowerClusters(unsigned eClusterSeed) const;


 private:
  const double ecalLSB;


  //// Min and max indices, nTow and nReg variables exclude padding. So, if 
  // iEtaMin=5,iEtaMax=10,iPhiMin=7,iPhiMax=12==>nTowEta=6,nTowPhi=6,
  // The digi collection actually runs iEta=4->11, iPhi=6->13, and covers
  // an 8x8 tower area rather than the stated 6x6 tower area. This is done
  // so that the padding remains "hidden" and no towers are double counted
  // by the RCT. nDigis variables DO include padding, because they are just for
  // error checking.
  //// Therefore you must ALWAYS use e/hcalDigis[getTowerInd(iEta,iPhi)]
  // to get the correct tower. Don't ever loop over all digis or try to get 
  // indices by hand with something like (iEta-iEtaMin)*nTowPhi+(iPhi-iPhiMin).
  // Aside from missing the possibility that phi is wrapped around and 
  // iPhiMin>iPhi, you will probably get the wrong item because you don't 
  // account for the padding.
  const int iEtaMinHcal; 
  const int iEtaMaxHcal;
  const int iEtaMinEcal; 
  const int iEtaMaxEcal;
  const unsigned iPhiMin;
  const unsigned iPhiMax;
  const unsigned nTowEtaEcal;
  const unsigned nTowEtaHcal;
  const unsigned nTowPhi;
  bool ecalSet;
  bool hcalSet;
  const bool allHF;


  //// Digi collections assumed to include one extra tower on each end in both
  // eta and phi (unless the card is at minimum or maximum eta). These 
  // "padding" regions (duplicated and passed in by CIOX cables) 
  // allow us to compute clusters without passing data between cards. 
  // See long comment above min/max declarations for details on how to get the 
  // right digi (tl;rd: use ecalDigis[getTowInd(iEta,iPhi)] to get the right 
  // one).
  EcalTrigPrimDigiCollection ecalDigis;
  HcalTrigPrimDigiCollection hcalDigis;

  // Use e/hcalDigis[getTowIndE/Hcal(iEta,iPhi)] to get the tower you want, 
  // even if it's owned by another card. It works for adjacent towers and 
  // that's all the hardware can do anyway.
  unsigned getTowIndEcal(int iEta, unsigned iPhi) const;
  unsigned getTowIndHcal(int iEta, unsigned iPhi) const;

  // Throws error if ecalDigis and hcalDigis are different sizes or both empty.
  // Otherwise returns true. 
  bool areDigisSet() const;

  // Helpers to print a digi collection for debugging. Includes padding.
  void printDigis(const EcalTrigPrimDigiCollection& digis);
  void printDigis(const HcalTrigPrimDigiCollection& digis);
};


#endif /*end of include guard: CTPCARD */
