#pragma once

#ifndef _common_h_
#define _common_h_

#include <stb_image.h>
#include <stb_image_write.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <shader.h>
#include <light.h>
#include <model.h>
#include <camera.h>
#include <config.h>
#include <pbr.h>

#include <iostream>
#include <vector>

/////// common ///////

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window, Camera& camera);

// render a 1x1 XY quad in NDC
void renderQuad();

// renders a 1x1 cube in NDC
void renderCube();

void renderSphere();

void bindTexture(Shader shader, const char* name, unsigned int& texture, unsigned int& index);

unsigned int gen_frame_buffer_object();

unsigned int gen_render_buffer_object();

/////// shadow ///////

unsigned int gen_depthmap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height);

void render_depthmap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, float near, float far, DirectionLight& light, std::vector<Model>& scene, Shader& shader);

unsigned int gen_depthcubemap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height);

void render_depthcubemap_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, PointLight& light, std::vector<Model>& scene, Shader& shader);

/////// HDR ///////

unsigned int gen_hdr16_texture(unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height);

void render_hdr16_texture(unsigned int& fbo, const unsigned int& width, const unsigned int& height, unsigned int& depthMapTexture, std::vector<unsigned int>& depthCubmappingTextures, Camera& camera, DirectionLight& dlight, std::vector<PointLight> plights, std::vector<Model>& scene, Shader& shader, unsigned int& skyboxTexture, Shader& skyboxShader);

void render_hdr16_texture_pbr_metalness(unsigned int& fbo, const unsigned int& width, const unsigned int& height, unsigned int& depthMapTexture, std::vector<unsigned int>& depthCubmappingTextures, Camera& camera, DirectionLight& dlight, std::vector<PointLight> plights, std::vector<Model>& scene, unsigned int& diffuseIrradianceMap, unsigned int& specularIrradianceMap, unsigned int& brdfLut, MetalnessSettings& settings, Shader& shader, unsigned int& skyboxTexture, Shader& skyboxShader);

/////// IBL ///////

unsigned int load_hdr_env(const char* file);

unsigned int gen_envcubemap_texture(unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height, unsigned int& hdrTexture, Shader& shader);

unsigned int gen_diffuseIrradianceMap(unsigned int& fbo, unsigned int& rbo, unsigned int& envCubemap, Shader& shader);

unsigned int gen_prefilterMap(unsigned int& fbo, unsigned int& rbo, unsigned int& envCubemap, const unsigned int& width, const unsigned int& height, Shader& shader);

unsigned int gen_brdfLut(const char* file, unsigned int& fbo, unsigned int& rbo, const unsigned int& width, const unsigned int& height, Shader& shader);

/////// Microfacet ///////

unsigned int gen_kcAvgELut(unsigned int& fbo, unsigned int& rbo, unsigned int& brdfLut, const unsigned int& width, std::vector<MicrofacetSettings>& settings, Shader& shader);

void render_hdr16_texture_pbr_microfacet(unsigned int& fbo, const unsigned int& width, const unsigned int& height, unsigned int& depthMapTexture, std::vector<unsigned int>& depthCubmappingTextures, Camera& camera, DirectionLight& dlight, std::vector<PointLight> plights, std::vector<Model>& scene, unsigned int& specularIrradianceMap, unsigned int& brdfLut, unsigned int& kcAvgELut, std::vector<MicrofacetSettings>& settings, Shader& shader, unsigned int& skyboxTexture, Shader& skyboxShader);

// imgui window
void camera_imgui_window(Camera& camera);

void model_imgui_window(Camera& camera, std::vector<Model>& scene);

void point_light_imgui_window(std::vector<PointLight>& pointLights);

void pbr_imgui_window(MetalnessSettings& settings);


#endif // !_common_h_
