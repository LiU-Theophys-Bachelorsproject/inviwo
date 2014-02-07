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
 * Main file authors: Timo Ropinski, Alexander Johansson, Erik Sund�n
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QSettings>


namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent), VoidObservable() {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertyListWidget_ = this;
    scrollArea_ = new QScrollArea(propertyListWidget_);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(300);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    listWidget_ = new QWidget();
    listWidgetLayout_ = new QVBoxLayout(listWidget_);
    listWidgetLayout_->setAlignment(Qt::AlignTop);
    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    developerViewMode_ = settings.value("developerViewMode",true).toBool();
    applicationViewMode_ = settings.value("applicationViewMode",false).toBool();
    settings.endGroup();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::addProcessorProperties(Processor* processor) {
    QWidget* processorPropertyWidget = getProcessorPropertiesItem(processor);

    if (processorPropertyWidget) {
        listWidgetLayout_->addWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(true);
        static_cast<CollapsibleGroupBoxWidgetQt*>(processorPropertyWidget)->show();
    }
}

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());

    if (it != propertyWidgetMap_.end()) {
        it->second->setVisible(false);
        listWidgetLayout_->removeWidget(it->second);
    }
}

void PropertyListWidget::removeAndDeleteProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());

    if (it != propertyWidgetMap_.end()) {
        it->second->setVisible(false);
        listWidgetLayout_->removeWidget(it->second);
        CollapsibleGroupBoxWidgetQt* collapsiveGropWidget = static_cast<CollapsibleGroupBoxWidgetQt*>(it->second);
        std::vector<PropertyWidgetQt*> propertyWidgets = collapsiveGropWidget->getPropertyWidgets();
        std::vector<Property*> properties = processor->getProperties();
        for (size_t i=0; i<propertyWidgets.size(); i++) {
            for (size_t j=0; j<properties.size(); j++)
                properties[j]->deregisterWidget(propertyWidgets[i]);
        }
        for (size_t i=0; i<propertyWidgets.size(); i++) {
            collapsiveGropWidget->removeWidget(propertyWidgets[i]);
            propertyWidgets[i]->hide();
            //TODO: Do not use deleteLater(). Widgets need to be deleted instantly or use deinitialize()
            //Reason: These are cached widgets which has children widgets.
            //deleteLater() keeps the children active for a while that causes invalidation of properties.
            //Hence deleteLater() cannot be used
            delete propertyWidgets[i];
        }
        it->second->deleteLater();
        propertyWidgetMap_.erase(it);
    }
}

void PropertyListWidget::changeName(std::string oldName, std::string newName) {
    // check if processor widget exists
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(oldName);

    if (it != propertyWidgetMap_.end()) {
        CollapsibleGroupBoxWidgetQt* processorPropertyWidget = dynamic_cast<CollapsibleGroupBoxWidgetQt*>(it->second);
        processorPropertyWidget->setIdentifier(newName);

        propertyWidgetMap_.erase(it);
        propertyWidgetMap_[newName] = processorPropertyWidget;
    }
}

void PropertyListWidget::cacheProcessorPropertiesItem(Processor* processor) {
    getProcessorPropertiesItem(processor);
}

QWidget* PropertyListWidget::getProcessorPropertiesItem(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;

    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else
        processorPropertyWidget = createNewProcessorPropertiesItem(processor);

    return processorPropertyWidget;
}

QWidget* PropertyListWidget::createNewProcessorPropertiesItem(Processor* processor) {
    // create property widget and store it in the map
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = new CollapsibleGroupBoxWidgetQt(processor->getIdentifier(),
            processor->getIdentifier());
    processorPropertyWidget->setParent(this);
    properties_ = processor->getProperties();
    std::vector<Property*> addedProperties;

    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];

        // check if the property is already added
        if (std::find(addedProperties.begin(),addedProperties.end(),curProperty) != addedProperties.end())
            continue;
        // add to group box if one is assigned to the property
        else if (curProperty->getGroupID() != "") {
            CollapsibleGroupBoxWidgetQt* group = new CollapsibleGroupBoxWidgetQt(curProperty->getGroupID(), curProperty->getGroupDisplayName());
            group->setIdentifier(curProperty->getGroupDisplayName());

            // add all the properties with the same group assigned
            for (size_t k=0; k<properties_.size(); k++) {
                Property* tmpProperty = properties_[k];

                if (curProperty->getGroupID() == tmpProperty->getGroupID()) {
                    group->addProperty(tmpProperty);
                    addedProperties.push_back(tmpProperty);
                }
            }

            group->generatePropertyWidgets();
            processorPropertyWidget->addWidget(group);
        }
        else {
            PropertyWidgetQt* propertyWidget =
                static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getRef().create(curProperty));

            if (propertyWidget) {
                curProperty->registerWidget(propertyWidget);
                processorPropertyWidget->addWidget(propertyWidget);
                connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
                addedProperties.push_back(curProperty);
            }
        }
    }

    propertyWidgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));
    return processorPropertyWidget;
}

void PropertyListWidget::propertyModified() {
    notifyObservers();
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

void PropertyListWidget::setDeveloperViewMode(bool value) {
    if (value)
        setViewMode(DEVELOPMENT);
}

void PropertyListWidget::setApplicationViewMode(bool value) {
    if (value)
        setViewMode(APPLICATION);
}

void PropertyListWidget::setViewMode(PropertyVisibilityMode viewMode) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    SystemSettings* mainSettings = inviwoApp->getSettingsByType<SystemSettings>();
    dynamic_cast<OptionPropertyInt*>(mainSettings->getPropertyByIdentifier("viewMode"))->set(static_cast<int>(viewMode));
    applicationViewMode_ = (viewMode == APPLICATION);
    developerViewMode_ = (viewMode == DEVELOPMENT);

    for (size_t i = 0; i < properties_.size(); i++)
        properties_[i]->updateVisibility();
}

void PropertyListWidget::saveState() {
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    settings.setValue("developerViewMode", developerViewMode_);
    settings.setValue("applicationViewMode",applicationViewMode_);
    settings.endGroup();
}

PropertyVisibilityMode PropertyListWidget::getViewMode() {
    if (developerViewMode_)
        return DEVELOPMENT;
    else
        return APPLICATION;
}

void PropertyListWidget::notify() {}

} // namespace