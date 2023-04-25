#pragma once
#include "Engine_Include.h"
#include "CameraSystem.h"

using namespace reality;

enum class SequenceType
{
	ViewSequence,
	TargetSequence,
};

struct SequenceTrack
{
	SequenceType sequence_type;
	XMVECTOR move_vector;
	XMFLOAT2 rotate_pitch_yaw;
	XMVECTOR sequence_target;
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
	SequenceType sequence_type = SequenceType::ViewSequence;
	XMVECTOR sequence_target_pos_ = {0, 0, 0, 0};

private:
	vector<SequenceCutInfo> cuts;
	vector<SequenceTrack> tracks;
	UINT cut_count = 0;
};

