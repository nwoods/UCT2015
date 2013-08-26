/*
 Package:    Layer1UCTProducer
 Class:      Layer1UCTProducer
 
class Layer1UCTProducer Layer1UCTProducer.cc $CMSSW_BASE/src/L1Trigger/UCT2015/src/Layer1UCTProducer.cc

 Description: simple first stab at a producer for the UCT2015 emulator

 Authors:  N. Woods, E. Friis, S. Dasu, T. Sarangi (UW Madison)
*/


#include "../interface/Layer1UCTProducer.h"
#include <vector>
using std::vector;
using namespace std;
using namespace edm;
#include <iostream>




// constructors and destructor
//
Layer1UCTProducer::Layer1UCTProducer(const edm::ParameterSet& params) :
  hcalDigiSrc(params.getParameter<InputTag>("hcalDigiSrc")),
  ecalDigis(params.getParameter<InputTag>("ecalDigis")),
  regionSource(params.getParameter<InputTag>("rctRegions")),
  emCandSource(params.getParameter<InputTag>("emCands")),
  cableParams(params.getParameter<ParameterSet>("RCTCableParams")),
  doDebug(params.getParameter<bool>("doDebug"))
{
  produces<vector<vector<CTPOutput>>>("CTPEClusters");
  produces<vector<vector<CTPOutput>>>("CTPOutEcal");
  produces<vector<vector<CTPOutput>>>("CTPOutHcal");

  produces<vector<vector<CTPOutput>>>("stage1Jets");
  produces<vector<vector<CTPOutput>>>("stage1RlxEG");
  produces<vector<vector<CTPOutput>>>("stage1IsoEG");
  produces<vector<vector<CTPOutput>>>("stage1RlxTau");
  produces<vector<vector<CTPOutput>>>("stage1IsoTau");
  produces<vector<unsigned>>("stage1Et");
  produces<vector<unsigned>>("stage1Met");
  produces<vector<unsigned>>("stage1Ht");
  produces<vector<unsigned>>("stage1Mht");
  produces<vector<unsigned>>("stage1PULevel");
}


Layer1UCTProducer::~Layer1UCTProducer()
{
}


