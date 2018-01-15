#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "MsgLua.h"
#include "NodeBuilder.h"

namespace luabridge {
	struct lua_State;
}


namespace Cog {

	class SceneLua;
	class NodeLua;
	class BehaviorLua;

	/**
	* NodeBuilder wrapper for Lua script
	*/
	class NodeBuilderLua{
	public:
		NodeBuilder bld;

		NodeLua* CreateNode(string name, SceneLua* scene);

		NodeLua* LoadNodeFromXml(xml_node& xml, NodeLua* parent, SceneLua* scene);

		NodeLua* LoadRefNodeFromXml(xml_node& contextXml, string nodeName, NodeLua* parent, SceneLua* scene);

		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace