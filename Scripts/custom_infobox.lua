--[[
    Author: Agustín Alejandro dos Santos
    Description: Custom Infobox for Crianosfera Hack v5 External edition
]]

-- Main default settings
local bEnableCustomInfobox = true

-- At rendering windows area...
local function OnRenderInterface()
    if bEnableCustomInfobox then
        if ImGui.Begin("Infobox", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse) then
            local playerTeam = LocalPlayer.GetTeam()
            local playerFlags = LocalPlayer.GetFlags()

            ImGui.Text("Player index: "..(LocalPlayer.GetIndex()))
            if playerTeam == 1 then
                ImGui.Text("Team: Terrorists")
            elseif playerTeam == 2 then
                ImGui.Text("Team: Counter-Terrorist")
            else
                ImGui.Text("Team: Spectator")
            end
            ImGui.Text("On ground: "..(((playerFlags & FL_ONGROUND) == FL_ONGROUND) and 'Yes' or 'No'))
            ImGui.Text("Ducking: "..(((playerFlags & FL_DUCKING) == FL_DUCKING) and 'Yes' or 'No'))
            ImGui.Text("Alive: "..(LocalPlayer.IsAlive() and 'Yes' or 'No'))
            ImGui.Text("Overlay FPS: "..(ImGui.GetFramesPerSecond()))
            ImGui.Text("Version: "..(_VERSION))
        end
        ImGui.End()
    end
end

local function OnRenderMenu()
    bEnableCustomInfobox = ImGui.Checkbox("Enable Custom Infobox", bEnableCustomInfobox)
end

local function OnLoadSettings(szFile)
    bEnableCustomInfobox = Utils.LoadBoolean(szFile, "Infobox")
end

local function OnSaveSettings(szFile)
    Utils.SaveBoolean(szFile, "Infobox", bEnableCustomInfobox)
end

Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_INTERFACE, OnRenderInterface)
Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_MENU, OnRenderMenu)
Hooks.RegisterCallback(CH5_CALLBACK_AT_LOAD_SETTINGS, OnLoadSettings)
Hooks.RegisterCallback(CH5_CALLBACK_AT_SAVE_SETTINGS, OnSaveSettings)