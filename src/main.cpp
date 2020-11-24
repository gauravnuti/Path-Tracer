// Assignment 03: Raytracing

#include "imgui_setup.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include "planelightsource.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../depends/stb/stb_image.h"
#include "../depends/stb/stb_image_write.h"

#define RENDER_BATCH_COLUMNS 20 // Number of columns to render in a single go. Increase to gain some display/render speed!

Camera *camera;
RenderEngine *engine;

int screen_width = 800, screen_height = 600; // This is window size, used to display scaled raytraced image.
int image_width = 1920, image_height = 1080; // This is raytraced image size. Change it if needed.
GLuint texImage;

World createScene1(World& world){

}

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    // Setup raytracer camera. This is used to spawn rays.
    Vector3D camera_position(0, 0, 10);
    Vector3D camera_target(0, 0, 0); //Looking down -Z axis
    Vector3D camera_up(0, 1, 0);
    float camera_fovy =  45;
    camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, image_width, image_height);

    //Create a world
    World *world = new World;
    world->setAmbient(Color(0.1));
    world->setBackground(Color(0.1, 0.3, 0.7));


    //Create Sandbox limits
    int front_z = 30;
    int back_z = -15;
    int right_x = 10;
    int left_x = -10;
    int bottom_y = 3;
    int top_y = -5;
    
    // //////////////////////////////////////////////////////////////////////////////////////////
    // Scene 1
    // //////////////////////////////////////////////////////////////////////////////////////////
    Material *m1 = new Material(world);
    m1->color = Color(0.1, 0.7, 0.5);
    m1->albedo = 2;

    Material *m2 = new Material(world);
    m2->color = Color(0.7, 0.1, 0.3);
    m2->albedo = 2; m2->specularity=1;


    Material *ground = new Material(world);
    ground->color = Color(0.95, 0.7, 0.7);
    ground->albedo = 1.3;ground->specularity=0.97;

    Material *wall = new Material(world);
    wall->color = Color(0.8, 0.1, 0.1);
    wall->albedo = 1.1;wall->emittance = Color(0.2,0.3,0.2);

    Material *back_wall = new Material(world);
    back_wall->color = Color(0.1, 0.7, 0.1);
    back_wall->albedo = 1.1;back_wall->emittance = Color(0.2,0.2,0.2);

    Material *ceiling = new Material(world);
    ceiling->color = Color(1, 0.7, 0.1);
    ceiling->albedo = 1.1;ceiling->emittance = Color(0.2,0.2,0.2);
    
    
    Object *sphere1 = new Sphere(Vector3D(-4, 0, -10), 3, m1);
    world->addObject(sphere1);

    Object *sphere2 = new Sphere(Vector3D(4, 0, -10), 3, m2);
    world->addObject(sphere2);


    // Creating Sandbox

    // Ground
    Object *ground_traingle1 = new Triangle(Vector3D(right_x, bottom_y, front_z), Vector3D(right_x, bottom_y, back_z), Vector3D(left_x, bottom_y, back_z), ground);
    world->addObject(ground_traingle1);
    Object *ground_traingle2 = new Triangle(Vector3D(left_x, bottom_y, back_z), Vector3D(left_x, bottom_y, front_z), Vector3D(right_x, bottom_y, front_z), ground);
    world->addObject(ground_traingle2);

    // Left Wall
    Object *left_traingle1 = new Triangle( Vector3D(left_x, top_y, back_z), Vector3D(left_x, bottom_y, back_z),Vector3D(left_x, bottom_y, front_z), wall);
    world->addObject(left_traingle1);
    Object *left_traingle2 = new Triangle(Vector3D(left_x, top_y, back_z), Vector3D(left_x, top_y, front_z), Vector3D(left_x, bottom_y, front_z), wall);
    world->addObject(left_traingle2);

    // Right Wall
    Object *right_traingle1 = new Triangle(Vector3D(right_x, bottom_y, back_z), Vector3D(right_x, top_y, back_z), Vector3D(right_x, bottom_y, front_z), wall);
    world->addObject(right_traingle1);
    Object *right_traingle2 = new Triangle(Vector3D(right_x, top_y, back_z), Vector3D(right_x, top_y, front_z), Vector3D(right_x, bottom_y, front_z), wall);
    world->addObject(right_traingle2);

    // Back Wall
    Object *back_traingle1 = new Triangle(Vector3D(left_x, bottom_y, back_z), Vector3D(left_x, top_y, back_z), Vector3D(right_x, bottom_y, back_z), back_wall);
    world->addObject(back_traingle1);
    Object *back_traingle2 = new Triangle(Vector3D(left_x, top_y, back_z), Vector3D(right_x, top_y, back_z), Vector3D(right_x, bottom_y, back_z), back_wall);
    world->addObject(back_traingle2);

    // Ceiling
    Object *top_traingle1 = new Triangle(Vector3D(left_x, top_y, back_z), Vector3D(left_x, top_y, front_z), Vector3D(right_x, top_y, front_z), ceiling);
    world->addObject(top_traingle1);
    Object *top_traingle2 = new Triangle(Vector3D(left_x, top_y, back_z), Vector3D(right_x, top_y, back_z), Vector3D(right_x, top_y, front_z), ceiling);
    world->addObject(top_traingle2);


    LightSource *light1 = new PlaneLightSource(world, Vector3D(-5, -5, 2), Vector3D(5, -5, 2), Vector3D(5, -5, -6), Color(1, 1, 1));
    world->addLight(light1);

    // LightSource *light4 = new PlaneLightSource(world, Vector3D(8, -3, 8), Vector3D(14, -3, 4), Vector3D(14, 3, 4), Color(1, 1, 1));
    // world->addLight(light4);

    // //////////////////////////////////////////////////////////////////////////////////////////


    engine = new RenderEngine(world, camera);

    //Initialise texture
    glGenTextures(1, &texImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool render_status;
        for(int i=0; i<RENDER_BATCH_COLUMNS; i++) 
            render_status = engine->renderLoop(); // RenderLoop() raytraces 1 column of pixels at a time.
        if(!render_status)
        {
            // Update texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texImage);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
        } 

        ImGui::Begin("Lumina", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Size: %d x %d", image_width, image_height);
        if(ImGui::Button("Save")){
          char filename[] = "img.png";
          stbi_write_png(filename, image_width, image_height, 3, camera->getBitmap(),0);        
        }
        //Display render view - fit to width
        int win_w, win_h;
        glfwGetWindowSize(window, &win_w, &win_h);
        float image_aspect = (float)image_width/(float)image_height;
        float frac = 0.95; // ensure no horizontal scrolling
        ImGui::Image((void*)(intptr_t)texImage, ImVec2(frac*win_w, frac*win_w/image_aspect), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteTextures(1, &texImage);

    cleanup(window);

    return 0;
}
