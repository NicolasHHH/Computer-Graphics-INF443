// Give access to the complete CGP library
#include "cgp/cgp.hpp" 
#include <iostream> 


using namespace cgp;


// ***************************************************************** //
// Global Variables
//  (These variables are available in any functions of this file)
// ***************************************************************** //


// A helper tool to factorize common default functions (Window initialization, code that starts frame, etc)
helper_common_scene helper_common;

// A storage for the current values of the inputs (mouse, keyboard, window dimension) that can be use for interaction purpose
inputs_interaction_parameters inputs;


bool gui_display_frame = true;
mesh_drawable global_frame;  // The standard global frame

mesh_drawable cube;
mesh_drawable ground;

scene_environment_basic_camera_spherical_coords environment; // Standard environment controler



// ***************************************************************** //
// Fonctions prototypes
//  (in C++ we need to declare the prototype (/signature) of the functions if they are called before their definition)
// ***************************************************************** //


GLFWwindow* standard_window_initialization(int width = 0, int height = 0);

void initialize();
void display_scene();
void display_gui();



// *********************************************** //
// Start of the program
// *********************************************** //


// In C++ the program will always start by a main function
int main(int, char* argv[])
{
	// Debug message (name of the program)
	std::cout << "Run " << argv[0] << std::endl;

	// ************************ //
	//     INITIALISATION
	// ************************ //
	
	// Standard Initialization with dimension in pixels
	GLFWwindow* window = standard_window_initialization(); 
	
	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	initialize();                                              
	std::cout << "Initialization success" << std::endl;


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout<<"Start animation loop ..."<<std::endl;
	while (!glfwWindowShouldClose(window))
	{
		// Reset the screen for a new frame
		helper_common.frame_begin(environment.background_color, window, inputs.window, inputs.mouse.on_gui);
		environment.projection.update_aspect_ratio(inputs.window.aspect_ratio());
		
		// Display the ImGUI interface (button, sliders, etc)
		display_gui();

		// Call the display of the scene
		display_scene();
		
		// End of ImGui display and handle GLFW events
		helper_common.frame_end(window);
	}
	
	// Cleanup
	imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



// This function is called only once at the beginning of the program
void initialize() 
{
	// Initialize the shapes of the scene
	// ***************************************** //

	// Set the behavior of the camera and its initial position
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ -1.0f, 4.0f, 2.0f } /*eye position*/, {0,0,0} /*target position*/);

	// Create a visual frame representing the coordinate system
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	// set a specific shader for the global_frame (to avoids modifying it)
	global_frame.shader = opengl_load_shader("shaders/mesh_reference/vert.glsl", "shaders/mesh_reference/frag.glsl"); 


	// Create a mesh structure (here a cube)
	mesh cube_mesh = mesh_primitive_cube(/*center*/{ 0,0,0 }, /*edge length*/ 1.0f);
	// a mesh is simply a container of vertex data (position,normal,color,uv) and triangle index
	// the mesh data are stored on the CPU memory - they will need to be sent to the GPU memory before being drawn


	// Initialize a "mesh_drawable" from a mesh structure
	//   - mesh : store buffer of data (vertices, indices, etc) on the CPU. The mesh structure is convenient to manipulate in the C++ code but cannot be displayed directly (data is not on GPU).
	//   - mesh_drawable : store VBO associated to elements on the GPU + associated uniform parameters. A mesh_drawable can be displayed using the function draw(mesh_drawable, scene). It only stores the indices of the buffers on the GPU - the buffer of data cannot be directly accessed in the C++ code through a mesh_drawable.
	//   Note: a mesh_drawable can be created from a mesh structure in calling [mesh_drawableName].initialize([meshName], [stringName])
	//         the stringName is an optional information used to help debuging in case of problem linked to the mesh_drawable
	cube.initialize(cube_mesh, "Cube"); 
	cube.shading.color = { 1,1,0 };  // set the color of the cube (R,G,B) - sent as uniform parameter to the shader when display is called.
	cube.transform.translation = { 1,1,0 }; // set the position of the cube - translation applied as uniform parameter to the shader when display is called.

	// Same process for the ground which is a plane
	mesh ground_mesh = mesh_primitive_quadrangle({ -3,-3,-0.51f }, { -3,3,-0.51f }, { 3,3,-0.51f }, { 3,-3,-0.5f });
	ground.initialize(ground_mesh, "Ground");
	ground.texture = opengl_load_texture_image("assets/checkboard.png");

}

// This function is constantly called at every frame
void display_scene()
{

	// Set the light to the current position of the camera
	environment.light = environment.camera.position();

	// the general syntax to display a mesh is:
	//   draw(mesh_drawableName, environment);
	//     Note: scene is used to set the uniform parameters associated to the camera, light, etc. to the shader
	draw(cube, environment);
	draw(ground, environment);

	// conditional display of the global frame (set via the GUI)
	if (gui_display_frame)
		draw(global_frame, environment);
	


}


// This function is constantly called at every frame
void display_gui()
{
	ImGui::Checkbox("Frame", &gui_display_frame);
}






// This function is called everytime the window is resized
void window_size_callback(GLFWwindow* , int width, int height)
{
	inputs.window = { width, height };
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
	inputs.mouse_position_update( { xpos, ypos } );

	// Default trackball mode - change this behavior as you wish
	camera_standard_behavior_rotation_spherical_coordinates(environment.camera, inputs);
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	inputs.mouse.click.update_from_glfw_click(button, action);
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int , int action, int /*mods*/)
{
	inputs.keyboard.update_from_glfw_key(key, action);
}

// Standard initialization procedure
GLFWwindow* standard_window_initialization(int width_target, int height_target)
{
	// Create the window using GLFW
	GLFWwindow* window = cgp::create_window(width_target, height_target);

	// Update storage for window size for the scene
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	inputs.window = { width, height };

	// Display debug information on command line
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	cgp::imgui_init(window);

	// Set the callback functions for the inputs
	glfwSetKeyCallback(window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window, window_size_callback);  // Event called when the window is rescaled        

	// Load default shader and initialize default frame
	helper_common.initialize();

	return window;
}


