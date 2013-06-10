#include "L1Trigger/UCT2015/interface/RCTCables.h"

RCTCables::RCTCables(const ParameterSet& config) : 
  iEtaBounds(config.getParameter<vector<int> >("iEtaBounds")),
  nEtaRegions(iEtaBounds.size()),
  iPhiBounds(config.getParameter<vector<unsigned> >("iPhiBounds")),
  nPhiRegions(iPhiBounds.size()),
  ecalLSB_(config.getParameter<double>("ecalLSB"))
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

  for(unsigned eta = 0; eta < nEtaRegions; ++eta)
    {
      for(unsigned phi = 0; phi < nPhiRegions; ++phi)
	{
	  cards.push_back(new CTPCard(ecalLSB_, this,
				      (eta==0 ? -32 : iEtaBounds.at(eta-1)), 
				      iEtaBounds.at(eta),
				      (phi==0 ? 1 : iPhiBounds.at(phi-1)),
				      iPhiBounds.at(phi)));
	}
    }
}

RCTCables::~RCTCables() 
{
  for(unsigned q = 0; q < cards.size(); q++)
    delete cards.at(q);
}

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

EcalTrigPrimDigiCollection RCTCables::selectDigis(unsigned CTPIndex,
				       const EcalTrigPrimDigiCollection& 
				       digisIn) const
{
  EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection ();
  detRegions regions = RCTCables::recoverRegions(CTPIndex);
  
  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      // If the digi is in this region, return it
      if((digisIn[i].id().ieta() <= iEtaBounds.at(regions.etaReg)
	  && (regions.etaReg == 0 ? 
	      true : 
	      digisIn[i].id().ieta() > iEtaBounds.at(regions.etaReg-1)))
	 && (unsigned(digisIn[i].id().iphi()) <= 
	     iPhiBounds.at(regions.phiReg)
	     && (regions.phiReg == 0 ? 
		 true : 
		 (unsigned(digisIn[i].id().iphi()) 
		  > iPhiBounds.at(regions.phiReg-1)))))
	output.push_back(digisIn[i]);
    }
  return output;
}

HcalTrigPrimDigiCollection RCTCables::selectDigis(unsigned CTPIndex,
				       const HcalTrigPrimDigiCollection& 
				       digisIn) const
{
  HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection ();
  detRegions regions = RCTCables::recoverRegions(CTPIndex);
  
  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      // If the digi is in this region, return it
      if((digisIn[i].id().ieta() <= iEtaBounds.at(regions.etaReg)
	  && (regions.etaReg == 0 ? 
	      true : 
	      digisIn[i].id().ieta() > iEtaBounds.at(regions.etaReg-1)))
	 && (unsigned(digisIn[i].id().iphi()) <= 
	     iPhiBounds.at(regions.phiReg)
	     && (regions.phiReg == 0 ? 
		 true : 
		 (unsigned(digisIn[i].id().iphi()) 
		  > iPhiBounds.at(regions.phiReg-1)))))
	output.push_back(digisIn[i]);
    }
  return output;
}

