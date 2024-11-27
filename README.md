# Simple Geant4 Simulation of Water-based Cherenkov Detector

For the class **Experimental Methods of Particle Physics** (Fall 2024).

Written by: Johannes Wüthrich

To be used on the _essos_ cluster of the Rubbia-Group.

## Compilation
To compile the simulation, run the following commands:

    source setup.sh
    cd build/

    cmake ..
    make

This should create the _SimpleCherenkovTank_ binary in the _build/_ directory.


## Running the Simulation
Ensure that Geant4 is setup and change into the build directory

    source setup.sh
    cd build/

### GUI Operation
When running from a terminal with X11 forwarding (GUI) activated, simply run

    ./SimpleCherenkovTank

This will open the visualizer GUI, from where individual events can be simulated.

### Batch Operation
When running from a non X11 terminal, or to run multiple events in batch mode, use

    ./SimpleCherenkovTank Cherenkov.mac

This (by default), will run 5 events with 1GeV electrons (e-) as the primary particles.
The resulting Cherenkov _images_ will be stored in the file _Cherenkov.root_.

The simulation can be configured (without recompilation) as following:

- To change the type of primary particle, change `/gun/particle e-`
- To change the primary particle energy, change `/gun/energy 1000 MeV`
- To change the output file name, change: `/analysis/setFileName Cherenkov_e-_1000MeV.root`
- To change the number of events, change: `/run/beamOn 5`


### Analysis
A very simplistic analysis, which shows the recorded Cherenkov _images_ is implemented in the `ShowImages.ipynb` python notebook.
Run this notebook by going to <https://essos2.ethz.ch/jupyter/>.

- Ensure that the **Neutrino Lecture 2024** kernel is used!



## Notes
This simulation is on purpose as slim as possible, in order to highlight the main principles of Geant4.
This implies, that the implementation is not as _clean_ as it should be done!

For example:

- Memory management is completely ignored, leading to memory leaks.
- Data storage is implemented in not a cleanest way.
- The simulation has very little configurability.

Given that the main point of this simulation is to give a first introduction to Geant4 these trade-offs are acceptable.

**But please do not take this as a copy-paste base for any future simulation! Rather take this as an inspiration to learn yourself how to implement the basic Geant4 concepts.**
