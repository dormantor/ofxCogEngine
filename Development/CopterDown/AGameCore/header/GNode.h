
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"
#include "IwGeomFMat2D.h"
#include "IwList.h"
#include "EnBounds.h"
#include "IwMap.h"
#include "Enums.h"
#include <list>
#include "SmartPointer.h"
#include <stdexcept>
#include <string>

using namespace std;

/**
* GNode - game object with attributes, behaviors and transformation matrix
*
*/
class GNode{
protected:
	// identifier incremental counter
	static int idCounter;
	// dummy message that is used for no-response sending
	static Msg _dummyMsg;
	// attributes, mapped by their keys
	map<int, Attr*> _attributes;
	// list of behaviors
	list<ABehavior*> _behaviors;
	// link to the parent
	GNode* _parent = nullptr;
	// list of childrens
	list<GNode*> _children;

	// id of this node
	const int _id;
	// tag or name
	string* _tag = nullptr;
	// object type {ROOT, SCENE, OBJECT, HUD, INFO}
	ObjType _type;
	// subtype (or category)
	int _subType = 0;
	// groups this object is in
	EnFlags* _groups = nullptr;
	// states this object has set
	EnFlags* _states = nullptr;
	// transformation matrix (size = 24)
	CIwFMat2D  _transform;
	
public:

	/**
	* Creates a new game object
	* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
	* @param subType subtype/category number
	* @param tag tag/name 
	*/
	GNode(ObjType type, int subType, string tag);

	GNode(const GNode& copy);

	~GNode();

	/**
	* Sends a message to behaviors and other game objects
	* @param msg message that will be sent
    * @param resp response that could be filled (at most once)
	*/
	void SendMessage(Msg& msg, Msg& resp);

	/**
	* Sends a message to behaviors and other game objects, expecting no response
	* @param msg message that will be sent
	*/
	void SendMessageNoResp(Msg& msg);

	/**
	* Updates behavior inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	* @param absMatrix current absolute matrix, traversed from parent
	*/
	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix);

	/**
	* Draws all objects
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	* @param absMatrix current absolute matrix, traversed from parent
	*/
	void Draw(const uint64 delta, const uint64 absolute, CIwFMat2D& absMatrix);

	/**
	* Adds a new behavior
	* @param beh behavior to add
	* @return true, if behavior has been added
	*/
	bool AddBehavior(ABehavior* beh);

	/**
	* Removes existing behavior (by its id)
	* @param beh behavior to remove
	* @return true, if behavior has been removed
	*/
	bool RemoveBehavior(ABehavior* beh);

	/**
	* Removes existing attribute (by its key)
	* @param key key of attribute that will be removed
	* @return true, if attribute has been removed
	*/
	bool RemoveAttr(int key);

	/**
	* Returns true, if this game object has an attribute with given key
	*/
	bool HasAttr(int key) const;

	/**
	* Gets copy of list of all behaviors
	*/
	list<ABehavior*> GetBehaviorsCopy() const;

	/**
	* Gets direct reference to list of all behaviors
	*/
	const list<ABehavior*>& GetBehaviors() const;

	/**
	* Gets copy of list of all children
	*/
	list<GNode*> GetChildrenCopy() const;

	/**
	* Gets direct reference to list of all children
	*/
	const list<GNode*>& GetChildren() const;

	/**
	* Adds a new child
	* @param child childto add
	* @return true, if child has been added
	*/
	bool AddChild(GNode* child);

	/**
	* Removes an existing child
	* @param child child to remove
	* @return true, if child has been removed
	*/
	bool RemoveChild(GNode* child);

	/**
	* Gets pointer to the parent of this game node
	*/
	GNode* GetParent();

	/**
	* Sets parent of this game object
	*/
	void SetParent(GNode* val);

	/**
	* Finds the first predecessor of given object type
	* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	GNode* FindPredecessor(ObjType type);

	/**
	* Gets the nearest parent that is a scene root (if exists)
	*/
	GNode* GetSceneRoot();

	/**
	* Gets the root of the game scene
	*/
	GNode* GetRoot();

	/**
	* Gets unique identifier of this game object
	*/
	int GetId() const;
	
	/**
	* Gets tag/name
	*/
	string GetTag() const;

	/**
	* Sets tag/name
	*/
	void SetTag(string tag);

	/**
	* Gets type/category of this game object {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	ObjType GetType() const;
	
	/**
	* Gets subtype
	*/
	int GetSubType() const;

	/**
	* Sets subtype 
	*/
	void SetSubType(int val);

	/**
	* Gets transformation matrix
	*/
	CIwFMat2D& GetTransform();

	/**
	* Sets transformation matrix
	*/
	void SetTransform(CIwFMat2D val);

	/**
	* Returns indicator, if groups has been initialized
	* Check this indicator first before asking for groups, because
	* they are lazy initialized
	*/
	const bool HasGroups() const{
		return _groups != nullptr;
	}

	/**
	* Gets flag array, representing list of all groups this object belongs to
	*/
	EnFlags& GetGroups();

	/**
	* Sets groups this objects belongs to
	*/
	void SetGroups(EnFlags val);

	/**
	* Returns indicator, if states has been initialized
	* Check this indicator first before asking for states, because
	* they are lazy initialized
	*/
	const bool HasStates() const{
		return _states != nullptr;
	}

	/**
	* Gets flag array, representing all states this object has set
	*/
	EnFlags& GetStates();

	/**
	* Sets states this object has set
	*/
	void SetStates(EnFlags val);

	/**
	* Adds a new attribute of value type
	* @param key key of the attribute
	* @param value reference
	* @param managed if true, attribute will be destroyed together with attribute
	*/
	template<class T> void AddAttr(int key, T value){
		if (HasAttr(key)){
			RemoveAttr(key);
		}

		_attributes[key] = new AttrR<T>(key, value, this);
	}
	
	/**
	* Finds an attribute or creates a new one, if it doesn't exist
	* @param key attribute key
	*/
	template<class T> T& GetAttr(int key){
		auto it = _attributes.find(key);
		if (it != _attributes.end()){
			AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
			return attr->GetValue();
		}
		else{
			T newObj;
			AttrR<T>* newAttr = new AttrR<T>(key, newObj, this);
			_attributes[key] = newAttr;
			return newAttr->GetValue();
		}
	}

	/**
	* Changes value of attribute
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T value){
		auto it = _attributes.find(key);
		if (it != _attributes.end()){
			AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
			attr->SetValue(value);
		}
		else{
			T newObj;
			AttrR<T>* newAttr = new AttrR<T>(key, newObj, this);
			_attributes[key] = newAttr;
			return newAttr->GetValue();
		}
	}

	bool operator==(int id){
		return this->_id == id;
	}

	bool operator==(const GNode& other){
		return _id == other._id;
	}

	bool operator!=(int id){
		return this->_id != id;
	}

	bool operator!=(const GNode& other){
		return _id != other._id;
	}
};


#endif