#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"
#include "interpolation.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 15.0f,6.0f,6.0f }, { 0,0,0 });

    float z = 5.0;
    // Key 3D positions
    buffer<vec3> key_positions =
            { {-1,1,0+z}, {0,1,0+z}, {1,1,0+z}, {1,2,0+z}, {2,2,0+z}, {2,2,1+z}, {2,0,1.5+z}, {1.5,-1,1+z}, {1.5,-1,0+z}, {1,-1,0+z}, {0,-0.5,0+z}, {-1,-0.5,0+z} };
    // Key times (time at which the position must pass in the corresponding position)
    buffer<float> key_times =
            { 0.0f, 1.0f, 2.0f, 2.5f, 3.0f, 3.5f, 3.75f, 4.5f, 5.0f, 6.0f, 7.0f, 8.0f };
    // Initialize the helping structure to display/interact with these positions
    keyframe.initialize(key_positions, key_times);
    // Set timer bounds
    // The timer must span a time interval on which the interpolation can be conducted
    // By default, set the minimal time to be key_times[1], and the maximal time to be key_time[N-2] (enables cubic interpolation)
    int N = key_times.size();
    timer.t_min = key_times[1];
    timer.t_max = key_times[N - 2];
    timer.t = timer.t_min;



	// Initialize the terrain
	int N_terrain_samples = 200;
	float terrain_length = 20;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.color = { 0.6f,0.85f,0.5f };
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material
	terrain.texture = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);

	// Initialize the trees
	tree.initialize(create_tree(), "Tree");
	tree_position = generate_positions_on_terrain(100, terrain_length);

	// Initialize the mushrooms
	mushroom.initialize(create_mushroom(), "Mushroom");
	mushroom_position = generate_positions_on_terrain(75, terrain_length);
	
	// Initialize the grass of billboards
	billboard.initialize(mesh_primitive_quadrangle({ -0.5f,0,0 }, { 0.5f,0,0 }, { 0.5f,0,1 }, { -0.5f,0,1 }), "Quad");
	billboard.texture = opengl_load_texture_image("assets/grass.png");
	billboard.shading.phong = { 0.4f, 0.6f,0,0.2 };
	billboard.transform.scaling = 0.6f;
	billboard_position = generate_positions_on_terrain(75, terrain_length);

    mesh_drawable body;
    mesh_drawable head;
    mesh_drawable beak;
    mesh_drawable left_eye;
    mesh_drawable right_eye;
    mesh_drawable left_arm;
    mesh_drawable right_arm;
    mesh_drawable left_wing;
    mesh_drawable right_wing;

    body.initialize(mesh_primitive_ellipsoid({1.8f, 1.1f, 0.9f},{0,0,0.0f},40,20),"Body");
    //body.transform.scaling = 0.5f;
    head.initialize(mesh_primitive_sphere(0.6f,{0,0,0}), "Head");
    beak.initialize(mesh_primitive_cone(0.25f, 0.5f, {0,0,0}, {std::cos(-0.4),0,std::sin(-0.4)}), "Beak");
    left_eye.initialize(mesh_primitive_sphere(0.12f), "Left eye");
    right_eye.initialize(mesh_primitive_sphere(0.12f), "Right eye");

    left_wing.initialize(mesh_primitive_quadrangle({-0.5f,0,0}, {1,0,0}, {1,1,0}, {0,1,0}), "Left wing");
    right_wing.initialize(mesh_primitive_quadrangle({0.5f,0,0}, {1,0,0}, {1,1,0}, {0,1,0}), "Right wing");
    left_arm.initialize(mesh_primitive_quadrangle({-0.2,0,0}, {1,0,0}, {0.5,-2,0}, {-0.5,-2,0}), "Left arm");
    right_arm.initialize(mesh_primitive_quadrangle({-0.2,0,0}, {1,0,0}, {0.5,2,0}, {-0.5,2,0}), "Right arm");


    vec3 orange = { 0.8f, 0.5f, 0.0f };
    vec3 black = {0,0,0};

    beak.shading.color = orange;
    left_eye.shading.color = black;
    right_eye.shading.color = black;

    hierarchy.add(body,"global frame",{0,0,5.0f});
    hierarchy.add(head,"Body",{2.1f,0,0.3f});
    hierarchy.add(left_eye,"Head",{0.43f,0.2f,0.2f});
    hierarchy.add(right_eye,"Head",{0.43f,-0.2f,0.2f});
    hierarchy.add(beak,"Head",{0.5f,0,-0.2f});

    hierarchy.add(left_arm,"Body",{0.0f,-0.8f,0.1f});
    hierarchy.add(right_arm,"Body",{0.0f,0.8,0.1f});

}





