/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExperimentDesignDummyImpl.h
 * Author: rafael.luiz.cancian
 *
 * Created on 10 de Outubro de 2018, 18:32
 */

#ifndef EXPERIMENTDESIGNDUMMYIMPL_H
#define EXPERIMENTDESIGNDUMMYIMPL_H

#include "ExperimentDesign_if.h"

/*!
 *
 */
class ExperimentDesignDummyImpl : public ExperimentDesign_if {
public:
	ExperimentDesignDummyImpl();
	virtual ~ExperimentDesignDummyImpl() = default;
public:
	virtual ExperimentManager_if* getProcessAnalyser() const;
public:
	virtual bool generate2krScenarioExperiments();
	virtual bool calculateContributionAndCoefficients();
	virtual std::list<FactorOrInteractionContribution*>* getContributions() const;
private:
	ExperimentManager_if* _processAnalyser; //= new Traits<ExperimentDesign_if>::ProcessAnalyserImplementation();
	std::list<FactorOrInteractionContribution*>* _contributions = new std::list<FactorOrInteractionContribution*>();
};
#endif /* EXPERIMENTDESIGNDUMMYIMPL_H */
