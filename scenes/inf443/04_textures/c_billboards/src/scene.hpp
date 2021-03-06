#pragma once

#include "cgp/cgp.hpp"

#include "terrain.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = true;
	bool display_wireframe  = false;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage
	
	cgp::mesh_drawable floor;
	cgp::mesh_drawable quad_1;
	cgp::mesh_drawable quad_2;
    cgp::mesh_drawable terrain;
    perlin_noise_parameters parameters;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	// Display function for semi-transparent shapes
	void display_semiTransparent();
};





