/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Seize.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 21 de Agosto de 2018, 16:17
 */

#include "Seize.h"
#include "Resource.h"
#include "Attribute.h"

Seize::Seize(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Seize>(), name) {
}

std::string Seize::show() {
	return ModelComponent::show() +
			",resourceType=" + std::to_string(static_cast<int> (this->_resourceType)) +
			",resource=\"" + this->_resource->name() + "\"" +
			",quantity=" + this->_quantityExpression;
}

void Seize::setLastMemberSeized(unsigned int _lastMemberSeized) {
	this->_lastMemberSeized = _lastMemberSeized;
}

unsigned int Seize::getLastMemberSeized() const {
	return _lastMemberSeized;
}

void Seize::setSaveAttribute(std::string _saveAttribute) {
	this->_saveAttribute = _saveAttribute;
}

std::string Seize::getSaveAttribute() const {
	return _saveAttribute;
}

void Seize::setRule(Resource::ResourceRule _rule) {
	this->_rule = _rule;
}

Resource::ResourceRule Seize::getRule() const {
	return _rule;
}

void Seize::setQuantity(std::string _quantity) {
	this->_quantityExpression = _quantity;
}

std::string Seize::getQuantity() const {
	return _quantityExpression;
}

void Seize::setResourceType(Resource::ResourceType _resourceType) {
	this->_resourceType = _resourceType;
}

Resource::ResourceType Seize::getResourceType() const {
	return _resourceType;
}

void Seize::setPriority(unsigned short _priority) {
	this->_priority = _priority;
}

unsigned short Seize::getPriority() const {
	return _priority;
}

void Seize::setAllocationType(unsigned int _allocationType) {
	this->_allocationType = _allocationType;
}

unsigned int Seize::getAllocationType() const {
	return _allocationType;
}

void Seize::setQueueName(std::string queueName) throw () {
	Queue* queue = dynamic_cast<Queue*> (_parentModel->elements()->element(Util::TypeOf<Queue>(), queueName));
	if (queue != nullptr) {
		_queue = queue;
	} else {
		throw std::invalid_argument("Queue does not exist");
	}
}

void Seize::_handlerForResourceEvent(Resource* resource) {
	Waiting* first = _queue->first();
	if (first != nullptr) { // there are entities waiting in the queue
		unsigned int quantity = _parentModel->parseExpression(this->_quantityExpression);
		if ((resource->getCapacity() - resource->getNumberBusy()) >= quantity) { //enought quantity to seize
			double tnow = _parentModel->simulation()->simulatedTime();
			resource->seize(quantity, tnow);
			_parentModel->futureEvents()->insert(new Event(tnow, first->getEntity(), this->nextComponents()->frontConnection()));
			_queue->removeElement(first);
			_parentModel->tracer()->traceSimulation(tnow, first->getEntity(), this, "Waiting entity " + std::to_string(first->getEntity()->entityNumber()) + " now seizes " + std::to_string(quantity) + " elements of resource \"" + resource->name() + "\"");

		}
	}
}

void Seize::setResourceName(std::string resourceName) throw () {
	Resource* resource = dynamic_cast<Resource*> (_parentModel->elements()->element(Util::TypeOf<Resource>(), resourceName));
	if (resource != nullptr) {
		_resource = resource;
	} else {
		throw std::invalid_argument("Resource does not exist");
	}
}

std::string Seize::getResourceName() const {
	return _resource->name();
}

std::string Seize::getQueueName() const {
	return _queue->name();
}

void Seize::setResource(Resource* resource) {
	this->_resource = resource;
	_resource->addResourceEventHandler(Resource::SetResourceEventHandler<Seize>(&Seize::_handlerForResourceEvent, this));
}

Resource* Seize::getResource() const {
	return _resource;
}

void Seize::setQueue(Queue* queue) {
	this->_queue = queue;
}

Queue* Seize::getQueue() const {
	return _queue;
}

