# AISimSystem

AISimSystem is a Plugin for Epic Games' Unreal Engine 4. It helps simulating pedestrian movement in architectural environments with an agent based AI controller. 

This is part of Virtual Places VR Research Project at Cornell University for Henry Richardson.

- [Getting Started](#Getting-Started)
  - [Prerequisites](#Prerequisites)
  - [Installation](#Installation)
- [Setting up the AISimSystem Plugin](#Setting-up-the-AISimSystem-Plugin)
  - [Setting up the Clock](#Setting-up-the-Clock)
  - [Setting up Navigation](#Setting-up-Navigation)
  - [Setting up Locations](#Setting-up-Locations)
  - [Setting up AI Characters](#Setting-up-AI-Characters)
- [Using the AISimSystem Plugin](#Using-the-AISimSystem-Plugin)
  - [Pedestrian Simulation](#Pedestrian-Simulation)
  - [Car Simulation](#Car-Simulation)
  - [The UMG](#The-UMG)
- [Example Level](#Example-Level)
  
## Getting Started

### Prerequisites

* Unreal Engine 4.22, 4.24, or 4.25

### Installation

1. Download the AISimSystem by clicking on "Clone or download" then "Download ZIP"
2. Create a new Unreal Engine project if you do not have one
3. Using the Windows File Explorer, navigate to your project folder
  * Create a folder named Plugins
  * Unzip the AISimSystem and place it under the folder Plugins
4. Restart the Unreal Editor. Click on yes when asked to rebuild the AISimSystem module
5. In Content Browser, click on View Options and select Show Plugin Content

## Setting up the AISimSystem Plugin

### Setting up the Clock

The Clock counts the day and time for AI controller. Should be used with Sun Position Calculator for correct sun position.

* Install Sun Position Calculator
  * In Edit -> Plugins, search for Sun Position Calculator
  * Enable it, and restart editor if required
  * In Content Broser -> View Options, make sure Show Engine Content and Show Plugin Content are both true
* Use Sun Position Calculator
  * In Content Broser -> Search Path, search for SunPosition
  * In SunPosition Content folder, find BP_SunPosition, and drag it into your level
  * Also place a BP_Sky_Sphere and a DirectionalLight into your level, both can be found in the Place Actors tab
  * Set BP_Sky_Sphere's Directional Light Actor to be the Directional Light
  * Set BP_SunPosition's Sky Sphere to be the BP_Sky_Sphere
* Adding the Clock
  * In AISimSystem Content folder, find BP_TheClock, and drag it into your level
  * In its Details tab -> Time section, you can change Speed Scale (The ratio between speed of time in game and that of the real time), and Start Hour (The hour when the game starts) 

### Setting up Navigation

The AI Agents require a navigation system for them to walk on.

* Adding Navigation
  * In Place Actors tab, search for Nav Mesh Bounds Volume, and drag it into the scene
  * Adjust this volume such that it covers all the walkable areas for agents
* Adding Collision to Static Meshes imported with Datasmith
  * In the folder of the imported model, open the Geometries folder
  * Select all items, right click -> Asset Actions -> Bulk Edit via Property Matrix
  * In StaticMesh -> Body Setup -> Collision Complexity, set it to Use Complex Collision As Simple
* Customizing Navigation
  * You can change the Area Class of meshes or add in Nav Modifier Volume to distinguish between different areas
  * NavArea_Road is the road for cars, AI agents would avoid these areas
  * NavArea_Crossing is the crossing for AI agents to go across roads

### Setting up Locations

Locations are used to define programs and boundaries of areas

* Adding Locations
  * In AISimSystem Content folder, find BP_TheLocation, and drag it into your level
  * Select it in the level and go to the Details tab
  * Select BoxComponent Under RootComponent and adjust its scale, select BP_TheLocation(Self) and adjust its location and rotation until the BoxComponent captures a certain area
* Setting Location Program
  * In the location's Details tab search for Tags, add one element to the Tags under the Actor section
  * You can add one of the following capital words as tags
    ```
    RESTAURANT // restaurant for eating
    OFFICE // office for working
    SHOP // shop for shoppint
    IDLE // idle areas for idling
    ATTRACTOR // attractor that draws agents' attention, for sightseeing
    GATE // gates that can generate agents mimicing people coming in from outside
    // Tag HOME is not needed because agents generated memorizes their home locations for sleeping
    ```
  * There is a LocationText attached to the location that you can customize, it is visible in game as default
  
* Customizing Locations

  There are some properties in Details panel of BP_TheLocation that can be customized for different functions
  * Character Staying Here tab
    * Character to Spawn (Reference to the character class that would be spawned when begin play. The spawned actors treat this location as home for sleeping. Used for residential areas to spawn people living there, and for GATES to spawn people living outside of the scene.) 
    * Character Count (The number of characters spawned when begin play) 
  * Character tab
    * Capacity (The maximum number of characters this place can withstand. Will stop letting characters in when capacity is reached. Default value is maximum int) 
  * For Attractor tab
    * Attraction Level (Int between 0-10. The higher it is, the highter possibility that idling people would be attracted) 
    * Attraction Duration Max, Attraction Duration Min (Together define the range of time a sightseeing agent would spend here) 
  * Character Passing By tab
    * Continuous Spawning (If true, the location would continuously spawn characters. Used for Gates.) 
    * Density Per Ten Sec Min, Density Per Ten Sec Max (Together defines the frequency of character spawning)
    * Pedestrian to Spawn (The character class for continuous spawning)
    
### Setting up AI Characters

AI Characters are the agents that populates the scene. They are controlled by AI controllers and each of them has a task list that acts like a schedule for the character.

* Creating your own Characters
  * In AISimSystem Content -> Characters, find BP_Char_Agent. This is the base class for characters.
  * You can create child classes of it and customize the Mesh for your agents
* Customizing Properties
  * In Details -> Stay Here or Not
    * Stay Here (Set true for character class spawn by homes in theirs Character Staying Here section, false otherwise)
  * Details -> Task
    * Task List (The 24 elements correspond to 24 hours/day. You can set the tasks for your characters)
* The characters you created can either be dragged into the level direactly, or spawned by the locations in the level during gameplay. 

## Using the AISimSystem Plugin

### Pedestrian Simulation

Once the set up process above is finished, the pedestrian simulation should be working in game play automatically.

### Car Simulation

Simple car simulation can be added to simulate cars moving on roads.

* Adding cars to the scene
  * In AISimSystem Content -> AIBasics -> Car, find BP_RoadTrack, and drag it into the scene.
  * There is a RoadSpline component that you can drag and place at the center location of one of your roads.
  * You can add multiple BP_RoadTracks for multiple roads
* Setting Properties of BP_RoadTrack
  * In Details -> Default
    * Is Forward (True if there are cars moving forward)
    * Forward Car Density (Number of car moving forward spawned per second)
    * Forward Road Lane Num (Number of lane of road for cars moving forward)
    * Is Backward
    * Backward Car Density
    * Backward Road Lane Num (Similar properties for cars moving backward)
* Customzing Car Meshes
  * In AISimSystem -> AIBasics -> Car, the blueprint BP_CarSimulator is the car spawned along the road track
  * Customize the mesh for car in BP_CarSimulator if necessary

### The UMG


There is a pre-built UMG system where you can visualize the whole scene through a screen, and select and monitor characters in the scene.

* Setting up UMG
  * In AISimSystem -> AI_UMG, find BP_AISimVolume, and drag it into the level
  * Select it in the level and go to the Details tab
  * Select SphereVolume Under Root and adjust its scale, select BP_AISimVolume(Self) and adjust its location until the SphereVolume captures all elements all buildings in your level
  * Select this actor again and in Details tab, change Auto Spawn UMG to be true
* Now when play, a mini axon of the area you captured with SphereVolume should be generated on the left side of your screen.

* Using the UMG
  * Panning, zooming, and adjusting perspective of the mini map with the sliders and buttons around it
  * Click on the mini map to select characters, UMG for controlling the character can then be spawned.
  
* Advanced UMG
  * When changing location/scale of the CameraView (the mini map), be sure to change LocationButton to the same location/scale. Then, in GetMouseXYRatio function, change the three commented values.
  * You can develop your own way to select characters.
  
## Example Level

There is an example level called AITestLevel located under the example folder of the plugin.