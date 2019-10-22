/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ToolManager.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 31 de Maio de 2019, 09:04
 */

#include "ToolManager.h"
#include "Traits.h"


ToolManager::ToolManager(Simulator* simulator) {
    _simulator = simulator;
    //
    _fitter = new Traits<Fitter_if>::Implementation();
    _sampler = new Traits<Sampler_if>::Implementation();
    _processAnalyser = new Traits<ProcessAnalyser_if>::Implementation();
}

ToolManager::ToolManager(const ToolManager& orig) {
}
// default destructor


Sampler_if* ToolManager::getSampler() const {
    return _sampler;
}

Fitter_if* ToolManager::getFitter() const {
    return _fitter;
}

ProcessAnalyser_if* ToolManager::getExperimentDesigner() const {
    return _processAnalyser;
}
