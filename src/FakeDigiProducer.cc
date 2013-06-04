#include "../interface/FakeDigiProducer.h"

#include <vector>
using std::vector;
using namespace std;
using namespace edm;
#include <iostream>




// constructors and destructor
//
FakeDigiProducer::FakeDigiProducer(const edm::ParameterSet& params) :
  ecalFileNames(params.getParameter<vector<string>>("ecalFileNames")),
  hcalFileNames(params.getParameter<vector<string>>("hcalFileNames")),
  numDigis(ecalFileNames.size() > hcalFileNames.size() ? 
	     ecalFileNames.size() :
	     hcalFileNames.size())
{
  produces<EcalTrigPrimDigiCollection>("fakeEcalDigis")
    .setBranchAlias("fakeEcalDigis");
  produces<HcalTrigPrimDigiCollection>("fakeHcalDigis")
    .setBranchAlias("fakeHcalDigis");

  edm::Service<TFileService> fs;

  tree = fs->make<TTree>("Ntuple","Ntuple");
  tree->Branch("run", &run_, "run/i");
  tree->Branch("lumi", &lumi_, "lumi/i");
  tree->Branch("evt", &event_, "evt/l");
}


FakeDigiProducer::~FakeDigiProducer()
{
}


void FakeDigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  run_ = iEvent.id().run();
  lumi_ = iEvent.id().luminosityBlock();
  event_ = iEvent.id().event();

  std::auto_ptr<EcalTrigPrimDigiCollection> 
    ecalOut(new EcalTrigPrimDigiCollection);
  std::auto_ptr<HcalTrigPrimDigiCollection> 
    hcalOut(new HcalTrigPrimDigiCollection);

  if(event_ < ecalFileNames.size())
    *(ecalOut) = makeEcalCollectionFromFile(ecalFileNames.at(event_));
  else
    *(ecalOut) = makeEcalCollection();

  if(event_ < hcalFileNames.size())
    *(hcalOut) = makeHcalCollectionFromFile(hcalFileNames.at(event_));
  else
    *(hcalOut) = makeHcalCollection();
  
  iEvent.put(ecalOut, "fakeEcalDigis");
  iEvent.put(hcalOut, "fakeHcalDigis");

  tree->Fill();
}

EcalTrigPrimDigiCollection 
FakeDigiProducer::makeEcalCollectionFromFile(string fileName)
{
  EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection(numDigis);

  vector<CTPOutput> goodEntries = getGoodFileEntries(fileName);

  unsigned nextGoodEntry = 0;
  for(int eta = -32; eta <= 32; ++eta)
    {
      if(eta == 0) continue;
      for(unsigned phi = 1; phi <= 72; ++phi)
	{
	  if(eta < goodEntries.at(nextGoodEntry).ieta ||
	     phi < goodEntries.at(nextGoodEntry).iphi ||
	     nextGoodEntry == goodEntries.size())
	    {
	      // Put in "blank" entry
	      uint16_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	      EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	      digi.setSample(0,EcalTriggerPrimitiveSample(0));
	      output.push_back(digi);
	    }
	  else
	    {
	      // Put in real entry
	      uint16_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	      EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	      digi.setSample(0, EcalTriggerPrimitiveSample(goodEntries
							   .at(nextGoodEntry++)
							   .et%0x100));
	      output.push_back(digi);
	    }
	}
    }
  return output;

}
// {
//   if(!f.is_open())
//     throw cms::Exception("BadFile") << "File failed to open" << endl;
  
//   EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection(numDigis);
  
//   unsigned nDigisPlaced = 0;
  
//   while(true)
//     {
//       int iEta;
//       unsigned iPhi;
//       unsigned et;
//       f >> iEta;
//       if(f.eof())
// 	throw cms::Exception("BadFile") 
// 	  << "Input file must have 3 columns" << endl;
//       f >> iPhi;
//       if(f.eof())
// 	throw cms::Exception("BadFile") 
// 	  << "Input file must have 3 columns" << endl;
// 	  f >> et;
// 	  uint16_t id = 0;
// 	  id |= (iEta > 0 ? (0x8000) : 0); // iEta sign
// 	  id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7); // iEta value
// 	  id |= iPhi; // iPhi value
// 	  EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
// 	  EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
// 	  digi.setSample(0,EcalTriggerPrimitiveSample(et%0x100));
// 	  output.push_back(digi);
	  
// 	  if(f.eof() || ++nDigisPlaced == numDigis)
// 	    break;
//     }
//   if(nDigisPlaced != numDigis)
//     cout << "WARNING The number of digis placed in the collection"
// 	 << " is wrong." << endl;
  
//   return output;
// }

EcalTrigPrimDigiCollection 
FakeDigiProducer::makeEcalCollection() 
{
  EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection(numDigis);
  
  for(int iEta = -32; iEta <= 32; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = 1; iPhi <= 72; ++iPhi)
	{
	  uint16_t id = 0;
	  id |= (iEta > 0 ? (0x8000) : 0); // iEta sign
	  id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7); // iEta value
	  id |= iPhi; // iPhi value
	  EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	  EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	  digi.setSample(0,EcalTriggerPrimitiveSample(0));
	  output.push_back(digi);
	}
    }
  
  return output;
}

