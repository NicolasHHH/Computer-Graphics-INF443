#pragma once


#include "cgp/cgp.hpp"

#include <list>

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
};


// Structure of a particle
struct particle_structure
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
};

// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                        // Standard GUI element storage


	// Timer used for the interpolation of the position
	cgp::timer_event_periodic timer;


	std::list<particle_structure> particles; // Storage of all currently active particles
	cgp::mesh_drawable sphere;
	cgp::mesh_drawable disc;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

};





