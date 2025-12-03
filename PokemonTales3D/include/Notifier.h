#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

class Notifier {
public:
	Notifier(){}
	~Notifier(){}

	//Subscribe the inst on the tag with the given callback (WARNING : call to unsubscribe is needed when the inst is destroyed)
	template<class T>
	void Subscribe(std::string tag, void(T::* call)(void), T* inst) {
		auto itr = callbacks.find(tag);
		if (itr == callbacks.end())
			itr = callbacks.insert(std::make_pair(tag, callback_container())).first;
		itr->second.push_back(std::make_pair(std::bind(call, inst), (void*) inst));
	}

	//Unsubsribe all callback relative to the object inst (WARNING : must be called at the destruction of inst)
	template<class T>
	void Unsubsribe(T* inst){
		for (auto& itr1 : callbacks) {
			for (auto itr2 = itr1.second.begin(); itr2 != itr1.second.end();) {
				if (itr2->second == inst)
					itr2 = itr1.second.erase(itr2);
				else
					itr2++;
			}
		}
	}

protected:
	using subsribe_type = std::function<void(void)>;
	using callback_container = std::vector<std::pair<subsribe_type, void*>>;

	void NotifyAll(std::string tag);

	std::unordered_map<std::string, callback_container> callbacks;
};