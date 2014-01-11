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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

/** \TransferFunctionEditorControlPoint class
 *         A point in the TransferFunctionEditor
 *
 *         Overloaded QGraphicsItem used in TransferFunctionEditor. Position of control points are used to
 *         calulate the TransferFunction Image values, currently has one parameter
 */
#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPainterPath>

namespace inviwo {

class TransferFunctionDataPoint;

class IVW_QTWIDGETS_API TransferFunctionEditorControlPoint : public QGraphicsItem {

public:
    /** \TransferFunctionEditorControlPoint constructor  
    *      Creates a TransferFunctionEditorControlPoint
    */        
    TransferFunctionEditorControlPoint(TransferFunctionDataPoint* dataPoint);
    
    /** \TransferFunctionEditorControlPoint destructor  
    */
    ~TransferFunctionEditorControlPoint();

    TransferFunctionDataPoint* getPoint() const;
	TransferFunctionEditorControlPoint* getLeftNeighbour() const;
	TransferFunctionEditorControlPoint* getRightNeighbour() const;

	void setLeftNeighbour(TransferFunctionEditorControlPoint*);
	void setRightNeighbour(TransferFunctionEditorControlPoint*);

	void notify();

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + 278 };
    int type() const  {return Type; }

protected:
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    //void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    /**Paint method
    * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
    */
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);


private:
    float size_; ///< size for drawing the points
    float viewWidth_;
    float viewHeight_;

    TransferFunctionDataPoint* datapoint_; ///<The TransferFunctionDataPoint the control point gets all its data from
	TransferFunctionEditorControlPoint* leftNeighbour_;
	TransferFunctionEditorControlPoint* rightNeighbour_;
};

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H