/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Assign.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 31 de Agosto de 2018, 10:10
 */

#include "Assign.h"
#include <string>
#include "Model.h"
#include "Variable.h"
#include "Attribute.h"
#include "Resource.h"

Assign::Assign(Model* model) : ModelComponent(model, Util::TypeOf<Assign>()) {
}

Assign::Assign(const Assign& orig) : ModelComponent(orig) {
}

Assign::~Assign() {
}

std::string Assign::show() {
    return ModelComponent::show() +
            "";
}

List<Assign::Assignment*>* Assign::getAssignments() const {
    return _assignments;
}

void Assign::_execute(Entity* entity) {
    Assignment* let;
    std::list<Assignment*>* lets = this->_assignments->getList();
    for (std::list<Assignment*>::iterator it = lets->begin(); it != lets->end(); it++) {
        let = (*it);
        double value = _model->parseExpression(let->getExpression());
        _model->getTracer()->trace(Util::TraceLevel::blockInternal, "Let \"" + let->getDestination() + "\" = " + std::to_string(value) + "  // " + let->getExpression());
        /* TODO: this is NOT the best way to do it (enum comparision) */
        if (let->getDestinationType() == DestinationType::Variable) {
            Variable* myvar = (Variable*) this->_model->getElementManager()->getElement(Util::TypeOf<Variable>(), let->getDestination());
            myvar->setValue(value);
        } else if (let->getDestinationType() == DestinationType::Attribute) {
            entity->setAttributeValue(let->getDestination(), value);
        }
    }

    this->_model->sendEntityToComponent(entity, this->getNextComponents()->first(), 0.0);
}

void Assign::_loadInstance(std::list<std::string> words) {
}

std::list<std::string>* Assign::_saveInstance() {
    std::list<std::string>* words = ModelComponent::_saveInstance();//Util::TypeOf<Assign>());
    Assignment* let;
    for (std::list<Assignment*>::iterator it=_assignments->getList()->begin(); it!=_assignments->getList()->end();it++){
        let = (*it);
        words->insert(words->end(), std::to_string(static_cast<int>(let->getDestinationType())));
        words->insert(words->end(), let->getDestination());
        words->insert(words->end(), let->getExpression());
    }
    return words;
}

bool Assign::_check(std::string* errorMessage) {
    Assignment* let;
    bool resultAll = true;
    for (std::list<Assignment*>::iterator it = _assignments->getList()->begin(); it != _assignments->getList()->end(); it++) {
        let = (*it);
        resultAll &= _model->checkExpression(let->getExpression(), "assignment", errorMessage);
        if (let->getDestinationType() == DestinationType::Attribute) { // TODO I dont like the way we check the destination
            resultAll &= _model->getElementManager()->checkElement(Util::TypeOf<Attribute>(), let->getDestination(), "destination", true, errorMessage);
        } else if (let->getDestinationType() == DestinationType::Variable) {
            resultAll &= _model->getElementManager()->checkElement(Util::TypeOf<Variable>(), let->getDestination(), "destination", true, errorMessage);
        }
    }
    return resultAll;
}