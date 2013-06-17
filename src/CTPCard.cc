#include "L1Trigger/UCT2015/interface/CTPCard.h"

#define DEBUG

CTPCard::CTPCard(double ecalLSB, 
		 int iEtaMin, int iEtaMax,
		 unsigned iPhiMin, unsigned iPhiMax) 
  : ecalLSB(ecalLSB),
    iEtaMin(iEtaMin),
    iEtaMax(iEtaMax),
    iPhiMin(iPhiMin),
    iPhiMax(iPhiMax),
    nTowEta((iEtaMin*iEtaMax < 0 ? // Don't save room for iEta=0
	     iEtaMax-iEtaMin : 
	     iEtaMax-iEtaMin+1)),
    nTowPhi((iPhiMin < iPhiMax ? // Get right # of phi indices even if wrapped
	     iPhiMax-iPhiMin+1 :
	     iPhiMax+73-iPhiMin))
{
  ecalDigis = EcalTrigPrimDigiCollection();
  hcalDigis = HcalTrigPrimDigiCollection();
}

CTPCard::~CTPCard() {}

bool CTPCard::areDigisSet() const
{
  bool response = false;

  if(ecalDigis.size() == 0)
    throw cms::Exception("EmptyCollection") << "Ecal digis never initialized."
					    << endl;
  if(hcalDigis.size() == 0)
    throw cms::Exception("EmptyCollection") << "Hcal digis never initialized."
					    << endl;

  if(ecalDigis.size() == hcalDigis.size() && ecalDigis.size() != 0)
    response = true;
  else
    throw cms::Exception("BadCollection") << "Ecal and Hcal digi collections "
					  << "are not the same size." 
					  << endl;
  return response;
}

double CTPCard::sumEt() const
{  

  if(!areDigisSet())
    return -1;

  double Et = 0;
  for(int iEta = iEtaMin; iEta <= iEtaMax; ++iEta)
    {
      if(iEta == 0) continue;

      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax%72)+1; 
	  iPhi = (iPhi%72)+1)
	{
	  Et += (double) hcalDigis[getTowInd(iEta,iPhi)].SOI_compressedEt();
	  Et += (double) (ecalDigis[getTowInd(iEta,iPhi)].compressedEt() 
			  * ecalLSB);
	}
    }

  return Et;
}

void CTPCard::setEcalDigis(EcalTrigPrimDigiCollection digis) 
{
  if(ecalDigis.size() == 0)
    {
      unsigned expectedSize = (nTowEta + 2) * (nTowPhi + 2);
      if(iEtaMin == -32 || iEtaMax == 32) // No padding at ends
	expectedSize -= (nTowPhi + 2);

//       cout << endl << "ecalDigis size (expected) = " << ecalDigis.size()
// 	   << "(" << expectedSize << ")" << endl
// 	   << "eta,phi,i,ind:" << endl;
   
//       cout << endl
// 	   << "ecalDigis size (expected) = " << digis.size()
// 	   << "(" << expectedSize << ") = " 
// 	   << (iEtaMin==-32||iEtaMax==32?nTowEta+1:nTowEta+2) 
// 	   << "*" << (nTowPhi+2) 
// 	   << endl << ") with eta(min,max) = (" << iEtaMin << ","
// 	   << iEtaMax << "), phi(min,max) = (" 
// 	   << iPhiMin << "," << iPhiMax << ")" << endl << endl
// 	   << "eta,phi,i,ind:" << endl << endl;
      
      if(digis.size() != expectedSize)
	{
	  throw cms::Exception("BadInput") << "Wrong number of ecal digis for "
					   << "this card." << endl;
	}

      ecalDigis = EcalTrigPrimDigiCollection(expectedSize);

      for(unsigned i = 0; i < expectedSize; ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  ecalDigis[getTowInd(e,p)] = digis[i];
	}
    }
  else
    {
      if(digis.size() != ecalDigis.size())
	throw cms::Exception("BadInput") << "Wrong number of ecal digis in new"
					 << " collection" << endl;

      for(unsigned i = 0; i < ecalDigis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  ecalDigis[getTowInd(e,p)] = digis[i];
	}	  
    }
}


void CTPCard::setHcalDigis(HcalTrigPrimDigiCollection digis) 
{
  if(hcalDigis.size() == 0)
    {
      unsigned expectedSize = (nTowEta + 2) * (nTowPhi + 2);
      if(iEtaMin == -32 || iEtaMax == 32) // No padding at ends
	expectedSize -= (nTowPhi + 2);
      
//       cout << endl << "hcalDigis size (expected) = " << hcalDigis.size()
// 	   << "(" << expectedSize << ")" << endl
// 	   << "eta,phi,i,ind:" << endl;
      
//       cout << endl
// 	   << "hcalDigis size (expected = " << digis.size()
// 	   << "(" << expectedSize << ") = " 
// 	   << (iEtaMin==-32||iEtaMax==32?nTowEta+1:nTowEta+2) 
// 	   << "*" << (nTowPhi+2) 
// 	   << endl << ") with eta(min,max) = (" << iEtaMin << ","
// 	   << iEtaMax << "), phi(min,max) = (" 
// 	   << iPhiMin << "," << iPhiMax << ")" << endl << endl
// 	   << "eta,phi,i,ind:" << endl << endl;
      
      if(digis.size() != expectedSize)
	{
	  throw cms::Exception("BadInput") << "Wrong number of hcal digis for "
					   << "this card." << endl;
	}

      hcalDigis = HcalTrigPrimDigiCollection(expectedSize);
      for(unsigned i = 0; i < expectedSize; ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

// 	  cout << e << "," << p << "," << i 
// 	       << "," << getTowInd(e,p) << endl;

	  hcalDigis[getTowInd(e,p)] = digis[i];
	}
    }
  else
    {
      if(digis.size() != hcalDigis.size())
	throw cms::Exception("BadInput") << "Wrong number of hcal digis in new"
					 << " collection" << endl;

      for(unsigned i = 0; i < hcalDigis.size(); ++i)
	{
	  int e = digis[i].id().ieta();
	  unsigned p = digis[i].id().iphi();

	  hcalDigis[getTowInd(e,p)] = digis[i];
	}	  
    }
}

