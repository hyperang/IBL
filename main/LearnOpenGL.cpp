// LearnOpenGL.cpp: 定义应用程序的入口点。
//

#include "LearnOpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <shader.h>
#include <camera.h>
#include <model.h>
#include <light.h>
#include <common.h>
#include <config.h>
#include <pbr.h>

#include <iostream>
#include <vector>

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

const unsigned int DEPTHMAP_WIDTH = 2048;
const unsigned int DEPTHMAP_HEIGHT = 2048;

const float LIGHT_FRUSTUM_NEAR = 5.0f;
const float LIGHT_FRUSTUM_FAR = 30.0f;

const unsigned int IBL_WIDTH = 512;
const unsigned int IBL_HEIGHT = 512;

const unsigned int BRDF_LUT_RESOLATION = 512;

const bool hdr = true;
const float exposure = 1.0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// dear imgui setting
#ifndef glsl_version
const char* glsl_version = "#version 130";
#endif // !glsl_version

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // camera
    // ------
    Camera camera(PERSP, glm::vec3(0.0f, 10.0f, 3.0f), glm::vec3(0.0f, 0.0f, -3.0f), 1.0f, 1.0f, 1.0f);

    // light
    // -----
    DirectionLight dirctionLight(glm::vec3(0.0f, 10.0f, -15.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f), 0.0f);
    std::vector<PointLight> pointLights;
    PointLight pointLight0(glm::vec3(3.0f, 5.0f, -3.0f), 50.0f, 0.2f, glm::vec3(1.0f), 0.0f);
    PointLight pointLight1(glm::vec3(-3.0f, 5.0f, 3.0f), 50.0f, 0.2f, glm::vec3(1.0f), 0.0f);
    pointLights.push_back(pointLight0);
    pointLights.push_back(pointLight1);

    // build and compile our shader program
    // ------------------------------------
    Shader shadowMappingDepth_shader("shader/shadow/shadowMappingDepth.vert", "shader/shadow/shadowMappingDepth.frag");
    Shader shadowCubmappingDepth_shader("shader/shadow/shadowCubmappingDepth.vert", "shader/shadow/shadowCubmappingDepth.geo", "shader/shadow/shadowCubmappingDepth.frag", "shadowCubmappingDepth");
    Shader debug_shader("shader/debug.vert", "shader/debug.frag");

    Shader hdrLighting_shader("Shader/lighting/lighting-common.vert", "Shader/lighting/blinn-phong-pcss.frag");
    Shader hdr_shader("Shader/hdr/hdr.vert", "Shader/hdr/hdr.frag");

    Shader equirectangularToCubemap_shader("shader/ibl/cubemap.vert", "shader/ibl/equirectangularToCubemap.frag");
    Shader backgroundShader("shader/ibl/background.vert", "shader/ibl/background.frag");
    Shader diffuseIrradiance_shader("shader/ibl/cubemap.vert", "shader/ibl/diffuseIrradianceConvolution.frag");
    Shader prefilter_shader("shader/ibl/cubemap.vert", "shader/ibl/prefilter.frag");
    Shader brdfLut_shader("shader/ibl/brdf.vert", "shader/ibl/brdf.frag");
    Shader kcAvgELut_shader("shader/microfacet/kc-avg-e.vert", "shader/microfacet/kc-avg-e.frag", "kcAvgELut");

    Shader metalness_shader("Shader/lighting/lighting-common.vert", "Shader/lighting/pbr-metalness.frag");
    Shader microfacet_shader("Shader/lighting/lighting-common.vert", "Shader/lighting/pbr-microfacet.frag", "microfacet");

    std::vector<Model> scene0;
    std::vector<Model> scene1;

    // Model nanosuit("assets/nanosuit/nanosuit.obj", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.4f, 0.4f, 0.4f));
    // scene.push_back(nanosuit);
    // Model floor("assets/floor/floor.obj", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(20.0f, 20.0f, 20.0f));
    // scene.push_back(floor);
    float xpos = -15.0f;
    for (int i = 0; i < 10; ++i)
    {
        Model mitsuba("assets/mitsuba/mitsuba-sphere.obj", glm::vec3(xpos, 0.0f, -3.0f), glm::vec3(1.5f, 1.5f, 1.5f));
        scene0.push_back(mitsuba);
        xpos += 4.0f;
    }

    Model mitsuba("assets/mitsuba/mitsuba-sphere.obj", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    scene1.push_back(mitsuba);
    

    // uncomment this call to draw in wireframe polygons.
#ifdef __POLYGON__
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    // configure depth map FBO
    // -----------------------
    // create depth textures
    std::vector<unsigned int> depthFbos;
    for (int i = 0; i < pointLights.size() + 1; i++)
    {
        depthFbos.push_back(gen_frame_buffer_object());
    }
    unsigned int depthMapTexture = gen_depthmap_texture(depthFbos[0], DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT);
    std::vector<unsigned int> depthCubmapTextures;
    for (size_t i = 0; i < pointLights.size(); i++)
    {
        depthCubmapTextures.push_back(gen_depthcubemap_texture(depthFbos[i + 1], DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT));
    }

    // configure hdrColorMap FBO
    // -----------------------
    unsigned int hdrFbo = gen_frame_buffer_object();
    unsigned int depthRbo = gen_render_buffer_object();
    unsigned int hdrTexture = gen_hdr16_texture(hdrFbo, depthRbo, SCR_WIDTH, SCR_HEIGHT);

    // settings
    MetalnessSettings pbrSettings(glm::vec3(1.0), 0.5f, 0.5f);
    std::vector<MicrofacetSettings> microSettingsList;
    float roughness = 0.1f;
    for (size_t i = 0; i < scene0.size(); i++)
    {
        MicrofacetSettings settings(glm::vec3(1.0f, 1.0f, 0.0f), roughness);
        microSettingsList.push_back(settings);
        roughness += 0.1f;
        if (roughness >= 1.0f)
        {
            roughness = 1.0f;
        }
    }
    
    // PBR
    // ---
    // pre computation
    unsigned int captureFbo = gen_frame_buffer_object();
    unsigned int captureRbo = gen_render_buffer_object();

    unsigned int iblTexture = load_hdr_env("assets/hdr/Mans_Outside_2k.hdr");
    unsigned int envCubemap = gen_envcubemap_texture(captureFbo, captureRbo, IBL_WIDTH, IBL_HEIGHT, iblTexture, equirectangularToCubemap_shader);

    unsigned int diffuseIrradianceMap = gen_diffuseIrradianceMap(captureFbo, captureRbo, envCubemap, diffuseIrradiance_shader);
    unsigned int specularIrradianceMap = gen_prefilterMap(captureFbo, captureRbo, envCubemap, IBL_WIDTH, IBL_HEIGHT, prefilter_shader);
    unsigned int brdfLut = gen_brdfLut("assets/brdf/brdf.png", captureFbo, captureRbo, BRDF_LUT_RESOLATION, BRDF_LUT_RESOLATION, brdfLut_shader);
    unsigned int kcAvgELut = gen_kcAvgELut(captureFbo, captureRbo, brdfLut, BRDF_LUT_RESOLATION, microSettingsList, kcAvgELut_shader);
    
    
    // dear imgui context creation
    // ---------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glEnable(GL_DEPTH_TEST);

        // render
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_depthmap_texture(depthFbos[0], DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, LIGHT_FRUSTUM_NEAR, LIGHT_FRUSTUM_FAR, dirctionLight, scene0, shadowMappingDepth_shader);
        for (int i = 0; i < depthCubmapTextures.size(); i++)
        {
            render_depthcubemap_texture(depthFbos[i + 1], DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, pointLights[i], scene0, shadowCubmappingDepth_shader);
        }

        // render_hdr16_texture_pbr_metalness(hdrFbo, SCR_WIDTH, SCR_HEIGHT, depthMapTexture, depthCubmapTextures, camera, dirctionLight, pointLights, scene1, diffuseIrradianceMap, specularIrradianceMap, brdfLut, pbrSettings, metalness_shader, envCubemap, backgroundShader);
        render_hdr16_texture_pbr_microfacet(hdrFbo, SCR_WIDTH, SCR_HEIGHT, depthMapTexture, depthCubmapTextures, camera, dirctionLight, pointLights, scene0, specularIrradianceMap, brdfLut, kcAvgELut, microSettingsList, microfacet_shader, envCubemap, backgroundShader);
        
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDisable(GL_DEPTH_TEST);
        hdr_shader.use();
        hdr_shader.setBool("hdr", hdr);
        hdr_shader.setFloat("exposure", exposure);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        renderQuad();

        // input
        processInput(window, camera);

        // dear imgui setup
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera_imgui_window(camera);
        model_imgui_window(camera, scene0);
        point_light_imgui_window(pointLights);
        pbr_imgui_window(pbrSettings);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    // dear imgui: terminate, clearing all previously allocated imgui resources.
    // -------------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
