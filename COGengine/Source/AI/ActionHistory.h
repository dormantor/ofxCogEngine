#pragma once

#include <vector>
using namespace std;

template<class S>
class ActionHistory {
protected:
	class ActionRecord {
	protected:
		S state;
		StringHash action;

	public:
		ActionRecord(S state, StringHash action) {
			this->state = state;
			this->action = action;
		}

		S GetState() {
			return state;
		}

		StringHash GetAction() {
			return action;
		}
	};

	map<int,vector<ActionRecord>> records;
	int recordsNum = 0;
public:
	void AddRecord(S state, StringHash action, int agentIndex) {
		
		if (records.find(agentIndex) == records.end()) records[agentIndex] = vector<ActionRecord>();
		
		auto& agentRecords = records[agentIndex];
		agentRecords.push_back(ActionRecord(state, action));
		recordsNum++;
	}

	S GetState(int agentIndex, int recordIndex) {
		return records[agentIndex][recordIndex].GetState();
	}

	StringHash& GetAction(int agentIndex, int recordIndex) {
		return records[agentIndex][recordIndex].GetAction();
	}

	int GetRecordsNum() {
		return recordsNum;
	}
};