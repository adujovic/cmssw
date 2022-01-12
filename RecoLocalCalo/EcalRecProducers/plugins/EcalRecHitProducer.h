#ifndef RecoLocalCalo_EcalRecProducers_EcalRecHitProducer_hh
#define RecoLocalCalo_EcalRecProducers_EcalRecHitProducer_hh

/** \class EcalRecHitProducer
 *   produce ECAL rechits from uncalibrated rechits
 *
 *  \author Shahram Rahatlou, University of Rome & INFN, March 2006
 *
 **/

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

//#include "RecoLocalCalo/EcalRecAlgos/interface/EcalRecHitAbsAlgo.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "RecoLocalCalo/EcalRecProducers/interface/EcalRecHitWorkerBaseClass.h"

class EcalCleaningAlgo;
class EBDetId;
class EEDetId;
class EcalTrigTowerDetId;
class EcalScDetId;

class EcalRecHitProducer : public edm::stream::EDProducer<> {
public:
  explicit EcalRecHitProducer(const edm::ParameterSet& ps);
  ~EcalRecHitProducer() override;
  void produce(edm::Event& evt, const edm::EventSetup& es) override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  const std::string ebRechitCollection_;  // secondary name to be given to EB collection of hits
  const std::string eeRechitCollection_;  // secondary name to be given to EE collection of hits

  const bool doEB_; // consume and use the EB uncalibrated RecHits. An EB collection is produced even if this is false
  const bool doEE_; // consume and use the EE uncalibrated RecHits. An EE collection is produced even if this is false
  const bool recoverEBIsolatedChannels_;
  const bool recoverEEIsolatedChannels_;
  const bool recoverEBVFE_;
  const bool recoverEEVFE_;
  const bool recoverEBFE_;
  const bool recoverEEFE_;
  const bool killDeadChannels_;

  std::unique_ptr<EcalRecHitWorkerBaseClass> worker_;
  std::unique_ptr<EcalRecHitWorkerBaseClass> workerRecover_;

  std::unique_ptr<EcalCleaningAlgo> cleaningAlgo_;

  edm::EDGetTokenT<EBUncalibratedRecHitCollection> ebUncalibRecHitToken_;
  edm::EDGetTokenT<EEUncalibratedRecHitCollection> eeUncalibRecHitToken_;
  edm::EDGetTokenT<std::set<EBDetId> > ebDetIdToBeRecoveredToken_;
  edm::EDGetTokenT<std::set<EEDetId> > eeDetIdToBeRecoveredToken_;
  edm::EDGetTokenT<std::set<EcalTrigTowerDetId> > ebFEToBeRecoveredToken_;
  edm::EDGetTokenT<std::set<EcalScDetId> > eeFEToBeRecoveredToken_;
  edm::ESGetToken<EcalChannelStatus, EcalChannelStatusRcd> ecalChannelStatusToken_;
};

#endif