void scene_structure::display()
{

	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

    timer.update();
    // clear trajectory when the timer restart
    if (timer.t < timer.t_min + 0.1f)
        keyframe.trajectory.clear();
    keyframe.display_key_positions(environment);
    vec3 p = interpolation(timer.t, keyframe.key_positions, keyframe.key_times);
    // Display the interpolated position (and its trajectory)
    keyframe.display_current_position(p, environment);

    hierarchy["Body"].transform.translation = p;

    hierarchy["Head"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0.2 }, 0.1*std::cos(timer.t));
    hierarchy["Left arm"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, std::cos(10*timer.t));
    hierarchy["Right arm"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -std::cos(10*timer.t));

    hierarchy.update_local_to_global_coordinates();
    draw(hierarchy, environment);
    if (gui.display_wireframe)
        draw_wireframe(hierarchy, environment);

	// Display the other elements:

	if (gui.display_terrain) {
		draw(terrain, environment);
		if (gui.display_wireframe)
			draw_wireframe(terrain, environment);
	}

	if(gui.display_tree)      display_trees();
	if(gui.display_mushroom)  display_mushroom();
	if(gui.display_billboard) display_billboard();

}


void scene_structure::display_trees()
{
	vec3 const offset = { 0,0,0.05f };
	for (vec3 position: tree_position) {
		tree.transform.translation = position - offset;
		draw(tree, environment);

		if (gui.display_wireframe)
			draw_wireframe(tree, environment);
	}
}

void scene_structure::display_mushroom()
{
	vec3 const offset = { 0,0,0.02f };
	for (vec3 position : mushroom_position) {
		mushroom.transform.translation = position - offset;
		draw(mushroom, environment);

		if(gui.display_wireframe)
			draw_wireframe(mushroom, environment);
	}
}

void scene_structure::display_billboard()
{
	// Find the rotation such that:
	//  the right-direction of the billboard is turned to match the right-direction of the camera => R*{1,0,0} = camera_right
	//  but the billboard remains oriented upward in the z direction => R*{0,0,1} = {0,0,1}
	vec3 camera_right = environment.camera.right();


	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,0,1 }, camera_right, { 0,0,1 });
	billboard.transform.rotation = R;


    // hty : apply texture to transparent background to represent complex forms
    // at first, all opaque objects are shown in the classic way
    // we enable COLOR BLENDING before the display of transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(false);

	vec3 const offset = { 0,0,0.02f };
	for (vec3 position : billboard_position) {
		billboard.transform.translation = position - offset;
		draw(billboard, environment);
	}

	glDepthMask(true);
	glDisable(GL_BLEND);


	if (gui.display_wireframe) {
		for (vec3 position : billboard_position) {
			billboard.transform.translation = position - offset;
			draw_wireframe(billboard, environment);
		}
	}
}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::Checkbox("terrain", &gui.display_terrain);
	ImGui::Checkbox("tree", &gui.display_tree);
	ImGui::Checkbox("mushroom", &gui.display_mushroom);
	ImGui::Checkbox("billboard", &gui.display_billboard);

    ImGui::SliderFloat("Time", &timer.t, timer.t_min, timer.t_max);
    ImGui::SliderFloat("Time scale", &timer.scale, 0.0f, 2.0f);

    // Display the GUI associated to the key position
    keyframe.display_gui();
}

void scene_structure::mouse_move()
{
    // Handle the picking (displacement of the position using mouse drag)
    keyframe.update_picking(inputs, environment);
}
