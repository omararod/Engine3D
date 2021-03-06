#include "Engine3D.h"

Camera::Camera()  : speed(0.07),rotationSpeed(0.05),pitch(0),yaw(0),fieldOfView(0.78539),aspectRatio(1.333)
{
	
	up = DirectX::XMVectorSet(0, 1, 0, 0);
	right = DirectX::XMVectorSet(1, 0, 0, 0);
	forward = DirectX::XMVectorSet(0, 0, 1, 0);
	cameraPosition = DirectX::XMVectorSet(0, 0, -1, 0);
	originalLookAtPoint = DirectX::XMVectorSet(0, 0, 1, 0);
	lookAtPoint = originalLookAtPoint;
}

void Camera::Update()
{
	//Create a rotation matrix for the given pitch and yaw
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
    forward = DirectX::XMVector4Transform(originalLookAtPoint, rotationMatrix);
    lookAtPoint = DirectX::XMVectorAdd(forward, cameraPosition);

	//we rotate the original up vector
	up = DirectX::XMVector4Normalize(DirectX::XMVector4Transform(DirectX::XMVectorSet(0, 1, 0, 0), rotationMatrix));
    //forward = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(lookAtPoint, cameraPosition));
    right = DirectX::XMVector3Cross(up, forward);

	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.00001, 1000.0); //TODO: remove his magic numbers
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, lookAtPoint, DirectX::XMVectorSet(0, 1, 0, 0));

	transformationsMatrix =  viewMatrix * perspectiveMatrix;
    
}

void Camera::Move(MOVE_DIRECTION direction)
{
	DirectX::XMVECTOR directionVector;

	switch (direction)
	{
	case MOVE_FRONT:

		directionVector = forward;
		break;
	case MOVE_BACK:
		directionVector = DirectX::XMVectorScale(forward,-1);
		break;
	case MOVE_LEFT:
		directionVector = DirectX::XMVectorScale(right, -1);
		break;
	case MOVE_RIGHT:
		directionVector = right;
		break;
	default:
		return;
	}
	cameraPosition = DirectX::XMVectorAdd(cameraPosition, DirectX::XMVectorScale(directionVector,speed));
	lookAtPoint = DirectX::XMVectorAdd(lookAtPoint, DirectX::XMVectorScale(directionVector, speed));
}