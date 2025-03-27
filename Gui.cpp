#include "Gui.hpp"

#include <Includes/CustomWidgets/Custom.hpp>
#include <Includes/CustomWidgets/WaterMarks.hpp>
#include <Includes/CustomWidgets/Notify.hpp>

#include <Gui/Pages/Combat.hpp>
#include <Gui/Pages/Visuals.hpp>
#include <Gui/Pages/Local.hpp>
#include <Gui/Pages/Exploits.hpp>
#include <Gui/Pages/World.hpp>
#include <Gui/Pages/Settings.hpp>

#include <Core/Features/Exploits/Exploits.hpp>
#include <Core/Features/Esp.hpp>

#include <Security/Api/api.hpp>

int PauseLoop;
inline std::mutex DrawMtx;

void Gui::Rendering()
{
    if (!g_MenuInfo.IsLogged)
    {
        g_MenuInfo.MenuSize = { 680, 500 };
    }
    ImGui::SetNextWindowSize(g_MenuInfo.MenuSize);

    if (!PauseLoop)
    {
        ImGui::SetNextWindowPos(g_Variables.g_vGameWindowSize / 2 - g_MenuInfo.MenuSize / 2);
        PauseLoop++;
    }

    ImGui::Begin(" ", nullptr, ImGuiWindowFlags);
    ImGui::PushFont(g_Variables.m_FontNormal);
    {
        Custom::DrawBackground(g_MenuInfo.IsLogged);
        if (g_MenuInfo.IsLogged)
        {
            ImGui::SetCursorPos(ImVec2(12, 12));
            ImGui::BeginGroup();
            ImGui::PushFont(g_Variables.m_FontSecundary);
            {
                ImGui::SetCursorPos(ImVec2(20, 10));
                if (ImGui::BeginTabBar("TopMenu", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton))
                {
                    if (ImGui::BeginTabItem(ICON_FA_PERSON_RIFLE))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.Combat;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem(ICON_FA_BINOCULARS))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.Visuals;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem(ICON_FA_USER))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.Local;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem(ICON_FA_GLOBE))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.World;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem(ICON_FA_BARS))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.Exploits;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem(ICON_FA_GEAR))
                    {
                        g_MenuInfo.iTabCount = g_MenuInfo.Settings;
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            }
            ImGui::PopFont();
            ImGui::EndGroup();
        }

        g_MenuInfo.TabAlpha = ImClamp(g_MenuInfo.TabAlpha + (5.f * ImGui::GetIO().DeltaTime * (g_MenuInfo.iTabCount == g_MenuInfo.iCurrentPage ? 1.f : -1.f)), 0.f, 1.f);

        if (g_MenuInfo.TabAlpha == 0.f)
            g_MenuInfo.iCurrentPage = g_MenuInfo.iTabCount;

        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, g_MenuInfo.TabAlpha * style->Alpha);

        if (g_MenuInfo.IsLogged)
        {
            ImGui::SetCursorPos(ImVec2(20, 50));
            ImGui::BeginGroup();

            switch (g_MenuInfo.iCurrentPage)
            {
            case g_MenuInfo.Combat:
                Combat::Render();
                break;
            case g_MenuInfo.Visuals:
                Visuals::Render();
                break;
            case g_MenuInfo.Local:
                Local::Render();
                break;
            case g_MenuInfo.World:
                World::Render();
                break;
            case g_MenuInfo.Exploits:
                Exploits::Render();
                break;
            case g_MenuInfo.Settings:
                Settings::Render();
                break;
            }

            ImGui::EndGroup();
        }
        ImGui::PopStyleVar();
    }
    ImGui::PopFont();
    ImGui::End();
}
