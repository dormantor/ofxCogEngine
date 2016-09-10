#pragma once

#include "Behavior.h"
#include "Scene.h"
#include "State.h"
#include "Node.h"

namespace Cog {

	/**x
	* Behavior for finite state machine
	*/
	class StateMachine : public Behavior {
		OBJECT_PROTOTYPE(StateMachine)
	private:
		
		State* currentState = nullptr;
		State* previousState = nullptr;
		vector<State*> globalStates;
		map<StringHash, State*> localStates;

	public:

		void OnInit() {
			for (auto& globalState : globalStates) {
				SetOwner(globalState, owner);
				globalState->Init();
			}

			for (auto& localState : localStates) {
				SetOwner(localState.second, owner);
				localState.second->Init();
			}
		}

		void OnStart() {

			for (auto& globalState : globalStates) {
				globalState->Start();
			}

			if (currentState != nullptr) {
				StartState(currentState);
			}
		}

		void OnMessage(Msg& msg) {
			if (msg.GetSourceObject()->GetId() == owner->GetId()) {

				for (auto& globalState : globalStates) {
					globalState->OnMessage(msg);
				}

				if (currentState != nullptr) currentState->OnMessage(msg);
			}
		}

		void AddGlobalState(State* globalState) {
			for (auto& global : globalStates) {
				if (global->GetState() == globalState->GetState()) {
					CogLogDebug("StateMachine", "An attempt to add already added global state");
					return;
				}
			}

			globalStates.push_back(globalState);
			globalState->SetParent(this);

			if (this->initialized && !globalState->IsInitialized()) {
				globalState->Init();
			}
		}
		
		void AddLocalState(State* localState) {
			for (auto& local : localStates) {
				if (local.second->GetState() == localState->GetState()) {
					CogLogDebug("StateMachine", "An attempt to add already added local state");
					return;
				}
			}

			localStates[localState->GetState()] = localState;
			localState->SetParent(this);

			if (this->initialized && !localState->IsInitialized()) {
				localState->Init();
			}
		}

		/**
		* Changes actual state
		*/
		void ChangeState(State* state) {
			this->previousState = currentState;

			if (this->currentState != nullptr) this->currentState->Finish();

			this->currentState = state;

			// add the state into map
			if (localStates.find(state->GetState()) == localStates.end()) {
				localStates[state->GetState()] = state;
			}

			state->SetParent(this);

			if (this->initialized && !state->IsInitialized()) {
				state->Init();
			}

			state->Start();
		}

		/**
		* Changes state according to its key
		*/
		bool ChangeState(StringHash state) {
			if (localStates.find(state) != localStates.end()) {
				this->ChangeState(localStates.find(state)->second);
				return true;
			}
			return false;
		}

		State* FindLocalState(StringHash state) {
			auto found = localStates.find(state);
			if (found != localStates.end()) return found->second;
			else return nullptr;
		}

		State* GetCurrentState() {
			return currentState;
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			for (auto& global : globalStates) {
				global->Update(delta, absolute);
			}

			if (currentState != nullptr) currentState->Update(delta, absolute);
		}

	protected:
		void StartState(State* state) {
			SetOwner(state, owner);
			state->Start();
		}
	};

}// namespace