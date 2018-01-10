
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>

#include "GLError.h"

using namespace std; 

struct ShaderProgramSource
{ 
    string VertexSource; 
    string FragmentSource; 
}; 

static ShaderProgramSource ParseShader(const string& filepath)
{ 
    ifstream stream(filepath); 

    enum class ShaderType
    { 
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    stringstream ss[2] ; 
    ShaderType type = ShaderType::NONE; 
    while (getline(stream, line))
    { 
        if (line.find("#shader") != string::npos)
        { 
            if (line.find("vertex") != string::npos)
               type = ShaderType::VERTEX; 
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT; 
        }
        else 
        { 
            ss[int(type)] << line << '\n'; 
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const string& source)
{ 
    //cout << "Compiling Shader" << endl; 
      
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
        cout << "Failed to compile shader!" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl; 
        cout << message << endl; 
        glDeleteShader(id); 
        return 0; 
    }

    return id; 
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentshader)
{
    //cout << "Creating Shader" << endl; 
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
        cout << "FAILED TO INITIALIZE GLFW!" << endl; 
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
        cout << "FAILED TO CREATE WINDOW!" << endl; 
        glfwTerminate();
        return -1;
    }

    //cout << "Create windowed mode" << endl;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //cout << "Window context current" << endl;
    //support for newer OpenGL releases, put the flag glewExperimental = GL_TRUE;
    glewExperimental = GL_TRUE;

 
    if (glewInit() != GLEW_OK)
        cout << "FAILED TO INITIALIZE GLEW!" <<  endl; 
    //cout << "Glew Initialized" << endl;
    
    //print open gl versions
    cout << glGetString(GL_VERSION) << endl; 

    //positions of vertex
    float positions[] = { 
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
       -0.5f,  0.5f,
    }; 

    //declares vertexes as indices
    unsigned int indices[] = { 
        0, 1, 2, 
        2, 3, 0
    };

    //id of the generated buffer
    unsigned int buffer; 
    //generate buffer(s)
    glGenBuffers(1, &buffer);
    //select buffer to render data
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //cout << "Binded buffer to opengl" << endl;
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);  
    //cout << "Generated buffer data addeed posistions" << endl;
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //code to reuse indices
    unsigned int ibo; 
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);  

    glEnableVertexAttribArray(0);
    //cout << "Enable Vertex Attrib Array" << endl;
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //cout << "Specified layout of vertex buffer" << endl;

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); 
    //Prints out text output of shaders
    /*
    cout << "VERTEX" << endl; 
    cout << source.VertexSource << endl; 
    cout << "FRAGMENT" << endl; 
    cout << source.FragmentSource << endl; 
    */

    //unsigned int shader = CreateShader(vertexShader, fragmentShader); 
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //cout << "Create shaders" << endl;
    check_gl_error(); 

    glUseProgram(shader);  
    //cout << "Use shaders" << endl;
    check_gl_error(); 
    
    //checks for linking errors
    int  success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << endl;
    }
    check_gl_error(); 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); 

    glfwTerminate();
    return 0;
}