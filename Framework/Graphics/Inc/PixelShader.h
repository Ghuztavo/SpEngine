#pragma once

namespace SpEngine::Graphics
{
	class PixelShader final
	{
	public:
		void Initialize(const std::filesystem::path& shaderFilePath);
		void Terminate();
		void Bind();

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
		ID3DBlob* mShaderBlob = nullptr;
		ID3DBlob* mErrorBlob = nullptr;
	};
}