#include "L1Trigger/UCT2015/interface/CTPCard.h"

#define DEBUG

CTPCard::CTPCard(double ecalLSB, 
		 int iEtaMin, int iEtaMax,
		 unsigned iPhiMin, unsigned iPhiMax,
		 unsigned regEtaMin, unsigned regEtaMax,
		 unsigned regPhiMin, unsigned regPhiMax)
  : ecalLSB(ecalLSB),
    iEtaMinHcal(iEtaMin),
    iEtaMaxHcal(iEtaMax),
    iEtaMinEcal((iEtaMin < -28 ? // Ecal can only be up to |iEta|=28
		 -28 :
		 (iEtaMin > 28 ?
		  28 :
		  iEtaMin))),
    iEtaMaxEcal((iEtaMax > 28 ?
		 28 :
		 (iEtaMax < -28 ? 
		  -28 :
		  iEtaMax))),
    iPhiMin(iPhiMin),
    iPhiMax(iPhiMax),
    nTowEtaEcal((iEtaMinEcal*iEtaMaxEcal < 0 ? // Don't save room for iEta=0
		 iEtaMaxEcal-iEtaMinEcal : 
		 iEtaMaxEcal-iEtaMinEcal+1)),
    nTowEtaHcal((iEtaMinHcal*iEtaMaxHcal < 0 ?
		 iEtaMaxHcal-iEtaMinHcal : 
		 iEtaMaxHcal-iEtaMinHcal+1)),
    nTowPhi((iPhiMin < iPhiMax ? // Get right # phi inds even if wrapped
	     iPhiMax-iPhiMin+1 :
	     iPhiMax+73-iPhiMin)),
    ecalSet(iEtaMinEcal==iEtaMaxEcal), //
    hcalSet(false),
    allHF(iEtaMinEcal==iEtaMaxEcal),
    eClusterSeed(5),
    jetSeed(7),
    regEtaMin(regEtaMin),
    regEtaMax(regEtaMax),
    regPhiMin(regPhiMin),
    regPhiMax(regPhiMax),
    nRegEta(regEtaMax - regEtaMin + 1),
    nRegPhi((regPhiMin < regPhiMax ? // Get right # phi inds even if wrapped
	     regPhiMax-regPhiMin+1 :
	     regPhiMax+19-regPhiMin)),
    regSet(false),
    candsSet(false),
    candsSorted(false),
    PUEtMax(7),
    nCandsOut(4),
    nJetsOut(6),
    coreRelIsoCut(.1),
    jetRelIsoCut(.1)
{
  ecalDigis = EcalTrigPrimDigiCollection();
  hcalDigis = HcalTrigPrimDigiCollection();

  regions = L1CaloRegionCollection();
  egtCands = L1CaloEmCollection();

  sinphi = vector<double>(18);
  cosphi = vector<double>(18);

  for(double i = 0; i < 10; ++i)
    {
      sinphi.at(i) = sin(i/9. * 3.1415927);
      cosphi.at(i) = cos(i/9. * 3.1415927);
    }
  for(double i = 10; i < 18; ++i)
    {
      sinphi.at(i) = sin((i/9. - 2.) * 3.1415927);
      cosphi.at(i) = cos((i/9. - 2.) * 3.1415927);
    }

}

CTPCard::~CTPCard() {}

bool CTPCard::areDigisSet() const
{
  bool response = false;

  if(!ecalSet)
    throw cms::Exception("EmptyCollection") << "Ecal digis never initialized."
					    << endl;
  if(!hcalSet)
    throw cms::Exception("EmptyCollection") << "Hcal digis never initialized."
					    << endl;
  response = true;
  return response;
}

double CTPCard::sumTowEt() const
{

  if(!areDigisSet())
    return -1;

  double Et = 0;

  for(int iEta = iEtaMinHcal; iEta <= iEtaMaxHcal; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax%72)+1; 
	  iPhi = (iPhi%72)+1)
	{
	  Et += (double) hcalDigis[getTowIndHcal(iEta,iPhi)].
	    SOI_compressedEt();
	}
    }

  if(!allHF)
    {
      for(int iEta = iEtaMinEcal; iEta <= iEtaMaxEcal; ++iEta)
	{
	  if(iEta == 0) continue;
	  for(unsigned iPhi = iPhiMin; 
	      iPhi != (iPhiMax%72)+1; 
	      iPhi = (iPhi%72)+1)
	    {
	      Et += (double) (ecalDigis[getTowIndEcal(iEta,iPhi)].
			      compressedEt() * ecalLSB);
	    }
	}
    }

  return Et;
}

