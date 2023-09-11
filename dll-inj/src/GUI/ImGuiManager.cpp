#include "ImGuiManager.h"
#include "../Application.h"
#include "../Injection/Injection.h"

static unsigned long pidForModulesTable = 0;
static std::string pNameForModulesTable = "";

ImGuiManager::ImGuiManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
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

    ImFontConfig config;
    io.Fonts->AddFontFromFileTTF("../Dependencies/imgui/misc/fonts/Roboto-Medium.ttf", 21.0f, &config);
    io.Fonts->Build();
}

ImGuiManager::~ImGuiManager()
{
    m_ShowProcessModulesWindow = false;

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

void ImGuiManager::DrawProcessTable()
{
    Application& app = Application::GetInstance();
    std::shared_ptr<ProcChunk> procChunk = app.GetProcChunk();
    std::vector<std::thread>& appWorkers = app.GetWorkerPool();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    std::lock_guard<std::mutex> lock(procChunk->m_Mutex);
    ImGui::Begin("DLL-inj", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);
    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_FittingPolicyDefault_ | ImGuiTabBarFlags_AutoSelectNewTabs))
    {
        if (ImGui::BeginTabItem("Processes", NULL, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton))
        {
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
                    if (ImGui::Button(it->m_Name.c_str()))
                    {
                        // TODO revision thread pool implementation
                        if (!m_ShowProcessModulesWindow && appWorkers.size() > 1 && appWorkers.rbegin()->joinable())
                        {
                            appWorkers.rbegin()->join();
                            appWorkers.pop_back();
                        }

                        pNameForModulesTable = it->m_Name;
                        m_ShowProcessModulesWindow = true;
                        pidForModulesTable = it->m_ProcessId;
                        if (appWorkers.size() < 2)
                            appWorkers.push_back(std::thread(RefreshProcModules, std::ref(pidForModulesTable), std::ref(m_ShowProcessModulesWindow)));
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", it->m_ProcessId);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", it->m_ParentProcessId);
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (m_ShowProcessModulesWindow)
            ImGuiManager::DrawProcModulesTable();

        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void ImGuiManager::DrawProcModulesTable()
{
    Application& app = Application::GetInstance();
    std::shared_ptr<ProcModulesChunk> procModulesChunk = app.GetProcModulesChunk();
    std::lock_guard<std::mutex> lock(procModulesChunk->m_Mutex);

    if (ImGui::BeginTabItem(pNameForModulesTable.c_str(), &m_ShowProcessModulesWindow))
    {
        ImGui::Text("Process ID: %d", pidForModulesTable);
        ImGui::SameLine();
        if (ImGui::Button("Inject"))
        {
            Injection::InjectDLL(WinODB::ShowOpenDialogBox(), pidForModulesTable);
        }
        ImGui::Separator();

        if (ImGui::BeginTable("table", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
        {
            ImGui::TableSetupColumn("Module name");
            ImGui::TableSetupColumn("Module path");
            ImGui::TableSetupColumn("Module size");
            ImGui::TableHeadersRow();
            for (auto it = procModulesChunk->m_ProcModules.rbegin(); it != procModulesChunk->m_ProcModules.rend(); it++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(it->m_Name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(it->m_Path.c_str());
                ImGui::SetItemTooltip(it->m_Path.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%d", it->m_Size);
            }

            ImGui::EndTable();
        }
        ImGui::EndTabItem();
    }
}

static void RefreshProcModules(unsigned long& pid, bool& showProcessModulesWindow)
{
    Application& app = Application::GetInstance();

    std::cout << "RefreshProcModules Worker started!" << std::endl;
    while (showProcessModulesWindow && app.m_Running)
    {
        app.GetProcModulesChunk()->UpdateProcModules(pid);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "RefreshProcModules Worker finished!" << std::endl;
}
