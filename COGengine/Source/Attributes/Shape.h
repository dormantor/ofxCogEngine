#pragma once

#include "ofxCogMain.h"
#include "Sprite.h"

namespace Cog {

	/**
	* Entity for rendering
	*/
	class Shape {
	protected:
		RenderType renderType;
		ofColor color;
	public:

		Shape() {
			renderType = RenderType::NONE;
		}

		Shape(RenderType renderType) : renderType(renderType) {

		}

		RenderType GetRenderType() {
			return renderType;
		}

		ofColor GetColor() {
			return color;
		}

		void SetColor(ofColor color) {
			this->color = color;
		}

		virtual float GetWidth() {
			return 1; // must be 1
		}

		virtual float GetHeight() {
			return 1; // must be 1
		}
	};

	class Image : public Shape {
	private:
		spt<ofImage> image;
	public:

		Image(spt<ofImage> img) : Shape(RenderType::IMAGE) {
			this->image = img;
		}

		spt<ofImage> GetImage() {
			return image;
		}

		void SetImage(spt<ofImage> img) {
			this->image = img;
		}

		float GetWidth() {
			return image->getWidth();
		}

		float GetHeight() {
			return image->getHeight();
		}
	};

	class Text : public Shape {
	private:
		spt<ofTrueTypeFont> font;
		float size;
		string text;

	public:

		Text(spt<ofTrueTypeFont> font, string text, float size) : Shape(RenderType::TEXT) {
			this->font = font;
			this->size = size;
			this->text = text;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}

		float GetSize() {
			return size;
		}

		void SetSize(float size) {
			this->size = size;
		}

		float GetWidth() {
			return size;
		}

		float GetHeight() {
			return size;
		
		}

		float GetTextWidth() {
			font->setGlobalDpi(size);
			return font->stringWidth(text);

		}

		float GetTextHeight() {
			font->setGlobalDpi(size);
			return font->stringHeight(text);
		}

		string GetText() {
			return text;
		}

		void SetText(string text) {
			this->text = text;
		}
	};

	class SpriteShape : public Shape {
	private:
		spt<Sprite> sprite;

	public:

		SpriteShape(spt<Sprite> sprite) : Shape(RenderType::SPRITE) {
			this->sprite = sprite;
		}

		spt<Sprite>& GetSprite() {
			return sprite;
		}
	};


}// namespace