#include "hzpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
//#include <SOIL2/SOIL2.h>

namespace Gold {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, bool DepthTexture)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height, DepthTexture);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, bool ImageFlag, bool Flip)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path, ImageFlag, Flip);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::vector<std::string> faces)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(faces);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
};
