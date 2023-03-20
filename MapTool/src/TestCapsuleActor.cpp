#include "TestCapsuleActor.h"

using namespace reality;

void TestCapsuleActor::Create()
{
	C_Camera player_camera;
	C_CapsuleCollision capsule_collision;
	C_StaticMesh static_mesh;
	static_mesh.static_mesh_id = "CapsuleMesh.stmesh";
	static_mesh.vertex_shader_id = "StaticMeshVS.cso";
	static_mesh.world = XMMatrixIdentity();
	static_mesh.local = XMMatrixIdentity();

	player_camera.SetLocalFrom(capsule_collision, 100);
	player_camera.tag = "Player";
	SCENE_MGR->GetRegistry().emplace<C_Camera>(entity_id_, player_camera);
	SCENE_MGR->GetRegistry().emplace<C_CapsuleCollision>(entity_id_, capsule_collision);
	SCENE_MGR->GetRegistry().emplace<C_StaticMesh>(entity_id_, static_mesh);

	transform_tree_.root_node = make_shared<TransformTreeNode>(TYPE_ID(C_CapsuleCollision));
	transform_tree_.AddNodeToNode(TYPE_ID(C_CapsuleCollision), TYPE_ID(C_Camera));
	transform_tree_.AddNodeToNode(TYPE_ID(C_CapsuleCollision), TYPE_ID(C_StaticMesh));

	translation = XMVectorZero();
}

void TestCapsuleActor::Update()
{	
	if (DINPUT->GetKeyState(DIK_W) == KEY_HOLD)
		translation += XMVectorSet(0, 0, 0.1, 0);

	if (DINPUT->GetKeyState(DIK_S) == KEY_HOLD)
		translation += XMVectorSet(0, 0, -0.1, 0);

	if (DINPUT->GetKeyState(DIK_A) == KEY_HOLD)
		translation += XMVectorSet(-0.1, 0, 0, 0);

	if (DINPUT->GetKeyState(DIK_D) == KEY_HOLD)
		translation += XMVectorSet(0.1, 0, 0, 0);

	transform_tree_.root_node->OnUpdate(SCENE_MGR->GetRegistry(), entity_id_, XMMatrixTranslationFromVector(translation));
	auto& view = SCENE_MGR->GetRegistry().view<C_CapsuleCollision>();
	auto& capsule = view.get<C_CapsuleCollision>(entity_id_);
	int a = 0;
}
