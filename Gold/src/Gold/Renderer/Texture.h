#pragma once
#include <string>

#include "Gold/Core/Base.h"

namespace Gold {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	
		virtual void bindFrameBuffer() = 0;

		virtual void bindFrameBuffer(uint32_t frameBuffer, uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetPixel(int x, int y) const = 0;


		virtual void cleanUp() = 0;

		virtual void createFrameBuffer() = 0;

		virtual void initialiseFrameBuffer() =0 ;


		virtual void unbindFramebuffer() = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, bool DepthTexture);
		static Ref<Texture2D> Create(const std::string& path, bool ImageFlag, bool Flip);
		static Ref<Texture2D> Create(const std::vector<std::string> faces);
	};
}