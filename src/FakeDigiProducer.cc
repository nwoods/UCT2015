#include "../interface/FakeDigiProducer.h"



// constructors and destructor
//
FakeDigiProducer::FakeDigiProducer(const edm::ParameterSet& params) :
  ecalFileNames(params.getParameter<vector<string>>("ecalFileNames")),
  hcalFileNames(params.getParameter<vector<string>>("hcalFileNames")),
  numDigis(ecalFileNames.size() > hcalFileNames.size() ? 
	   ecalFileNames.size() :
	   hcalFileNames.size()),
  doDebug(params.getParameter<bool>("doDebug"))
{
  produces<EcalTrigPrimDigiCollection>("fakeEcalDigis");

  produces<HcalTrigPrimDigiCollection>("fakeHcalDigis");

//   edm::Service<TFileService> fs;

//   tree = fs->make<TTree>("Ntuple","Ntuple");
//   tree->Branch("run", &run_, "run/i");
//   tree->Branch("lumi", &lumi_, "lumi/i");
//   tree->Branch("evt", &event_, "evt/l");
}


FakeDigiProducer::~FakeDigiProducer()
{
}


void FakeDigiProducer::produce(edm::Event& iEvent, 
			       const edm::EventSetup& iSetup)
{
  run_ = iEvent.id().run();
  lumi_ = iEvent.id().luminosityBlock();
  event_ = iEvent.id().event();

  std::auto_ptr<EcalTrigPrimDigiCollection> 
    ecalOut(new EcalTrigPrimDigiCollection);
  std::auto_ptr<HcalTrigPrimDigiCollection>
    hcalOut(new HcalTrigPrimDigiCollection);

  if(event_-1 < ecalFileNames.size())
    *(ecalOut) = makeEcalCollectionFromFile(ecalFileNames.at(event_-1));
  else
    {
      if(doDebug)
	cout << endl 
	     << "***********************************"
	     << "*******************************" << endl 
	     << "***** Done with Ecal digis. "
	     << "Further Ecal output is all zeros *****" << endl
	     << "***********************************"
	     << "*******************************" << endl << endl;

      *(ecalOut) = makeEcalCollection();
    }

  if(event_-1 < hcalFileNames.size())
    *(hcalOut) = makeHcalCollectionFromFile(hcalFileNames.at(event_-1));
  else
    {
      if(doDebug)
	cout << endl 
	     << "***********************************"
	     << "*******************************" << endl 
	     << "***** Done with Hcal digis. "
	     << "Further Hcal output is all zeros *****" << endl
 	     << "***********************************"
	     << "*******************************" << endl << endl;

      *(hcalOut) = makeHcalCollection();
    }  

  iEvent.put(ecalOut, "fakeEcalDigis");
  iEvent.put(hcalOut, "fakeHcalDigis");

  //  tree->Fill();
}

