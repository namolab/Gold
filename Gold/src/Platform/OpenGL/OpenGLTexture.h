#pragma once

#include "Gold/Renderer/Texture.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace Gold {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, bool DepthTexture);
		OpenGLTexture2D(const std::string& path, bool ImageFlag, bool Flip);
		OpenGLTexture2D(const std::vector<std::string> faces);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void bindFrameBuffer() override;
		
		virtual void bindFrameBuffer(uint32_t frameBuffer, uint32_t width, uint32_t height) override;

		virtual uint32_t GetPixel(int x, int y) const override;


		virtual void cleanUp() override;

		virtual void createFrameBuffer() override;

		virtual void initialiseFrameBuffer() override;


		virtual void unbindFramebuffer() override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}


	private:
		std::string m_Path;
		std::vector<std::string> m_Faces;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_FramebufferID;

		GLenum m_InternalFormat, m_DataFormat;
		int width, height;
		std::vector<unsigned char> images;
	};

}