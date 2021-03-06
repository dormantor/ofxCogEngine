
#include "HitEvent.h"
#include "ofxCogEngine.h"


#ifdef ANDROID
#include "ofxAndroidVibrator.h"
#endif

namespace Cog {

	void HitEvent::Update(const uint64 delta, const uint64 absolute) {

		if (owner->HasState(StrId(STATE_HITTABLE))) {

			// get inverse matrix
			ofMatrix4x4 inverse = owner->GetTransform().CalcAbsMatrix().getInverse();

			bool atLeastOneTouch = false;

			for (InputAct* touch : CogGetPressedPoints()) {

				// calculate vector in local space
				ofVec2f touchVector = touch->position;
				ofVec3f touchTrans = ofVec3f(touchVector)*inverse;

				// do hit test
				bool hasHitTest = DoHitTest(touchTrans, touchVector);

				// check if this touch hasn't been already handled
				if (hasHitTest && (touch->handlerNodeId == -1 || touch->handlerNodeId == owner->GetId())) {
				
					if (touch->started) {
#ifdef ANDROID
						if (vibrate) ofxAndroidVibrator::vibrate(50);
#endif
						atLeastOneTouch = true;
						hitStarted = true;
						hitStartedTouchId = touch->touchId;

						// when the touch starts, a request into input handler is registered 
						// and the object with highest z-index will later be selected
						auto inputHandler = GETCOMPONENT(InputHandler);
						inputHandler->RegisterRequest(owner, touch, -1);
					}
					else if (touch->ended) {

						owner->ResetState(StrId(STATE_HIT));
						
						// send messages according to actual state

						if (hitStarted) {
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_ENDED, new InputEvent(touch));
							else SendMessageToBehavior(ACT_OBJECT_HIT_ENDED, new InputEvent(touch), handlerBehId);
						}
						else {
							// hit has been lost
							hitLost = true;
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_LOST, new InputEvent(touch));
							else SendMessageToBehavior(ACT_OBJECT_HIT_LOST, new InputEvent(touch), handlerBehId);
						}

						if (hitStarted && hitStartedTouchId == touch->touchId) {
							hitStarted = false;
						}
					}
					else {
						atLeastOneTouch = true;
						if (!owner->HasState(StrId(STATE_HIT))) {
#ifdef ANDROID
							if (vibrate) ofxAndroidVibrator::vibrate(50);
#endif
							// this object hasn't been hit
							owner->SetState(StrId(STATE_HIT));
						}
						// hit started and goes on
						if (hitLost) {
							hitLost = false;
							// hit started, lost and started again
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_STARTED, new InputEvent(touch));
							else SendMessageToBehavior(ACT_OBJECT_HIT_STARTED, new InputEvent(touch), handlerBehId);
						}
						else {
							// hit started but not on first touch
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_OVER, new InputEvent(touch));
							else SendMessageToBehavior(ACT_OBJECT_HIT_OVER, new InputEvent(touch), handlerBehId);
						}
					}
				}
				else {
					if (touch->ended && touch->touchId == hitStartedTouchId) {
						// object wasn't hit and this hit has already ended
						hitStarted = false;
					}
				}
			}

			if (!atLeastOneTouch) {
				// object has probably lost its hit
				if (owner->HasState(StrId(STATE_HIT))) {
					owner->ResetState(StrId(STATE_HIT));
					hitLost = true;
					if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_LOST);
					else SendMessageToBehavior(ACT_OBJECT_HIT_LOST, handlerBehId);
				}
			}
		}
	}

	bool HitEvent::DoHitTest(ofVec3f touchTrans, ofVec3f touchVector) {
		bool hasHitTest = false;

		// check hit according to the type of the mesh
		if (owner->HasMeshType(MESH_IMAGE)) {
			if (preciseTest) hasHitTest = ImagePreciseHitTest(owner->GetMesh<Image>()->GetImage(), touchTrans);
			else hasHitTest = CShapeHitTest(owner->GetMesh<Image>(), touchTrans);
		}
		else if (owner->HasMeshType(MESH_MULTISPRITE)) {
			hasHitTest = CShapeHitTest(owner->GetMesh<MultiSpriteMesh>(), touchTrans);
		}
		else if (owner->HasMeshType(MESH_SPRITE)) {
			hasHitTest = CShapeHitTest(owner->GetMesh<MultiSpriteMesh>(), touchTrans);
		}
		else if (owner->HasMeshType(MESH_RECTANGLE)) {
			hasHitTest = CShapeHitTest(owner->GetMesh<FRect>(), touchTrans);
		}
		else if (owner->HasMeshType(MESH_BOUNDING_BOX)) {
			hasHitTest = BoundingBoxHitTest(owner->GetMesh<BoundingBoxMesh>(), touchVector);
		}
		return hasHitTest;
	}

	bool HitEvent::BoundingBoxHitTest(spt<BoundingBoxMesh> bbox, ofVec2f touchVector) {
		// recalculate bounding box first
		bbox->Recalc(owner);
		ofRectangle bboxRect = bbox->GetBoundingBox();

		if (bboxRect.x <= touchVector.x && (bboxRect.x + bboxRect.width) >= touchVector.x
			&& bboxRect.y <= touchVector.y && (bboxRect.y + bboxRect.height) >= touchVector.y) return true;
		return false;
	}

	bool HitEvent::ImagePreciseHitTest(spt<ofImage> image, ofVec3f testPos) {
		ofColor col = image->getColor((int)testPos.x, (int)testPos.y);
		// return a hit if the specified local alpha value is greater than half
		return ((int)col.a) > 0x80;
	}
}// namespace