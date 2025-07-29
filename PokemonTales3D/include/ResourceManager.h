#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

template<typename Derived, typename T>
class ResourceManager {
public:

	ResourceManager(const std::string& l_argumentsFile){
		LoadArguments(l_argumentsFile);
	}
	virtual ~ResourceManager() { 
		PurgeResources(); 
	}

	/*Load arguments from a file*/
	void LoadArguments(const std::string& l_argumentsFile) {
		std::ifstream paths;
		paths.open(l_argumentsFile);
		if (paths.is_open()) {
			std::string line;
			while (std::getline(paths, line)) {
				std::stringstream keystream(line);
				std::string name;
				std::string argument;
				keystream >> name;
				auto itr = m_arguments.emplace(name, std::vector<std::string>());
				while (keystream >> argument) {
					itr.first->second.push_back(argument);
				}
			}
			paths.close();
			return;
		}
		std::cout << "Failed loading the arguments file " << l_argumentsFile << std::endl;
	}

	/*Purge all the resources in the manager*/
	void PurgeResources() {
		while (m_resources.begin() != m_resources.end()) {
			std::cout << "Purge : " << m_resources.begin()->first << std::endl;
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	/* Get the reference of a resource */
	T* GetResource(const std::string& l_id) {
		auto res = Find(l_id);
		return (res ? res->first : nullptr);
	}

	/* Get the arguments for the loading of a resource */
	const std::vector<std::string>* GetArguments(const std::string& l_id) {
		auto args = m_arguments.find(l_id);
		return (args == m_arguments.end() ? &args->second : nullptr);
	}

	/* Require a resource, must be paired with a call to ReleaseResource method */
	bool RequireResource(const std::string& l_id) {
		auto res = Find(l_id);
		if (res) {
			res->second++;
			return true;
		}
		auto args = m_arguments.find(l_id);
		if (args == m_arguments.end())
			return false;
		std::cout << "Load : " << l_id << std::endl;
		T* resource = Load(&args->second);
		if (!resource) { return false; }
		m_resources.emplace(l_id, std::make_pair(resource, 1));
		return true;
	}

	/* Require a resource and return the reference to this resource */
	T* RequireGetResource(const std::string& l_id) {
		if (!RequireResource(l_id))
			return nullptr;
		return GetResource(l_id);
	}

	/* Release a resource */
	bool ReleaseResource(const std::string& l_id) {
		auto res = Find(l_id);
		if (!res) { return false; }
		--res->second;
		if (!res->second) { Unload(l_id); }
		return true;
	}

	/* Load the data of a resource */
	T* Load(const std::vector<std::string>* l_args) {
		return static_cast<Derived*>(this)->Load(l_args);
	}

protected:
	std::pair<T*, unsigned int>* Find(const std::string& l_id) {
		auto itr = m_resources.find(l_id);
		return (itr != m_resources.end()) ? &itr->second : nullptr;
	}

	bool Unload(const std::string& l_id) {
		auto itr = m_resources.find(l_id);
		if (itr == m_resources.end())
			return false;
		std::cout << "Unload : " << l_id << std::endl;
		delete itr->second.first;
		m_resources.erase(itr);
		return true;
	}

	std::unordered_map<std::string, std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::vector<std::string>> m_arguments;
};