// Copyright (C) 2008-2011 by Philipp Muenzel. All rights reserved
// Released under the terms of the GNU General Public License version 2 or later
// as published by the Free Software Foundation, Inc.

#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H

#include <vector>
#include <string>

namespace PPL {

class TextureHolder
{
public:
    TextureHolder();
    int loadTexture(const std::string& filename);
private:
    std::vector<int> m_textures;
};

}

#endif // TEXTUREHOLDER_H
