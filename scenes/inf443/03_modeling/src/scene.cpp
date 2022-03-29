#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 15.0f,6.0f,6.0f }, { 0,0,0 });

	int N_terrain_samples = 300;
	float terrain_length = 50;

	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize(terrain_mesh, "terrain");  // cgp::mesh_drawable::initialize
	terrain.shading.color = { 0.6f,0.85f,0.5f };
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

    // texture
    GLuint const texture_image_id = opengl_load_texture_image("../04_textures/a_texture_uv/assets/texture_grass.jpg",GL_REPEAT,GL_REPEAT);
    terrain.texture = texture_image_id;

    mesh const tree_mesh = create_tree();
    tree.initialize(tree_mesh, "tree");  // cgp::mesh_drawable::initialize
    tree.shading.phong.specular = 0.0f; // non-specular terrain material

    tree_positions = generate_positions_on_terrain(20,10);

}




void scene_structure::display()
{

	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(terrain, environment);

    for(vec3 point : tree_positions){
        tree.transform.translation = point;
        draw(tree, environment);
    }
	if (gui.display_wireframe){
        draw_wireframe(terrain, environment);
        draw_wireframe(tree, environment);
    }

}





void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}


