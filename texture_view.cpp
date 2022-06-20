//
// Created by jacob on 20/6/22.
//

#include "texture_view.hpp"
#include "image_views.hpp"


void TextureView::setup() {
    createImageView(device, texture.get_image(), VK_FORMAT_R8G8B8A8_SRGB, textureImageView);
}

