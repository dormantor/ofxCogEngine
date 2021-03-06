#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* Behavior that consists of other behaviors, allowing them
	* to be updated at once; the behavior is finished when all inner
	* behaviors are finished
	* Note that if this behavior is updated once without any behavior attached,
	* it will finish immediately
	*/
	class CompositeBehavior : public Behavior {
	protected:
		vector<Behavior*> behaviors;
	public:
		CompositeBehavior() {

		}

		CompositeBehavior(Behavior* beh1) {
			behaviors.push_back(beh1);
		}

		CompositeBehavior(Behavior* beh1, Behavior* beh2) {
			behaviors.push_back(beh1);
			behaviors.push_back(beh2);
		}

		CompositeBehavior(Behavior* beh1, Behavior* beh2, Behavior* beh3) {
			behaviors.push_back(beh1);
			behaviors.push_back(beh2);
			behaviors.push_back(beh3);
		}

		CompositeBehavior(Behavior* beh1, Behavior* beh2, Behavior* beh3, Behavior* beh4) {
			behaviors.push_back(beh1);
			behaviors.push_back(beh2);
			behaviors.push_back(beh3);
			behaviors.push_back(beh4);
		}

		~CompositeBehavior() {
			for (auto beh : behaviors) {
				delete beh;
			}
		}

		void AddBehavior(Behavior* beh);

		bool RemoveBehavior(Behavior* beh);
		
		void OnInit();

		void OnStart();

		void OnFinish();

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute);
	};


}// namespace