void CTPCard::setEcalDigis(EcalTrigPrimDigiCollection digis) 
{
  if(allHF)
    return;

  if(ecalDigis.size() == 0)
    {
      unsigned expectedSize = (nTowEtaEcal + 2) * (nTowPhi + 2);

      if(iEtaMinEcal == -28) // No padding at left edge
	expectedSize -= (nTowPhi + 2);
      if(iEtaMaxEcal == 28) // No padding at right edge
	expectedSize -= (nTowPhi + 2);

      ecalDigis = EcalTrigPrimDigiCollection(expectedSize);

      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  ecalDigis[getTowIndEcal(e,p)] = digis[i];
	}
    }
  else
    {
      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  ecalDigis[getTowIndEcal(e,p)] = digis[i];
	}	  
    }

  // Undo zero suppression
  int etaStart;
  if(iEtaMinEcal == 1)
    etaStart = -1;
  else if(iEtaMinEcal == -28)
    etaStart = -28;
  else
    etaStart = iEtaMinEcal - 1;
  int etaEnd;
  if(iEtaMaxEcal == -1)
    etaEnd = 1;
  else if(iEtaMaxEcal == 28)
    etaEnd = 28;
  else
    etaEnd = iEtaMaxEcal + 1;
  unsigned phiStart;
  if(iPhiMin == 1)
    phiStart = 72;
  else
    phiStart = iPhiMin - 1;
  unsigned phiEnd = (iPhiMax % 72) + 1;
  //  cout << endl << "Ecal:" << endl;
  for(int iEta = etaStart; iEta <= etaEnd; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = phiStart; iPhi != (phiEnd%72)+1; iPhi = (iPhi%72)+1)
	{
	  //	  cout << iEta << "," << iPhi << endl;
	  unsigned ind = getTowIndEcal(iEta,iPhi);
	  if(ecalDigis[ind].id().null())
	    {
	      uint32_t id = 0;
	      id |= (iEta > 0 ? (0x8000) : 0); // iEta sign DIFFERENT FROM HCAL
	      id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7);
	      id |= iPhi;
	      EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	      EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	      ecalDigis[ind] = EcalTriggerPrimitiveDigi(id);
	      ecalDigis[ind].setSize(1);
	      ecalDigis[ind].setSample(0,EcalTriggerPrimitiveSample(0));
	    }
	}
    }

  ecalSet = true;
}


void CTPCard::setHcalDigis(HcalTrigPrimDigiCollection digis) 
{
  if(hcalDigis.size() == 0)
    {
      unsigned expectedSize = (nTowEtaHcal + 2) * (nTowPhi + 2);
      if(iEtaMinHcal == -32) // No padding at left edge
	expectedSize -= (nTowPhi + 2);
      if(iEtaMaxHcal == 32) // No padding at right edge
	expectedSize -= (nTowPhi + 2);

      hcalDigis = HcalTrigPrimDigiCollection(expectedSize);
      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  hcalDigis[getTowIndHcal(e,p)] = digis[i];
	}
    }
  else
    {
      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  hcalDigis[getTowIndHcal(e,p)] = digis[i];
	}	  
    }

  // Undo zero suppression
  int etaStart;
  if(iEtaMinHcal == 1)
    etaStart = -1;
  else if(iEtaMinHcal == -32)
    etaStart = -32;
  else
    etaStart = iEtaMinHcal - 1;
  int etaEnd;
  if(iEtaMaxHcal == -1)
    etaEnd = 1;
  else if(iEtaMaxHcal == 32)
    etaEnd = 32;
  else
    etaEnd = iEtaMaxHcal + 1;
  unsigned phiStart;
  if(iPhiMin == 1)
    phiStart = 72;
  else
    phiStart = iPhiMin - 1;
  unsigned phiEnd = (iPhiMax % 72) + 1;
  //  cout << endl << "Hcal: " << endl;
  for(int iEta = etaStart; iEta <= etaEnd; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = phiStart; iPhi != (phiEnd%72)+1; iPhi = (iPhi%72)+1)
	{
	  //	  cout << iEta << "," << iPhi << endl;
	  unsigned ind = getTowIndHcal(iEta,iPhi);
	  if(hcalDigis[ind].id().null())
	    {
	      HcalTrigTowerDetId id = HcalTrigTowerDetId(iEta,iPhi);
	      hcalDigis[ind] = HcalTriggerPrimitiveDigi(id);
	      hcalDigis[ind].setSize(1);
	      hcalDigis[ind].setSample(0,HcalTriggerPrimitiveSample(0));
	    }
	}
    }

  hcalSet = true;
}

vector<CTPOutput> CTPCard::topNEcalCands(unsigned n) const
{
  if(allHF)
    return vector<CTPOutput>();

  if(n==0)
    {
      cout << "WARNING: returning top 0 Ecal cands" << endl;
      return vector<CTPOutput>();
    }
  if(!areDigisSet())
    return vector<CTPOutput>();

  vector<unsigned> topCands = vector<unsigned>(n,0);
  vector<unsigned> topCandsEt = vector<unsigned>(n,0);

  for(int iEta = iEtaMinEcal; iEta <= iEtaMaxEcal; ++iEta)
    {
      if(iEta==0) continue;
      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned ind = getTowIndEcal(iEta,iPhi); // digi index

	  unsigned candEt = ecalDigis[ind].compressedEt();

	  if(candEt <= topCandsEt.back())
	    continue;

	  topCands.back() = ind;
	  topCandsEt.back() = candEt;
	  int temp = n - 1;

	  while(topCandsEt.at(temp) > topCandsEt.at(temp-1))
	    {
	      topCands.at(temp) = topCands.at(temp-1);
	      topCands.at(temp-1) = ind;
	      topCandsEt.at(temp) = topCandsEt.at(temp-1);
	      topCandsEt.at(temp-1) = candEt;

	      if(--temp == 0)
		break;
	    }
	}
    }
  
  vector<CTPOutput> out = vector<CTPOutput>(n);
  for(unsigned i = 0; i < n; ++i)
    {
      CTPOutput cand;
      cand.et = topCandsEt.at(i) * ecalLSB;
      if(cand.et == 0)
	{
	  cand.ieta = 0;
	  cand.iphi = 0;
	}
      else
	{
	  cand.ieta = ecalDigis[topCands.at(i)].id().ieta();
	  cand.iphi = ecalDigis[topCands.at(i)].id().iphi();
	}
      out.at(i) = cand;
    }
  
  return out;
}

vector<CTPOutput> CTPCard::topNHcalCands(unsigned n) const
{
  if(n==0)
    {
      cout << "WARNING: returning top 0 Hcal cands" << endl;
      return vector<CTPOutput>();
    }
  if(!areDigisSet())
    return vector<CTPOutput>();

  vector<unsigned> topCands = vector<unsigned>(n,0);
  vector<unsigned> topCandsEt = vector<unsigned>(n,0);

  for(int iEta = iEtaMinHcal; iEta <= iEtaMaxHcal; ++iEta)
    {
      if(iEta==0) continue;
      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned ind = getTowIndHcal(iEta,iPhi); // digi index

	  unsigned candEt = hcalDigis[ind].SOI_compressedEt();

	  if(candEt <= topCandsEt.back())
	    continue;

	  topCands.back() = ind;
	  topCandsEt.back() = candEt;
	  int temp = n - 1;

	  while(topCandsEt.at(temp) > topCandsEt.at(temp-1))
	    {
	      topCands.at(temp) = topCands.at(temp-1);
	      topCands.at(temp-1) = ind;
	      topCandsEt.at(temp) = topCandsEt.at(temp-1);
	      topCandsEt.at(temp-1) = candEt;

	      if(--temp == 0)
		break;
	    }
	}
    }
  
  vector<CTPOutput> out = vector<CTPOutput>(n);
  for(unsigned i = 0; i < n; ++i)
    {
      CTPOutput cand;
      cand.et = topCandsEt.at(i);
      if(cand.et == 0)
	{
	  cand.ieta = 0;
	  cand.iphi = 0;
	}
      else
	{
	  cand.ieta = hcalDigis[topCands.at(i)].id().ieta();
	  cand.iphi = hcalDigis[topCands.at(i)].id().iphi();
	}
      out.at(i) = cand;
    }
  
  return out;
}

