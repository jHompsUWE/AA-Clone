#pragma once

#include "gameobject.h"
#include "GameData.h"
class OrthographicCamera : public GameObject
{
public:
	OrthographicCamera(int win_x, int win_y);
	~OrthographicCamera();

	virtual void Tick(GameData* _GD) override;

	virtual void Draw(DrawData* _DD) override;

	Matrix GetProj() { return projection_matrix; }
	Matrix GetView() { return view_matrix; }

	void Input(GameData* _GD);
	void MouseInput(GameData* _GD, int win_x, int win_y);
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void ZoomIn();
	void ZoomOut();
	void RecalculateProjViewPos();

private:
	float camera_speed = 0.7f;

	Vector3 camera_target;
	Vector3 camera_constraint = Vector3(10.0f, 10.0f, 0.0f);

	Vector3 vertical_movement = camera_speed * Vector3(0, 1, 0);
	Vector3 horizontal_movement = camera_speed / 4 * Vector3(0, 0, 1);

	float camera_width = 100.0f;
	float camera_height = 50.0f;
	float near_plane = 0.1f;
	float far_plane = 1000.0f;

	float zoom_value = 1.5f;
	float zoom_min = 0.2f;
	float zoom_max = 3.0f;

	float scroll_value = 0.0f;

	int _win_x;
	int _win_y;

protected:
	XMMATRIX projection_matrix;
	XMMATRIX view_matrix;
	XMVECTOR camera_position;
	Vector3 camera_up = Vector3::UnitY;
};


