#include "TransformBuilder.h"
#include "Node.h"

namespace Cog {


	void TransformBuilder::BuildAndReset(Node* element) {
		Build(element);
		SetDefaultVals();
	}

	void TransformBuilder::Build(Node* element) {

		Node* parent = element->GetParent();
		Trans nodeTransform = Trans(0, 0);

		CalcTransform(nodeTransform, element, parent);


		// for rectangles, width and height are set directly instead of scale
		switch (element->GetMesh()->GetMeshType()) {
		case MESH_NONE:
		case MESH_RECTANGLE:
			auto rectShape = element->GetMesh<FRect>();
			rectShape->SetWidth(rectShape->GetWidth()*nodeTransform.scale.x);
			rectShape->SetHeight(rectShape->GetHeight()*nodeTransform.scale.y);
			nodeTransform.scale = ofVec3f(1);
			break;
		}

		// refresh transform (recalculate from parent)
		nodeTransform.CalcAbsTransform(parent->GetTransform());
		element->SetTransform(nodeTransform);
	}

	void TransformBuilder::Build(Node* element, Node* parent) {
		Trans nodeTransform = Trans(0, 0);
		CalcTransform(nodeTransform, element, parent);


		// for rectangles, width and height are set directly instead of scale
		switch (element->GetMesh()->GetMeshType()) {
		case MESH_NONE:
		case MESH_RECTANGLE:
			auto rectShape = element->GetMesh<FRect>();
			rectShape->SetWidth(rectShape->GetWidth()*nodeTransform.scale.x);
			rectShape->SetHeight(rectShape->GetHeight()*nodeTransform.scale.y);
			nodeTransform.scale = ofVec3f(1);
			break;
		}
		element->SetTransform(nodeTransform);
	}

	void TransformBuilder::Calculate(Trans& outputTrans, Node* element) {
		Node* parent = element->GetParent();
		CalcTransform(outputTrans, element, parent);
		SetDefaultVals();
	}

	void TransformBuilder::CalcTransform(Trans& outputTrans, Node* node, Node* parent) {

		Trans& parentTrans = parent->GetTransform();

		// calculate scale
		ofVec2f scale = CalcScale(node, parent, size.x, size.y, sType);
		// calculate position
		ofVec2f absPos = CalcPosition(node, parent, pos, pType);


		// fix position according to the anchor
		absPos.x += (0.0f - anchor.x) * node->GetMesh()->GetWidth()*scale.x;
		absPos.y += (0.0f - anchor.y) * node->GetMesh()->GetHeight()*scale.y;

		// if zIndex is equal to 0, the value will be taken from the parent
		if (zIndex == 0) zIndex = (int)parentTrans.localPos.z;

		// set transformation
		outputTrans.localPos = ofVec3f(absPos.x, absPos.y, (float)zIndex);

		outputTrans.scale = ofVec3f(scale.x, scale.y, 1);
		outputTrans.rotationCentroid = ofVec2f(node->GetMesh()->GetWidth(), node->GetMesh()->GetHeight()) * rotationCentroid * (scale); // multiply by abs scale
		outputTrans.rotation = rotation;
	}

	ofVec2f TransformBuilder::CalcPosition(Node* node, Node* parent, ofVec2f pos, stenum posCalc) {
		Trans& parentTrans = parent->GetTransform();
		ofVec2f absPos;


		switch (posCalc) {
		case CALCTYPE_ABS:
			// absolute position in pixels
			absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CALCTYPE_LOC:
			// local position is scaled according to the absolute scale of the parent
			absPos = pos;
			break;
		case CALCTYPE_PER:
			// relative percentage -> parent size is 1.0 x 1.0
			absPos = ofVec2f(pos.x*parent->GetMesh()->GetWidth(),
				pos.y*parent->GetMesh()->GetHeight());
			break;
		}

		return absPos;
	}

	ofVec3f TransformBuilder::CalcScale(Node* node, Node* parent, float width, float height, stenum scaleCalc) {

		Trans& parentTrans = parent->GetTransform();

		float scaleX = 1;
		float scaleY = 1;

		switch (scaleCalc) {
		case CALCTYPE_ABS:
			// absolute scale
			scaleX = width / parentTrans.absScale.x;
			scaleY = height / parentTrans.absScale.y;
			break;
		case CALCTYPE_LOC:
			// local scale, multiplied by scale of the parent
			if (width == 0 && height == 0) {
				width = 1;
				height = 1;
			}
			scaleX = width;
			scaleY = height;
			break;
		case CALCTYPE_PER:
			// relative percentage scale ->1.0 x 1.0 will fit the whole parent
			scaleX = (width * parent->GetMesh()->GetWidth() / node->GetMesh()->GetWidth());
			scaleY = (height * parent->GetMesh()->GetHeight() / node->GetMesh()->GetHeight());
			break;
		}

		// correct auto size
		if (width != 0 && height != 0) return ofVec3f(scaleX, scaleY, 1);
		else if (width == 0) return ofVec3f(scaleY, scaleY, 1);
		else return ofVec3f(scaleX, scaleX, 1); // height = 0

	}

} // namespace