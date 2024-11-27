#include "SimpleCherenkovTankConstruction.hh"

#include "HitManager.hh"
#include "PositionDetector.hh"

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

SimpleCherenkovTankConstruction::SimpleCherenkovTankConstruction(HitManager* hits)
{
  this->hits = hits;


  // Define / retrieve the necessary materials
  fAir = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  fWater = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");


  // Optical properties for water
  // Values for water from:
  // https://github.com/WCSim/WCSim/blob/develop/src/WCSimConstructMaterials.cc
  std::vector<G4double> water_Energy =
  {
    1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV,
    1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV,
    1.74647e-09*GeV, 1.77142e-09*GeV, 1.7971e-09*GeV, 1.82352e-09*GeV,
    1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV,
    1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
    2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV,
    2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV,
    2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV,
    2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV,
    2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
    3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV,
    3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV,
    3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV,
    4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV,
    5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV
  };

  std::vector<G4double> water_RIndex =
  {
    1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014,
    1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
    1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
    1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
    1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
    1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
    1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
    1.38362, 1.39074, 1.39956, 1.41075, 1.42535
  };

  std::vector<G4double> water_Absorption =
  {
    16.1419*cm,  18.278*cm, 21.0657*cm, 24.8568*cm, 30.3117*cm,
    38.8341*cm, 54.0231*cm, 81.2306*cm, 120.909*cm, 160.238*cm,
    193.771*cm, 215.017*cm, 227.747*cm,  243.85*cm, 294.036*cm,
    321.647*cm,  342.81*cm, 362.827*cm, 378.041*cm, 449.378*cm,
    739.434*cm, 1114.23*cm, 1435.56*cm, 1611.06*cm, 1764.18*cm,
    2100.95*cm,  2292.9*cm, 2431.33*cm,  3053.6*cm, 4838.23*cm,
    6539.65*cm, 7682.63*cm, 9137.28*cm, 12220.9*cm, 15270.7*cm,
    19051.5*cm, 23671.3*cm, 29191.1*cm, 35567.9*cm,   42583*cm,
    49779.6*cm, 56465.3*cm,   61830*cm, 65174.6*cm, 66143.7*cm,
    64820*cm,   61635*cm, 57176.2*cm, 52012.1*cm, 46595.7*cm,
    41242.1*cm, 36146.3*cm, 31415.4*cm, 27097.8*cm, 23205.7*cm,
    19730.3*cm, 16651.6*cm, 13943.6*cm, 11578.1*cm, 9526.13*cm
  };

  std::vector<G4double> water_Rayleight =
  {
     386929*cm,  366249*cm,  346398*cm,  327355*cm,  309097*cm,
     291603*cm,  274853*cm,  258825*cm,  243500*cm,  228856*cm,
     214873*cm,  201533*cm,  188816*cm,  176702*cm,  165173*cm,
     154210*cm,  143795*cm,  133910*cm,  124537*cm,  115659*cm,
     107258*cm, 99318.2*cm, 91822.2*cm,   84754*cm, 78097.3*cm,
    71836.5*cm,   65956*cm, 60440.6*cm, 55275.4*cm, 50445.6*cm,
      45937*cm, 41735.2*cm, 37826.6*cm, 34197.6*cm, 30834.9*cm,
    27725.4*cm, 24856.6*cm, 22215.9*cm, 19791.3*cm, 17570.9*cm,
      15543*cm, 13696.6*cm, 12020.5*cm, 10504.1*cm, 9137.15*cm,
    7909.45*cm,  6811.3*cm, 5833.25*cm,  4966.2*cm, 4201.36*cm,
    3530.28*cm, 2944.84*cm, 2437.28*cm, 2000.18*cm,  1626.5*cm,
    1309.55*cm, 1043.03*cm, 821.016*cm,  637.97*cm, 488.754*cm
  };

  G4MaterialPropertiesTable* fWater_mt = new G4MaterialPropertiesTable();
  fWater_mt->AddProperty("RINDEX", water_Energy, water_RIndex);
  fWater_mt->AddProperty("ABSLENGTH", water_Energy, water_Absorption);
  fWater_mt->AddProperty("RAYLEIGH", water_Energy, water_Rayleight);

  fWater->SetMaterialPropertiesTable(fWater_mt);
}


G4VPhysicalVolume* SimpleCherenkovTankConstruction::Construct()
{
  // Geometrical sizes
  G4double tank_x = 30 * CLHEP::m;
  G4double tank_y = 30 * CLHEP::m;
  G4double tank_z = 30 * CLHEP::m;

  G4double sensor_thickness = 2 * CLHEP::mm;

  // The experimental hall walls are all 1m away from the water volume
  G4double expHall_x = tank_x + 1. * m;
  G4double expHall_y = tank_y + 1. * m;
  G4double expHall_z = tank_z + 1. * m;

  // Create experimental hall
  auto fExperimentalHall_box = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box, fAir, "expHall_log");
  auto fExperimentalHall_phys = new G4PVPlacement(nullptr, G4ThreeVector(), fExperimentalHall_log, "expHall", nullptr, false, 0);

  // Do not show the experimental hall in the visualization
  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::GetInvisible());


  // Create the main water volume
  auto fWater_box = new G4Box("water_box", tank_x, tank_y, tank_z);
  fWater_log = new G4LogicalVolume(fWater_box, fWater, "water_log");
  auto fWater_phys = new G4PVPlacement(nullptr, G4ThreeVector(), fWater_log, "water", fExperimentalHall_log, false, 0);


  // Create the (fake) sensitive volume
  // In order to not have reflections, our senstive volume is also water
  auto fSensor_box = new G4Box("sensor_box", tank_x, tank_y, sensor_thickness);
  fSensor_log = new G4LogicalVolume(fSensor_box, fWater, "sensor_log");
  auto fSensor_phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, tank_z + sensor_thickness), fSensor_log, "sensor", fExperimentalHall_log, false, 0);

  return fExperimentalHall_phys;
}

void SimpleCherenkovTankConstruction::ConstructSDandField()
{
  // Assign a detector handler to our (fake) sensor volume
  PositionDetector *sensDet = new PositionDetector("PositionDetector", hits);
  fSensor_log->SetSensitiveDetector(sensDet);
}