EcalTrigPrimDigiCollection 
FakeDigiProducer::makeEcalCollectionFromFile(string fileName)
{
  EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection();

  vector<CTPOutput> goodEntries = getGoodFileEntries(fileName);

  if(doDebug)
    {
      cout << "-------------------- Ecal Digis for event " << event_
	   << " --------------------" << endl;

      cout << "\nInput file was: \n" << endl;
      ifstream g;
      string line;
      g.open(fileName.c_str());
      if(g.is_open())
	{
	  while(getline(g,line))
	    cout << line << endl;
	  cout << endl << endl;
	}
      else
	cout << "FILE FAILED TO READ" << endl << endl;
      
      cout << "Which became digi collection" << endl
	   << "#:    (iEta,iPhi,et):" << endl;
    }


  unsigned nextGoodEntry = 0;
  for(int eta = -32; eta <= 32; ++eta)
    {
      if(eta == 0) continue;
      for(unsigned phi = 1; phi <= 72; ++phi)
	{
	  if((nextGoodEntry == goodEntries.size()?
	      true : // if we've done all the file digis, the rest are blank
	      (eta < goodEntries.at(nextGoodEntry).ieta ||
	       phi < goodEntries.at(nextGoodEntry).iphi)))
	    {
	      // Put in "blank" entry
	      uint32_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign DIFFERENT FROM HCAL
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	      EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	      digi.setSize(1);
	      digi.setSample(0,EcalTriggerPrimitiveSample(0));
	      output.push_back(digi);
	    }
	  else
	    {
	      // Put in real entry
	      uint32_t id = 0;
	      id |= (eta > 0 ? (0x8000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	      EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	      digi.setSize(1);
	      uint16_t theSample = goodEntries.at(nextGoodEntry++).et;
	      digi.setSample(0, EcalTriggerPrimitiveSample(theSample));
	      output.push_back(digi);
	    }
	}
    }

  if(doDebug)
    {
      for(unsigned q = 0; q < output.size(); ++q)
	{
	  cout << q << ":    (" << output[q].id().ieta() << ","
	       << output[q].id().iphi() << ","
	       << output[q].compressedEt() << ")" << endl;
	}
      cout << endl;
    }
	  
  return output;
}

EcalTrigPrimDigiCollection 
FakeDigiProducer::makeEcalCollection() 
{
  EcalTrigPrimDigiCollection output = EcalTrigPrimDigiCollection();
  
  for(int iEta = -32; iEta <= 32; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = 1; iPhi <= 72; ++iPhi)
	{
	  uint32_t id = 0;
	  id |= (iEta > 0 ? (0x8000) : 0); // iEta sign
	  id |= (iEta > 0 ? iEta<<7 : (-1*iEta)<<7); // iEta value
	  id |= iPhi; // iPhi value
	  EcalTrigTowerDetId tow = EcalTrigTowerDetId(id);
	  EcalTriggerPrimitiveDigi digi = EcalTriggerPrimitiveDigi(tow);
	  digi.setSize(1);
	  digi.setSample(0,EcalTriggerPrimitiveSample(0));
	  output.push_back(digi);
	}
    }

  return output;
}

HcalTrigPrimDigiCollection 
FakeDigiProducer::makeHcalCollectionFromFile(string fileName)
{
  HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection();

  vector<CTPOutput> goodEntries = getGoodFileEntries(fileName);

  if(doDebug)
    {
      cout << "-------------------- Hcal Digis for event " << event_
	   << " --------------------" << endl
	   << "#:    (iEta,iPhi,et):";

      cout << "\nInput file was: \n" << endl;
      ifstream g;
      string line;
      g.open(fileName.c_str());
      if(g.is_open())
	{
	  while(getline(g,line))
	    cout << line << endl;
	  cout << endl << endl;
	}
      else
	cout << "FILE FAILED TO READ" << endl << endl;
      
      cout << "Which became digi collection" << endl
	   << "#:    (iEta,iPhi,et):" << endl;
    }

  unsigned nextGoodEntry = 0;
  for(int eta = -32; eta <= 32; ++eta)
    {
      if(eta == 0) continue;
      for(unsigned phi = 1; phi <= 72; ++phi)
	{
	  if((nextGoodEntry == goodEntries.size()?
	      true : // if we've done all the file digis, the rest are blank
	      (eta < goodEntries.at(nextGoodEntry).ieta ||
	       phi < goodEntries.at(nextGoodEntry).iphi)))
	    {
	      // Put in "blank" entry
	      uint32_t id = 0;
	      id |= (eta > 0 ? (0x2000) : 0); // iEta sign DIFFERENT FROM ECAL
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
	      HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
	      digi.setSize(1);
	      digi.setSample(0,HcalTriggerPrimitiveSample(0));
	      output.push_back(digi);
	    }
	  else
	    {
	      // Put in real entry
	      uint32_t id = 0;
	      id |= (eta > 0 ? (0x2000) : 0); // iEta sign
	      id |= (eta > 0 ? eta<<7 : (-1*eta)<<7); // iEta value
	      id |= phi; // iPhi value
	      HcalTrigTowerDetId tow = HcalTrigTowerDetId(id);
	      HcalTriggerPrimitiveDigi digi = HcalTriggerPrimitiveDigi(tow);
	      digi.setSize(1);
	      uint16_t theSample = goodEntries.at(nextGoodEntry++).et;
	      digi.setSample(0, HcalTriggerPrimitiveSample(theSample));
	      output.push_back(digi);
	    }
	}
    }

  if(doDebug)
    {
      for(unsigned q = 0; q < output.size(); ++q)
	{
	  cout << q << ":    (" << output[q].id().ieta() << ","
	       << output[q].id().iphi() << ","
	       << output[q].SOI_compressedEt() << ")" << endl;
	}
      cout << endl;
    }

  return output;

}

HcalTrigPrimDigiCollection 
FakeDigiProducer::makeHcalCollection() 
{
  HcalTrigPrimDigiCollection output = HcalTrigPrimDigiCollection();
  
  for(int iEta = -32; iEta <= 32; ++iEta)
    {
      if(iEta == 0) continue;
      for(unsigned iPhi = 1; iPhi <= 72; ++iPhi)
	{
	  uint32_t id = 0;
	  id |= (iEta > 0 ? (0x2000) : 0); // iEta sign
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
  if(!f.is_open())
    throw cms::Exception("BadFile") << "File " << fileName 
				    << " does not exist, is empty, or "
				    << "cannot be opened" << endl;

  vector<CTPOutput> output = vector<CTPOutput>();

  int lastEta = -32;
  unsigned lastPhi = 0;
  int nloops = 0;
  while(true)
    {
      int iEta;
      unsigned iPhi;
      unsigned et;
      
      if(!(f >> iEta))
	break;

      if(iEta < lastEta)
	throw cms::Exception("BadFile") << fileName 
					<< " Etas out of order, loop " 
					<< nloops
					<< endl;
      if(iEta == 0 || iEta > 32)
	throw cms::Exception("BadFile") << fileName
					<< " Invalid eta, loop " << nloops
					<< endl;
      
      if(iEta > lastEta)
	{
	  lastPhi = 0;
	  lastEta = iEta;
	}

      if(!(f >> iPhi))
	throw cms::Exception("BadFile") << fileName
					<< " broke reading phi, loop " 
					<< nloops
					<< endl;

      if(iPhi <= lastPhi)
	throw cms::Exception("BadFile") << fileName
					<< " Phis out of order, loop "
					<< nloops
					<< endl;
      if(iPhi > 72)
	throw cms::Exception("BadFile") << fileName
					<< " invalid phi, loop "
					<< nloops
					<< endl;
      lastPhi = iPhi;

      if(!(f >> et))
	throw cms::Exception("BadFile") << fileName 
					<< " broke reading et, loop " 
					<< nloops
					<< endl;

      if(et > 0)
	{
	  if(et >= 0x100) // saturated (et is 8 bit)
	    {
	      cout << "Saturated: changing " << et << " to " << 0xFF << endl;
	      et = 0xFF;
	    }

	 
	  CTPOutput out;
	  out.ieta = iEta;
	  out.iphi = iPhi;
	  out.et = et;
	  output.push_back(out);
	}

      ++nloops;
    }

  f.close();

  return output;
}

DEFINE_FWK_MODULE(FakeDigiProducer);
