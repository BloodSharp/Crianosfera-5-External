--[[
    Author: Agustín Alejandro dos Santos
    Description: Custom ESP for Crianosfera Hack v5 External edition
]]

-- Main default settings
local bEnableCustomESP = false
local bRenderName = false
local bEnableOutlines = true
local iCustomESPBox = 0 -- Disabled=0 / Rectangle=1 / Only Corners=2
local colorTerrorist = ImColor(255, 128, 0, 255)
local colorCounterTerrorist = ImColor(153, 204, 255, 255)

-- I need to write W2S function this way because the wrapper that I use don't support arguments as references
local function WorldToScreen(vInput, vOutput)
	if Utils.ConvertToScreen(vInput) then
		local vConverted = Utils.GetLastConvertedToScreenVector()
		vOutput.x = vConverted.x
		vOutput.y = vConverted.y
		return true
	end
	return false
end

local function DrawRectangleCorners(vEye2D, iBoxWidth, iBoxHeight, iPointOffset, theColor, thickness)
	-- Left side
	ImGui.Render.AddLine(ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y), ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y + iPointOffset), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y), ImVec2(vEye2D.x - iBoxWidth / 2 + iPointOffset, vEye2D.y), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y + iBoxHeight), ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y + iBoxHeight - iPointOffset), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x - iBoxWidth / 2, vEye2D.y + iBoxHeight), ImVec2(vEye2D.x - iBoxWidth / 2 + iPointOffset, vEye2D.y + iBoxHeight), theColor, thickness)
	-- Right side
	ImGui.Render.AddLine(ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y), ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y + iPointOffset), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y), ImVec2(vEye2D.x + iBoxWidth / 2 - iPointOffset, vEye2D.y), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y + iBoxHeight), ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y + iBoxHeight - iPointOffset), theColor, thickness)
	ImGui.Render.AddLine(ImVec2(vEye2D.x + iBoxWidth / 2, vEye2D.y + iBoxHeight), ImVec2(vEye2D.x + iBoxWidth / 2 - iPointOffset, vEye2D.y + iBoxHeight), theColor, thickness)
end

local function DrawRegularRectangle(x1, y1, iBoxWidth, iBoxHeight, theColor, thickness)
	ImGui.Render.AddRect(ImVec2(x1, y1), ImVec2(x1 + iBoxWidth, y1 + iBoxHeight), theColor, 0, ImDrawCornerFlags_All, thickness)
end

-- Inside the Lua Scripts tab...
local function OnRenderMenu()
    if ImGui.TreeNode("Custom ESP##CUSTOM_ESP", true) then
        ImGui.BeginGroup()
        bEnableCustomESP = ImGui.Checkbox("Enable##CUSTOM_ESP", bEnableCustomESP)
        bRenderName = ImGui.Checkbox("Name##CUSTOM_ESP", bRenderName)
        colorCounterTerrorist = ImGui.ColorEdit4("CT Color##CUSTOM_ESP", colorCounterTerrorist, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)
        ImGui.EndGroup()

        ImGui.SameLine()
        ImGui.BeginGroup()
        bEnableOutlines = ImGui.Checkbox("Outlines##CUSTOM_ESP", bEnableOutlines)
        ImGui.PushItemWidth(100)
        iCustomESPBox = ImGui.Combo("Box##CUSTOM_ESP", iCustomESPBox, "Disabled\0Rectangle\0Corners\0\0")
        colorTerrorist = ImGui.ColorEdit4("TT Color##CUSTOM_ESP", colorTerrorist, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)
        ImGui.PopItemWidth()
        ImGui.EndGroup()

        ImGui.NewLine()
        ImGui.TreePop()
    end
end

