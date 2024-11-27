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

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

  void AddHit(double, double);

private:
  std::map<int, std::vector< std::pair<double, double> > > data;

  std::mutex data_mutex;
};