vector<CTPOutput> CTPCard::eTowerClusters() const
{
  if(allHF)
    return vector<CTPOutput>();

  if(!areDigisSet())
    return vector<CTPOutput>();

  vector<CTPOutput> output = vector<CTPOutput>();

  for(int iEta = iEtaMinEcal; iEta <= iEtaMaxEcal; ++iEta)
    {
      if(iEta == 0) continue;

      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned ind = getTowIndEcal(iEta,iPhi);

	  double centerEt = ecalDigis[ind].compressedEt();
	  if((centerEt <= eClusterSeed))
	    continue;
	  
	  double et = centerEt * ecalLSB;

	  bool clusterCenter = true;
	  for(int etaDir = (iEta == -28 ? 
			    0 : // if centerEta=-28, don't check below
			    -1); 
	      clusterCenter && // current tower must be center
		etaDir <= (iEta == 28 ?
			   0 : // if centerEta=32, don't check above
			   1);  
	      ++etaDir)
	    {
	      for(int phiDir = -1;
		  phiDir <= 1 && clusterCenter;
		  ++phiDir)
		{
		  if(etaDir == 0 && phiDir == 0)
		    continue;
		  
		  int neighborEta;
		  if(iEta == -1 && etaDir == 1)
		    neighborEta = 1;
		  else if(iEta == 1 && etaDir == -1)
		    neighborEta = -1;
		  else
		    neighborEta = iEta + etaDir;

		  int neighborPhi;
		  if(iPhi == 1 && phiDir == -1)
		    neighborPhi = 72;
		  else if(iPhi == 72 && phiDir == 1)
		    neighborPhi = 1;
		  else
		    neighborPhi = iPhi + phiDir;
		  
		  unsigned neighborEt = 
		    ecalDigis[getTowIndEcal(neighborEta,
					    neighborPhi)].compressedEt();

		  if((neighborEt >= centerEt && (etaDir == 1 || 
						 (etaDir == 0 &&
						  phiDir == 1)))
		     || (neighborEt > centerEt && (etaDir == -1 ||
						   (etaDir == 0 &&
						    phiDir == -1))))
		    {
		      clusterCenter = false;
		      break;
		    }
		  et += neighborEt * ecalLSB;
		}
	    }
	  if(clusterCenter)
	    {
	      CTPOutput cluster;
	      cluster.et = et;
	      cluster.ieta = iEta;
	      cluster.iphi = iPhi;

	      output.push_back(cluster);
	    }
	}
    }

  if(output.size() == 0)
    {
      CTPOutput zeros;
      zeros.ieta = 0;
      zeros.iphi = 0;
      zeros.et = 0;

      output.push_back(zeros);
    }
  
  return output;
}

unsigned CTPCard::getTowIndEcal(int iEta, unsigned iPhi) const
{

#ifdef DEBUG
  // iEta must be valid, and must be within the card boundaries or the 
  // 1-column padding on either end (if it is padded--at the edge it's not)
  // and we must accound for the fact that -1 touches +1, not 0
  if(iEta == 0                      // Invalid
     || iEta > 28                   // Invalid
     || iEta < -28                  // Invalid
     || iEta < (iEtaMinEcal == 1 ?  // Check if below lower pad
		-1 :                //     If the pad is just below 0
		iEtaMinEcal - 1)    //     If we don't cross 0
     || iEta > (iEtaMaxEcal == -1 ? // Check if above upper pad 
		1 :                 //     If the pad is just above 0
		iEtaMaxEcal + 1))   //     If we don't cross 0
    {
      throw cms::Exception("BadInput") << "Asked card for invalid tower eta " 
				       << iEta 
				       << " For the card with (min,max) = ("
				       << iEtaMinEcal << "," << iEtaMaxEcal 
				       << ")" << endl;
    }

  // iPhi must be valid and within the card boundaries or 1-row padding. Must
  // check that the card does not wrap around from iPhi=72 to iPhi=1
  if(iPhi == 0                         // Invalid
     || iPhi > 72                      // Invalid
     || (iPhiMin < iPhiMax &&        //// Card doesn't wrap
	 ((iPhiMin == 1 &&             // If the padding wraps around below
	   (iPhi != 72                 //     But iPhi is not on it 
	    && iPhi > iPhiMax+1))      //     Or the card or other boundary
	  || (iPhiMax == 72 &&         // If the padding wraps around above
	      (iPhi != 1               //     But iPhi is not on it
	       && iPhi < iPhiMin-1))   //     Or the card or other boundary
	  || ((iPhiMin != 1            // If nothing wraps around
	       && iPhiMax != 72) &&    // But
	      (iPhi < iPhiMin-1        //     iPhi is below the bottom padding
	       || iPhi > iPhiMax+1)))) //     or above the top padding
     || (iPhiMin > iPhiMax &&        //// Card does wrap
	 (iPhi > iPhiMax+1             // And iPhi is between
	  && iPhi < iPhiMin-1)))       //     the two pads
    {
      throw cms::Exception("BadInput") << "Asked card for invalid tower phi " 
				       << iPhi
				       << " for the card with (min,max) = ("
				       << iPhiMin << "," << iPhiMax << ")"
				       << endl;
    }
#endif //ifdef DEBUG

  unsigned etaInd;

  if(iEta == iEtaMinEcal-1 || (iEta == -1 && iEtaMinEcal == 1))
    etaInd = 0; // Include other lower padding here to avoid unsigned(0)-1
  else //if(iEta >= iEtaMin)
    {
      etaInd = iEta - iEtaMinEcal + 1;
      if(iEtaMinEcal == -28) // No lower padding when we're at the bottom
	etaInd -= 1;
      if(iEta*iEtaMinEcal < 0) // account for lack of eta=0
	etaInd -= 1;
    }

  unsigned phiInd;
  if(iPhi >= iPhiMin)                   // Doesn't wrap, not in lower padding 
    phiInd = (iPhi - iPhiMin + 1) % 72; // (or iPhi=72 iPhiMin=1)
  else if(iPhi == iPhiMin - 1)          // Doesn't wrap, in lower padding
    phiInd = iPhiMin - iPhi - 1; 
  else                                  // Wraps
    phiInd = iPhi + 73 - iPhiMin; 

  return etaInd * (nTowPhi + 2) // 2 extra columns for padding
    + phiInd;
}

