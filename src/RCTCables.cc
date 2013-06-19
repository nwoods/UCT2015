#include "L1Trigger/UCT2015/interface/RCTCables.h"

RCTCables::RCTCables(const ParameterSet& config) : 
  iEtaBounds(config.getParameter<vector<int> >("iEtaBounds")),
  nCardsEta(iEtaBounds.size()),
  iPhiBounds(config.getParameter<vector<unsigned> >("iPhiBounds")),
  nCardsPhi(iPhiBounds.size()),
  ecalLSB_(config.getParameter<double>("ecalLSB"))
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
				      (e==0 ? // Must be correct for column 0
				       -32 :
				       (iEtaBounds.at(e-1) == -1 ? // no eta=0
					1 : 
					iEtaBounds.at(e-1)+1)), 
				      iEtaBounds.at(e),
				      (p==0 ? // Must be correct for row 0
				       (iPhiBounds.at(nCardsPhi-1)%72)+1 : 
				       iPhiBounds.at(p-1)+1),
				      iPhiBounds.at(p));
	  cards.push_back(card);
	}
    }
}

RCTCables::~RCTCables()
{
  for(int i = cards.size()-1; i >= 0; --i)
    delete cards.at(i);
}
    

vector<unsigned> RCTCables::getCTPIndex(int iEta, unsigned iPhi) const
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
//   cout << endl << "For ieta = -26, iphi = 71, CTP index = " << endl;
//   vector<unsigned> c = getCTPIndex(-26,71);
//   for(unsigned d = 0; d < c.size(); ++d)
//     cout << c.at(d) << ",";
//   cout << endl << endl;
  

//   if(digisIn.size() != 4032)
//     {
//       throw cms::Exception("BadInput") << "Wrong number of Ecal digis ("
// 				       << digisIn.size() << ")"
// 				       << endl;
//     }
  
  vector <EcalTrigPrimDigiCollection> digiSets = 
    vector<EcalTrigPrimDigiCollection>(getNCards());

  //  EcalTrigPrimDigiCollection digis = resortCollection(digisIn);

  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      int iEta = digisIn[i].id().ieta();
      unsigned iPhi = digisIn[i].id().iphi();
      vector<unsigned> cardInds = getCTPIndex(iEta,iPhi);

//       if(digisIn[i].compressedEt() > 0)
// 	{
// 	  cout << endl << "Digi (" << digisIn[i].id().ieta()
// 	       << "," << digisIn[i].id().iphi()
// 	       << "," << digisIn[i].compressedEt()
// 	       << ")" << endl << "Will be placed in card(s)" << endl;
// 	}

      for(unsigned j = 0; j < cardInds.size(); ++j)
	{
	  digiSets.at(cardInds.at(j)).push_back(digisIn[i]);
// 	  if(digisIn[i].compressedEt() > 0) 
// 	    cout << cardInds.at(j) << " " << endl;
	}
    }

  for(unsigned k = 0; k < digiSets.size(); ++k)
    {
//       cout << endl << "Digis going in:" << endl;
//       for(unsigned q = 0; q < digiSets.at(k).size(); ++q)
// 	{
// 	  cout << digiSets.at(k)[q].id().ieta() << ","
// 	       << digiSets.at(k)[q].id().iphi() << ","
// 	       << digiSets.at(k)[q].compressedEt() << ",";

// 	  vector<unsigned> a = getCTPIndex(digiSets.at(k)[q].id().ieta(),
// 					   digiSets.at(k)[q].id().iphi());
// 	  for(unsigned b = 0; b < a.size(); ++b)
// 	    cout << a.at(b) << ",";
// 	  cout << endl;
	  
// 	}
      cards.at(k)->setEcalDigis(digiSets.at(k));
    }
}

void RCTCables::setHcalDigis(const HcalTrigPrimDigiCollection& digisIn)
{
//   if(digisIn.size() != 4608)
//     {
//       cout << endl << "Towers coming in:" << endl;
//       for(unsigned i = 0; i < digisIn.size(); ++i)
// 	{
// 	  cout << "(" << digisIn[i].id().ieta()
// 	       << "," << digisIn[i].id().iphi()
// 	       << "," << digisIn[i].SOI_compressedEt() << ")"
// 	       << endl;
// 	}
//       cout << endl << "Number of hcal digis: " << digisIn.size() << endl;
//       throw cms::Exception("BadInput") << "Wrong number of Hcal digis"
// 				       << endl;
//     }

  vector <HcalTrigPrimDigiCollection> digiSets = 
    vector<HcalTrigPrimDigiCollection>(getNCards());

  //  HcalTrigPrimDigiCollection digis = resortCollection(digisIn);

  for(unsigned i = 0; i < digisIn.size(); ++i)
    {
      int iEta = digisIn[i].id().ieta();
      unsigned iPhi = digisIn[i].id().iphi();
      vector<unsigned> cardInds = getCTPIndex(iEta,iPhi);

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
      et += cards.at(i)->sumEt();

  return et;
}


vector<vector<CTPOutput> > RCTCables::eTowerClusters(const unsigned 
						     eClusterSeed) const
{
  vector<vector<CTPOutput> > output = vector<vector<CTPOutput> >();

  for(unsigned i = 0; i < cards.size(); ++i)
    {
      vector<CTPOutput> thisOut = cards.at(i)->eTowerClusters(eClusterSeed);

      if(thisOut.size() == 0 || thisOut.at(0).et == 0)
	continue;
      
      output.push_back(thisOut);
    }
  
  return output;
}
