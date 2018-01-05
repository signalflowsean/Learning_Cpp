
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLError.h"


static unsigned int CompileShader(unsigned int type, const std::string& source)
{ 
    //std::cout << "Compiling Shader" << std::endl; 
      
    unsigned int id = glCreateShader(type);
    //pointer to the begining of our data
    const char* src = source.c_str(); 
    glShaderSource(id, 1, &src, nullptr); 
    glCompileShader(id); 

    //Error Handling: Anything wrong with the shder?
    int result; 
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); 

    if (result == GL_FALSE)
    { 
        int length; 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); 
        glGetShaderInfoLog(id, length, &length, message); 
        std::cout << "Failed to compile shader!" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl; 
        std::cout << message << std::endl; 
        glDeleteShader(id); 
        return 0; 
    }

    return id; 
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentshader)
{
    //std::cout << "Creating Shader" << std::endl; 

    unsigned int program = glCreateProgram(); 
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(program, vs);  
    glAttachShader(program, fs);
    glLinkProgram(program); 
    glValidateProgram(program); 

    glDeleteShader(vs); 
    glDeleteShader(fs); 

    return program; 
}

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    
    if (!glfwInit()){
        std::cout << "FAILED TO INITIALIZE GLFW!" << std::endl; 
        return -1;
    }

    //COMMANDS TO GET EVERYTHING RUNNING WITH MAC
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)    
    {
        std::cout << "FAILED TO CREATE WINDOW!" << std::endl; 
        glfwTerminate();
        return -1;
    }

    std::cout << "Create windowed mode" << std::endl;
    check_gl_error(); 
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout << "Window context current" << std::endl;
    check_gl_error(); 

    //support for newer OpenGL releases, put the flag glewExperimental = GL_TRUE;
    glewExperimental = GL_TRUE;

    //GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
        std::cout << "FAILED TO INITIALIZE GLEW!" << std:: endl; 

    std::cout << "Glew Initialized" << std::endl;
    check_gl_error(); 

    //print open gl versions
    //std::cout << glGetString(GL_VERSION) << std::endl; 

    //positions of vertex
    float positions[6] = { 
        -0.5f, -0.5,
        -0.0f, 0.5f,
         0.5f, -0.5f
    }; 

    //id of the generated buffer
    unsigned int buffer; 
    //generate buffer(s)
    glGenBuffers(1, &buffer);
    //select buffer to render data
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    std::cout << "Binded buffer to opengl" << std::endl;
    check_gl_error(); 

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);  
    std::cout << "Generated buffer data addeed posistions" << std::endl;
    check_gl_error(); 

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    std::cout << "Enable Vertex Attrib Array" << std::endl;
    check_gl_error(); 
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    std::cout << "Specified layout of vertex buffer" << std::endl;
    check_gl_error(); 

    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec2 position;\n"
        "\n"
        "void main()\n"
        "{\n"
         "  gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
        "}\n";
       
    std::string fragmentShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    
    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    std::cout << "Create shaders" << std::endl;
    check_gl_error(); 

    glUseProgram(shader);  

    std::cout << "Use shaders" << std::endl;
    check_gl_error(); 


    //checks for linking errors
    int  success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}