unsigned CTPCard::getTowIndHcal(int iEta, unsigned iPhi) const
{

#ifdef DEBUG
  // iEta must be valid, and must be within the card boundaries or the 
  // 1-column padding on either end (if it is padded--at the edge it's not)
  // and we must accound for the fact that -1 touches +1, not 0
  if(iEta == 0                          // Invalid
     || iEta > 32                       // Invalid
     || iEta < -32                      // Invalid
     || iEta < (iEtaMinHcal == 1 ?      // Check if below lower pad
		-1 :                    //     If the pad is just below 0
		iEtaMinHcal - 1)        //     If we don't cross 0
     || iEta > (iEtaMaxHcal == -1 ?     // Check if above upper pad 
		1 :                     //     If the pad is just above 0
		iEtaMaxHcal + 1))       //     If we don't cross 0
    {
      throw cms::Exception("BadInput") << "Asked card for invalid tower eta " 
				       << iEta 
				       << " For the card with (min,max) = ("
				       << iEtaMinHcal << "," << iEtaMaxHcal 
				       << ")" << endl;
    }

  // iPhi must be valid and within the card boundaries or 1-row padding. Must
  // check that the card does not wrap around from iPhi=72 to iPhi=1
  if(iPhi == 0                         // Invalid
     || iPhi > 72                      // Invalid
     || (iPhiMin < iPhiMax &&        //// Card doesn't wrap
	 ((iPhiMin == 1 &&             // If the padding wraps around below
	   (iPhi != 72                 //     But iPhi is not on it 
	    && iPhi > iPhiMax+1))      //     Or the card or other boundary
	  || (iPhiMax == 72 &&         // If the padding wraps around above
	      (iPhi != 1               //     But iPhi is not on it
	       && iPhi < iPhiMin-1))   //     Or the card or other boundary
	  || ((iPhiMin != 1            // If nothing wraps around
	       && iPhiMax != 72) &&    // But
	      (iPhi < iPhiMin-1        //     iPhi is below the bottom padding
	       || iPhi > iPhiMax+1)))) //     or above the top padding
     || (iPhiMin > iPhiMax &&        //// Card does wrap
	 (iPhi > iPhiMax+1             // And iPhi is between
	  && iPhi < iPhiMin-1)))       //     the two pads
    {
      throw cms::Exception("BadInput") << "Asked card for invalid tower phi " 
				       << iPhi
				       << " for the card with (min,max) = ("
				       << iPhiMin << "," << iPhiMax << ")"
				       << endl;
    }
#endif //ifdef DEBUG

  unsigned etaInd;

  if(iEta == iEtaMinHcal-1 || (iEta == -1 && iEtaMinHcal == 1))
    etaInd = 0; // Include other lower padding here to avoid unsigned(0)-1
  else //if(iEta >= iEtaMinHcal)
    {
      etaInd = iEta - iEtaMinHcal + 1;
      if(iEtaMinHcal == -32) // No lower padding when we're at the bottom
	etaInd -= 1;
      if(iEta*iEtaMinHcal < 0) // account for lack of eta=0
	etaInd -= 1;
    }

  unsigned phiInd;
  if(iPhi >= iPhiMin)                   // Doesn't wrap, not in lower padding 
    phiInd = (iPhi - iPhiMin + 1) % 72; // (or iPhi=72 iPhiMin=1)
  else if(iPhi == iPhiMin - 1)          // Doesn't wrap, in lower padding
    phiInd = iPhiMin - iPhi - 1; 
  else                                  // Wraps
    phiInd = iPhi + 73 - iPhiMin; 

  return etaInd * (nTowPhi + 2) // 2 extra columns for padding
    + phiInd;
}

