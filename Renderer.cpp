#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

// Vertices coordinates
GLfloat vertices[] =
{ // COORDINATES  /  TexCoord //
    -1.f, -1.f, 0.0f, 0.0f, 1.0f, // Lower left corner
    -1.f,  1.f, 0.0f, 0.0f, 0.0f, // Upper left corner
     1.f,  1.f, 0.0f, 1.0f, 0.0f, // Upper right corner
     1.f, -1.f, 0.0f, 1.0f, 1.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
    0, 2, 1, // Upper triangle
    0, 3, 2 // Lower triangle
};

Renderer::Renderer() 
{
    Init();
}

void Renderer::Init()
{
    shaderDefault = std::make_shared<Shader>("default.vert", "default.frag");
    shaderUnlit = std::make_shared<Shader>("unlit.vert", "unlit.frag");
    shaderInverted = std::make_shared<Shader>("unlit.vert", "inverted.frag");
    shaderRepeating = std::make_shared<Shader>("unlit.vert", "repeating.frag");
    shaderSkybox = std::make_shared<Shader>("skybox.vert", "skybox.frag");
    shaderBarrels = std::make_shared<Shader>("barrel.vert", "barrel.frag");

    /*GLint tex0handle = glGetUniformLocation(shaderDefault->ID, "tex0");
    GLint tex1handle = glGetUniformLocation(shaderDefault->ID, "tex1");
    glUniform1i(tex0handle, 0);
    glUniform1i(tex1handle, 1);*/

    // Generates Vertex Array Object and binds it
    vao_quad = std::make_shared<VAO>();
    vao_quad->Bind();

    // Generates Vertex Buffer Object and links it to vertices
    vbo = std::make_shared<VBO>(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    ebo = std::make_shared<EBO>(indices, sizeof(indices));

    // Links VBO to VAO
    vao_quad->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    vao_quad->LinkAttrib(*vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    vao_quad->Unbind();
    vbo->Unbind();
    ebo->Unbind();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawSprite(Texture& texture, glm::mat4 proj, glm::vec2 position,
    glm::vec2 size, float rotate, glm::vec3 color, string shaderName)
{

    std::shared_ptr<Shader> shader = shaderUnlit;
    if (shaderName == "inverted") {
        shader = shaderInverted;
    }
    if (shaderName == "repeating") {
        shader = shaderRepeating;
    }
    if (shaderName == "barrel") {
        shader = shaderBarrels;
    }
    
    // prepare transformations
    shader->Activate();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size/2.0f, 1.0f));

    shader->SetMatrix4("model", model);
    shader->SetMatrix4("view", view);
    shader->SetMatrix4("proj", proj);
    shader->SetVector3f("tint", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    vao_quad->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao_quad->Unbind();
}

//double prevTime = glfwGetTime();

void Renderer::DrawMesh(Mesh& mesh, Texture& texture, glm::mat4 projection, glm::vec3 position, float xrotation, float yrotation, float scale, Camera cam,
    Texture& normalsTexture, string shaderName)
{

    std::shared_ptr<Shader> shader = shaderDefault;
    if (shaderName == "barrel") {
        shader = shaderBarrels;
    }

    // prepare transformations
    shader->Activate();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    
    model = glm::translate(model, glm::vec3(position));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    model = glm::rotate(model, glm::radians(xrotation), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yrotation), glm::vec3(0.0f, 1.0f, 0.0f));

    view = glm::lookAt(cam.position, cam.position + cam.orientation, cam.up);
    proj = glm::perspective(glm::radians(45.0f), (float)(800 / 800), 0.1f, 100.0f);

    shader->SetMatrix4("model", model);
    shader->SetMatrix4("view", view);
    shader->SetMatrix4("proj", proj);
    shader->SetFloat("t", t);


    glActiveTexture(GL_TEXTURE0 + 0);
    texture.Bind();
    shader->SetTextureSampler("tex0", 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    normalsTexture.Bind();
    shader->SetTextureSampler("texNormals", 1);

    mesh.VAO.Bind();
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    mesh.VAO.Unbind();
}


void Renderer::DrawSkybox(Mesh& mesh, Texture& texture, glm::mat4 projection, Camera cam)
{

    std::shared_ptr<Shader> shader = shaderSkybox;

    // prepare transformations
    shader->Activate();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 0.1f, 0.0f));
    model = glm::translate(model, glm::vec3(0, 0, 0));

    view = glm::lookAt(glm::vec3(0, 0, 0), cam.orientation, cam.up);
    proj = glm::perspective(glm::radians(45.0f), (float)(800 / 800), 0.1f, 100.0f);

    shader->SetMatrix4("model", model);
    shader->SetMatrix4("view", view);
    shader->SetMatrix4("proj", proj);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    //cout << mesh.indices.size() << endl;

    mesh.VAO.Bind();
    glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    mesh.VAO.Unbind();
}

void Renderer::SetTime(float time) {
    t = time;
}

void Renderer::Delete() {
    vao_quad->Delete();
    vbo->Delete();
    ebo->Delete();

    shaderDefault->Delete();
    shaderInverted->Delete();
}