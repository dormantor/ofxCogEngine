
--[[
 -- methods for testing in lua console

function BehaviorPrototype()
 return  {
  RegisterDelegate = function(self, tb)
    print("Registering delegate "..tb.classname)
  end
  }
  end

function CogRegisterBehaviorPrototype(tb)
  print("Registering behavior prototype "..tb.classname)
end

--]]

-- create behavior base object
Behavior = {
	-- function for creating instances from C++ code
	NewCpp = function(self, bproxy)
		local meta = self.CreateMeta(self)
		self:Constructor()
		bproxy:RegisterDelegate(meta)
		meta.proxy = bproxy
		return meta
	end,
	-- function for creating instances
	New = function(self, ...)
		local meta = self.CreateMeta(self)
		self:Constructor(...)
		local beh = BehaviorProxy()
		beh:RegisterDelegate(meta)
		meta.proxy = beh

		return meta
	end,
	-- function for defining new class
	Extend = function(self, classname, subtype)
		if (subtype) then
            -- copy methods into subtype
			subtype.NewCpp = self.NewCpp
			subtype.New = self.New
			subtype.Extend = self.Extend
			subtype.CreateMeta = self.CreateMeta
			subtype.Constructor = subtype.Constructor or self.Constructor
			subtype.OnInit = subtype.OnInit or self.OnInit
			subtype.OnMessage = subtype.OnMessage or self.OnMessage
			subtype.Update = subtype.Update or self.Update
			subtype.SubscribeForMessages = self.SubscribeForMessages
			subtype.SendMessage = self.SendMessage
		else
			subtype = self
		end

		if not(classname) or classname == "" then error("When called Extend(classname), a name of the new class must be specified") end

		local meta = subtype.CreateMeta(subtype)
		meta.classname = classname
		CogRegisterBehaviorPrototype(meta, classname)
		return meta
	end,
	-- create metatable
	CreateMeta = function(self, subtype)
		local meta = { __index = subtype }
		output = setmetatable( { }, {
			__index = self,
			-- Behavior() and Behavior:new() are the same
			__call = function(self, ...)
				return self:New(...)
			end
		} )

		return output
	end,
	Constructor = function(self, ...) end,
	OnInit = function(self) end,
	OnMessage = function(self, msg) end,
	Update = function(self, delta, absolute) end,
	SubscribeForMessages = function(self, ...) return self.proxy:SubscribeForMessages(...) end,
	SendMessage = function(self, msg) return self.proxy:SendMessage(msg) end
}