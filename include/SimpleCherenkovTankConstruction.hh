#pragma once

#include "G4VUserDetectorConstruction.hh"
#include <CLHEP/Units/SystemOfUnits.h>

class HitManager;

class G4Box;
class G4Material;
class G4LogicalVolume;

class SimpleCherenkovTankConstruction : public G4VUserDetectorConstruction
{
public:
  SimpleCherenkovTankConstruction(HitManager*);
  ~SimpleCherenkovTankConstruction() override = default;

  // Function which will construct / define the physical layout of the simulation
  G4VPhysicalVolume* Construct() override;
  // Function which will define which parts of the simulation are sensitive / detectors
  void ConstructSDandField() override;

private:
  // References to the different volumes in the simulation
  G4LogicalVolume* fExperimentalHall_log = nullptr;
  G4LogicalVolume* fWater_log = nullptr;
  G4LogicalVolume* fSensor_log = nullptr;

  // Materials
  G4Material* fAir = nullptr;
  G4Material* fWater = nullptr;

  HitManager* hits;
};
