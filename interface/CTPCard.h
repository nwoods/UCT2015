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
#include <math.h>

// User include files
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"

#include "L1Trigger/UCT2015/interface/helpers.h"


struct CTPOutput
{
  unsigned et;
  int ieta;
  unsigned iphi;

  char crate;
  char card;
  char region;
#define CTPOUTPUT // So we don't define this struct again
};

// Comparator for sorting CTPOutput structs
inline bool operator<(const CTPOutput& a, const CTPOutput& b)
{
  return a.et < b.et;
}

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
		   unsigned iPhiMin, unsigned iPhiMax,
		   unsigned regEtaMin, unsigned regEtaMax,
		   unsigned regPhiMin, unsigned regPhiMax);
  ~CTPCard();

  // Returns total et of towers (Ecal+Hcal). Ignores padding.
  double sumTowEt() const;

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
  vector<CTPOutput> eTowerClusters() const;



  // Region-level algo methods
  unsigned s1Et() const;
  unsigned s1Met() const;
  unsigned s1Ht();
  unsigned s1Mht();
  void setRegions(L1CaloRegionCollection rgns); // PU corrects automatically
  void setEGTCands(L1CaloEmCollection cands);
  vector<CTPOutput> s1Jets();
  vector<CTPOutput> s1RlxEG();
  vector<CTPOutput> s1IsoEG();
  vector<CTPOutput> s1RlxTau();
  vector<CTPOutput> s1IsoTau();
  unsigned getS1PULevel() {return (regSet ? PULevel : 0);}



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
  const unsigned eClusterSeed;


  //// Digi collections assumed to include one extra tower on each end in both
  // eta and phi (unless the card is at minimum or maximum eta). These 
  // "padding" regions (duplicated and passed in by CIOX cables) 
  // allow us to compute clusters without passing data between cards. 
  // See long comment above min/max declarations for details on how to get the 
  // right digi 
  ////(tl;rd: use ecalDigis[getTowInd(iEta,iPhi)] to get the right one).
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



  // Region-level algo data
  L1CaloRegionCollection regions;
  L1CaloEmCollection egtCands;

  const unsigned jetSeed;
  const unsigned regEtaMin;
  const unsigned regEtaMax;
  const unsigned regPhiMin;
  const unsigned regPhiMax;
  const unsigned nRegEta;
  const unsigned nRegPhi;
  bool regSet;
  bool candsSet;
  vector<double> sinphi;
  vector<double> cosphi;
  unsigned PULevel;
  bool jetsFound;
  bool candsSorted;
  const unsigned PUEtMax;
  unsigned ht;
  unsigned mht;
  vector<CTPOutput> jets;
  vector<CTPOutput> rlxTaus;
  vector<CTPOutput> isoTaus;
  vector<CTPOutput> rlxEGs;
  vector<CTPOutput> isoEGs;
  const unsigned nCandsOut;
  const unsigned nJetsOut;
  const double coreRelIsoCut;
  const double jetRelIsoCut;

  // Region-level algo methods
  unsigned getRegInd(unsigned regEta, unsigned regPhi) const;
  bool areRegionsSet() const; // throws error if not
  bool areCandsSet() const; // throws error if not
  void PUCorrect(); // sets PULevel and adjusts rank of all regions
  void makeJetClusters(); // finds jets, places in vector jets
  void sortCands();
};


#endif /*end of include guard: CTPCARD */
