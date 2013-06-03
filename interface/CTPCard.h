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
using namespace std;
using namespace edm;
using std::vector;

struct CTPOutput 
{
  unsigned et;
  int ieta;
  unsigned iphi;
  bool isHcal; // if false, is ecal
};

class CTPCard
{
 public:
  explicit CTPCard(double ecalLSB);
  ~CTPCard();
  double sumEt
    (const HcalTrigPrimDigiCollection& hcal, 
     const EcalTrigPrimDigiCollection& ecal);
  // outputs top n digis sorted highest to lowest et
  vector<CTPOutput> topNCands(unsigned n, 
			      const EcalTrigPrimDigiCollection& digis);
  vector<CTPOutput> topNCands(unsigned n, 
			      const HcalTrigPrimDigiCollection& digis);


 private:
  const double ecalLSB;
};


#endif /*end of include guard: CTPCARD */
