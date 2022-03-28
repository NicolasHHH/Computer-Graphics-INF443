#include "scene.hpp"

#include "models_textures.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,0.0f,3.0f }, { 0,0,0 });


	// Geometry creation
	//-----------------------------------
	// Create a quadrangle as a mesh
	mesh quadrangle_mesh;
	quadrangle_mesh.position = {  {-1, 1,0},{-1,-1,0} ,{ 1,-1,0},{ 1, 1,0}};
	//quadrangle_mesh.uv = { {0,1}, {1,1}, {1,0}, {0,0} }; // Associate Texture-Coordinates to the vertices of the quadrangle
    quadrangle_mesh.uv = { {0,2}, {2,2}, {2,0}, {0,0} };
    //quadrangle_mesh.uv = { {0.0,0.5}, {0.5,0.5}, {0.5,0.0}, {0.0,0.0} };
    //quadrangle_mesh.uv = { {0,3}, {1,3}, {1,0}, {0,0} };
	quadrangle_mesh.connectivity = { {1,3,0}, {1,2,3} };

	quadrangle_mesh.fill_empty_field(); // (fill with some default values the other buffers (colors, normals) that we didn't filled before)

	// Convert the mesh structure into a mesh_drawable structure
	//shape.initialize(quadrangle_mesh, "Shape");
    //shape.initialize(torus_with_texture(),"torus");
    trunk.initialize(cylinder_with_texture(),"cyl");
    disc.initialize(disc_with_texture(-2),"disc");
    disc2.initialize(disc_with_texture(2),"disc2");
	// Texture Image load and association
	//-----------------------------------	

	// Load an image from a file, and send the result to the GPU
	// return its identifier texture_image_id
    //GLuint const texture_image_id = opengl_load_texture_image("assets/squirrel.jpg",GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER);
    //GLuint const texture_image_id = opengl_load_texture_image("assets/squirrel.jpg",GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE);
    GLuint const texture_image_id2 = opengl_load_texture_image("assets/tree-ring.png",GL_REPEAT,GL_REPEAT);
    GLuint const texture_image_id = opengl_load_texture_image("assets/trunk.jpg",GL_REPEAT,GL_REPEAT);

	// Associate the texture_image_id to the image texture used when displaying visual
	trunk.texture = texture_image_id;
    disc.texture = texture_image_id2;
    disc2.texture = texture_image_id2;
}





void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(trunk, environment);
    draw(disc, environment);
    draw(disc2, environment);
    if (gui.display_wireframe){
        draw_wireframe(trunk, environment, { 1,0,0 });
        draw_wireframe(disc, environment, { 1,0,0 });
    }

}





void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}


