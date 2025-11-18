#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

class Notifier {
public:
	Notifier(){}
	~Notifier(){}

	template<class T>
	void Subscribe(std::string tag, void(T::* call)(void), T* inst) {
		auto itr = callbacks.find(tag);
		if (itr == callbacks.end())
			itr = callbacks.insert(std::make_pair(tag, callback_container())).first;
		itr->second.push_back(std::bind(call, inst));
	}

protected:
	using subsribe_type = std::function<void(void)>;
	using callback_container = std::vector<subsribe_type>;

	void NotifyAll(std::string tag);

	std::unordered_map<std::string, callback_container> callbacks;
};