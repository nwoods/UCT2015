#include "L1Trigger/UCT2015/interface/RCTCables.h"

RCTCables::RCTCables(const ParameterSet& config) : 
  iEtaBounds(config.getParameter<vector<int> >("iEtaBounds")),
  nEtaRegions(iEtaBounds.size()),
  iPhiBounds(config.getParameter<vector<unsigned> >("iPhiBounds")),
  nPhiRegions(iPhiBounds.size())
{
  for(unsigned i = 0; i < nEtaRegions; ++i)
    if(iEtaBounds.at(i) < -32 
       || iEtaBounds.at(i) > 32 
       || iEtaBounds.at(i) == 0)
      throw cms::Exception("BadInput")
	<< "It looks like your eta boundaries are invalid" << std::endl;

  for(unsigned i = 0; i < nPhiRegions; ++i)
    if(iPhiBounds.at(i) > 72
       || iPhiBounds.at(i) == 0)
      throw cms::Exception("BadInput")
	<< "It looks like your phi boundaries are invalid" << std::endl;

  sort(iEtaBounds.begin(),iEtaBounds.end());
  sort(iPhiBounds.begin(),iPhiBounds.end());
}

RCTCables::~RCTCables() {}

unsigned RCTCables::getCTPIndex(int iEta, unsigned iPhi) const
{
  if(iEta < -32 || iEta > 32 || iEta == 0)
    throw cms::Exception("BadInput") << "Invalid Eta index" << std::endl;

  if(iPhi == 0 || iPhi > 72)
    throw cms::Exception("BadInput") << "Invalid Phi index" << std::endl;
  
  unsigned etaReg = 0;
  unsigned phiReg = 0;

  while(iEtaBounds.at(etaReg) < iEta)
    ++etaReg;

  while(iPhiBounds.at(phiReg) < iPhi)
    ++phiReg;

  return etaReg * nPhiRegions + phiReg;
}


