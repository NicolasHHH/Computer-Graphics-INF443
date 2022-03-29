#include "scene.hpp"
#include "terrain.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 0.0f,2.0f,2.0f }, { 0,0,0 });


	//floor.initialize(mesh_primitive_quadrangle({ -1.5f,-1.5f,0.0f }, { 1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f }, { -1.5f,1.5f,0.0f }), "Floor");
	//floor.shading.color = { 0.75f, 0.8f, 0.5f };


	mesh quad_mesh = mesh_primitive_quadrangle({ 0.0f,0,0.0 }, { 0.1f,0,0.0 }, { 0.1f,0,0.2 }, { 0.0f,0,0.2 });

    mesh terrain_mesh = create_terrain_mesh();
    terrain.initialize(terrain_mesh,"terrain");
    update_terrain(terrain_mesh,terrain,parameters);
	quad_1.initialize(quad_mesh, "Quad 1");

    terrain.texture = opengl_load_texture_image("assets/texture_grass.jpg");
	quad_1.texture = opengl_load_texture_image("assets/grass.png");
	quad_1.shading.phong = { 0.4f, 0.6f,0,1 };
    int const N = std::sqrt(terrain_mesh.position.size());
    float x = 0.0;
    float y = 0.5f;
	quad_1.transform.translation = { x,y,terrain_mesh.position[y*N*N+x*N].z };
}



void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(terrain, environment);
    //draw(quad_1, environment);

	if (gui.display_wireframe) {
		draw_wireframe(quad_1, environment);
		draw_wireframe(terrain, environment);
	}


	// We display the semi-transparent shapes after the non-transparent ones 
	//   (as semi-transparent shapes are not associated to depth buffer write)
	display_semiTransparent();
	
}

void scene_structure::display_semiTransparent()
{
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)

    // mode blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);


	// Re-orient the grass shape to always face the camera direction
	vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0 }); // front-vector of the camera without z-component
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	quad_1.transform.rotation = R;

    draw(quad_1, environment);
    /*
    // Sort transparent shapes by depth to camera
	//   This step can be skipped, but it will be associated to visual artifacts

	// Transform matrix (the same matrix which is applied in the vertices in the shader: T = Projection x View)
	mat4 T = environment.projection.matrix() * environment.camera.matrix_view();
	// Projected vertices (center of quads) in homogeneous coordinates
	vec4 p1 = T * vec4{ 0, -0.5f, 0, 1 };
	vec4 p2 = T * vec4{ 0, +0.5f, 0, 1 };
	// Depth to camera
	float z1 = p1.z / p1.w;
	float z2 = p2.z / p2.w;
	// Display the quads relative to their depth
	if (z1 <= z2) {
		//draw(terrain, environment);
		draw(quad_1, environment);
	}
	else {
		draw(quad_1, environment);
		//draw(terrain, environment);
	}
    */



	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}


