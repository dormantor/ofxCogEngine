#pragma once

#include "Definitions.h"
#include "Component.h"
#include <unordered_set>

namespace Cog {

	class NetOutputMessage;
	class NetworkManager;

	/**
	* State of network communicator
	*/
	enum class NetworkComState {
		NONE,					/** undefined */
		LISTENING,				/** listening */
		DISCOVERING,			/** discovering peers */
		CONNECTING,				/** connecting to peers */
		COMMUNICATING			/** communicating with peers */
	};

	/**
	* Component that communicates over network using UDP protocol and
	* sends synchronization messages
	* 
	* May be used primarily for multiplayer
	* All sent messages have their synchronization id that must be accepted
	* by target by setting the acceptedId property in NetInputMessage. The 
	* only exception are DELTA messages that don't need to be accepted since
	* it is not so critical when some deltas are lost
	*
	* The frequency of message exchanging can also be regulated
	*/
	class NetworkCommunicator : public Component {
		
	private:
		NetworkManager* network = nullptr;
		NetworkComState networkState = NetworkComState::NONE;
		tBYTE applicationId;
		int myPort;
		int peerPort;
		string peerIp = "";

		// collection of ids of messages for acceptation
		set<tBYTE> forAcceptationMessageIds;
		// collection of times of messages for acceptation
		set<uint64> forAcceptationMessageTimes;
		// identifier of last received message
		tBYTE lastReceivedMsgId = -1;
		// time of last received message
		uint64 lastReceivedMsgTime = 0;
		// identifier of last sent message
		tBYTE lastSentMsgId = 0;
		// collection of messages that will be sent in the next update
		vector<spt<NetOutputMessage>> messagesToSend;
		// collection of discovered peers, mapped by their ips
		map<string, uint64> discoveredPeers;
		// collection of not confirmed messages that will be re-sent repeatedly
		map<tBYTE, spt<NetOutputMessage>> unconfirmedMessages;
		
		// number of broadcasts per second
		float broadcastingFrequency = 1;
		// number of discovers per second
		float discoveringFrequency = 4;
		// number of sent messages per second
		float sendingFrequency = 10;
		// number of reconnects per second
		float connectingFrequency = 2;
		
		uint64 lastBroadcastTime = 0;
		uint64 lastDiscoveringTime = 0;
		uint64 lastSendingTime = 0;
		uint64 lastConnectingTime = 0;
		// number of seconds after which the connection attempts
		// will be made again
		int reconnectTimeout = 4;
		// number of seconds the other station will be automatically disconnected
		int disconnectTimeout = 20;
		// indicator whether the client should connect to the first peer that it founds
		bool autoConnect = false;
		// indicator whether this communicator is a host
		bool isHost = false;
	public:

		/**
		* Gets network current status
		*/
		NetworkComState GetNetworkState() const {
			return networkState;
		}

		/**
		* Gets id of application that is checked with each incomming message
		*/
		tBYTE GetApplicationId() const {
			return applicationId;
		}

		/**
		* Gets port this communicator listens on
		*/
		int GetMyPort() const {
			return myPort;
		}

		/**
		* Gets ip address of the peer
		*/
		string GetPeerIp() const {
			return peerIp;
		}

		/**
		* Gets port of the peer
		*/
		int GetPeerPort() const {
			return peerPort;
		}

		/**
		* Gets indicator whether the client should connect to the first peer that it founds
		*/
		bool AutoConnect() {
			return autoConnect;
		}

		/**
		* Sets indicator whether the client should connect to the first peer that it founds
		*/
		void SetAutoConnect(bool autoConnect) {
			this->autoConnect = autoConnect;
		}

		/**
		* Gets time the last message was received
		*/
		uint64 GetLastReceivedMsgTime() const {
			return lastReceivedMsgTime;
		}

		/**
		* Gets collection of discovered peers, mapped
		* by their ips
		*/
		map<string, uint64>& GetDiscoveredPeers() {
			return discoveredPeers;
		}

		/**
		* Gets number of broadcasts per second
		*/
		float GetBroadcastingFrequency() const {
			return broadcastingFrequency;
		}

		/**
		* Sets number of broadcasts per second
		*/
		void SetBroadcastingFrequency(float frequency) {
			this->broadcastingFrequency = frequency;
		}

		/**
		* Gets number of sent messages per second
		*/
		float GetSendingFrequency() const {
			return sendingFrequency;
		}

		/**
		* Sets number of sent messages per second
		*/
		void SetSendingFrequency(float frequency) {
			this->sendingFrequency = frequency;
		}

		/**
		* Gets number of connection attempts per second
		*/
		float GetConnectingFrequency() const {
			return connectingFrequency;
		}

		/**
		* Sets number of connection attempts per second
		*/
		void SetConnectingFrequency(float frequency) {
			this->connectingFrequency = frequency;
		}

		/**
		* Gets number of seconds after which the connection attempts will 
		* be made again
		*/
		int GetReconnectTimeout() const {
			return reconnectTimeout;
		}

		/**
		* Sets number of seconds after which the connection attempts will
		* be made again
		*/
		void SetReconnectTimeout(int reconnectTimeout) {
			this->reconnectTimeout = reconnectTimeout;
		}

		/**
		* Gets number of seconds after which the connection will be closed
		*/
		int GetDisconnectTimeout() const {
			return disconnectTimeout;
		}

		/**
		* Sets number of seconds after which the connection will be closed
		*/
		void SetDisconnectTimeout(int disconnectTimeout) {
			this->disconnectTimeout = disconnectTimeout;
		}

		/**
		* Initializes broadcasting mode, trying to found a peer
		* @param applicationId id of application that defines the connection; is 
		* checked in every received message
		* @param myPort port this communicator will listen on
		* @param peerPort port of the peer
		*/
		void InitBroadcast(tBYTE applicationId, int myPort, int peerPort);

		/**
		* Initializes listening mode, waiting for other peers to connect
		* @param applicationId id of application that defines the connection; is
		* checked in every received message
		* @param myPort port this communicator will listen on
		*/
		void InitListening(tBYTE applicationId, int myPort);

		/**
		* Pushes message that will be sent with the next update
		*/
		void PushMessageForSending(spt<NetOutputMessage> msg);
		
		/**
		* Closes the connection
		*/
		void Close();

		/**
		* Connects to peer with given ip address
		*/
		void ConnectToPeer(string ip);

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:

		/** Update for listening state */
		void UpdateListening(const uint64 absolute);
		/** Update for discovering state */
		void UpdateDiscovering(const uint64 absolute);
		/** Update for connecting state */
		void UpdateConnecting(const uint64 absolute);
		/** Update for communicating state */
		void UpdateCommunicating(const uint64 absolute);
		
		/**
		* Sends required messages and re-sends those that
		* still weren't confirmed 
		*/
		void SendMessages(uint64 time);
	};

}// namespace