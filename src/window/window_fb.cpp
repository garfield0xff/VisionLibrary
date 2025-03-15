#include "window_fb.hpp"

void WindowFrameBuffer::initializeWindow() {
    if(!glfwInit()) {
        return;
    }
    if(__APPLE__) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }
    m_window = glfwCreateWindow(m_width, m_height, m_window_name, NULL, NULL);
    if(!m_window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    if(glewInit() != GLEW_OK) {
        return;
    }
    
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

}

GLFWwindow* WindowFrameBuffer::getWindow() {
    return m_window;
}

GLuint WindowFrameBuffer::compileShaderFile(const char* file_path, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);

    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);
    if(shaderStream.is_open()) {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    } else {
        return 0;
    }

    const char *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, NULL);
    glCompileShader(shaderId);

    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader Compilation Failed\n%s\n", infoLog);
        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}

GLuint WindowFrameBuffer::compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success; 
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader Compilation Failed\n%s\n", infoLog);
    }
    return shader;
}


GLuint WindowFrameBuffer::loadShader(const char* vertex_file_path, const char* fragment_file_path) {
    GLuint vertexShaderId = compileShaderFile(vertex_file_path, GL_VERTEX_SHADER);
    GLuint fragmentShaderId = compileShaderFile(fragment_file_path, GL_FRAGMENT_SHADER);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader Linking Failed\n%s\n", infoLog);
        glDeleteProgram(programId);
        programId = 0;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    m_programId = programId;
    return programId;
}

void WindowFrameBuffer::render2dVideo() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glfwSwapBuffers(m_window);

}

void WindowFrameBuffer::render2dPoint(const std::vector<float>& vertices, float fov_deg) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_PROGRAM_POINT_SIZE); 

    float aspect_ratio = static_cast<float>(m_width) / m_height;
    glm::mat4 projection = glm::ortho(-200.f * aspect_ratio, 200.f * aspect_ratio, -200.f, 200.0f, -1.0f, 1.0f);

    GLuint projectionLoc = glGetUniformLocation(m_programId, "projection");
    glUseProgram(m_programId);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    static GLuint vertexArrayId = 0;
    static GLuint vertexBufferId = 0;

    if (vertexArrayId == 0) {
        glGenVertexArrays(1, &vertexArrayId);
        glGenBuffers(1, &vertexBufferId);
    }

    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);

    glfwSwapBuffers(m_window);
}


bool WindowFrameBuffer::isCloseWindow() {
    return glfwWindowShouldClose(m_window);
}

void WindowFrameBuffer::closeWindow() {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}
