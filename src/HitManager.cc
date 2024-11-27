#include "HitManager.hh"

#include "globals.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

HitManager::HitManager()
{
  // Default configuration of the data saving
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
}

void HitManager::BeginOfRunAction(const G4Run* run)
{
  // Clear all previous data at the beginning of a run
  std::lock_guard<std::mutex> lock(data_mutex);
  data.clear();
}
void HitManager::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  auto filename = analysisManager->GetFileName();

  std::vector<double> Xv;
  std::vector<double> Yv;

  // Configure the data table in the output file
  // Two columns: One for X and one for Y positions.
  // One row per event.
  analysisManager->CreateNtuple("Hits", "X and Y Position of detected Photons");
  analysisManager->CreateNtupleDColumn("X", Xv);
  analysisManager->CreateNtupleDColumn("Y", Yv);
  analysisManager->FinishNtuple();

  if(!filename.empty())
  {
    std::lock_guard<std::mutex> lock(data_mutex);

    G4AnalysisManager::Instance()->OpenFile();

    for (const auto& [key, value]: data)
    {
      // Store the X and Y hit positions into the current row
      Xv.clear(); Yv.clear();
      for (const auto& hit: value)
      {
        Xv.push_back(hit.first);
        Yv.push_back(hit.second);
      }
      // Finish the current and start a new row of data
      analysisManager->AddNtupleRow();
    }

    // Write out the data and close the output file
    analysisManager->Write();
    analysisManager->CloseFile();
  }

}

void HitManager::BeginOfEventAction(const G4Event* event)
{
  std::lock_guard<std::mutex> lock(data_mutex);

  // Create a new data storage entry for each event
  data[event->GetEventID()] = std::vector<std::pair<double, double> >();
  data[event->GetEventID()].reserve(100);
}
void HitManager::EndOfEventAction(const G4Event* event)
{
}

void HitManager::AddHit(double x, double y)
{
  auto event = G4RunManager::GetRunManager()->GetCurrentEvent();
  if(event == nullptr) return;

  // Add the X and Y positions to the data storage
  std::lock_guard<std::mutex> lock(data_mutex);
  data[event->GetEventID()].push_back({x, y});
}
