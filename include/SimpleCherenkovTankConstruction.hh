#pragma once

#include "G4Cache.hh"
#include "G4VUserDetectorConstruction.hh"
#include <CLHEP/Units/SystemOfUnits.h>

class FakePixelDetector;

class G4Box;
// class G4Element;
class G4Material;
// class G4MaterialPropertiesTable;
class G4LogicalVolume;
class G4VPhysicalVolume;

class SimpleCherenkovTankConstruction : public G4VUserDetectorConstruction
{
public:
  SimpleCherenkovTankConstruction();
  ~SimpleCherenkovTankConstruction() override = default;

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

private:
  void DefineMaterials();

  G4Box* fExperimentalHall_box = nullptr;
  G4LogicalVolume* fExperimentalHall_log = nullptr;
  G4VPhysicalVolume* fExperimentalHall_phys = nullptr;

  G4Box* fWater_box = nullptr;
  G4LogicalVolume* fWater_log = nullptr;
  G4VPhysicalVolume* fWater_phys = nullptr;

  // Materials
  G4Material* fAir = nullptr;
  G4Material* fWater = nullptr;

  // Geometry
  G4double fTank_x = 30 * CLHEP::m;
  G4double fTank_y = 30 * CLHEP::m;
  G4double fTank_z = 30 * CLHEP::m;

  // Sensitive Detector
  G4Cache<FakePixelDetector*> fPixel_SD;
};
