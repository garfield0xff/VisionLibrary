#ifndef WINDOW_FB_HPP
#define WINDOW_FB_HPP


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


#ifndef __APPLE__
#   define __APPLE__ 0
#endif




class WindowFrameBuffer
{
public:

    WindowFrameBuffer(const char* window_name, int width, int height) 
    : m_window_name(window_name), m_width(width), m_height(height)
    {};

    void initializeWindow();
    GLFWwindow* getWindow();
    GLuint compileShader(const char* source, GLenum shaderType);
    GLuint compileShaderFile(const char* file_path, GLenum shaderType);
    GLuint loadShader(const char* vertex_file_path, const char* fragment_file_path);
    void render2dPoint(const std::vector<float>&  verticies, float fov_deg);
    void render2dVideo();
    void render2dVideo(const std::vector<float>& verticies, float fov_deg);
    bool isCloseWindow();
    void closeWindow();
    
    
protected:
    GLFWwindow* m_window;
    GLuint m_programId;
    GLuint m_texture;
    const char* m_window_name;
    int m_width;
    int m_height;
};



#endif // WINDOW_FB_HPP_