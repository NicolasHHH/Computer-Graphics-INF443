#include "scene.hpp"

#include "models_textures.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,-2.0f,3.0f }, { 0,0,0 });


	// Geometry creation
	//-----------------------------------
	// Create a quadrangle as a mesh
	mesh quadrangle_mesh;
	quadrangle_mesh.position = { {-1,-1,0}, { 1,-1,0}, { 1, 1,0}, {-1, 1,0} };
	quadrangle_mesh.uv = { {0,2}, {2,2}, {2,0}, {0,0} }; // Associate Texture-Coordinates to the vertices of the quadrangle
	quadrangle_mesh.connectivity = { {0,1,2}, {0,2,3} };

	quadrangle_mesh.fill_empty_field(); // (fill with some default values the other buffers (colors, normals) that we didn't filled before)

	// Convert the mesh structure into a mesh_drawable structure
	shape.initialize(quadrangle_mesh, "Shape");

	
	// Texture Image load and association
	//-----------------------------------	

	// Load an image from a file, and send the result to the GPU
	// return its identifier texture_image_id
	GLuint const texture_image_id = opengl_load_texture_image("assets/squirrel.jpg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);

	// Associate the texture_image_id to the image texture used when displaying visual
	shape.texture = texture_image_id;
}





void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(shape, environment);
	draw_wireframe(shape, environment, { 1,0,0 });

}





void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}


