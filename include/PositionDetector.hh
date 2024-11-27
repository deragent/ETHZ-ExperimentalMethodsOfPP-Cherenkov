#pragma once

#include "G4VSensitiveDetector.hh"

class HitManager;

class PositionDetector : public G4VSensitiveDetector
{
public:
	PositionDetector(G4String, HitManager*);
	~PositionDetector() = default;

private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  HitManager* hits;
};
