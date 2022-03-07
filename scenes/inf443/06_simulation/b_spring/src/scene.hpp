#pragma once


#include "cgp/cgp.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
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

	void simulation_step(float dt);
	void draw_segment(cgp::vec3 const& a, cgp::vec3 const& b);

	// Drawable structure to display the particles and the spring
	cgp::mesh_drawable particle_sphere;
	cgp::segments_drawable segment;

	// Timer used for the animation
	cgp::timer_basic timer;

	// Particles:
	cgp::vec3 pA; // position of particle A
	cgp::vec3 pB; // position of particle B
	cgp::vec3 vA; // velocity of particle A
	cgp::vec3 vB; // velocity of particle B
	float L0; // Rest-length of spring



	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

};