// Prints all digis including padding
void CTPCard::printDigis(const EcalTrigPrimDigiCollection& digis)
{
  if(digis.size() > 0)
    {
      cout << endl << "Digis:" << endl << "#: (eta,phi,et):" << endl;
      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  cout << i << " (" << digis[i].id().ieta()
	       << "," << digis[i].id().iphi()
	       << "," << digis[i].compressedEt() << ")" << endl;
	}
    }
  else
    cout << endl << "EMPTY DIGI COLLECTION" << endl;
  cout << endl;
}

// Prints all digis including padding
void CTPCard::printDigis(const HcalTrigPrimDigiCollection& digis)
{
  if(digis.size() > 0)
    {
      cout << endl << "Digis:" << endl << "#: (eta,phi,et):" << endl;
      for(unsigned i = 0; i < digis.size(); ++i)
	{
	  cout << i << " (" << digis[i].id().ieta()
	       << "," << digis[i].id().iphi()
	       << "," << digis[i].SOI_compressedEt() << ")" << endl;
	}
    }
  else
    cout << endl << "EMPTY DIGI COLLECTION" << endl;
  cout << endl;
}





unsigned CTPCard::getRegInd(unsigned regEta, unsigned regPhi) const
{
  unsigned etaInd;

  etaInd = regEta - regEtaMin;
  if(regEtaMin != 0)
    etaInd += 1; // include padding if there is any

  unsigned phiInd;
  phiInd = (regPhi + 19 - regPhiMin) % 18;

  return etaInd * (nRegPhi + 2) // 2 extra columns for padding
    + phiInd;
}



void CTPCard::setRegions(L1CaloRegionCollection rgns)
{
  if(regions.size() == 0)
    {
      unsigned expectedSize = (nRegEta + 2) * (nRegPhi + 2);
      if(regEtaMin == 0) // No padding at left edge
	expectedSize -= (nRegPhi + 2);
      if(regEtaMax == 17) // No padding at right edge
	expectedSize -= (nRegPhi + 2);

      regions = L1CaloRegionCollection(expectedSize);
      for(unsigned i = 0; i < rgns.size(); ++i)
	{
	  int e = rgns.at(i).id().ieta();
	  unsigned p = rgns.at(i).id().iphi();

	  regions.at(getRegInd(e,p)) = rgns.at(i);
	}
    }
  else
    {
      for(unsigned i = 0; i < rgns.size(); ++i)
	{
	  int e = rgns.at(i).id().ieta();
	  unsigned p = rgns.at(i).id().iphi();

	  regions.at(getRegInd(e,p)) = rgns.at(i);
	}	  
    }

  // Undo zero suppression
  unsigned etaStart;
  if(regEtaMin == 0)
    etaStart = 0;
  else
    etaStart = regEtaMin - 1;
  unsigned etaEnd;
  if(regEtaMax == 17)
    etaEnd = 17;
  else
    etaEnd = regEtaMax + 1;
  unsigned phiStart = (regPhiMin + 17) % 18;
  unsigned phiEnd = (regPhiMax + 1) % 18;

  for(unsigned regEta = etaStart; regEta <= etaEnd; ++regEta)
    {
      for(unsigned regPhi = phiStart; 
	  regPhi != (phiEnd+1)%18; 
	  regPhi = (regPhi+1)%18)
	{
	  unsigned ind = getRegInd(regEta,regPhi);
	  if(regions.at(ind).empty())
	    {
	      regions.at(ind) = 
		L1CaloRegion::makeRegionFromGctIndices(0,
					 false,
					 false,
					 false,
					 false,
					 regEta,
					 regPhi);
	    }
	}
    }

  PUCorrect();

  regSet = true;
  jetsFound = false;
}



void CTPCard::setEGTCands(L1CaloEmCollection cands)
{
  egtCands = cands;

  candsSet = true;
  candsSorted = false;
}