void Layer1UCTProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get HCAL digi collection
  Handle<HcalTrigPrimDigiCollection> hcalTpgs;
  iEvent.getByLabel(hcalDigiSrc, hcalTpgs);

  // Get ECAL digi collection
  Handle<EcalTrigPrimDigiCollection> ecalTpgs;
  iEvent.getByLabel(ecalDigis, ecalTpgs);

  // Get RCT regions
  Handle<L1CaloRegionCollection> rctRegions;
  iEvent.getByLabel(regionSource, rctRegions);

  // Get RCT candidates
  Handle<L1CaloEmCollection> emCands;
  iEvent.getByLabel(emCandSource, emCands);

  if(!hcalTpgs.isValid())
    throw cms::Exception("BadInput")
      << "Hcal collection handle invalid" << std::endl;
  if(!ecalTpgs.isValid())
    throw cms::Exception("BadInput")
      << "Ecal collection handle invalid" << std::endl;
  if(!(hcalTpgs.isValid() || ecalTpgs.isValid()))
    return;

  run_ = iEvent.id().run();
  lumi_ = iEvent.id().luminosityBlock();
  event_ = iEvent.id().event();

  RCTCables cables = RCTCables(cableParams);

  std::auto_ptr<vector<vector<CTPOutput>>> 
    eClusterOut (new vector<vector<CTPOutput>>);
  std::auto_ptr<vector<vector<CTPOutput>>> 
    ecalOut(new vector<vector<CTPOutput>>);
  std::auto_ptr<vector<vector<CTPOutput>>> 
    hcalOut(new vector<vector<CTPOutput>>);

  cables.setEcalDigis(*ecalTpgs);
  
  cables.setHcalDigis(*hcalTpgs);
  
  *(ecalOut) = cables.topNEcalCands(3);
  
  *(hcalOut) = cables.topNHcalCands(3);

  *(eClusterOut) = cables.eTowerClusters(5);


  auto_ptr<vector<vector<CTPOutput>>> s1JetsOut(new vector<vector<CTPOutput>>);
  auto_ptr<vector<vector<CTPOutput>>> 
    s1RlxTauOut(new vector<vector<CTPOutput>>);
  auto_ptr<vector<vector<CTPOutput>>> 
    s1IsoTauOut(new vector<vector<CTPOutput>>);
  auto_ptr<vector<vector<CTPOutput>>> 
    s1RlxEGOut(new vector<vector<CTPOutput>>);
  auto_ptr<vector<vector<CTPOutput>>> 
    s1IsoEGOut(new vector<vector<CTPOutput>>);
  auto_ptr<vector<unsigned>> puLevelOut(new vector<unsigned>);
  auto_ptr<vector<unsigned>> s1EtOut(new vector<unsigned>);
  auto_ptr<vector<unsigned>> s1MetOut(new vector<unsigned>);
  auto_ptr<vector<unsigned>> s1HtOut(new vector<unsigned>);
  auto_ptr<vector<unsigned>> s1MhtOut(new vector<unsigned>);

  *(s1JetsOut) = cables.s1Jets();
  *(s1RlxTauOut) = cables.s1RlxTau();
  *(s1IsoTauOut) = cables.s1IsoTau();
  *(s1RlxEGOut) = cables.s1RlxEG();
  *(s1IsoEGOut) = cables.s1IsoEG();
  *(puLevelOut) = cables.s1PULevel();
  *(s1EtOut) = cables.s1Et();
  *(s1MetOut) = cables.s1Met();
  *(s1HtOut) = cables.s1Ht();
  *(s1MhtOut) = cables.s1Mht();


  if(doDebug)
    {
      cout << endl << "Stage 1 Results:" << endl << endl;

      cout << endl << "PU level:" << endl;
      for(unsigned i = 0; i < puLevelOut->size(); ++i)
	cout << puLevelOut->at(i);

      cout << "Total Et:" << endl;
      for(unsigned i = 0; i < s1EtOut->size(); ++i)
	cout << s1EtOut->at(i);

      cout << endl << "Total Met:" << endl;
      for(unsigned i = 0; i < s1MetOut->size(); ++i)
	cout << s1MetOut->at(i);

      cout << endl << "Total Ht:" << endl;
      for(unsigned i = 0; i < s1HtOut->size(); ++i)
	cout << s1HtOut->at(i);

      cout << endl << "Total Mht:" << endl;
      for(unsigned i = 0; i < s1MhtOut->size(); ++i)
	cout << s1MhtOut->at(i);

      cout << endl << "Jets:" << endl << "(iEta,iPhi,et)" << endl << endl;
      for(unsigned i = 0; i < s1JetsOut->size(); ++i)
	{
	  cout << "Card " << i << ":" << endl;
	  for(unsigned j = 0; j < s1JetsOut->at(i).size(); ++j)
	    {
	      cout << "(" << s1JetsOut->at(i).at(j).ieta << ","
		   << s1JetsOut->at(i).at(j).iphi << ","
		   << s1JetsOut->at(i).at(j).et << ")" << endl;
	    }
	  cout << endl;
	}

      cout << endl << "Relaxed EGammas:" 
	   << endl << "(crate,card,region,et)" << endl << endl;
      for(unsigned i = 0; i < s1RlxEGOut->size(); ++i)
	{
	  cout << "Card " << i << ":" << endl;
	  for(unsigned j = 0; j < s1RlxEGOut->at(i).size(); ++j)
	    {
	      cout << "(" << s1RlxEGOut->at(i).at(j).crate << ","
		   << s1RlxEGOut->at(i).at(j).card << ","
		   << s1RlxEGOut->at(i).at(j).region << ","
		   << s1RlxEGOut->at(i).at(j).et << ")" << endl;
	    }
	  cout << endl;
	}

      cout << endl << "Isolated EGammas:" 
	   << endl << "(crate,card,region,et)" << endl << endl;
      for(unsigned i = 0; i < s1IsoEGOut->size(); ++i)
	{
	  cout << "Card " << i << ":" << endl;
	  for(unsigned j = 0; j < s1IsoEGOut->at(i).size(); ++j)
	    {
	      cout << "(" << s1IsoEGOut->at(i).at(j).crate << ","
		   << s1IsoEGOut->at(i).at(j).card << ","
		   << s1IsoEGOut->at(i).at(j).region << ","
		   << s1IsoEGOut->at(i).at(j).et << ")" << endl;
	    }
	  cout << endl;
	}

      cout << endl << "Relaxed Taus:" 
	   << endl << "(crate,card,region,et)" << endl << endl;
      for(unsigned i = 0; i < s1RlxTauOut->size(); ++i)
	{
	  cout << "Card " << i << ":" << endl;
	  for(unsigned j = 0; j < s1RlxTauOut->at(i).size(); ++j)
	    {
	      cout << "(" << s1RlxTauOut->at(i).at(j).crate << ","
		   << s1RlxTauOut->at(i).at(j).card << ","
		   << s1RlxTauOut->at(i).at(j).region << ","
		   << s1RlxTauOut->at(i).at(j).et << ")" << endl;
	    }
	  cout << endl;
	}

      cout << endl << "Isolated Taus:" 
	   << endl << "(crate,card,region,et)" << endl << endl;
      for(unsigned i = 0; i < s1IsoTauOut->size(); ++i)
	{
	  cout << "Card " << i << ":" << endl;
	  for(unsigned j = 0; j < s1IsoTauOut->at(i).size(); ++j)
	    {
	      cout << "(" << s1IsoTauOut->at(i).at(j).crate << ","
		   << s1IsoTauOut->at(i).at(j).card << ","
		   << s1IsoTauOut->at(i).at(j).region << ","
		   << s1IsoTauOut->at(i).at(j).et << ")" << endl;
	    }
	  cout << endl;
	}
    }


