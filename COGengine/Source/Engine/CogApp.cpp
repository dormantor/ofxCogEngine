#pragma once

#include "CogApp.h"
#include "CogEngine.h"
#include "Stage.h"

namespace Cog {

	void CogApp::InitEngine() {
		if (!xmlConfig) {
			CogEngine::GetInstance().Init();
		}
		else {
			CogEngine::GetInstance().Init(xmlConfig);
		}
	}

void CogApp::setup(){
	// never set vertical sync
	ofSetVerticalSync(false);
	ofSetFrameRate(this->fps);
	ofEnableAntiAliasing();
	// initialize COG engine
	
	CogEngine::GetInstance().SetFps(this->fps);
}

void CogApp::setupEngine() {
	if (!this->splashScreen.empty()) {
		DrawSplashScreen();
	}

	
	this->InitEngine();
	this->InitComponents();

	auto stage = GETCOMPONENT(Stage);
	this->InitStage(stage);

	// initialize time
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();

	engineInitialized = true;
}

void CogApp::draw(){
	if (engineInitialized) {
		// drawing loop
		CogEngine::GetInstance().Draw(delta, absolute);
	}
}

void CogApp::update(){

	if (!engineInitialized) {
		// a small hack that provides the possibility to
		// show a splashscreen during the engine initialization 
		setupEngine();
	}
	else {
		// update loop
		delta = ofGetSystemTime() - absolute;
		absolute = ofGetSystemTime();

		float fpsThreshold = 1000 / this->GetFps();

		uint64 semiFixedDelta = (delta < fpsThreshold) ? fpsThreshold : (delta < (2 * fpsThreshold)) ? delta : (2 * fpsThreshold);

		CogEngine::GetInstance().Update(semiFixedDelta, absolute);
	}
}

void CogApp::keyPressed(int key){
	CogEngine::GetInstance().environment->OnKeyAction(key, true);
}

void CogApp::keyReleased(int key){
	CogEngine::GetInstance().environment->OnKeyAction(key, false);
}

void CogApp::windowResized(int w, int h){
	if (engineInitialized) {
		CogEngine::GetInstance().environment->OnScreenSizeChanged(w, h);
	}
}



#ifdef WIN32

void CogApp::mouseMoved(int x, int y){

}

void CogApp::mouseDragged(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchMotion(x, y, button);

}

void CogApp::mousePressed(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, true);
}

void CogApp::mouseReleased(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, false); 
}

void CogApp::dragEvent(ofDragInfo dragInfo){

}


#else


void CogApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

void CogApp::pause(){

}

void CogApp::stop(){

}

void CogApp::resume(){

}

void CogApp::reloadTextures(){

}

bool CogApp::backPressed(){
	CogEngine::GetInstance().stage->SwitchBackToScene(TweenDirection::NONE);
	return true;
}

void CogApp::okPressed(){

}

void CogApp::cancelPressed(){

}

void CogApp::touchDown(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, true);
}

void CogApp::touchMoved(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchMotion(x, y, id);
}

void CogApp::touchUp(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, false);
}

void CogApp::touchDoubleTap(int x, int y, int id){

}

void CogApp::touchCancelled(int x, int y, int id){

}

#endif

void CogApp::DrawSplashScreen() {
#ifdef WIN32
	ofImage image = ofImage();
	image.loadImage(this->splashScreen);
	GLFWwindow* window = glfwGetCurrentContext();
	glfwShowWindow(window);
	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadIdentityMatrix();
	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadIdentityMatrix();
	ofScale(-1, 1);
	ofRotate(180);
	ofClear(0, 0, 0);
	image.update();
	image.draw(-1, -1, 2, 2);
	glfwSwapBuffers(window);
	glFlush();

	//set the context back to main for rest of setup
	glfwMakeContextCurrent(window);
	ofSetupScreen();
#endif
}

}// namespace