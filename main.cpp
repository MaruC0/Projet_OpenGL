#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include <string>

int main()
{

    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer;

    std::string shader_dir = viewer.shader_dir;

    viewer.run();
}