#include "L1Trigger/UCT2015/interface/CTPCard.h"

CTPCard::CTPCard(double ecalLSB, const RCTCables* cables,
		 int iEtaMin, int iEtaMax,
		 unsigned iPhiMin, unsigned iPhiMax) 
  : ecalLSB(ecalLSB),
    iEtaMin(iEtaMin),
    iEtaMax(iEtaMax),
    iPhiMin(iPhiMin),
    iPhiMax(iPhiMax),
    nTowEta((iEtaMin*iEtaMax < 0 ? iEtaMax-iEtaMin-1 : iEtaMax-iEtaMin)),
    nTowPhi(iPhiMax-iPhiMin),
    cables(cables)
{}

CTPCard::~CTPCard() {}

double CTPCard::sumEt(const HcalTrigPrimDigiCollection& hcalIn, 
		      const EcalTrigPrimDigiCollection& ecalIn)
{  
  double Et = 0;
  for(unsigned i = 0; i < hcalIn.size(); i++)
    {
      HcalTriggerPrimitiveDigi hcal = hcalIn[i];

      Et += double(hcal.SOI_compressedEt());
    }
  for(unsigned i = 0; i < ecalIn.size(); i++)
    {
      EcalTriggerPrimitiveDigi ecal = ecalIn[i];
      Et += double(ecal.compressedEt()) * ecalLSB;
    }

  return Et;
}

vector<CTPOutput> CTPCard::topNCands(unsigned n, 
				     const EcalTrigPrimDigiCollection& 
				     digis) const
{
  if(n==0)
    {
      cout << "WARNING: returning top 0 Ecal cands" << endl;
      return vector<CTPOutput>();
    }

  vector<unsigned> topCands = vector<unsigned>(n,0);
  vector<unsigned> topCandsEt = vector<unsigned>(n,0);

  for(unsigned i = 0; i < digis.size(); ++i)
    {
      unsigned candEt = digis[i].compressedEt();

      if(candEt <= topCandsEt.back())
	continue;
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
	  cand.ieta = digis[topCands.at(i)].id().ieta();
	  cand.iphi = digis[topCands.at(i)].id().iphi();
	}
      out.at(i) = cand;
    }

  return out;
}

vector<CTPOutput> CTPCard::topNCands(unsigned n, 
				     const HcalTrigPrimDigiCollection& 
				     digis) const
{
  if(n==0)
    {
      cout << "WARNING: returning top 0 Hcal cands" << endl;
      return vector<CTPOutput>();
    }

  vector<unsigned> topCands = vector<unsigned>(n,0);
  vector<unsigned> topCandsEt = vector<unsigned>(n,0);

  for(unsigned i = 0; i < digis.size(); ++i)
    {
      unsigned candEt = digis[i].SOI_compressedEt();
      if(candEt <= topCandsEt.back())
	continue;
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
	  cand.ieta = digis[topCands.at(i)].id().ieta();
	  cand.iphi = digis[topCands.at(i)].id().iphi();
	}
      out.at(i) = cand;
    }
  return out;
}

// vector<CTPOutput> CTPCard::ETowerClusters(unsigned eClusterSeed, 
// 					  const EcalTrigPrimDigiCollection&
// 					  digis)
// {
  
// }