-- Rendering at the screen...
local function OnRenderBackground()
    if bEnableCustomESP then
        for index = 1, 32 do
            if index ~= LocalPlayer.GetIndex() then
                if EntityManager.IsAlive(index) then
                    local vOrigin = EntityManager.GetOrigin(index)
                    local vOriginTop = EntityManager.GetOrigin(index)
                    local vOriginBottom = EntityManager.GetOrigin(index)
                    local vOrigin2D = Vector2D(0, 0)
                    local vTop2D = Vector2D(0, 0)
                    local vBottom2D = Vector2D(0, 0)
                    vOriginTop.z = vOriginTop.z + 20
                    vOriginBottom.z = vOriginBottom.z - 40
                    if WorldToScreen(vOrigin, vOrigin2D) and WorldToScreen(vOriginTop, vTop2D) and WorldToScreen(vOriginBottom, vBottom2D) then
                        local iBoxHeight = math.abs(vTop2D.y - vBottom2D.y)
                        local iBoxWidth = iBoxHeight / 1.8
                        local playerTeam = EntityManager.GetTeam(index)
                        local playerColor = ImColor(255, 255, 255, 255)
                        local outlinesColor = ImColor(0, 0, 0, 255)

                        if playerTeam == 1 then -- Terrorist!
                            playerColor = colorTerrorist
                        elseif playerTeam == 2 then -- Counter-Terrorist
                            playerColor = colorCounterTerrorist
                        end

                        if iCustomESPBox == 1 then
                            if bEnableOutlines then
                                DrawRegularRectangle(vTop2D.x - (iBoxWidth / 2) -1, vTop2D.y -1, iBoxWidth, iBoxHeight, outlinesColor, 2)
                                DrawRegularRectangle(vTop2D.x - (iBoxWidth / 2) -1, vTop2D.y +1, iBoxWidth, iBoxHeight, outlinesColor, 2)
                                DrawRegularRectangle(vTop2D.x - (iBoxWidth / 2) +1, vTop2D.y -1, iBoxWidth, iBoxHeight, outlinesColor, 2)
                                DrawRegularRectangle(vTop2D.x - (iBoxWidth / 2) +1, vTop2D.y +1, iBoxWidth, iBoxHeight, outlinesColor, 2)
                            end
                            DrawRegularRectangle(vTop2D.x - (iBoxWidth / 2), vTop2D.y, iBoxWidth, iBoxHeight, playerColor, 2)
                        elseif iCustomESPBox == 2 then
                            local iPointOffset = iBoxWidth * 0.25
                            if bEnableOutlines then
                                DrawRectangleCorners(vTop2D + Vector2D(-1,-1), iBoxWidth, iBoxHeight, iPointOffset, outlinesColor, 2)
                                DrawRectangleCorners(vTop2D + Vector2D(-1, 1), iBoxWidth, iBoxHeight, iPointOffset, outlinesColor, 2)
                                DrawRectangleCorners(vTop2D + Vector2D( 1,-1), iBoxWidth, iBoxHeight, iPointOffset, outlinesColor, 2)
                                DrawRectangleCorners(vTop2D + Vector2D( 1, 1), iBoxWidth, iBoxHeight, iPointOffset, outlinesColor, 2)
                            end
                            DrawRectangleCorners(vTop2D, iBoxWidth, iBoxHeight, iPointOffset, playerColor, 2)
                        end

                        if bRenderName then
                            if bEnableOutlines then
                                ImGui.Render.AddText(ImVec2(vOrigin2D.x - 1, vOrigin2D.y + 4 - 1), outlinesColor, "Name: "..EntityManager.GetName(index))
                                ImGui.Render.AddText(ImVec2(vOrigin2D.x - 1, vOrigin2D.y + 4 + 1), outlinesColor, "Name: "..EntityManager.GetName(index))
                                ImGui.Render.AddText(ImVec2(vOrigin2D.x + 1, vOrigin2D.y + 4 - 1), outlinesColor, "Name: "..EntityManager.GetName(index))
                                ImGui.Render.AddText(ImVec2(vOrigin2D.x + 1, vOrigin2D.y + 4 + 1), outlinesColor, "Name: "..EntityManager.GetName(index))
                            end
                            ImGui.Render.AddText(ImVec2(vOrigin2D.x, vOrigin2D.y + 4), playerColor, "Name: "..EntityManager.GetName(index))
                        end
                    end
                end
            end
        end
    end
end

local function OnLoadSettings(szFile)
    bEnableCustomESP = Utils.LoadBooleanInSection(szFile, "CustomESP", "Enable")
    bRenderName = Utils.LoadBooleanInSection(szFile, "CustomESP", "Name")
    bEnableOutlines = Utils.LoadBooleanInSection(szFile, "CustomESP", "Outlines")
    iCustomESPBox = Utils.LoadIntegerInSection(szFile, "CustomESP", "Box")
    colorTerrorist = Utils.LoadColorInSection(szFile, "CustomESP", "Terrorists")
    colorCounterTerrorist = Utils.LoadColorInSection(szFile, "CustomESP", "CounterTerrorists")
end

local function OnSaveSettings(szFile)
    Utils.SaveBooleanInSection(szFile, "CustomESP", "Enable", bEnableCustomESP)
    Utils.SaveBooleanInSection(szFile, "CustomESP", "Name", bRenderName)
    Utils.SaveBooleanInSection(szFile, "CustomESP", "Outlines", bEnableOutlines)
    Utils.SaveIntegerInSection(szFile, "CustomESP", "Box", iCustomESPBox)
    Utils.SaveColorInSection(szFile, "CustomESP", "Terrorists", colorTerrorist)
    Utils.SaveColorInSection(szFile, "CustomESP", "CounterTerrorists", colorCounterTerrorist)
end

Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_MENU, OnRenderMenu)
Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_BACKGROUND, OnRenderBackground)
Hooks.RegisterCallback(CH5_CALLBACK_AT_LOAD_SETTINGS, OnLoadSettings)
Hooks.RegisterCallback(CH5_CALLBACK_AT_SAVE_SETTINGS, OnSaveSettings)