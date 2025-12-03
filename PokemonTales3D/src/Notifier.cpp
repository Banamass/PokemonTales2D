#include "Notifier.h"

void Notifier::NotifyAll(std::string tag) {
	auto itr = callbacks.find(tag);
	if (itr == callbacks.end())
		return;
	for (auto& f : itr->second) {
		f.first();
	}
}