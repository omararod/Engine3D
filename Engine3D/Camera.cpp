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
	//Get the direction of the look-at point by rotating the original point
	DirectX::XMVECTOR lookAtDirection = DirectX::XMVector4Normalize( DirectX::XMVector4Transform(originalLookAtPoint, rotationMatrix));
	//Get the length of the current look-at point vector
	float magnitude = DirectX::XMVectorGetX(DirectX::XMVector4Length(lookAtPoint));
	//Update the look-at point with the look-at direction vector scaled by the length of the previous look-at point vector
	lookAtPoint = DirectX::XMVectorScale(lookAtDirection, magnitude );
	//The forward vector is a unit vector going from the camera to the look-at point
	forward = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(lookAtPoint,cameraPosition));
	//we rotate the original up vector
	up = DirectX::XMVector4Normalize(DirectX::XMVector4Transform(DirectX::XMVectorSet(0, 1, 0, 0), rotationMatrix));
	//is handy to have a right vector, the cross product of the up and forward vectors
	right = DirectX::XMVector3Cross(up, forward);

	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.00001, 1000.0); //TODO: remove his magic numbers
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, lookAtPoint, up);

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