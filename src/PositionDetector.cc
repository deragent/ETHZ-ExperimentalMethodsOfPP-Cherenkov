#include "PositionDetector.hh"

#include "HitManager.hh"

#include "G4ParticleTypes.hh"

PositionDetector::PositionDetector(G4String name, HitManager* hits) : G4VSensitiveDetector(name)
{
  this->hits = hits;
}

G4bool PositionDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
  // Get the track which entered the senstive volume
	G4Track *track = aStep->GetTrack();

  // We only detect optical photons (PDG ID -22)
  if(track->GetDefinition()->GetPDGEncoding() == -22)
  {
    // Remove / kill the track
    // This emulates that the sensitive volume absorbs the photon.
  	track->SetTrackStatus(fStopAndKill);

  	G4ThreeVector posPhoton = aStep->GetPreStepPoint()->GetPosition();
    hits->AddHit(posPhoton[0]/1000, posPhoton[1]/1000); // Convert to [m] from [mm]
  }

	return 0;
}