void Seize::_execute(Entity* entity) {
	/*  \todo: +: not implemented yet */
	Resource* resource = nullptr;
	if (this->_resourceType == Resource::ResourceType::SET) {
		/*  \todo: +: not implemented yet */
	} else {
		resource = this->_resource;
	}
	unsigned int quantity = _parentModel->parseExpression(this->_quantityExpression);
	if (resource->getCapacity() - resource->getNumberBusy() < quantity) { // not enought free quantity to allocate. Entity goes to the queue
		WaitingResource* waitingRec = new WaitingResource(entity, this, _parentModel->simulation()->simulatedTime(), quantity);
		this->_queue->insertElement(waitingRec); // ->list()->insert(waitingRec);
		_parentModel->tracer()->traceSimulation(_parentModel->simulation()->simulatedTime(), entity, this, "Entity starts to wait for resource in queue \"" + _queue->name() + "\" with " + std::to_string(_queue->size()) + " elements");

	} else { // alocate the resource
		_parentModel->tracer()->traceSimulation(_parentModel->simulation()->simulatedTime(), entity, this, "Entity seizes " + std::to_string(quantity) + " elements of resource \"" + resource->name() + "\" (capacity:" + std::to_string(resource->getCapacity()) + ", numberbusy:" + std::to_string(resource->getNumberBusy()) + ")");
		resource->seize(quantity, _parentModel->simulation()->simulatedTime());
		_parentModel->sendEntityToComponent(entity, this->nextComponents()->frontConnection(), 0.0);
	}
}

void Seize::_initBetweenReplications() {
	this->_lastMemberSeized = 0;
	this->_queue->initBetweenReplications();
	this->_resource->initBetweenReplications();
}

bool Seize::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		this->_allocationType = std::stoi((*(fields->find("allocationType"))).second);
		this->_priority = std::stoi((*(fields->find("priority"))).second);
		this->_quantityExpression = ((*(fields->find("quantity"))).second);
		this->_resourceType = static_cast<Resource::ResourceType> (std::stoi((*(fields->find("resourceType"))).second));
		this->_rule = static_cast<Resource::ResourceRule> (std::stoi((*(fields->find("rule"))).second));
		this->_saveAttribute = ((*(fields->find("saveAttribute"))).second);
		//Util::identitifcation queueId = std::stoi((*(fields->find("queueId"))).second);
		//Queue* queue = dynamic_cast<Queue*> (_model->elements()->element(Util::TypeOf<Queue>(), queueId));
		std::string queueName = ((*(fields->find("queueName"))).second);
		Queue* queue = dynamic_cast<Queue*> (_parentModel->elements()->element(Util::TypeOf<Queue>(), queueName));
		this->_queue = queue;
		//Util::identitifcation resourceId = std::stoi((*(fields->find("resourceId"))).second);
		//Resource* resource = dynamic_cast<Resource*> (_model->elements()->element(Util::TypeOf<Resource>(), resourceId));
		std::string resourceName = ((*(fields->find("resourceName"))).second);
		Resource* resource = dynamic_cast<Resource*> (_parentModel->elements()->element(Util::TypeOf<Resource>(), resourceName));
		this->_resource = resource;
		_resource->addResourceEventHandler(Resource::SetResourceEventHandler<Seize>(&Seize::_handlerForResourceEvent, this));

	}
	return res;
}

std::map<std::string, std::string>* Seize::_saveInstance() {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(); //Util::TypeOf<Seize>());
	fields->emplace("allocationType", std::to_string(this->_allocationType));
	fields->emplace("priority=", std::to_string(this->_priority));
	fields->emplace("quantity", "\"" + this->_quantityExpression + "\"");
	fields->emplace("queueId", std::to_string(this->_queue->id()));
	fields->emplace("queueName", (this->_queue->name()));
	fields->emplace("resourceType", std::to_string(static_cast<int> (this->_resourceType)));
	fields->emplace("resourceId", std::to_string(this->_resource->id()));
	fields->emplace("resourceName", (this->_resource->name()));
	fields->emplace("rule", std::to_string(static_cast<int> (this->_rule)));
	fields->emplace("saveAttribute", this->_saveAttribute);
	return fields;
}

bool Seize::_check(std::string* errorMessage) {
	bool resultAll = true;
	resultAll &= _parentModel->checkExpression(_quantityExpression, "quantity", errorMessage);
	resultAll &= _parentModel->elements()->check(Util::TypeOf<Resource>(), _resource, "Resource", errorMessage);
	resultAll &= _parentModel->elements()->check(Util::TypeOf<Queue>(), _queue, "Queue", errorMessage);
	resultAll &= _parentModel->elements()->check(Util::TypeOf<Attribute>(), _saveAttribute, "SaveAttribute", false, errorMessage);
	return resultAll;
}

PluginInformation* Seize::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Seize>(), &Seize::LoadInstance);
	info->insertDynamicLibFileDependence("queue.so");
	info->insertDynamicLibFileDependence("resource.so");
	return info;
}

ModelComponent* Seize::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	Seize* newComponent = new Seize(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;

}

