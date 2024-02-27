#pragma once
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"



class guiDialog
{
public:
    guiDialog(GLFWwindow *w);

    void CreateGuiDialog(bool &rotateflag);
    bool  SetBufferCoord (float *vertices, size_t countVert);
    void ShowGui();

    virtual ~guiDialog();
private:
    GLFWwindow* window;

    char textBuffer[64];
};


