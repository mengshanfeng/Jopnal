// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4189)
#pragma warning(disable: 4244)
#include <Jopnal/Graphics/stb/stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace
{
    void flip(const unsigned int width, const unsigned int height, const unsigned int bpp, unsigned char* pixels)
    {
        unsigned int rowSize = width * bpp;

        for (std::size_t y = 0; y < height; ++y)
        {
            unsigned char* left = pixels + y * rowSize;
            unsigned char* right = pixels + (y + 1) * rowSize - bpp;

            for (std::size_t x = 0; x < width / 2; ++x)
            {
                std::swap_ranges(left, left + bpp, right);

                left += bpp;
                right -= bpp;
            }
        }
    }
}

namespace jop
{
    Texture2D::Texture2D(const std::string& name)
        : Texture(name, gl::TEXTURE_2D)
    {}

    //////////////////////////////////////////////

    bool Texture2D::load(const std::string& path)
    {
        if (path.empty())
            return false;

        std::vector<unsigned char> buf;
        FileLoader::read(path, buf);

        int x = 0, y = 0, bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = false;
        if (colorData && checkDepthValid(bpp))
        {
            flip(x, y, bpp, colorData);
            success = load(x, y, bpp, colorData);
        }

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel)
    {
        return load(width, height, bytesPerPixel, nullptr);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel, const unsigned char* pixels)
    {
        if (width > getMaximumSize() || height > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }
        else if (!checkDepthValid(bytesPerPixel))
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Pixel depth (" << bytesPerPixel << ") is invalid. Must be between 1 and 4");
            return false;
        }

        destroy();
        bind();

        m_width = width;
        m_height = height;
        m_bytesPerPixel = bytesPerPixel;

        const GLenum depthEnum = getFormatEnum(bytesPerPixel);
        glCheck(gl::TexImage2D(gl::TEXTURE_2D, 0, getInternalFormatEnum(depthEnum), width, height, 0, depthEnum, gl::UNSIGNED_BYTE, pixels));

        return true;
    }

    //////////////////////////////////////////////

    void Texture2D::setPixels(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const unsigned char* pixels)
    {
        if ((x + width > m_width) || (y + height > m_height))
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Would cause overflow");
            return;
        }
        else if (!pixels)
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Pixel pointer is null");
            return;
        }

        bind();
        glCheck(gl::TexSubImage2D(gl::TEXTURE_2D, 0, x, y, width, height, getFormatEnum(m_bytesPerPixel), gl::UNSIGNED_BYTE, pixels));
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getWidth() const
    {
        return m_width;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getHeight() const
    {
        return m_height;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const int id)
    {
        std::vector<unsigned char> buf;
        if (!FileLoader::readFromDll(id, buf))
            return false;

        int x, y, bpp;
        unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = false;
        if (pix && checkDepthValid(bpp))
        {
            flip(x, y, bpp, pix);
            success = load(x, y, bpp, pix);
        }

        stbi_image_free(pix);

        return success;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getFormatEnum(const unsigned int bytesPerPixel)
    {
        switch (bytesPerPixel)
        {
            case 1:
                return gl::RED;
            case 2:
                return gl::RG;
            case 3:
                return gl::RGB;
            case 4:
            default:
                return gl::RGBA;
        }
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getInternalFormatEnum(const unsigned int formatEnum)
    {
        switch (formatEnum)
        {
            case gl::RED:
                return gl::R8;
            case gl::RG:
                return gl::RG8;
            case gl::RGB:
                return gl::RGB8;
            case gl::RGBA:
            default:
                return gl::RGBA8;
        }
    }

    //////////////////////////////////////////////

    bool Texture2D::checkDepthValid(const unsigned int depth)
    {
        return depth >= 1 && depth <= 4;
    }
}