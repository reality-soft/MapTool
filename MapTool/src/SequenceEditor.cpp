#include "SequenceEditor.h"
#include "Widgets.h"

using namespace reality;

void SequenceEditor::Active()
{

	if (GUI->FindWidget<SequenceEditor>("sequence") == nullptr)
	{
		GUI->AddWidget<SequenceEditor>("sequence");
	}
	else
	{
		NOT(GUI->FindWidget<SequenceEditor>("sequence")->open_);
	}
}

void SequenceEditor::Update()
{
}

void SequenceEditor::Render()
{
	if (ImGui::Button("Take Cut"))
	{
		TakeCut();
	}

	if (ImGui::Button("Save Track"))
	{
		SaveTrack("../../Contents/BinaryPackage/DND_SequenceTrack_1.mapdat");
	}

	int i = 0;
	for (auto& track : tracks)
	{
		auto vector = VectorToString(track.move_vector);

		ImGui::Text(string("[Track" + to_string(i++) + "]").c_str());
		ImGui::Text("Move Vector");
		ImGui::Text(vector.c_str());
		ImGui::Text(to_string(track.length).c_str());
		ImGui::Text(to_string(track.rotate_angle_x).c_str());
		ImGui::Text(to_string(track.rotate_angle_y).c_str());
	}
}

void SequenceEditor::TakeCut()
{
	SequenceCutInfo new_cut;
	new_cut.current_world_pos = sys_camera_->GetCamera()->camera_pos;
	
	new_cut.current_camera_pitch = sys_camera_->GetCamera()->pitch_yaw.x;
	new_cut.current_camera_yaw = sys_camera_->GetCamera()->pitch_yaw.y;

	cuts.push_back(new_cut);

	if (cut_count > 0)
	{
		SequenceTrack new_track;
		XMVECTOR move_vector = cuts[cut_count].current_world_pos - cuts[cut_count - 1].current_world_pos;
		new_track.move_vector = XMVector3Normalize(move_vector);
		new_track.length = Vector3Length(move_vector);
		new_track.rotate_angle_x = cuts[cut_count].current_camera_pitch - cuts[cut_count - 1].current_camera_pitch;
		new_track.rotate_angle_y = cuts[cut_count].current_camera_yaw - cuts[cut_count - 1].current_camera_yaw;
	
		tracks.push_back(new_track);
	}

	cut_count++;
}

void SequenceEditor::SaveTrack(string filename)
{
	FileTransfer out_mapdata(filename, WRITE);

	SequenceCutInfo start_cut = cuts.front();
	out_mapdata.WriteBinary<SequenceCutInfo>(&start_cut, 1);

	UINT num_tracks = tracks.size();
	out_mapdata.WriteBinary<UINT>(&num_tracks, 1);

	for (const auto& track : tracks)
	{
		SequenceTrack track_ = track;
		out_mapdata.WriteBinary<SequenceTrack>(&track_, 1);	
	}
}
