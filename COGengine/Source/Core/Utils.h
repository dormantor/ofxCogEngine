#pragma once



namespace Cog {


#define GETCOMPONENT(className) CogGetEntityStorage()->GetComponent<className>(#className)
#define GETBEHAVIOR(className) COGEngine.entityStorage->GetBehaviorPrototype<className>(#className)
#define CREATE_BEHAVIOR(className) COGEngine.entityStorage->GetBehaviorPrototype<className>(#className)->CreatePrototype()
#define REGISTER_COMPONENT(object) COGEngine.entityStorage->RegisterComponent(object)
#define REGISTER_BEHAVIOR(className) COGEngine.entityStorage->RegisterBehaviorPrototype(#className, new className())

#define OBJECT_VIRTUAL() \
public: \
virtual string GetClassName() { \
	return string(); \
} 

#define PROTOTYPE_VIRTUAL(compName) \
public: \
virtual compName* CreatePrototype(){ \
 return nullptr; \
}

#define OBJECT(compName) \
  public: \
  string GetClassName() { \
      return GetClassNameStatic(); \
  } \
  \
  static string GetClassNameStatic() { \
	  return string(#compName); \
  }

#define DEFAULT_CONST(compName) \
public: \
  compName(){ \
  } 

#define OBJECT_PROTOTYPE(compName) \
  OBJECT(compName) \
  DEFAULT_CONST(compName) \
  compName* CreatePrototype(){ \
	return new compName(); \
  }

	// assertion with formatted message
#ifdef DEBUG
#   define MASSERT(condition, module, message, ...) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << printf(message, ##__VA_ARGS__) << std::endl; \
             CogLogError(module,message, ##__VA_ARGS__); \
			 CogLoggerFlush(); \
				        } \
		    } while (false)
#else
#   define MASSERT(condition, message, ...) do { } while (false)
#endif

// macro for debug-only logging
#ifdef DEBUG
#   define MLOGDEBUG(module, message, ...) \
    do { \
             CogLogDebug(module, message, ##__VA_ARGS__); \
				    } while (false)
#else
#   define MLOGDEBUG(message, ...) do { } while (false)
#endif


/**
* Creates formatted string
*/
	std::string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, std::ostream& ss);


} // namespace