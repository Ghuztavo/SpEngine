#pragma once

namespace SpEngine::Graphics
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		virtual ~ConstantBuffer();

		void Initialize(uint32_t bufferSize);
		void Terminate();

		void Update(const void* data);
		
		void BindVs(uint32_t slot) const;
		void BindPs(uint32_t slot) const;


	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
	};
}