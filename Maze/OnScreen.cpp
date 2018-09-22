#include <OnScreen.h>
#include <GLInclude.h>
#include <Log.h>
#include <math.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

OnScreen::OnScreen(int x, int y, int width, int height, TierGine::IMaterial* material, TierGine::IContext& context): x(x), y(y), width(width), height(height), pipeline(nullptr), material(material)
{
    mesh = context.CreateMesh();
    mesh->AddAtribute(0, TG::CreateTensor(6, 3, {
                                              -1.0f, 1.0f, -1.0f,
                                              1.0f, -1.0f, -1.0f,
                                              1.0f, 1.0f, -1.0f,
                                              -1.0f, 1.0f, -1.0f,
                                              -1.0f, -1.0f, -1.0f,
                                              1.0f, -1.0f, -1.0f
                                          }));
}

OnScreen::OnScreen(int x, int y, int width, int height, TG::IMesh* mesh, TierGine::IMaterial* material, TierGine::IContext& context): mesh(mesh),
    x(x), y(y), width(width), height(height), pipeline(nullptr), material(material)
{
}

void OnScreen::Draw() {

    BindViewport();
    if(material != nullptr) {
        material->Activate(pipeline, textureVar, TG::UniformVariable{});
    }

    mesh->Bind();
    mesh->GetRenderer().RenderWithMode(TG::RM_FILL, TG::PRS_BOTH);
    UnbindViewport();
}

void OnScreen::BindViewport()
{
    glGetIntegerv(GL_VIEWPORT, oldViewport);
    glViewport(x, y, width, height);
}

void OnScreen::UnbindViewport()
{
    glViewport(oldViewport[0], oldViewport[1],
            oldViewport[2], oldViewport[3]);
}

Map::Map(int x, int y, int width, int height, TierGine::IMesh* walls, TierGine::IContext& context):
    OnScreen(x, y, width, height, walls, nullptr, context), player(context.CreateMesh()), posX(0.0f), posY(0.0f), angle(0.0f), scale(0.1f)
{
    player->AddAtribute(0, TG::CreateTensor(3, 3, {
                                                 0.0f, -0.2f, -2.0f,
                                                 0.4f, 0.0f, -2.0f,
                                                 0.0f, 0.2f, -2.0f,
                                             }));
}

void Map::Draw() {
    auto color = GetPipeline()->GetUniformVariable("color");
    auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale)) * glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, angle))) * glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, -1.0f ));
    color.Set(glm::vec3(0.3, 0.2, 0.7));
    GetPipeline()->GetUniformVariable("position").Set(transform);
    BindViewport();
    //glClearColor(0.0f, 0.0f, 0.0f, 0.2f);
    glClear(GL_DEPTH_BUFFER_BIT);
    UnbindViewport();
    OnScreen::Draw();
    color.Set(glm::vec3(0.9, 0.2, 0.2));
    GetPipeline()->GetUniformVariable("position").Set(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f)) *glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.0f, 0.0f )));
    BindViewport();
    player->Bind();
    player->GetRenderer().RenderWithMode(TG::RM_FILL, TG::PRS_BOTH);
    UnbindViewport();
}
