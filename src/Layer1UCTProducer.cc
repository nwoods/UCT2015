/*
 Package:    Layer1UCTProducer
 Class:      Layer1UCTProducer
 
class Layer1UCTProducer Layer1UCTProducer.cc $CMSSW_BASE/src/L1Trigger/UCT2015/src/Layer1UCTProducer.cc

 Description: simple first stab at a producer for the UCT2015 emulator

 Authors:  N. Woods, E. Friis, S. Dasu, T. Sarangi (UW Madison)
*/

//#define DEBUG

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
  // Do we actually need ecal and hcal to be different in initialization? 
  cableParams(params.getParameter<ParameterSet>("RCTCableParams")),
  doDebug(params.getParameter<bool>("doDebug"))
{
  produces<vector<vector<CTPOutput>>>("CTPOutEcal");
  produces<vector<vector<CTPOutput>>>("CTPOutHcal");

//   edm::Service<TFileService> fs;

//   tree = fs->make<TTree>("Ntuple","Ntuple");
//   tree->Branch("run", &run_, "run/i");
//   tree->Branch("lumi", &lumi_, "lumi/i");
//   tree->Branch("evt", &event_, "evt/l");

//   etHisto = fs->make<TH1D>("etHisto", "Uncalibrated Total Calo Et", 
// 			   100, 0., 1500.);
}


Layer1UCTProducer::~Layer1UCTProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


void Layer1UCTProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get HCAL digi collection
  edm::Handle<HcalTrigPrimDigiCollection> hcalTpgs;
  iEvent.getByLabel(hcalDigiSrc, hcalTpgs);

  // Get ECAL digi collection
  edm::Handle<EcalTrigPrimDigiCollection> ecalTpgs;
  iEvent.getByLabel(ecalDigis, ecalTpgs);

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
    ecalOut(new vector<vector<CTPOutput>>);
  std::auto_ptr<vector<vector<CTPOutput>>> 
    hcalOut(new vector<vector<CTPOutput>>);

  for(unsigned i = 0; i < cables.getNCards(); ++i)
    {
      const EcalTrigPrimDigiCollection curEcalDigis = 
	cables.selectDigis(i,*ecalTpgs);
      ecalOut->push_back(cables.getCard(i).
			 topNCands(3,curEcalDigis));

      const HcalTrigPrimDigiCollection curHcalDigis = 
	cables.selectDigis(i,*hcalTpgs);
      hcalOut->push_back(cables.getCard(i).
			 topNCands(3,curHcalDigis));
    }

  if(doDebug)
    {
      cout << "Which gave final Ecal outputs:" << endl
	   << "(iEta,iPhi,et)" << endl;
      for(unsigned iCard = 0; iCard < ecalOut->size(); ++iCard)
	{
	  cout << "Card " << iCard << ":" << endl;
	  for(unsigned iDigi = 0; iDigi < ecalOut->at(iDigi).size(); ++iDigi)
	    {
	      cout << iDigi << ": (" << ecalOut->at(iCard).at(iDigi).ieta 
		   << "," << ecalOut->at(iCard).at(iDigi).iphi << ","
		   << ecalOut->at(iCard).at(iDigi).et << ")" << endl;
	    }
	}
      cout << endl;

      cout << "And final Hcal outputs:" << endl
	   << "(iEta,iPhi,et)" << endl;
      for(unsigned iCard = 0; iCard < hcalOut->size(); ++iCard)
	{
	  cout << "Card " << iCard << ":" << endl;
	  for(unsigned iDigi = 0; iDigi < hcalOut->at(iDigi).size(); ++iDigi)
	    {
	      cout << iDigi << ": (" << hcalOut->at(iCard).at(iDigi).ieta 
		   << "," << hcalOut->at(iCard).at(iDigi).iphi << ","
		   << hcalOut->at(iCard).at(iDigi).et << ")" << endl;
	    }
	}
      cout << endl;
    }
  
  iEvent.put(ecalOut, "CTPOutEcal");
  iEvent.put(hcalOut, "CTPOutHcal");


  //  etHisto->Fill(Et);

  //  tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(Layer1UCTProducer);











