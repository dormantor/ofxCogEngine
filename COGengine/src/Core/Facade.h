#pragma once

#include "Msg.h"
#include "InputAct.h"
#include "Soundfx.h"
#include "SheetAnim.h"
#include "Definitions.h"
#include "Settings.h"
#include "pugixml.h"

using namespace pugi;

namespace Cog {

	class Behavior;
	class Component;
	class BaseComponent;
	class ComponentStorage;
	class Scene;
	class Node;
	class Msg;
	class MsgPayload;
	class InputAct;
	class SheetAnim;
	class GeneralAnim;
	class AttribAnim;
	class AsyncProcess;

	ComponentStorage* CogGetComponentStorage();

	/**********************************************************************************/
	/* Facade methods that may be used for centralized access to important components */
	/**********************************************************************************/

	// =================== ofxCogEngine ====================
	
	int CogGetFrameCounter();
	uint64 CogGetAbsoluteTime();

	// =================== Environment ====================
	
	void CogAddSound(Soundfx* sound);
	void CogPlaySound(Soundfx* sound);
	vector<InputAct*>& CogGetPressedKeys();
	vector<InputAct*>& CogGetPressedPoints();
	vector<Soundfx*>& CogGetPlayedSounds();
	void CogStopAllSounds();
	bool CogIsKeyPressed(int key);
	Vec2i CogGetMousePosition();
	Vec2i CogGetMouseScroll();
	int CogGetScreenWidth();
	int CogGetVirtualWidth();
	int CogGetScreenHeight();
	int CogGetVirtualHeight();
	float CogGetVirtualAspectRatio();
	float CogGetScreenAspectRatio();
	Vec2i CogGetScreenSize();
	Vec2i CogGetVirtualScreenSize();
	void CogRunProcess(AsyncProcess* thread);

	
	// =================== Scene ========================
	
	void CogRegisterGlobalListener(StrId action, BaseComponent* listener);
	void CogUnregisterGlobalListener(StrId action, BaseComponent* listener);
	void CogSendMessage(Msg& msg);

	// =================== Logger =========================

	void CogLogError(const char* module, const char* format, ...);
	void CogLogInfo(const char* module, const char* format, ...);
	void CogLogTree(const char* module, int logLevel, const char* format, ...);
	void CogLogDebug(const char* module, const char* format, ...);
	void CogLoggerFlush();

	void CogWriteLogStage();
	void CogWriteLogActualScene();
	void CogWriteTimeReport(bool restart);

	// =================== Renderer =========================
	
	void CogPushNodeForRendering(Node* node);

	// =================== Resources =======================

	spt<ofImage> CogGet2DImage(string path);
	spt<ofImage> CogPreload2DImage(string path);
	spt<ofVboMesh> CogGetVboMesh(string path);
	spt<ofTrueTypeFont> CogGetFont(string path, int size);
	Soundfx* CogGetSound(string path);
	spt<xml_document> CogPreloadXMLFile(string path);
	spt<xml_document> CogLoadXMLFile(string path);
	spt<GeneralAnim> CogGetAnimation(string name);
	spt<SheetAnim> CogGetSheetAnimation(string name);
	spt<AttribAnim> CogGetAttribAnimation(string name);
		
	void CogStoreAnimation(spt<SheetAnim> anim);
	Settings& CogGetGlobalSettings();
	Settings& CogGetDefaultSettings();
	Settings& CogGetProjectSettings();


}// namespace