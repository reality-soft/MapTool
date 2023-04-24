#pragma once
#include "Engine_Include.h"
#include "CameraSystem.h"

using namespace reality;

struct SequenceTrack
{
	XMVECTOR move_vector;
	float rotate_angle_x;
	float rotate_angle_y;
	float length;
};

struct SequenceCutInfo
{
	XMVECTOR current_world_pos;
	float current_camera_pitch;
	float current_camera_yaw;
};


class SequenceEditor : public reality::GuiWidget
{
public:
	void Active();
	virtual void Update() override;
	virtual void Render() override;

	void TakeCut();
	void SaveTrack(string filename);

	reality::CameraSystem* sys_camera_ = nullptr;
private:
	vector<SequenceCutInfo> cuts;
	vector<SequenceTrack> tracks;
	UINT cut_count = 0;
};