void CTPCard::PUCorrect()
{
  PULevel = 0;
  unsigned puCount = 0;

  for(unsigned p = 0; p < regions.size(); ++p)
    {
      if(regions.at(p).et() <= PUEtMax)
	{
	  PULevel += regions.at(p).et();
	  ++puCount;
	}
    }

  PULevel /= puCount;

  for(unsigned q = 0; q < regions.size(); ++q)
    {
      uint16_t newData = regions.at(q).raw();
      
      if(regions.at(q).et() < PULevel) 
	{ 
	  // set ET to zero if all pileup
	  // this just zeros the last 8 (HF) or 11 (B/E) bits
	  if(regions.at(q).isHf())
	    newData &= 0xFF00;
	  else
	    newData &= 0xFC00;
	}
      else // otherwise just subtract PU
	newData -= PULevel;
      
      regions.at(q).setRawData(newData);
    }
}

unsigned CTPCard::s1Et() const
{
  unsigned et = 0;

  for(unsigned eta = regEtaMin; eta <= regEtaMax; ++eta)
    {
      for(unsigned phi = regPhiMin; phi != (regPhiMax+1)%18; phi = (phi+1)%18)
	{
	  et += regions.at(getRegInd(eta,phi)).et();
	}
    }

  return et;
}



unsigned CTPCard::s1Met() const
{
  double ex = 0.;
  double ey = 0.;
  for(unsigned eta = regEtaMin; eta <= regEtaMax; ++eta)
    {
      for(unsigned phi = regPhiMin; phi != (regPhiMax+1)%18; phi = (phi+1)%18)
	{
	  ex += regions.at(getRegInd(eta,phi)).et() * cosphi.at(phi);
	  ey += regions.at(getRegInd(eta,phi)).et() * sinphi.at(phi);
	}
    }

  return unsigned(sqrt(ex*ex + ey*ey));
}


unsigned CTPCard::s1Ht()
{
  if(!jetsFound)
    makeJetClusters();

  return ht;
}



unsigned CTPCard::s1Mht()
{
  if(!jetsFound)
    makeJetClusters();

  return mht;
}


void CTPCard::makeJetClusters()
{
  ht = 0;
  double hx = 0;
  double hy = 0;

  if(!areRegionsSet())
    return;

  if(jets.size() != 0)
    jets.clear();

  jets = vector<CTPOutput>();

  for(unsigned regEta = regEtaMin; regEta <= regEtaMax; ++regEta)
    {
      for(unsigned regPhi = regPhiMin; 
	  regPhi != (regPhiMax + 1) % 18; 
	  regPhi = (regPhi + 1) % 18)
	{
	  unsigned ind = getRegInd(regEta,regPhi);

	  double centerEt = regions.at(ind).et();
	  if((centerEt <= jetSeed))
	    continue;
	  
	  double et = centerEt;
	  double ex = centerEt * cosphi.at(regPhi);
	  double ey = centerEt * sinphi.at(regPhi);

	  bool clusterCenter = true;
	  for(int etaDir = (regEta == 0 ? 
			    0 : // if centerEta=0, don't check below
			    -1); 
	      clusterCenter && // current tower must be center
		etaDir <= (regEta == 21 ?
			   0 : // if centerEta=21, don't check above
			   1);
	      ++etaDir)
	    {
	      for(int phiDir = -1;
		  phiDir <= 1 && clusterCenter;
		  ++phiDir)
		{
		  if(etaDir == 0 && phiDir == 0)
		    continue;
		  
		  int neighborEta = regEta + etaDir;

		  int neighborPhi = (regPhi + 18 + phiDir) % 18;
	  
		  unsigned neighborEt = 
		    regions.at(getRegInd(neighborEta, neighborPhi)).et();

		  if((neighborEt >= centerEt && (etaDir == 1 || 
						 (etaDir == 0 &&
						  phiDir == 1)))
		     || (neighborEt > centerEt && (etaDir == -1 ||
						   (etaDir == 0 &&
						    phiDir == -1))))
		    {
		      clusterCenter = false;
		      break;
		    }
		  et += neighborEt;
		  ex += neighborEt * cosphi.at(neighborPhi);
		  ey += neighborEt * sinphi.at(neighborPhi);
		}
	    }
	  if(clusterCenter)
	    {
	      CTPOutput cluster;
	      cluster.et = et;
	      cluster.ieta = regEta;
	      cluster.iphi = regPhi;

	      jets.push_back(cluster);

	      ht += unsigned(et);
	      hx += ex;
	      hy += ey;
	    }
	}
    }

  mht = unsigned(sqrt(hx*hx + hy*hy));

  if(jets.size() == 0)
    {
      CTPOutput zeros;
      zeros.ieta = 0;
      zeros.iphi = 0;
      zeros.et = 0;

      jets.push_back(zeros);

      mht = 0;
    }
  
  jetsFound = true;
}