vector<CTPOutput> CTPCard::topNEcalCands(unsigned n) const
{
  if(n==0)
    {
      cout << "WARNING: returning top 0 Ecal cands" << endl;
      return vector<CTPOutput>();
    }
  if(!areDigisSet())
    return vector<CTPOutput>();

  vector<unsigned> topCands = vector<unsigned>(n,0);
  vector<unsigned> topCandsEt = vector<unsigned>(n,0);

  for(int iEta = iEtaMin; iEta <= iEtaMax; ++iEta)
    {
      if(iEta==0) continue;
      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned candEt = ecalDigis[getTowInd(iEta,iPhi)].compressedEt();

	  if(candEt <= topCandsEt.back())
	    continue;

	  unsigned i = getTowInd(iEta,iPhi); // digi index

	  topCands.back() = i;
	  topCandsEt.back() = candEt;
	  int temp = n - 1;

	  while(topCandsEt.at(temp) > topCandsEt.at(temp-1))
	    {
	      topCands.at(temp) = topCands.at(temp-1);
	      topCands.at(temp-1) = i;
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

  for(int iEta = iEtaMin; iEta <= iEtaMax; ++iEta)
    {
      if(iEta==0) continue;
      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned candEt = hcalDigis[getTowInd(iEta,iPhi)].SOI_compressedEt();

	  if(candEt <= topCandsEt.back())
	    continue;

	  unsigned i = getTowInd(iEta,iPhi); // digi index

	  topCands.back() = i;
	  topCandsEt.back() = candEt;
	  int temp = n - 1;

	  while(topCandsEt.at(temp) > topCandsEt.at(temp-1))
	    {
	      topCands.at(temp) = topCands.at(temp-1);
	      topCands.at(temp-1) = i;
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

vector<CTPOutput> CTPCard::eTowerClusters(unsigned eClusterSeed) const
{
  if(!areDigisSet())
    return vector<CTPOutput>();

  vector<CTPOutput> output = vector<CTPOutput>();

  for(int iEta = iEtaMin; iEta <= iEtaMax; ++iEta)
    {
      if(iEta == 0) continue;

      for(unsigned iPhi = iPhiMin; 
	  iPhi != (iPhiMax % 72) + 1; 
	  iPhi = (iPhi % 72) + 1)
	{
	  unsigned ind = getTowInd(iEta,iPhi);
	  double centerEt = ecalDigis[ind].compressedEt();
	  if((centerEt <= eClusterSeed))
	    continue;
	  
	  double et = centerEt * ecalLSB;

	  bool clusterCenter = true;
	  for(int etaDir = (iEta == -32 ? 
			    0 : // if centerEta=-32, don't check below
			    -1); 
	      clusterCenter && // current tower must be center
		etaDir <= (iEta == 32 ?
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
		    ecalDigis[getTowInd(neighborEta,
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

unsigned CTPCard::getTowInd(int iEta, unsigned iPhi) const
{

#ifdef DEBUG
  // iEta must be valid, and must be within the card boundaries or the 
  // 1-column padding on either end (if it is padded--at the edge it's not)
  // and we must accound for the fact that -1 touches +1, not 0
  if(iEta == 0                  // Invalid
     || iEta > 32               // Invalid
     || iEta < -32              // Invalid
     || iEta < (iEtaMin == 1 ?  // Check if below lower pad
		-1 :            //     If the pad is just below 0
		iEtaMin - 1)    //     If we don't cross 0
     || iEta > (iEtaMax == -1 ? // Check if above upper pad 
		1 :             //     If the pad is just above 0
		iEtaMax + 1))   //     If we don't cross 0
    {
      throw cms::Exception("BadInput") << "Asked card for invalid tower eta " 
				       << iEta 
				       << " For the card with (min,max) = ("
				       << iEtaMin << "," << iEtaMax << ")"
				       << endl;
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

  if(iEta == iEtaMin-1 || (iEta == -1 && iEtaMin == 1))
    etaInd = 0; // Include other lower padding here to avoid unsigned(0)-1
  else //if(iEta >= iEtaMin)
    {
      etaInd = iEta - iEtaMin + 1;
      if(iEtaMin == -32) // No lower padding when we're at the bottom
	etaInd -= 1;
      if(iEta*iEtaMin < 0) // account for lack of eta=0
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


