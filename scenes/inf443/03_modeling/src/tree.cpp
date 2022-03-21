#include "tree.hpp"

using namespace cgp;


mesh create_cylinder_mesh(float radius, float height)
{
    mesh m;

    int N = 9;
    float cx = 0;
    float cy = 0;

    for(int u=0; u<N; u++){
        float x = std::cos(u*2*3.14/N)*radius + cx;
        float y = std::sin(u*2*3.14/N)*radius + cy;

        m.position.push_back(vec3{x,y,0});
        m.position.push_back(vec3{x,y,height});
    }
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});
    for(int k=0; k<m.position.size()-2; k+=2) {
        uint3 triangle = {k, k+1, k+2};
        uint3 triangle2 = {k+1, k+3, k+2};
        m.connectivity.push_back(triangle);
        m.connectivity.push_back(triangle2);
    }

    uint3 triangle = {2*N-2, 2*N-1, 0};
    m.connectivity.push_back(triangle);
    triangle = {2*N-1, 1, 0};
    m.connectivity.push_back(triangle);


    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_tree()
{
    float h = 6.0f; // trunk height
    float r = 1.0f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});

    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4*r, 6*r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 2*r));   // middle-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 4*r));   // top-cone
    foliage.position += vec3(0,0,h);                 // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    // The tree is composted of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}


mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m;

    int N = 20;
    float cx = 0;
    float cy = 0;


    m.position.push_back(vec3{cx,cy,height+z_offset});

    for(int u=0; u<N; u++){
        float x = std::cos(u*2*3.14/N)*radius + cx;
        float y = std::sin(u*2*3.14/N)*radius + cy;
        m.position.push_back(vec3{x,y,z_offset});
    }

    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});
    for(int k=1; k<m.position.size()-1; k++) {
        uint3 triangle = {0, k, k+1};
        m.connectivity.push_back(triangle);
    }
    uint3 triangle = {0, N, 1};
    m.connectivity.push_back(triangle);
    m.fill_empty_field();
    return m;
}


