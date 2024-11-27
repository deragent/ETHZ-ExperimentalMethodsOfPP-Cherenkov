//
// Geant4 Include Files
//
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VUserActionInitialization.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//
// Project Include Files
//
#include "SimpleCherenkovTankConstruction.hh"
#include "HitManager.hh"


//
// Primary particle generation class
//
class ParticleGun : public G4VUserPrimaryGeneratorAction
{
  public:
    ParticleGun()
    {
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

      G4int n_particle;
      fParticleGun = new G4ParticleGun(n_particle = 1);

      fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
      fParticleGun->SetParticleEnergy(1. * MeV);

      fParticleGun->SetParticlePosition(G4ThreeVector(0. ,0., 0.));
    }
    ~ParticleGun() override = default;

    void GeneratePrimaries(G4Event* event) override
    {
      fParticleGun->GeneratePrimaryVertex(event);
    }

    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun = nullptr;
};


//
// Event / Run handler initialization
//
class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(HitManager* hits)
  {
    this->hits = hits;
  }
  ~ActionInitialization() override = default;

  void BuildForMaster() const override
  {
    // Handles runs
    SetUserAction((G4UserRunAction*)hits);
  }

  void Build() const override
  {
    // Handles primary particle generation
    SetUserAction(new ParticleGun());

    // Handles events
    SetUserAction((G4UserEventAction*)hits);
  }

private:
  HitManager* hits;
};


//
// Inspired from LXe Geant4 Example: https://geant4-userdoc.web.cern.ch/Doxygen/examples_doc/html/ExampleLXe.html
//
int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if(argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }
  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);


  // Configure the Physics of the Simulation
  //
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

  // Manually enable optical physics models
  auto opticalPhysics = new G4OpticalPhysics();
  auto opticalParams  = G4OpticalParameters::Instance();

  opticalParams->SetCerenkovMaxPhotonsPerStep(100);
  opticalParams->SetCerenkovMaxBetaChange(10.0);
  opticalParams->SetCerenkovTrackSecondariesFirst(false);

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);


  // Helper class to handle the data storage
  auto hits = new HitManager();

  // Create the Detector
  auto det = new SimpleCherenkovTankConstruction(hits);
  runManager->SetUserInitialization(det);

  // Initialize user defined actions
  runManager->SetUserInitialization(new ActionInitialization(hits));


  // Setup the run manager / visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if(ui)
  {
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else
  {
    // Batch mode
    G4String command  = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  return 0;
}
