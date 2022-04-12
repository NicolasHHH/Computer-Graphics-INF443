#include "scene.hpp"



using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-4.0f,2.0f }, { 0,0,0 });

	// Particles are displayed as spheres
	float const sphere_radius = 0.05f;
	sphere.initialize(mesh_primitive_sphere(sphere_radius), "sphere");
	sphere.shading.color = { 0.5f,0.5f,1.0f };

	// Plane described as a disc
	float const disc_radius = 2.0f;
	disc.initialize(mesh_primitive_disc(disc_radius), "disc");
	disc.transform.translation = { 0,0,-sphere_radius };

	// Initial timing between the creation of two particles.
	timer.event_period = 0.6f;
}

void scene_structure::display()
{
	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(disc, environment);

	// Update the current time
	float dt = timer.update();

	bool const new_particle = timer.event;
	if (new_particle == true) {
		vec3 const p0 = { 0,0,0 }; // Initial pose of the particle

		// Initial random velocity (x,y) components are uniformly distributed along a circle.
		const float theta = rand_interval(0, 2 * Pi);
        const float height = rand_interval(3.0f, 7.0f);
		const vec3 v0 = vec3(std::cos(theta), std::sin(theta), 5.0f);

		particles.push_back({ p0,v0 });
	}

	// Evolve position of particles with the potential field
	const vec3 g = { 0.0f,0.0f,-9.81f };
	for (particle_structure& particle : particles)
	{
		const float m = 0.01f; // particle mass

		vec3& p = particle.p; // position
		vec3& v = particle.v; // velocity

		const vec3 F = m * g; // gravity

		// Numerical integration
		v = v + dt * F / m;  // a = F/m
		p = p + dt * v;
	}

	// To do: 
	//  Add the collision handling/bouncing to the particles here
    for (particle_structure& particle : particles)
    {
        vec3& p = particle.p;
        if (p.x*p.x + p.y*p.y < 4.0f && p.x*p.x + p.y*p.y >0.01*0.01){
            vec3& v = particle.v;
            if (p.z < -0.01) {
                p.z = 0;
                v.z = -v.z * 0.6 ;
                v.y *= 0.9 ;
                v.x *= 0.9;
            }
        }
    }

	// Remove particles that are too low
	for (auto it = particles.begin(); it != particles.end(); ) {
		if (it->p.z < -1)
			it = particles.erase(it);
		if (it != particles.end())
			++it;
	}

	// Display particles
	for (particle_structure& particle : particles)
	{
		sphere.transform.translation = particle.p;
		draw(sphere, environment);
	}


}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::SliderFloat("Time scale", &timer.scale, 0.0f, 2.0f);
	ImGui::SliderFloat("Particle emission", &timer.event_period, 0.1f, 2.0f);

}