//   if(doDebug)
//     {
//       cout << endl << "Total Et in these Digis = ";
//       cout << cables.globalEtSum() 
// 	   << endl << endl;
//     }

//   if(doDebug)
//     {
//       cout << "Which gave top Ecal cands:" << endl
// 	   << "(iEta,iPhi,et)" << endl;
//       for(unsigned iCard = 0; iCard < ecalOut->size(); ++iCard)
// 	{
// 	  if(ecalOut->at(iCard).at(0).et != 0)
// 	    {
// 	      cout << "Card " << iCard << ":" << endl;
// 	      for(unsigned iDigi = 0; 
// 		  iDigi < ecalOut->at(iCard).size(); 
// 		  ++iDigi)
// 		{
// 		  cout << iDigi << ": (";
// 		  cout << ecalOut->at(iCard).at(iDigi).ieta 
// 		       << "," << ecalOut->at(iCard).at(iDigi).iphi << ","
// 		       << ecalOut->at(iCard).at(iDigi).et << ")" << endl;
// 		}
//  	    }
// 	}
//       cout << "Done with Ecal cands" << endl << endl;

//       cout << "And top Hcal cands:" << endl
// 	   << "(iEta,iPhi,et)" << endl;
//       for(unsigned iCard = 0; iCard < hcalOut->size(); ++iCard)
// 	{
// 	  cout << "Card " << iCard << ":" << endl;
// 	  for(unsigned iDigi = 0; iDigi < hcalOut->at(iCard).size(); ++iDigi)
// 	    {
// 	      cout << iDigi << ": (" << hcalOut->at(iCard).at(iDigi).ieta 
// 		   << "," << hcalOut->at(iCard).at(iDigi).iphi << ","
// 		   << hcalOut->at(iCard).at(iDigi).et << ")" << endl;
// 	    }
// 	}
//       cout << endl << endl;

//       cout << "And E Tower Clusters" << endl << "(iEta,iPhi,et)" << endl;
//       for(unsigned q = 0; q < eClusterOut->size(); ++q)
// 	{
// 	  cout << "Card " << q << ":" << endl;
// 	  for(unsigned p = 0; p < eClusterOut->at(q).size(); ++p)
// 	    {
// 	      cout << p << ": (" << eClusterOut->at(q).at(p).ieta << ","
// 		   << eClusterOut->at(q).at(p).iphi << ","
// 		   << eClusterOut->at(q).at(p).et << ")" << endl;
// 	    }
// 	}
//       cout << endl; 
//     }
  
  iEvent.put(ecalOut, "CTPOutEcal");
  iEvent.put(hcalOut, "CTPOutHcal");
  iEvent.put(eClusterOut, "CTPEClusters");

  iEvent.put(s1JetsOut, "s1Jets");
  iEvent.put(s1RlxTauOut, "s1RlxTau");
  iEvent.put(s1IsoTauOut, "s1IsoTau");
  iEvent.put(s1RlxEGOut, "s1RlxEG"); 
  iEvent.put(s1IsoEGOut, "s1IsoEG"); 
  iEvent.put(puLevelOut, "s1PULevel");
  iEvent.put(s1EtOut, "s1Et");
  iEvent.put(s1MetOut, "s1Met");
  iEvent.put(s1HtOut, "s1Ht");
  iEvent.put(s1MhtOut, "s1Mht");

}

//define this as a plug-in
DEFINE_FWK_MODULE(Layer1UCTProducer);











