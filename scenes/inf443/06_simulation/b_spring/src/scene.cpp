#include "scene.hpp"



using namespace cgp;


// Spring force applied on particle p_i with respect to position p_j.

// K : stiffness (raideur)
// L0 : static length of the

vec3 spring_force(vec3 const& p_i, vec3 const& p_j, float L0, float K)
{
	vec3 const p = p_i - p_j;
	float const L = norm(p);
	vec3 const u = p / L;

	vec3 const F = -K * (L - L0) * u;
	return F;
}


void scene_structure::simulation_step(float dt)
{
	// Simulation parameters
	float m = 0.1f;       // particle mass
	float K = 1.0f;        // spring stiffness
	float mu = 0.01f;      // damping coefficient

	vec3 g = { 0,0,-9.81f }; // gravity

	// Forces
	vec3 fB_spring = spring_force(pB, pA, L0, K) + spring_force(pB, pC, L0, K);
	vec3 fB_weight = m * g;
	vec3 fB_damping = -mu * vB;

    vec3 fC_spring = spring_force(pC, pB, L0, K);
    vec3 fC_weight = m * g;
    vec3 fC_damping = -mu * vC;



	vec3 fB = fB_spring + fB_weight + fB_damping;
    vB = vB + dt * fB / m;
	pB = pB + dt * vB;


    vec3 fC = fC_spring + fC_weight + fC_damping ;
    vC = vC + dt * fC / m;
    pC = pC + dt * vC;


}


void scene_structure::draw_segment(vec3 const& a, vec3 const& b)
{
	segment.update({ a, b });
	draw(segment, environment);
}

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });


	// Initial position and speed of particles
	// ******************************************* //
	pA = { 0,0,0 };
	vB = { 0,0,0 };

	pB = { 0.0f,0.3f,0.0f };
	vB = { 0,0,0 };

    pC = { 0.1f,0.3f,0.0f };
    vC = { 0,0,0 };

	L0 = 0.4f;

	particle_sphere.initialize(mesh_primitive_sphere(0.05f));
	segments_drawable::default_shader = curve_drawable::default_shader;
	segment.initialize({ {0,0,0},{1,0,0} });

    particle_sphere2.initialize(mesh_primitive_sphere(0.05f));
    segment2.initialize({ {0,0,0},{0.3,0,0} });


}





void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();


	simulation_step(timer.scale * 0.01f);

	particle_sphere.transform.translation = pA;
	particle_sphere.shading.color = { 0,0,0 };
	draw(particle_sphere, environment);

	particle_sphere.transform.translation = pB;
	particle_sphere.shading.color = { 1,0,0 };
	draw(particle_sphere, environment);

	draw_segment(pA, pB);

    particle_sphere2.transform.translation = pC;
    particle_sphere2.shading.color = { 0,1,0 };
    draw(particle_sphere2, environment);

    draw_segment(pB, pC);

}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}
