#pragma once

#include "cgp/cgp.hpp"
#include "key_positions_structure.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = true;
	bool display_wireframe  = false;
	bool display_terrain   = true;
	bool display_tree      = true;
	bool display_mushroom  = true;
	bool display_billboard = true;
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
	
	cgp::mesh_drawable terrain;

	cgp::mesh_drawable tree;
	cgp::mesh_drawable mushroom;
	cgp::mesh_drawable billboard;

	std::vector<cgp::vec3> tree_position;
	std::vector<cgp::vec3> mushroom_position;
	std::vector<cgp::vec3> billboard_position;

    // Timer used for the animation
    // cgp::timer_basic timer;

    // A helper structure used to store and display the key positions/time
    keyframe_structure keyframe;
    // Timer used for the interpolation of the position
    cgp::timer_interval timer;


    // The entire hierarchy
    cgp::hierarchy_mesh_drawable hierarchy;
	
	void display_trees();
	void display_mushroom();
	void display_billboard();


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
    void mouse_move();

};





