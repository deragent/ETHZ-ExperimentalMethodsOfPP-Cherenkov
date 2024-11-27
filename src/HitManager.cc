#include "HitManager.hh"

#include "globals.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

HitManager::HitManager()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  analysisManager->SetDefaultFileType("csv");
}

void HitManager::BeginOfRunAction(const G4Run* run)
{
  std::lock_guard<std::mutex> lock(data_mutex);
  data.clear();
}
void HitManager::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  auto filename = analysisManager->GetFileName();

  std::vector<double> Xv;
  std::vector<double> Yv;

  analysisManager->CreateNtuple("Hits", "X and Y Position of detected Photons");
  analysisManager->CreateNtupleDColumn("X", Xv);
  analysisManager->CreateNtupleDColumn("Y", Yv);
  analysisManager->FinishNtuple();

  if(!filename.empty())
  {
    // Save the data to a CSV-like file
    std::lock_guard<std::mutex> lock(data_mutex);

    G4AnalysisManager::Instance()->OpenFile();

    for (const auto& [key, value]: data)
    {
      Xv.clear(); Yv.clear();

      // G4cout << "Event [" << key << "]" << G4endl;
      for (const auto& hit: value)
      {
        // G4cout << '[' << key << "] = (" << hit.first << ", " << hit.second << ")" << G4endl;
        Xv.push_back(hit.first);
        Yv.push_back(hit.second);
      }
      analysisManager->AddNtupleRow();
    }

    analysisManager->Write();
    analysisManager->CloseFile();
  }

}

void HitManager::BeginOfEventAction(const G4Event* event)
{
  std::lock_guard<std::mutex> lock(data_mutex);

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

  std::lock_guard<std::mutex> lock(data_mutex);
  data[event->GetEventID()].push_back({x, y});
}
