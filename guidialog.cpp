#include "guidialog.h"
#include "backends/imgui_impl_opengl3.h"

guiDialog::guiDialog(GLFWwindow *w):window(w)
{
    //Инициализация интерфейса ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void guiDialog::CreateGuiDialog(bool &rotateflag)
{
    //Запуск нового окна ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Элементы интерфейса в окне
    ImGui::Text("Create, rotate line and set Coord with 2,3 button");
    ImGui::Text("Line: %s",textBuffer);
    if (ImGui::Button("Press Me"))
    {
        rotateflag = rotateflag != false ? false : true;
    }
}

#include <string>
bool guiDialog::SetBufferCoord(float *vertices, size_t countVert)
{
    std::string result;
    result += "("+ std::to_string(vertices[0]) + "," +std::to_string(vertices[1])+")";
    result += "("+ std::to_string(vertices[2]) + "," +std::to_string(vertices[3])+")";
    snprintf(textBuffer, sizeof(textBuffer),  "%s", result.c_str());
    return true;
}

void guiDialog::ShowGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

guiDialog::~guiDialog()
{
    //Освобождение ресурсов ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
