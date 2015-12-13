#include "NodeStorage.h"
#include "Scene.h"

namespace Cog {

	void NodeStorage::LoadSceneFromXml(spt<ofxXml> xml) {
		Scene* sc = new Scene();
		sc->LoadFromXml(xml);
		this->SetRootObject(sc->GetSceneNode()); // todo
	}

	void NodeStorage::SendMessage(Msg& msg, Node* actualNode) {
		// there is no such callback or behavior that listens to that type of message
		if (!IsRegisteredListener(msg.GetAction())) return;

		BubblingType& trav = msg.GetBubblingType();

		if (trav.scope == Scope::DIRECT_NO_TRAVERSE) {
			// no BubblingType - just iterate over the proper collection of behaviors and callbacks
			SendDirectMessage(msg);

		}
		else SendBubblingMessage(msg, actualNode);
	}


	void NodeStorage::SendDirectMessageToBehavior(Msg& msg, int targetId) {
		Behavior* beh = FindBehaviorById(targetId);

		if (beh != nullptr) {
			beh->OnMessage(msg);
		}
	}

	void NodeStorage::SendDirectMessageToNode(Msg& msg, int targetId) {
		Node* node = FindNodeById(targetId);

		if (node != nullptr) {
			SendBubblingMessage(msg, node);
		}
	}


	Node* NodeStorage::FindNodeById(int id) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	Behavior* NodeStorage::FindBehaviorById(int id) const {
		for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	int NodeStorage::GetNodesCountByTag(string tag) const {
		int counter = 0;
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) counter++;
		}
		return counter;
	}

	Node* NodeStorage::FindNodeByTag(string tag) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) return (*it);
		}
		return nullptr;
	}

	vector<Node*> NodeStorage::FindNodesByTag(char* tag) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
		}
		return output;
	}

	int NodeStorage::GetNodesCountBySubType(int subtype) const {
		int counter = 0;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) counter++;
		}
		return counter;
	}

	Node* NodeStorage::FindNodeBySubType(int subtype) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) return (*it);
		}
		return nullptr;
	}

	vector<Node*> NodeStorage::FindNodesBySubType(int subtype) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;
	}


	void NodeStorage::SendMessageToBehaviors(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it) {
			Behavior* beh = (*it);
			if ((beh->GetBehState() == BehState::ACTIVE_MESSAGES || beh->GetBehState() == BehState::ACTIVE_ALL) &&
				(beh->GetId() != msg.GetBehaviorId())) {
				if (IsRegisteredListener(msg.GetAction(), beh)) {
					beh->OnMessage(msg);
				}
			}
		}
	}

	void NodeStorage::SendBubblingMessageToChildren(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it) {
			CogSendMessage(msg, (*it));
		}
	}


	void NodeStorage::SendBubblingMessage(Msg& msg, Node* actualNode) {

		BubblingType& trav = msg.GetBubblingType();

		if (trav.scope == Scope::ROOT) {
			trav.scope = Scope::OBJECT;
			// find root and call recursion
			Node* root = actualNode->GetRoot();
			if (root != nullptr) {
				// call this method again from the root
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
				SendMessageToBehaviors(msg, root);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
			}
			return;
		}
		else if (trav.scope == Scope::SCENE) {
			trav.scope = Scope::OBJECT;
			// find scene and call recursion
			Node* scRoot = actualNode->GetSceneRoot();
			if (scRoot != nullptr) {
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
				SendMessageToBehaviors(msg, scRoot);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
			}
			return;
		}

		if (trav.scope == Scope::OBJECT) {
			trav.scope = Scope::OBJECT;
			// call children and itself
			if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
			SendMessageToBehaviors(msg, actualNode);
			if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
		}
		else if (trav.scope == Scope::CHILDREN) {
			trav.scope = Scope::OBJECT;
			// call children only
			SendBubblingMessageToChildren(msg, actualNode);
		}
	}

	void NodeStorage::SendDirectMessage(Msg& msg) {
		auto behaviors = behListeners.find(msg.GetAction());

		if (behaviors != behListeners.end()) {
			vector<Behavior*>& behs = behaviors->second;

			for (auto it = behs.begin(); it != behs.end(); ++it) {
				if (((*it)->GetBehState() == BehState::ACTIVE_MESSAGES || (*it)->GetBehState() == BehState::ACTIVE_ALL)) {
					(*it)->OnMessage(msg);
				}
			}
		}
	}

}// namespace