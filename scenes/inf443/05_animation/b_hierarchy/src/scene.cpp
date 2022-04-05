#include "scene.hpp"



using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });


	// Create the hierarchy
	// ************************************ //
    // struct hierarchy_mesh_drawable node
    //     mesh_drawable node.drawable
    //     string        node.name_parent
    //     tf            node.transform
    //     tf            node.global_transform
    //     void add(mesh_drawable, name_parent,affine_rts);
    //     root = "global_frame"
    //
    //     hierarchy[nomElement].transformation.translation/rotation =
    //     hierarchy.update_local_to_global_coordinates()


	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable cube_base;
	mesh_drawable cylinder_base;
	mesh_drawable sphere_junction;
	mesh_drawable cylinder1;
	mesh_drawable cube1;
	mesh_drawable cylinder1_son;

    mesh_drawable cylinder2;
    mesh_drawable cylinder3;

	// Create the geometry of the meshes
	// Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	cube_base.initialize(mesh_primitive_cube(), "Cube base"); cube_base.transform.scaling = 0.15f;
	cylinder_base.initialize(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 0,0,0.5f }), "Cylinder base");
	sphere_junction.initialize(mesh_primitive_sphere(0.1f), "Sphere junction");
	cylinder1.initialize(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 1.0f,0,0 }), "Cylinder1");
	cube1.initialize(mesh_primitive_cube(), "Cube1"); cube1.transform.scaling = 0.15f;
	cylinder1_son.initialize(mesh_primitive_cylinder(0.03f, { 0,0,-0.25f }, { 0.0f,0,0.25f }), "Cylinder1 son");

    cylinder2.initialize(mesh_primitive_cylinder(0.03f, { -0.2f,0,-0.25f }, { 0.2f,0,-0.25f }),"Cylinder2");
    cylinder3.initialize(mesh_primitive_cylinder(0.03f, { -0.2f,0,+0.25f }, { 0.2f,0,+0.25f }),"Cylinder3");

	// Set the color of some elements
	vec3 color1 = { 0.8f, 0.5f, 0.7f };
	cylinder1.shading.color = color1;
	cube1.shading.color = color1;
	cylinder1.shading.color = color1;
	cylinder1_son.shading.color = color1;

    vec3 color2 = {0.5f,0.9f,0.3f};
    cylinder2.shading.color = color2;
    cylinder3.shading.color = color2;

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	hierarchy.add(cube_base);
	hierarchy.add(cylinder_base, "Cube base");
	hierarchy.add(sphere_junction, "Cylinder base", { 0,0,0.5f }); // the translation is used to place the sphere at the extremity of the cylinder
	hierarchy.add(cylinder1, "Sphere junction");
	hierarchy.add(cube1, "Cylinder1", { 1.0f,0,0 }); // the translation is used to place the cube at the extremity of the cylinder
	hierarchy.add(cylinder1_son, "Cube1");
    hierarchy.add(cylinder2,"Cylinder1 son",{0,0,0});
    hierarchy.add(cylinder3,"Cylinder1 son",{0,0,0});
}





void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	// Apply transformation to some elements of the hierarchy
	hierarchy["Cylinder1"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, timer.t);
	hierarchy["Cube1"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -3 * timer.t);

    hierarchy["Cylinder2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -3 * timer.t);
    hierarchy["Cylinder3"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -3 * timer.t);

	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);
}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}
