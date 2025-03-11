

// #include <fcntl.h>
// #include <unistd.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>


// using namespace vl::lidar::serial;

// namespace vl {
// namespace lidar {


// void showPointCloud(std::shared_ptr<vl::lidar::BaseLidarController> controller) {
//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW\n";
//         return;
//     }


//     GLFWwindow* window = initializeWindow("PointCloud Viewer");
//     if (!window) return;
    
//     // pointcloud shader
//     const char* vertexShaderSource = R"(
//         #version 150 core
//         in vec2 aPos;
//         in float aIntensity; // 사용하지 않음
//         uniform mat4 mvp;
//         void main() {
//             gl_Position = mvp * vec4(aPos, 0.0, 1.0);
//             gl_PointSize = 3.0;  // 필요에 따라 점 크기 설정
//         }
//     )";
    
//     const char* fragmentShaderSource = R"(
//         #version 150 core
//         out vec4 FragColor;
//         void main() {
//             FragColor = vec4(1.0); // 흰색으로 출력
//         }
//     )";
    
//     GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    
//     GLuint VAO, VBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
    
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
//     size_t maxPoints = 50; // 필요에 따라 조정
//     glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(vl::lidar::PointCloud), nullptr, GL_DYNAMIC_DRAW);
    
//     // attribute 0: vec2 aPos (x, y) -> offset 0, 2개의 float
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vl::lidar::PointCloud), (void*)0);
//     glEnableVertexAttribArray(0);
    
//     // attribute 1: intensity -> uint8_t, 오프셋은 x, y, distance (3 * sizeof(float)) 후에 위치
//     glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vl::lidar::PointCloud), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);
    
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
    
//     // 점 크기 설정 (필요에 따라 조정)
//     glPointSize(10.0f);
    
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
        
//         // 최신 pointCloud 데이터 가져오기
//         std::vector<vl::lidar::PointCloud> pointCloud = controller->getPointCloud();
//         std::cout <<  "point cloud size : " << pointCloud.size() << std::endl;
        
//         // VBO 업데이트: 새 pointCloud 데이터를 GPU로 복사
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferSubData(GL_ARRAY_BUFFER, 0, pointCloud.size() * sizeof(vl::lidar::PointCloud), pointCloud.data());
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
        
//         // Orthographic projection: 좌표계를 [-halfSize, halfSize] 범위로 매핑
//         glm::mat4 proj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -1.0f, 1.0f);
        
//         // Model 행렬: 모든 점을 (-centerX, -centerY)로 이동하여 중앙 정렬
//         glm::mat4 model = glm::mat4(1.0f);
        
//         // 최종 MVP 변환
//         glm::mat4 mvp = proj * model;
        
//         // 셰이더에 mvp 매트릭스 업데이트
//         glUseProgram(shaderProgram);
//         GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
//         glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        
//         // 화면 클리어 (배경 검정색)
//         glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);
        
//         // 점들을 그리기
//         glBindVertexArray(VAO);
//         glDrawArrays(GL_POINTS, 0, pointCloud.size());
//         glBindVertexArray(0);
        
//         glfwSwapBuffers(window);
//     }
    
    
//     // 자원 해제
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteProgram(shaderProgram);
    
//     glfwDestroyWindow(window);
//     glfwTerminate();
// }

// } // namespace lidar
// } // namespace vl