#pragma once

#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"

#include <map>
#include <vector>
#include <utility>
#include <mutex>

class G4Run;
class G4Event;

class HitManager : public G4UserRunAction, G4UserEventAction
{
public:
  HitManager();

  // Functions called at the start and end of each run
  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

  // Functions called at the start and end of each event (1 simulated primary particle)
  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

  // Function called by the PositionDetector to add a detected photon
  void AddHit(double, double);

private:
  std::map<int, std::vector< std::pair<double, double> > > data;

  std::mutex data_mutex;
};
