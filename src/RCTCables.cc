#include "L1Trigger/UCT2015/interface/RCTCables.h"

RCTCables::RCTCables(const ParameterSet& config) : 
  iEtaBounds(config.getParameter<vector<int> >("iEtaBounds")),
  nCardsEta(iEtaBounds.size()),
  iPhiBounds(config.getParameter<vector<unsigned> >("iPhiBounds")),
  nCardsPhi(iPhiBounds.size()),
  ecalLSB_(config.getParameter<double>("ecalLSB")),
  regEtaBounds(config.getParameter<vector<unsigned> >("regEtaBounds")),
  nCardsEtaS1(regEtaBounds.size()),
  regPhiBounds(config.getParameter<vector<unsigned> >("regPhiBounds")),
  nCardsPhiS1(regPhiBounds.size())
{
  for(unsigned i = 0; i < nCardsEta; ++i)
    if(iEtaBounds.at(i) < -32 
       || iEtaBounds.at(i) > 32 
       || iEtaBounds.at(i) == 0)
      throw cms::Exception("BadInput")
	<< "It looks like your eta boundaries are invalid" << std::endl;
  
  for(unsigned i = 0; i < nCardsPhi; ++i)
    if(iPhiBounds.at(i) > 72
       || iPhiBounds.at(i) == 0)
      throw cms::Exception("BadInput")
	<< "It looks like your phi boundaries are invalid" << std::endl;

  sort(iEtaBounds.begin(),iEtaBounds.end());
  sort(iPhiBounds.begin(),iPhiBounds.end());
  
  for(unsigned e = 0; e < nCardsEta; ++e)
    {
      for(unsigned p = 0; p < nCardsPhi; ++p)
	{
	  CTPCard* card = new CTPCard(ecalLSB_, // LSB
				      // iEtaMin
				      (e==0 ? // Must be correct for column 0
				       -32 :
				       (iEtaBounds.at(e-1) == -1 ? // no eta=0
					1 : 
					iEtaBounds.at(e-1)+1)), 
				      // iEtaMax
				      iEtaBounds.at(e),
				      // iPhiMin
				      (p==0 ? // Must be correct for row 0
				       (iPhiBounds.at(nCardsPhi-1)%72)+1 : 
				       iPhiBounds.at(p-1)+1),
				      // iPhiMax
				      iPhiBounds.at(p),
				      // reg eta,phi min,max (dummy)
				      1,2,1,2);
	  cards.push_back(card);
	}
    }

  sort(regEtaBounds.begin(),regEtaBounds.end());
  sort(regPhiBounds.begin(),regPhiBounds.end());

  for(unsigned e = 0; e < nCardsEtaS1; ++e)
    {
      for(unsigned p = 0; p < nCardsPhiS1; ++p)
	{
	  CTPCard* card = new CTPCard(ecalLSB_,
				      // tower eta,phi min,max (dummy)
				      1,2,1,2,
				      // regEtaMin
				      (e == 0 ? 0 : regEtaBounds.at(e-1)+1),
				      // regEtaMax
				      regEtaBounds.at(e),
				      // regPhiMin
				      (p == 0 ? 
				       (regPhiBounds.at(nCardsPhiS1-1) + 1)%18:
				       regPhiBounds.at(p-1)+1),
				      // regPhiMax
				      regPhiBounds.at(p));
	  cardsS1.push_back(card);
	}
    }
}

RCTCables::~RCTCables()
{
  for(int i = cards.size()-1; i >= 0; --i)
    delete cards.at(i);

  for(int i = cardsS1.size()-1; i >= 0; --i)
    delete cardsS1.at(i);
}
    

vector<unsigned> RCTCables::getTowCTPIndex(int iEta, unsigned iPhi) const
{
  if(iEta < -32 || iEta > 32 || iEta == 0)
    throw cms::Exception("BadInput") << "Invalid Eta index" << std::endl;

  if(iPhi == 0 || iPhi > 72)
    throw cms::Exception("BadInput") << "Invalid Phi index" << std::endl;
  
  unsigned etaReg = 0;
  unsigned phiReg = 0;
  vector<unsigned> out;

  while(iEtaBounds.at(etaReg) < iEta)
    ++etaReg;

  if(iPhi <= iPhiBounds.at(nCardsPhi-1)) // otherwise wraps to 0
    {
      while(iPhiBounds.at(phiReg) < iPhi && phiReg < nCardsPhi)
	++phiReg;
    }

  out.push_back(etaReg * nCardsPhi + phiReg);

  int etaRegShift = 0;
  int phiRegShift = 0;

  if(etaReg != nCardsEta-1 && iEta == iEtaBounds.at(etaReg))
    etaRegShift = 1;
  else if(etaReg != 0)
    {
      // Do as two ifs to avoid accessing invalid vector element
      if(iEta == iEtaBounds.at(etaReg-1)+1
	 || (iEta == 1 && iEtaBounds.at(etaReg-1) == -1))
	{
	  etaRegShift = -1;
	}
    }

  if(iPhi == iPhiBounds.at(phiReg))
    phiRegShift = 1;
  //  else if(iPhi == 1 && iPhiBounds.at(nCardsPhi-1) == 72)
  else if(phiReg != 0 &&
	  iPhi == iPhiBounds.at(phiReg-1)+1)
    {
      phiRegShift = -1;
    }
  else if(phiReg == 0 && iPhi == (iPhiBounds.at(nCardsPhi-1)%72)+1)
    {
      phiRegShift = nCardsPhi - 1;
    }
    

  if(etaRegShift != 0)
    out.push_back((etaReg+etaRegShift) * nCardsPhi + phiReg);
  if(phiRegShift != 0)
    {
      out.push_back(etaReg * nCardsPhi 
		    + ((phiReg + phiRegShift) % iPhiBounds.size()));
      if(etaRegShift != 0)
	out.push_back((etaReg+etaRegShift) * nCardsPhi 
		      + ((phiReg + phiRegShift) % iPhiBounds.size()));
    }

  return out;
}

