/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationReporterDefaultImpl1.h
 * Author: rafael.luiz.cancian
 *
 * Created on 8 de Maio de 2019, 10:59
 */

#ifndef SIMULATIONREPORTERDEFAULTIMPL1_H
#define SIMULATIONREPORTERDEFAULTIMPL1_H

#include "SimulationReporter_if.h"
#include "ModelSimulation.h"
#include "Model.h"

class SimulationReporterDefaultImpl1 : public SimulationReporter_if {
public:
    SimulationReporterDefaultImpl1(ModelSimulation* simulation, Model* model);
    SimulationReporterDefaultImpl1(const SimulationReporterDefaultImpl1& orig);
    virtual ~SimulationReporterDefaultImpl1();
public:
    virtual void showReplicationStatistics();
    virtual void showSimulationStatistics(List<StatisticsCollector*>* cstatsSimulation);
private:
    List<StatisticsCollector*>* getStatisticsFromParent(std::string parent);
private:
    ModelSimulation* _simulation;
    Model* _model;
};

#endif /* SIMULATIONREPORTERDEFAULTIMPL1_H */
