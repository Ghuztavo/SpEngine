#pragma once

namespace SpEngine::Graphics
{
	class VertexShader final
	{
	public:
		template<class VertexType>
		void Initialize(const std::filesystem::path& shaderPath)
		{
			Initialize(shaderPath, VertexType::Format);
		}
		void Initialize(const std::filesystem::path& shaderPath, uint32_t format);
		void Terminate();
		void Bind() const;

	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};
}