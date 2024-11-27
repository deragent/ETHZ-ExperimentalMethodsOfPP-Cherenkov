#include "PositionDetector.hh"

#include "HitManager.hh"

#include "G4ParticleTypes.hh"

PositionDetector::PositionDetector(G4String name, HitManager* hits) : G4VSensitiveDetector(name)
{
  this->hits = hits;
}

G4bool PositionDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){

	G4Track *track = aStep->GetTrack(); // Info of track that enters sensitive volume

  // We only detect optical photons
  if(track->GetDefinition()->GetPDGEncoding() == -22)
  {
  	track->SetTrackStatus(fStopAndKill);

  	G4ThreeVector posPhoton = aStep->GetPreStepPoint()->GetPosition();
    hits->AddHit(posPhoton[0]/1000, posPhoton[1]/1000); // Convert to [m]
  }

	return 0;
}
