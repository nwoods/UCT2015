/*
 * =====================================================================================
        Filename:  HcalTpgMipEmbedder.cc

       Description: Modify the HCAL TPGs according to the proposed HTR
       modification.  If the HCAL is above a given energy threshold, set the MIP
       bit.

         Author:  Evan Friis, evan.friis@cern.ch
         Company:  UW Madison
 * =====================================================================================
 */

#include<iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"

class HcalTpgMipEmbedder : public edm::EDProducer {
  public:
    HcalTpgMipEmbedder(const edm::ParameterSet& pset);
    virtual ~HcalTpgMipEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    double threshold_;
    int rawthreshold_;
    bool cutOnRawBits_;
    edm::InputTag src_;
    bool debug_;
};

HcalTpgMipEmbedder::HcalTpgMipEmbedder(const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  threshold_ = pset.getParameter<double>("threshold");
  rawthreshold_ = pset.getParameter<unsigned int>("rawThreshold");
  cutOnRawBits_ = pset.getParameter<bool>("cutOnRawBits");
  debug_ = pset.exists("debug") ? pset.getParameter<bool>("debug") : false;
  produces<HcalTrigPrimDigiCollection>();
}
void HcalTpgMipEmbedder::produce(edm::Event& evt, const edm::EventSetup& es) {

  edm::ESHandle<L1CaloHcalScale> hcalScale;
  es.get<L1CaloHcalScaleRcd>().get(hcalScale);

  if (debug_) {
    std::cout << "==== event ====" << std::endl;
    hcalScale->print(std::cout);
  }

  edm::Handle<HcalTrigPrimDigiCollection> tpgs;
  evt.getByLabel(src_, tpgs);

  std::cout << std::endl << "Digis going into MIP embedder:" << std::endl;
  for(unsigned q = 0; q < tpgs->size(); ++q)
    {
      std::cout << "(" << (*tpgs)[q].id().ieta() 
		<< "," << (*tpgs)[q].id().iphi()
		<< "," << (*tpgs)[q].SOI_compressedEt() << ")" << std::endl;
    }
  std::cout << std::endl;

  std::auto_ptr<HcalTrigPrimDigiCollection> output(new HcalTrigPrimDigiCollection);
  output->reserve(tpgs->size());

  for (size_t i = 0; i < tpgs->size(); ++i) {
    HcalTriggerPrimitiveDigi tpg = (*tpgs)[i];
    short ieta = std::abs(tpg.id().ieta());
    short zside = tpg.id().zside();
    // Get the real energy
    double energy = hcalScale->et(tpg.SOI_compressedEt(), ieta, zside);
    if (debug_) {
      std::cout << ieta << " " << energy << std::endl;
    }

    bool setMIP = false;
    if (cutOnRawBits_) {
      if (tpg.SOI_compressedEt() > rawthreshold_)
        setMIP = true;
    } else {
      if (energy > threshold_) {
        setMIP = true;
      }
    }
    if (setMIP) {
      //std::cout << "Setting mip for: et: " << tpg.SOI_compressedEt() << " eta: " << ieta << " z: " << zside <<  " real: " << energy << std::endl;
      // Set the MIP bit
      HcalTriggerPrimitiveSample new_t0(tpg.t0().raw() | 0x100);
      tpg.setSample(tpg.presamples(), new_t0);
    }
    output->push_back(tpg);
  }

  std::cout << "Digis leaving MIP Embedder:" << std::endl;
  for(unsigned q = 0; q < output->size(); ++q)
    {
      std::cout << "(" << (*output)[q].id().ieta() 
	   << "," << (*output)[q].id().iphi()
	   << "," << (*output)[q].SOI_compressedEt() << ")" << std::endl;
    }
  std::cout << std::endl;

  evt.put(output);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HcalTpgMipEmbedder);
