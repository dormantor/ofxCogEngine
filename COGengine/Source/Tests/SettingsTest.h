#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "cpplinq.hpp"

#ifdef TESTING


TEST_CASE("Settings test", "[class]")
{

	SECTION("Simple settings parser")
	{
#define SETTINGS "<setting name=\"test_settings\">\
		            <item key=\"test_key\" value=\"test_val\" />\
			      </setting>"

		// load xml from string
		spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
		xml->loadFromBuffer(SETTINGS);

		auto res = GETCOMPONENT(ResourceCache);
		auto settings = Settings(res->LoadSettingsFromXml(xml));

		auto val = settings.GetSettingVal("test_settings", "test_key");

		REQUIRE(val.compare("test_val") == 0);
	}
}

#endif

