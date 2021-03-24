#include "CH5_External.h"

int CH5::Aimbot::Target = -1;

void VectorAngles(const float* forward, Vector& angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = ((float)atan2(forward[1], forward[0]) * 180.0f / (float)M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = ((float)atan2(-forward[2], tmp) * 180.0f / (float)M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;

	while (angles[0] < -89) { angles[0] += 180; angles[1] += 180; }
	while (angles[0] > 89) { angles[0] -= 180; angles[1] += 180; }
	while (angles[1] < -180) { angles[1] += 360; }
	while (angles[1] > 180) { angles[1] -= 360; }
}

inline float AngleBetween(Vector firstVector, Vector other)
{
	firstVector = firstVector.Normalize();
	other = other.Normalize();

	return acos(DotProduct(firstVector, other)) * static_cast<float>(180.0 / M_PI);
}

bool VerifyTeam(int index)
{
	if ((CH5::Settings::Aimbot::Team == 0) && (CH5::LocalPlayer::GetTeam() != vPlayers[index].GetTeam()))
		return true;
	else if ((CH5::Settings::Aimbot::Team == 1) && (CH5::LocalPlayer::GetTeam() == vPlayers[index].GetTeam()))
		return true;
	else if (CH5::Settings::Aimbot::Team == 2)
		return true;
	return false;
}

void CH5::Aimbot::Run()
{
	int index;
	CH5::Aimbot::Target = -1;
	if (CH5::Settings::Aimbot::Enable)
	{
		if (CH5::LocalPlayer::IsAlive())
		{
			float flBestFOV = 360.0f;
			float flBestDistance = 8192.0f;
			Vector vForward = CH5::LocalPlayer::GetForwardVector();
			Vector vEye = CH5::LocalPlayer::GetEyePosition();

			if (CH5::Settings::Aimbot::PrioritizeIfSomeOneIsAtShortDistance)
			{
				for (index = 1; index < CH5_MAX_PLAYERS; index++)
				{
					if (index != CH5::LocalPlayer::GetIndex())
					{
						if (vPlayers[index].IsAlive())
						{
							if (VerifyTeam(index))
							{
								float flPlayerDistance = Vector(vPlayers[index].m_vOrigin - vEye).Length();
								//220 Units equals 10 Units from EntityManager.GetDistance(index)
								if ((flPlayerDistance <= 220) && (flPlayerDistance < flBestDistance))
								{
									flBestDistance = flPlayerDistance;
									CH5::Aimbot::Target = index;
								}
							}
						}
					}
				}
			}

			//if we don't have a target by prioritize method
			if (CH5::Aimbot::Target == -1)
			{
				for (index = 1; index < CH5_MAX_PLAYERS; index++)
				{
					if (index != CH5::LocalPlayer::GetIndex())
					{
						if (vPlayers[index].IsAlive())
						{
							if (VerifyTeam(index))
							{
								if (CH5::Settings::Aimbot::AimingMethod == 0)
								{
									Vector vPlayerDistance = vPlayers[index].m_vOrigin - vEye;
									float flPlayerFOV = AngleBetween(vForward, vPlayerDistance);
									if (flPlayerFOV < flBestFOV)
									{
										flBestFOV = flPlayerFOV;
										CH5::Aimbot::Target = index;
									}
								}
								else if (CH5::Settings::Aimbot::AimingMethod == 1)
								{
									Vector vPlayerDistance = vPlayers[index].m_vOrigin - vEye;
									float flPlayerDistance = vPlayerDistance.Length();
									if (flPlayerDistance < flBestDistance)
									{
										flBestDistance = flPlayerDistance;
										CH5::Aimbot::Target = index;
									}
								}
							}
						}
					}
				}
			}

			// Then aim if we have a target
			if (CH5::Aimbot::Target != -1)
			{
				Vector vAimAngles(0, 0, 0);
				Vector vAimTargetOrigin;
				Vector vAimDistance;
				VectorCopy(vPlayers[CH5::Aimbot::Target].m_vOrigin, vAimTargetOrigin);

				VectorSubtract(vAimTargetOrigin, vEye, vAimDistance);
				VectorAngles(vAimDistance, vAimAngles);

				if ((CH5::LocalPlayer::GetButtons() & IN_ATTACK) == IN_ATTACK)
					CH5::LocalPlayer::SetViewAngles(vAimAngles);
			}
		}
	}
}