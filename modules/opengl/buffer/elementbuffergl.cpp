/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel J�nsson
 *
 **********************************************************************/

#include <modules/opengl/buffer/elementbuffergl.h>

namespace inviwo {


ElementBufferGL::ElementBufferGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage)
    : BufferRepresentation(size, format, type, usage), glFormat_(getGLFormats()->getGLFormat(format->getId())){
    initialize();
}

ElementBufferGL::~ElementBufferGL() {
    deinitialize();
}

const Buffer* ElementBufferGL::getAttribute() const {
    return attrib_;
}

GLenum ElementBufferGL::getFormatType() const {
    return glFormat_.type;
}

GLuint ElementBufferGL::getId() const {
    return id_;
}

void ElementBufferGL::enable() const {
    bind();
}

void ElementBufferGL::disable() const {

}

void ElementBufferGL::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void ElementBufferGL::specifyLocation() const {
    (this->*locationPointerFunc_)();
}

void ElementBufferGL::upload(const void* data, size_t size)
{
    //Get GL Format
    glFormat_ = getGLFormats()->getGLFormat(getDataFormat()->getId());

    //Specify location and state
    switch(getBufferType())
    {
    case COLOR_ATTRIB:
        locationPointerFunc_ = &ElementBufferGL::colorPointer;
        break;
    case NORMAL_ATTRIB:
        locationPointerFunc_ = &ElementBufferGL::normalPointer;
        break;
    case TEXCOORD_ATTRIB:
        locationPointerFunc_ = &ElementBufferGL::texCoordPointer;
        break;
    case POSITION_ATTRIB:
        locationPointerFunc_ = &ElementBufferGL::vertexPointer;
        break;
    default:
        locationPointerFunc_ = &ElementBufferGL::emptyFunc;
        break;
    }

    //Specify usage
    switch(getBufferUsage())
    {
    case DYNAMIC:
        usageGL_ = GL_DYNAMIC_DRAW;
        break;
    default:
        usageGL_ = GL_STATIC_DRAW;
        break;
    }
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usageGL_);
    specifyLocation();
}

void ElementBufferGL::reupload(const void* data, size_t size){
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}


void ElementBufferGL::colorPointer() const {
    glColorPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void ElementBufferGL::normalPointer() const {
    glNormalPointer(glFormat_.type, 0, 0);
}

void ElementBufferGL::texCoordPointer() const {
    glTexCoordPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void ElementBufferGL::vertexPointer() const {
    glVertexPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void ElementBufferGL::emptyFunc() const {}

ElementBufferGL* ElementBufferGL::clone() const{
    return new ElementBufferGL(*this);
}

void ElementBufferGL::initialize()
{
    //Generate a new buffer
    glGenBuffers(1, &id_);
}

void ElementBufferGL::deinitialize()
{
    glDeleteBuffers(1, &id_);
}


BufferRAM2ElementGLConverter::BufferRAM2ElementGLConverter()
: RepresentationConverterType<ElementBufferGL>()
{}

BufferRAM2ElementGLConverter::~BufferRAM2ElementGLConverter() {}

DataRepresentation* BufferRAM2ElementGLConverter::createFrom(const DataRepresentation* source) {     
    const BufferRAM* bufferRAM = static_cast<const BufferRAM*>(source);
    ElementBufferGL* bufferGL = new ElementBufferGL(bufferRAM->getSize(), bufferRAM->getDataFormat(), bufferRAM->getBufferType(), bufferRAM->getBufferUsage());
    bufferGL->upload(bufferRAM->getData(), bufferRAM->getSize()*bufferRAM->getSizeOfElement());

    return bufferGL;
}
void BufferRAM2ElementGLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferRAM* src = static_cast<const BufferRAM*>(source);
    ElementBufferGL* dst = static_cast<ElementBufferGL*>(destination);
    dst->reupload(src->getData(), src->getSize()*src->getSizeOfElement());

}

} // namespace

