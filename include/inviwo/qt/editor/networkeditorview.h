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

#ifndef IVW_NETWORKEDITORVIEW_H
#define IVW_NETWORKEDITORVIEW_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>

#include "networkeditor.h"

namespace inviwo {

class IVW_QTEDITOR_API NetworkEditorView : public QGraphicsView {

public:
    NetworkEditorView(QWidget* parent=0);
    ~NetworkEditorView();

    void setNetworkEditor(NetworkEditor* networkEditor);
    NetworkEditor* getNetworkEditor() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* re);
    void wheelEvent(QWheelEvent* e);

private:
    NetworkEditor* networkEditor_;

    int zoomLevel_;
    float zoomValue_;

    void setZoomLevel(int zoomLevel);
    float calculateScaleFor(int zoomLevel) const;
    int calculateZoomLevelFor(float scale) const;
};

} // namespace

#endif // IVW_NETWORKEDITORVIEW_H