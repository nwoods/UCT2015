/*
 Package:    Layer1UCTProducer
 Class:      Layer1UCTProducer
 
class Layer1UCTProducer Layer1UCTProducer.cc $CMSSW_BASE/src/L1Trigger/UCT2015/src/Layer1UCTProducer.cc

 Description: simple first stab at a producer for the UCT2015 emulator

 Authors:  N. Woods, E. Friis, S. Dasu, T. Sarangi (UW Madison)
*/

#define DEBUG

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
  cableParams(params.getParameter<ParameterSet>("RCTCableParams"))
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

  vector<CTPCard> cards = vector<CTPCard>(cables.getNCards(), CTPCard(0.5));

  std::auto_ptr<vector<vector<CTPOutput>>> 
    ecalOut(new vector<vector<CTPOutput>>);
  std::auto_ptr<vector<vector<CTPOutput>>> 
    hcalOut(new vector<vector<CTPOutput>>);

  for(unsigned i = 0; i < cables.getNCards(); ++i)
    {
      ecalOut->push_back(cards.at(i).
			 topNCands(3,cables.selectDigis(i,*ecalTpgs)));
      hcalOut->push_back(cards.at(i).
			 topNCands(3,cables.selectDigis(i,*hcalTpgs)));
    }
  
  iEvent.put(ecalOut, "CTPOutEcal");
  iEvent.put(hcalOut, "CTPOutHcal");

  //  etHisto->Fill(Et);

  //  tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(Layer1UCTProducer);











