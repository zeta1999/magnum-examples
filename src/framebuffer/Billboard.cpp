/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Billboard.h"

#include "Buffer.h"
#include <Primitives/Square.h>
#include "SceneGraph/Camera.h"

namespace Magnum { namespace Examples {

Billboard::Billboard(Trade::ImageData2D* image, Buffer* colorCorrectionBuffer, SceneGraph::Object2D* parent): Object2D(parent), mesh(Mesh::Primitive::TriangleStrip, 4) {
    Primitives::Square square;

    Buffer* buffer = mesh.addBuffer(Mesh::BufferType::NonInterleaved);
    buffer->setData(*square.positions(0), Buffer::Usage::StaticDraw);
    mesh.bindAttribute<ColorCorrectionShader::Position>(buffer);

    texture.setWrapping({AbstractTexture::Wrapping::ClampToBorder, AbstractTexture::Wrapping::ClampToBorder});
    texture.setMagnificationFilter(AbstractTexture::Filter::LinearInterpolation);
    texture.setMinificationFilter(AbstractTexture::Filter::LinearInterpolation);
    texture.setData(0, AbstractTexture::Format::RGBA, image);

    colorCorrectionTexture.setBuffer(BufferedTexture::Components::Red|BufferedTexture::ComponentType::Float, colorCorrectionBuffer);

    scale(Vector2::yScale(GLfloat(image->size()[1])/image->size()[0]));
}

void Billboard::draw(const Matrix3& transformationMatrix, SceneGraph::Camera2D* camera) {
    shader.use();
    shader.setMatrixUniform(camera->projectionMatrix()*transformationMatrix);
    texture.bind(ColorCorrectionShader::TextureLayer);
    colorCorrectionTexture.bind(ColorCorrectionShader::ColorCorrectionTextureLayer);

    mesh.draw();
}

}}