void RCTCables::setEcalDigis(const EcalTrigPrimDigiCollection& digisIn)
{  
  vector <EcalTrigPrimDigiCollection> digiSets = 
    vector<EcalTrigPrimDigiCollection>(getNCards());

  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      int iEta = digisIn[i].id().ieta();
      unsigned iPhi = digisIn[i].id().iphi();
      vector<unsigned> cardInds = getTowCTPIndex(iEta,iPhi);

      for(unsigned j = 0; j < cardInds.size(); ++j)
	{
	  digiSets.at(cardInds.at(j)).push_back(digisIn[i]);
	}
    }

  for(unsigned k = 0; k < digiSets.size(); ++k)
    {
      cards.at(k)->setEcalDigis(digiSets.at(k));
    }
}

void RCTCables::setHcalDigis(const HcalTrigPrimDigiCollection& digisIn)
{
  vector <HcalTrigPrimDigiCollection> digiSets = 
    vector<HcalTrigPrimDigiCollection>(getNCards());

  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      int iEta = digisIn[i].id().ieta();
      unsigned iPhi = digisIn[i].id().iphi();
      vector<unsigned> cardInds = getTowCTPIndex(iEta,iPhi);

      for(unsigned j = 0; j < cardInds.size(); ++j)
	digiSets.at(cardInds.at(j)).push_back(digisIn[i]);
    }

  for(unsigned k = 0; k < digiSets.size(); ++k)
    cards.at(k)->setHcalDigis(digiSets.at(k));
}

vector<vector<CTPOutput> > RCTCables::topNEcalCands(int n) const
{
  vector<vector<CTPOutput> > output = vector<vector<CTPOutput>>();

  for(unsigned i = 0; i < cards.size(); ++i)
    {
      vector<CTPOutput> thisOut = cards.at(i)->topNEcalCands(n);
      if(thisOut.at(0).et > 0)
	output.push_back(thisOut);
    }

  return output;
}

vector<vector<CTPOutput> > RCTCables::topNHcalCands(int n) const
{
  vector<vector<CTPOutput> > output = vector<vector<CTPOutput> >();

  for(unsigned i = 0; i < cards.size(); ++i)
    {
      vector<CTPOutput> thisOut = cards.at(i)->topNHcalCands(n);
      if(thisOut.at(0).et > 0)
	output.push_back(thisOut);
    }

  return output;
}


double RCTCables::globalEtSum() const
{
  double et = 0;
  for(unsigned i = 0; i < cards.size(); ++i)
    et += cards.at(i)->sumTowEt();

  return et;
}


vector<vector<CTPOutput> > RCTCables::eTowerClusters(const unsigned 
						     eClusterSeed) const
{
  vector<vector<CTPOutput> > output = vector<vector<CTPOutput> >();

  for(unsigned i = 0; i < cards.size(); ++i)
    {
      vector<CTPOutput> thisOut = cards.at(i)->eTowerClusters();

      if(thisOut.size() == 0 || thisOut.at(0).et == 0)
	continue;
      
      output.push_back(thisOut);
    }
  
  return output;
}



//// Stage 1/region-level algorithm methods & helpers

