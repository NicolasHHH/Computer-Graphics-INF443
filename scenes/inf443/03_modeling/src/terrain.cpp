
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height_basic(float x, float y)
{
    vec2 p_0 = { 0, 0 };
    float h_0 = 2.0f;
    float sigma_0 = 3.0f;

    float d = norm(vec2(x, y) - p_0) / sigma_0;

    float z = h_0 * std::exp(-d * d);

    return z;
}

float evaluate_terrain_height(float x, float y)
{
    vec2 p_i[4] = { {-10,-10}, {5,5}, {-3,4}, {6,4} };
    float h_i[4] = {3.0f, -1.5f, 1.0f, 2.0f};
    float sigma_i[4] = {10.0f, 3.0f, 4.0f, 4.0f};

    float z = 0;
    float di = 0;
    for(int i = 0; i<4; i++){
        di = norm(vec2(x, y) - p_i[i]) / sigma_i[i];
        z += h_i[i] *5* std::exp(-di * di);
    }

    float persistency = 1.8f;
    float frequency_gain = 2.0f;
    int octave = 3;
    float terrain_height = 0.5f;

    float const noise = noise_perlin({x/50, y/10}, octave, persistency, frequency_gain);

    return z + terrain_height*noise;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // texture
    terrain.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // texture
            terrain.uv[kv+N*ku] = {u*10,v*10};

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<vec3> generate_positions_on_terrain(int N, float terrain_length){

    std::vector<vec3> positions;
    for(int i = 0; i<N; i++){
        float x = rand_interval(-terrain_length,terrain_length);
        float y = rand_interval(-terrain_length,terrain_length);
        float z = evaluate_terrain_height(x,y);
        // std::cout << "z"<<std::endl;
        vec3 pi = {x,y,z};
        positions.push_back(pi);
    }
    return positions;
}

