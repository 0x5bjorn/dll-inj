#include "ImGuiManager.h"
#include "../Application.h"

ImGuiManager::ImGuiManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& app = Application::GetInstance();
    GLFWwindow* window = app.GetWindow();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiManager::~ImGuiManager()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();
}

void ImGuiManager::EndFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    //io.DisplaySize = ImVec2((float)app.GetWindowData().Width, (float)app.GetWindowData().Height);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiManager::DrawTable()
{
    Application& app = Application::GetInstance();
    std::shared_ptr<ProcChunk> procChunk = app.GetProcChunk();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("Processes", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    
    std::lock_guard<std::mutex> lock(procChunk->m_Mutex);
    if (ImGui::BeginTable("table", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
    {
        ImGui::TableSetupColumn("Process name");
        ImGui::TableSetupColumn("Process ID");
        ImGui::TableSetupColumn("Parent process ID");
        ImGui::TableHeadersRow();

        for (auto it = procChunk->m_Processes.rbegin(); it != procChunk->m_Processes.rend(); it++) 
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::SmallButton(it->m_Name.c_str())) { std::cout << std::endl; }
            ImGui::TableNextColumn();
            ImGui::Text("%d", it->m_ProcessId);
            ImGui::TableNextColumn();
            ImGui::Text("%d", it->m_ParentProcessId);
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
}