vector<unsigned> RCTCables::getRegCTPIndex(unsigned iEta, unsigned iPhi) const
{
  if(iEta > 21)
    throw cms::Exception("BadInput") << "Invalid region Eta index" 
				     << endl;
  
  if(iPhi > 17)
    throw cms::Exception("BadInput") << "Invalid region Phi index" 
				     << endl;
  
  unsigned etaInd = 0;
  unsigned phiInd = 0;
  vector<unsigned> out;

  while(regEtaBounds.at(etaInd) < iEta)
    ++etaInd;

  if(iPhi <= regPhiBounds.at(nCardsPhiS1-1)) // otherwise wraps to 0
    {
      while(regPhiBounds.at(phiInd) < iPhi && phiInd < nCardsPhiS1)
	++phiInd;
    }

  out.push_back(etaInd * nCardsPhiS1 + phiInd);

  int etaIndShift = 0;
  int phiIndShift = 0;

  if(etaInd != nCardsEtaS1-1 && iEta == regEtaBounds.at(etaInd))
    etaIndShift = 1;
  else if(etaInd != 0)
    {
      // Do as two ifs to avoid accessing invalid vector element
      if(iEta == regEtaBounds.at(etaInd-1)+1)
	{
	  etaIndShift = -1;
	}
    }

  if(iPhi == regPhiBounds.at(phiInd))
    phiIndShift = 1;
  else if(phiInd == (regPhiBounds.at((phiInd + nCardsPhiS1 - 1) % nCardsPhiS1)
		     + 1) % 18) // if first phi in card
    {
      phiIndShift = -1;
    }

  bool etaShifted = etaIndShift != 0;
  if(etaShifted)
    out.push_back((etaInd+etaIndShift) * nCardsPhiS1 + phiInd);
  if(phiIndShift != 0)
    {
      out.push_back(etaInd * nCardsPhiS1 
		    + ((phiInd + nCardsPhiS1 + phiIndShift) % nCardsPhiS1));
      if(etaShifted)
	out.push_back((etaInd+etaIndShift) * nCardsPhiS1 
		      + ((phiInd + nCardsPhiS1 + phiIndShift) % nCardsPhiS1));
    }

  return out;
}


void RCTCables::setRegions(const L1CaloRegionCollection& regionsIn)
{
  vector <L1CaloRegionCollection> regColls = 
    vector<L1CaloRegionCollection>(getNCardsS1());

  for(unsigned i = 0; i < regionsIn.size(); ++i)
    {
      int iEta = regionsIn.at(i).id().ieta();
      unsigned iPhi = regionsIn.at(i).id().iphi();
      vector<unsigned> cardInds = getRegCTPIndex(iEta,iPhi);

      for(unsigned j = 0; j < cardInds.size(); ++j)
	regColls.at(cardInds.at(j)).push_back(regionsIn.at(i));
    }

  for(unsigned k = 0; k < regColls.size(); ++k)
    cardsS1.at(k)->setRegions(regColls.at(k));  
}


void RCTCables::setEGTCands(const L1CaloEmCollection& candsIn)
{
  vector<L1CaloEmCollection> emColls = 
    vector<L1CaloEmCollection>(getNCardsS1());

  for(unsigned i = 0; i < candsIn.size(); ++i)
    {
      unsigned iEta = candsIn.at(i).regionId().ieta();
      unsigned iPhi = candsIn.at(i).regionId().iphi();
      // don't care about candidates in padding region, so just use element 0
      unsigned cardInd = getRegCTPIndex(iEta,iPhi).at(0);

      emColls.at(cardInd).push_back(candsIn.at(i));
    }

  for(unsigned k = 0; k < emColls.size(); ++k)
    cardsS1.at(k)->setEGTCands(emColls.at(k));
}


vector<vector<CTPOutput> > RCTCables::s1Jets()
{
  vector<vector<CTPOutput> > out = 
    vector<vector<CTPOutput> >(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1Jets();
    }

  return out;
}


vector<vector<CTPOutput> > RCTCables::s1RlxTau()
{
  vector<vector<CTPOutput> > out = 
    vector<vector<CTPOutput> >(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1RlxTau();
    }

  return out;
}


vector<vector<CTPOutput> > RCTCables::s1IsoTau()
{
  vector<vector<CTPOutput> > out = 
    vector<vector<CTPOutput> >(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1IsoTau();
    }

  return out;
}


vector<vector<CTPOutput> > RCTCables::s1RlxEG() 
{
  vector<vector<CTPOutput> > out = 
    vector<vector<CTPOutput> >(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1RlxEG();
    }

  return out;
}


vector<vector<CTPOutput> > RCTCables::s1IsoEG() 
{
  vector<vector<CTPOutput> > out = 
    vector<vector<CTPOutput> >(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1IsoEG();
    }

  return out;
}


vector<unsigned> RCTCables::s1PULevel() const
{
  vector<unsigned> out = 
    vector<unsigned>(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->getS1PULevel();
    }

  return out;
}


vector<unsigned> RCTCables::s1Et() const
{
  vector<unsigned> out = 
    vector<unsigned>(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1Et();
    }

  return out;
}


vector<unsigned> RCTCables::s1Met() const
{
  vector<unsigned> out = 
    vector<unsigned>(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1Met();
    }

  return out;
}


vector<unsigned> RCTCables::s1Ht() 
{
  vector<unsigned> out = 
    vector<unsigned>(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1Ht();
    }

  return out;
}


vector<unsigned> RCTCables::s1Mht()
{
  vector<unsigned> out = 
    vector<unsigned>(getNCardsS1());

  for(unsigned i = 0; i < getNCardsS1(); ++i)
    {
      out.at(i) = cardsS1.at(i)->s1Mht();
    }

  return out;
}

