#include "AsyncProcess.h"
#include "Scene.h"
#include "Stage.h"
#include "ComponentStorage.h"

namespace Cog {

	SceneLoader::SceneLoader(Scene* scene, TweenDirection tweenDir)
		: scene(scene), tweenDir(tweenDir) {

	}

	void SceneLoader::DoJob() {
		
		spt<ofxXml> config = spt<ofxXml>(new ofxXml());
		config->loadFile(ofToDataPath(PATH_SCENES));

		config->popAll();
		config->pushTag("resources");

		for (int i = 0; i < config->getNumTags("scene"); i++) {
			config->pushTag("scene", i);

			// just load scene from XML and switch to it
			string name = config->getAttributex("name", "");
			if (name.compare(scene->GetName()) == 0) {
				scene->LoadFromXml(config);
				config->popTag();
				break;
			}

			config->popTag();
		}

		config->popAll();

		// switch to scene
		auto stage = GETCOMPONENT(Stage);
		scene->GetSceneNode()->SubmitChanges(true);
		stage->SwitchToScene(scene, tweenDir);
	}

	void AsyncProcess::threadedFunction()
	{
		COGLOGDEBUG("AsyncProcess", "Running threaded job %s", typeid(this->job).name());
		job->DoJob();
		COGLOGDEBUG("AsyncProcess", "Finishing threaded job %s", typeid(this->job).name());
		delete job;
	}
} // namespace