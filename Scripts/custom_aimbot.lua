--[[
    Author: Agustín Alejandro dos Santos
    Description: Custom Aimbot for Crianosfera Hack v5 External edition
]]

-- Main default settings
local bCustomAimbot = false
local bRenderOrigin = false
local bPrioritizeIfSomeOneIsAtShortDistance = true
local iAimingMethod = 0 -- Field of View: 0 / Distance: 1
local iAimTeam = 0 -- Enemies: 0 / Teammates: 1 / All: 2
local colorNonTarget = ImColor(255, 255, 255, 255) -- White color
local colorAimTarget = ImColor(0, 255, 0, 255) -- Green color

local iTargetAimbot = -1


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

local function DotProduct(vFirstVector, vSecondVector)
    return (vFirstVector.x * vSecondVector.x + vFirstVector.y * vSecondVector.y + vFirstVector.z * vSecondVector.z)
end

local function AngleBetween(vForward, vDifference)
    local vNormalizedForward = vForward:Normalize()
    local vNormalizedDifference = vDifference:Normalize()
    return (math.acos(DotProduct(vNormalizedForward, vNormalizedDifference)) * (180 / math.pi))
end

local function VectorAngles(forward, angles)
	local tmp
	local yaw
	local pitch

	if forward.y == 0 and forward.x == 0 then
		yaw = 0
		if forward.z > 0 then
			pitch = 270
		else
			pitch = 90
		end
	else
		yaw = (math.atan(forward.y, forward.x)) * 180 / math.pi
		if yaw < 0 then
			yaw = yaw + 360
		end

		tmp = math.sqrt(forward.x * forward.x + forward.y * forward.y)
		pitch = (math.atan(-forward.z, tmp)) * 180 / math.pi
		if pitch < 0 then
			pitch = pitch + 360
		end
	end

	angles.x = pitch
	angles.y = yaw
	angles.z = 0

	while angles.x < -89 do
		angles.x = angles.x + 180
		angles.y = angles.y + 180
	end
	while angles.x > 89 do
		angles.x = angles.x - 180
		angles.y = angles.y + 180
	end
	while angles.y < -180 do
		angles.y = angles.y + 360
	end
	while angles.y > 180 do
		angles.y = angles.y - 360
	end
end

local function VerifyTeam(index)
    if (iAimTeam == 0) and (LocalPlayer.GetTeam() ~= EntityManager.GetTeam(index)) then
        return true
    elseif (iAimTeam == 1) and (LocalPlayer.GetTeam() == EntityManager.GetTeam(index)) then
        return true
    elseif (iAimTeam == 2) then
        return true
    end
    return false
end

-- Inside the Lua Scripts tab...
local function OnRenderMenu()
    if ImGui.TreeNode("Custom Aimbot##CUSTOM_AIMBOT", true) then
        bCustomAimbot = ImGui.Checkbox("Enable##CUSTOM_AIMBOT", bCustomAimbot)
        ImGui.BeginGroup()
        bRenderOrigin = ImGui.Checkbox("Show origin##CUSTOM_AIMBOT", bRenderOrigin)
        bPrioritizeIfSomeOneIsAtShortDistance = ImGui.Checkbox("Prioritize short distance##CUSTOM_AIMBOT", bPrioritizeIfSomeOneIsAtShortDistance)
        colorAimTarget = ImGui.ColorEdit4("AimOrigin##CUSTOM_ESP", colorAimTarget, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)
        ImGui.EndGroup()

        ImGui.SameLine()
        ImGui.BeginGroup()
        ImGui.PushItemWidth(120)
        iAimingMethod = ImGui.Combo("Aiming method##CUSTOM_AIMBOT", iAimingMethod, "Field of View\0Distance\0\0")
        iAimTeam = ImGui.Combo("Aim team##CUSTOM_AIMBOT", iAimTeam, "Enemies\0Teammates\0All\0\0")
        colorNonTarget = ImGui.ColorEdit4("NonAimOrigin##CUSTOM_ESP", colorNonTarget, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)
        ImGui.PopItemWidth()
        ImGui.EndGroup()

        ImGui.NewLine()
        ImGui.TreePop()
    end
end

-- Rendering at the screen...
local function OnRenderBackground()
    if bRenderOrigin then
        for index = 1, 32 do
            if index ~= LocalPlayer.GetIndex() then
                if EntityManager.IsAlive(index) then
                    local vOrigin = EntityManager.GetOrigin(index)
                    local vOrigin2D = Vector2D(0, 0)
                    if WorldToScreen(vOrigin, vOrigin2D) then
                        local originColor = ImColor(0, 0, 0, 0)
                        if index == iTargetAimbot then
                            originColor = colorAimTarget
                        else
                            originColor = colorNonTarget
                        end
                        ImGui.Render.AddRectFilled(ImVec2(vOrigin2D.x - 2, vOrigin2D.y - 2), ImVec2(vOrigin2D.x + 2, vOrigin2D.y + 2), originColor, 0, ImDrawCornerFlags_All)
                    end
                end
            end
        end
    end
