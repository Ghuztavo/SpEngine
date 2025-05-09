#pragma once

namespace SpEngine::Graphics
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		void SetMode(ProjectionMode mode);

		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void SetLookAt(const Math::Vector3& target);

		//Perspective params
		void SetFOV(float fov);
		void SetAspectRatio(float ratio);

		//Orthographic params
		void SetSize(float width, float height);
		float GetSize() const;

		//shared internal
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		//constrols
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		//look controls
		void Yaw(float radiants);
		void Pitch(float radiants);

		void Zoom(float amount);

		//getters
		const Math::Vector3& GetPosition() const;
		const Math::Vector3& GetDirection() const;

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetProjectionMatrix() const;

		Math::Matrix4 GetPerspectiveMatrix() const;
		Math::Matrix4 GetOrthographicMatrix() const;

	private:
		ProjectionMode mProjectionMode = ProjectionMode::Perspective;

		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFOV = 60.0f * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;

		float mWidth = 0.0f;
		float mHeight = 0.0f;

		float mNearPlane = 0.1f;
		float mFarPlane = 1000.0f;
	};
}