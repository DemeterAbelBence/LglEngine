#include "UserInterface.hxx"

namespace lgl {
    void UserInterface::renderObjectPicker() {
        static float cameraSpeed = 8.0f;
        static float objectSpeed = 0.05f;

        // BEGIN WIDGET
        ImGui::Begin("Select object");

        if (ImGui::SliderFloat("Camera move speed", &cameraSpeed, 0.01f, 50.0f, "%.2f", 0)) {
            m_scene->setCameraMoveSpeed(cameraSpeed);
        }

        if (ImGui::SliderFloat("Object move speed", &objectSpeed, 0.01f, 2.0f, "%.2f", 0)) {
            m_scene->setObjectMoveSpeed(objectSpeed);
        }

        bool moveCamera = m_scene->getMoveCamera();
        utl::cstr cameraText = moveCamera ? "Move objects" : "Move camera";
        if (ImGui::Button(cameraText)) {
            m_scene->setMoveCamera(!moveCamera);
        }

        if (ImGui::Button("Next object")) {
            m_scene->getNextObject();
        }

        if (moveCamera) {
            ImGui::Text("Currently moving camera");
        }
        else {
            ImGui::Text("Currently moving object");
        }
        ImGui::Text("W-up, S-down, A-left, D-right");
        ImGui::Text("LSHIFT-forward, LCTRL-backward");
        ImGui::Text("Alt-toggle camera mouse capture");


        // END WIDGET
        ImGui::End();
    }

    void UserInterface::renderPrimitivePicker() {
        static utl::cstr items[] = { "triangle", "line" };
        static int currentItem;

        ImGui::Begin("Select primitive");
        ImGui::ListBox("primitives", &currentItem, items, IM_ARRAYSIZE(items));
        ImGui::End();

        currentItem == 0 ? m_scene->setPrimitiveType(GL_TRIANGLES) :
            m_scene->setPrimitiveType(GL_LINE);
    }

    void UserInterface::renderFrameRateBox() {
        ImGui::Begin("Performance:");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }

    void UserInterface::renderSceneObjectLabels() {
		/*const auto& camera = m_scene->getCamera();

        for (const auto& sceneObject : m_scene->getSceneObjects()) {
            utl::str label = sceneObject->getName();
            glm::vec3 objectPos = sceneObject->getPhysicsSolver()->Body.X;

            // Project world position to clip space using camera projection and view
            glm::vec4 clipPos = camera->getP() * camera->getV() * glm::vec4(objectPos, 1.0f);

            // Skip points behind the camera or at infinity
            if (clipPos.w <= 0.0f) {
                continue;
            }

            glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w; // normalized device coords (-1..1)

            // Only draw labels for objects inside the view frustum
            if (ndc.x < -1.0f || ndc.x > 1.0f || ndc.y < -1.0f || ndc.y > 1.0f || ndc.z < -1.0f || ndc.z > 1.0f) {
                continue;
            }

            // Convert NDC to screen coordinates (ImGui uses top-left origin)
            ImVec2 displaySize = ImVec2(io->DisplaySize.x, io->DisplaySize.y);
            float sx = (ndc.x * 0.5f + 0.5f) * displaySize.x;
            float sy = (1.0f - (ndc.y * 0.5f + 0.5f)) * displaySize.y;

            ImDrawList* drawList = ImGui::GetForegroundDrawList();
            ImU32 col = ImGui::GetColorU32(ImGuiCol_Text);
            drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(sx, sy), col, label.c_str());
        }*/
    }

    void UserInterface::renderAdditionalUI() {}

    UserInterface::UserInterface(GLFWwindow* window, utl::sptr<Scene> scene) {
        ImGui::CreateContext();
        io = &ImGui::GetIO(); (void)*io;
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io->WantCaptureKeyboard = false;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        m_scene = scene;
    }

    UserInterface::~UserInterface() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UserInterface::renderUI() {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        renderObjectPicker();
        renderPrimitivePicker();
        renderFrameRateBox();
        renderSceneObjectLabels();

        renderAdditionalUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}