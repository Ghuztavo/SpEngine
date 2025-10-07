#pragma once

#include "ConstantBuffer.h"
#include "Pixelshader.h"
#include "VertexShader.h"
#include "DirectionalLight.h"

namespace SpEngine::Graphics
{
	class Camera;
	class RenderObject;

	class StandardEffect final
	{
	public:
		void Initialize(const std::filesystem::path& path);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp; // world view projection
			Math::Matrix4 world; // world matrix
			Math::Vector3 viewPosition; // camera position
			float padding = 0.0f; // padding to maintain 16 byte alignment
		};

		using TransformBuffer = TypedconstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		using LightBuffer = TypedconstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
	};
}