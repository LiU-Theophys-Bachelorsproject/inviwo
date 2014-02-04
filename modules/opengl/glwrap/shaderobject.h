 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_SHADEROBJECT_H
#define IVW_SHADEROBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderObject {

public:
    ShaderObject(GLenum shaderType, std::string fileName, bool compileShader=true);
    ~ShaderObject();

    GLuint getID() { return id_; }
    std::string getFileName() { return fileName_; }
    std::string getAbsoluteFileName() { return absoluteFileName_; }
    std::vector<std::string> getIncludeFileNames() { return includeFileNames_; }
    GLenum getShaderType() { return shaderType_; }

    bool loadSource(std::string fileName);
    void preprocess();
    void upload();
    bool compile();
    std::string getShaderInfoLog();

    bool build();
    bool rebuild();

    void addShaderDefine(std::string name, std::string value="");
    void removeShaderDefine(std::string name);
    void clearShaderDefines();

    void addOutDeclaration(std::string name);

private:

    bool initialize(bool compileShader);
    void deinitialize();

    std::string embeddDefines(std::string source);
    std::string embeddOutDeclarations(std::string source);
    std::string embeddIncludes(std::string source, std::string fileName);

    int getLogLineNumber(const std::string& compileLogLine);
    std::string reformatShaderInfoLog(const std::string compileLog);

    GLenum shaderType_;
    std::string fileName_;
    std::string absoluteFileName_;
    std::vector<std::string> includeFileNames_;

    GLuint id_;
    std::string source_;
    std::string sourceProcessed_;

    std::vector<std::string> outDeclarations_;
    std::vector<std::pair<std::string, std::string> > shaderDefines_;

    std::vector<std::pair<std::string, unsigned int> > lineNumberResolver_;
};

} // namespace

#endif // IVW_SHADEROBJECT_H
