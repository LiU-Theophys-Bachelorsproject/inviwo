#include "pymodule.h"

#include "../pyinviwo.h"

namespace inviwo{

    std::map<PyObject*,PyModule*> PyModule::instances_;

PyModule::PyModule(std::string moduleName,std::vector<PyMethod*> methods):moduleName_(moduleName){
    moduleObject_ = PyInviwo::getPtr()->registerPyModule(this);
    if(!moduleObject_){
        LogWarn("Module could not be initzialied ");
        return;
    }

    instances_[moduleObject_] = this;

    name_ = PyString_FromString(moduleName_.c_str());
    dict_ = PyModule_GetDict(moduleObject_);

    std::vector<PyMethod*>::iterator it;
    for(it = methods.begin();it!=methods.end();++it)
        addMethod(*it);
    addMethod(new PyInfoMethod());
}

void PyModule::addMethod(PyMethod* method){
    methods_.push_back(method);
    PyObject* pyFunc = PyCFunction_NewEx(method->getDef(),moduleObject_,name_);

    int a = PyDict_SetItemString(dict_, method->getName(), pyFunc);

    Py_DECREF(pyFunc);
}

const char* PyModule::getModuleName(){return moduleName_.c_str();}

void PyModule::printInfo(){
    for(int i = 0;i<methods_.size();i++){
        std::string msg = "print \"";
        msg += methods_[i]->getDesc();
        msg += "\" ";
        PyRun_SimpleString(msg.c_str());
        PyRun_SimpleString("print \" \"");
    }
}



PyModule* PyModule::getModuleByPyObject(PyObject *obj){
    return instances_[obj];
}



}//namespace




PyObject* py_info(PyObject* self, PyObject* args){
    inviwo::PyModule::getModuleByPyObject(self)->printInfo();
    Py_RETURN_NONE;
}