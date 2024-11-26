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


// class CherenkovAction : public G4UserRunAction, G4UserEventAction
// {
// public:
//   CherenkovAction()
//
//   void BeginOfRunAction(const G4Run*) override
//   {
//
//   }
//   void EndOfRunAction(const G4Run*) override
//   {
//
//   }
// }


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


class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization() = default;
  ~ActionInitialization() override = default;

  void Build() const override
  {
    // Handles primary particle generation
    SetUserAction(new ParticleGun());

    // Handles runs and events
    // SetUserAction(new CherenkovAction()); // TODO
  }
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



  // Create the Detector
  auto det = new SimpleCherenkovTankConstruction();
  runManager->SetUserInitialization(det);



  // Configure the Physics of the Simulation
  //
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

  // Manually enable optical physics models
  auto opticalPhysics = new G4OpticalPhysics();
  auto opticalParams  = G4OpticalParameters::Instance();

  opticalParams->SetCerenkovMaxPhotonsPerStep(100);
  opticalParams->SetCerenkovMaxBetaChange(10.0);
  opticalParams->SetCerenkovTrackSecondariesFirst(true);

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);



  // Initialize user defined actions
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize G4 kernel
  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if(ui)
  {
    // interactive mode
    if(ui->IsGUI())
    {
      UImanager->ApplyCommand("/control/execute gui.mac");
    } else
    {
      UImanager->ApplyCommand("/control/execute vis.mac");
    }
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

  // Job termination
  delete visManager;
  delete runManager;
  return 0;


}