end

-- At update function, use only logical functions here...
local function OnUpdate()
    iTargetAimbot = -1
    if bCustomAimbot then
        if LocalPlayer.IsAlive() then
            local flBestFOV = 360
            local flBestDistance = 8192
            local vForward = LocalPlayer.GetForwardVector()
            local vEye = LocalPlayer.GetEyePosition()

            if bPrioritizeIfSomeOneIsAtShortDistance then
                for index = 1, 32 do
                    if index ~= LocalPlayer.GetIndex() then
                        if EntityManager.IsAlive(index) then
                            if VerifyTeam(index) then
                                local flPlayerDistance = EntityManager.GetActualDistance(index)
                                -- 220 Units equals 10 Units from EntityManager.GetDistance(index)
                                if (flPlayerDistance <= 220) and (flPlayerDistance < flBestDistance) then
                                    flBestDistance = flPlayerDistance
                                    iTargetAimbot = index
                                end
                            end
                        end
                    end
                end
            end

            -- if we don't have a target by prioritize method
            if iTargetAimbot == -1 then
                for index = 1, 32 do
                    if index ~= LocalPlayer.GetIndex() then
                        if EntityManager.IsAlive(index) then
                            if VerifyTeam(index) then
                                if iAimingMethod == 0 then
                                    local vPlayerDistance = EntityManager.GetOrigin(index) - vEye
                                    local flPlayerFOV = AngleBetween(vForward, vPlayerDistance)
                                    if flPlayerFOV < flBestFOV then
                                        flBestFOV = flPlayerFOV
                                        iTargetAimbot = index
                                    end
                                elseif iAimingMethod == 1 then
                                    local flPlayerDistance = EntityManager.GetActualDistance(index)
                                    if flPlayerDistance < flBestDistance then
                                        flBestDistance = flPlayerDistance
                                        iTargetAimbot = index
                                    end
                                end
                            end
                        end
                    end
                end
            end

            -- Then aim if we have a target
            if iTargetAimbot ~= -1 then
                local vAimAngles = Vector(0, 0, 0)
                local vAimTargetOrigin = EntityManager.GetOrigin(iTargetAimbot)

                VectorAngles(vAimTargetOrigin - vEye, vAimAngles)

                if (LocalPlayer.GetButtons() & IN_ATTACK) == IN_ATTACK then
                    LocalPlayer.SetViewAngles(vAimAngles)
                end
            end
        end
    end
end

local function OnLoadSettings(szFile)
    bCustomAimbot = Utils.LoadBooleanInSection(szFile, "CustomAimbot", "Enable")
    bRenderOrigin = Utils.LoadBooleanInSection(szFile, "CustomAimbot", "Origin")
    bPrioritizeIfSomeOneIsAtShortDistance = Utils.LoadBooleanInSection(szFile, "CustomAimbot", "PrioritizeShortDistance")
    iAimingMethod = Utils.LoadIntegerInSection(szFile, "CustomAimbot", "Method")
    iAimTeam = Utils.LoadIntegerInSection(szFile, "CustomAimbot", "Team")
    colorNonTarget = Utils.LoadColorInSection(szFile, "CustomAimbot", "ColorNonTarget")
    colorAimTarget = Utils.LoadColorInSection(szFile, "CustomAimbot", "ColorTarget")
end

local function OnSaveSettings(szFile)
    Utils.SaveBooleanInSection(szFile, "CustomAimbot", "Enable", bCustomAimbot)
    Utils.SaveBooleanInSection(szFile, "CustomAimbot", "Origin", bRenderOrigin)
    Utils.SaveBooleanInSection(szFile, "CustomAimbot", "PrioritizeShortDistance", bPrioritizeIfSomeOneIsAtShortDistance)
    Utils.SaveIntegerInSection(szFile, "CustomAimbot", "Method", iAimingMethod)
    Utils.SaveIntegerInSection(szFile, "CustomAimbot", "Team", iAimTeam)
    Utils.SaveColorInSection(szFile, "CustomAimbot", "ColorNonTarget", colorNonTarget)
    Utils.SaveColorInSection(szFile, "CustomAimbot", "ColorTarget", colorAimTarget)
end

Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_MENU, OnRenderMenu)
Hooks.RegisterCallback(CH5_CALLBACK_AT_RENDERING_BACKGROUND, OnRenderBackground)
Hooks.RegisterCallback(CH5_CALLBACK_AT_UPDATE, OnUpdate)
Hooks.RegisterCallback(CH5_CALLBACK_AT_LOAD_SETTINGS, OnLoadSettings)
Hooks.RegisterCallback(CH5_CALLBACK_AT_SAVE_SETTINGS, OnSaveSettings)