HcalTrigPrimDigiCollection 
FakeDigiProducer::makeHcalCollectionFromFile(string fileName)
{
  HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection(numDigis);

  vector<CTPOutput> goodEntries = getGoodFileEntries(fileName);

  unsigned nextGoodEntry = 0;
  for(int eta = -32; eta <= 32; ++eta)
    {
      if(eta == 0) continue;
      for(unsigned phi = 1; phi <= 72; ++phi)
	{
	  if(eta < goodEntries.at(nextGoodEntry).ieta ||
	     phi < goodEntries.at(nextGoodEntry).iphi ||
	     nextGoodEntry == goodEntries.size())
	    {
	      // Put in "blank" entry
	      uint16_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
	      HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
	      digi.setSample(0,HcalTriggerPrimitiveSample(0));
	      output.push_back(digi);
	    }
	  else
	    {
	      // Put in real entry
	      uint16_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
	      HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
	      digi.setSample(0, HcalTriggerPrimitiveSample(goodEntries
							   .at(nextGoodEntry++)
							   .et%0x100));
	      output.push_back(digi);
	    }
	}
    }
  return output;

}
// {
//   if(!f.is_open())
//     throw cms::Exception("BadFile") << "File failed to open" << endl;
  
//   HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection(numDigis);
  
//   unsigned nDigisPlaced = 0;

//   while(true)
//     {
//       int iEta;
//       unsigned iPhi;
//       unsigned et;
//       f >> iEta;
//       if(f.eof())
// 	throw cms::Exception("BadFile") 
// 	  << "Input file must have 3 columns" << endl;
//       f >> iPhi;
//       if(f.eof())
// 	    throw cms::Exception("BadFile") 
// 	      << "Input file must have 3 columns" << endl;
//       f >> et;
//       uint16_t id = 0;
//       id |= (iEta > 0 ? (0x8000) : 0); // iEta sign
//       id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7); // iEta value
//       id |= iPhi; // iPhi value
//       HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
//       HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
//       digi.setSample(0,HcalTriggerPrimitiveSample(et%0x100));
//       output.push_back(digi);
      
//       if(f.eof() || ++nDigisPlaced == numDigis)
// 	break;
//     }
//   if(nDigisPlaced != numDigis)
//     cout << "WARNING The number of digis placed in the collection"
// 	 << " is wrong." << endl;
  
//   return output;
// }

HcalTrigPrimDigiCollection 
FakeDigiProducer::makeHcalCollection() 
{
  HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection(numDigis);
  
  for(int iEta = -32; iEta <= 32; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = 1; iPhi <= 72; ++iPhi)
	{
	  uint16_t id = 0;
	  id |= (iEta > 0 ? (0x8000) : 0); // iEta sign
	  id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7); // iEta value
	  id |= iPhi; // iPhi value
	  HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
	  HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
	  digi.setSample(0,HcalTriggerPrimitiveSample(0));
	  output.push_back(digi);
	}
    }
  
  return output;
}

vector<CTPOutput> FakeDigiProducer::getGoodFileEntries(string fileName)
{
  ifstream f;
  f.open(fileName.c_str());
  if((!f.is_open()) || f.eof())
    throw cms::Exception("BadFile") << "File does not exist, is empty, or "
				    <<"cannot be opened" << endl;

  vector<CTPOutput> output = vector<CTPOutput>();
  int lastEta = -32;
  unsigned lastPhi = 1;
  while(true)
    {
      int iEta;
      unsigned iPhi;
      unsigned et;
      
      f >> iEta;
      if(f.eof() || iEta < lastEta || iEta > 32 || iEta == 0)
	throw cms::Exception("BadFile") << "File does not exist or is "
					<< "improperly formatted" << endl
					<< "Make sure that your file has three"
					<< " columns (eta, phi, et) in "
					<< "ascending order in eta, and within"
					<< " each eta, in ascending order in "
					<< "phi" << endl;
      
      if(iEta > lastEta)
	{
	  lastPhi = 0;
	  lastEta = iEta;
	}

      f >> iPhi;
      if(f.eof() || iPhi < lastPhi || iPhi > 72)
	throw cms::Exception("BadFile") << "File does not exist or is "
					<< "improperly formatted" << endl
					<< "Make sure that your file has three"
					<< " columns (eta, phi, et) in "
					<< "ascending order in eta, and within"
					<< " each eta, in ascending order in "
					<< "phi" << endl;
      lastPhi = iPhi;

      f >> et;

      if(et > 0)
	{
	  CTPOutput out;
	  out.ieta = iEta;
	  out.iphi = iPhi;
	  out.et = et;
	  output.push_back(out);
	}

      if(f.eof())
	break;
    }

  f.close();
  return output;
}

DEFINE_FWK_MODULE(FakeDigiProducer);