vector<CTPOutput> CTPCard::s1Jets()
{
  if(!jetsFound)
    makeJetClusters();

  return vector<CTPOutput>(jets.end() - (nJetsOut - 1), jets.end());
}


bool CTPCard::areRegionsSet() const
{
  bool response = false;

  if(!regSet)
    throw cms::Exception("EmptyCollection") << "RCT regions never initialized."
					    << endl;

  response = true;
  return response;
}


bool CTPCard::areCandsSet() const
{
  bool response = false;

  if(!candsSet)
    throw cms::Exception("EmptyCollection") 
      << "EM candidates never initialized."
      << endl;

  response = true;
  return response;
}


vector<CTPOutput> CTPCard::s1RlxTau()
{
  if(!candsSorted)
    sortCands();

  return vector<CTPOutput>(rlxTaus.end() - (nCandsOut - 1), rlxTaus.end());
}



vector<CTPOutput> CTPCard::s1IsoTau()
{
  if(!candsSorted)
    sortCands();

  return vector<CTPOutput>(isoTaus.end() - (nCandsOut - 1), isoTaus.end());
}



vector<CTPOutput> CTPCard::s1RlxEG()
{
  if(!candsSorted)
    sortCands();

  return vector<CTPOutput>(rlxEGs.end() - (nCandsOut - 1), rlxEGs.end());
}



vector<CTPOutput> CTPCard::s1IsoEG()
{
  if(!candsSorted)
    sortCands();

  return vector<CTPOutput>(isoEGs.end() - (nCandsOut - 1), isoEGs.end());
}



void CTPCard::sortCands()
{
  if(!candsSet)
    throw cms::Exception("EmptyData") << "Cannot sort cands--they are not set"
				      << endl;

  if(candsSorted)
    {
      rlxEGs.clear();
      isoEGs.clear();
      rlxTaus.clear();
      isoTaus.clear();
    }

  vector<CTPOutput> output = vector<CTPOutput>();

  for(unsigned i = 0; i < egtCands.size(); ++i)
    {
      L1CaloEmCand candIn = egtCands.at(i);
      CTPOutput candOut;
      candOut.et = candIn.rank();
      candOut.crate = candIn.rctCrate();
      candOut.card = candIn.rctCard();
      candOut.region = candIn.rctRegion();
      unsigned eta = candIn.regionId().ieta();
      unsigned phi = candIn.regionId().iphi();

      unsigned ind = getRegInd(eta,phi);
      L1CaloRegion reg = regions.at(ind);

      double coreRelIso = reg.et() - candOut.et / candOut.et;

      double jetEt = 0;
      for(int etaDir = (eta == 0 ? 
			0 : // if centerEta=0, don't check below
			-1); 
	  etaDir <= (eta == 21 ?
		     0 : // if centerEta=21, don't check above
		     1);
	  ++etaDir)
	{
	  for(int phiDir = -1; phiDir <= 1; ++phiDir)
	    {		  
	      int neighborEta = eta + etaDir;
	      int neighborPhi = (phi + 18 + phiDir) % 18;
	  
	      unsigned neighborEt = 
		regions.at(getRegInd(neighborEta, neighborPhi)).et();

	      jetEt += neighborEt;
	    }
	}
      double jetRelIso = (jetEt - candOut.et) / candOut.et;

      bool isolated = coreRelIso > coreRelIsoCut && 
	jetRelIso > jetRelIsoCut;

      rlxTaus.push_back(candOut);
      if(isolated)
	isoTaus.push_back(candOut);
      
      if(!reg.tauVeto() && !reg.mip()) // may need to change?
	{
	  rlxEGs.push_back(candOut);
	  if(isolated)
	    isoEGs.push_back(candOut);
	}
    }

  sort(rlxEGs.begin(),rlxEGs.end());
  sort(isoEGs.begin(),isoEGs.end());
  sort(rlxTaus.begin(),rlxTaus.end());
  sort(isoTaus.begin(),isoTaus.end());

  candsSorted = true;
}



