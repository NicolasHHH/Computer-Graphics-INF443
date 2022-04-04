#pragma once

#include "cgp/cgp.hpp"


cgp::mesh create_cylinder_mesh(float radius, float height); // trunk
cgp::mesh create_cone_mesh(float radius, float height, float z_offset); // leaf tops

cgp::mesh create_tree();

cgp::mesh create_mushroom();