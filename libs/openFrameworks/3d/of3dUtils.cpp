#include "of3dUtils.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"


ofVboMesh & cachedAxesVbo(){
    static ofVboMesh * axis = new ofVboMesh(ofMesh::axis());
    return *axis;
}


void ofDrawAxis(float size) {
	if (ofGetGLProgrammableRenderer()){
		ofPushMatrix();
		ofScale(size, size, size);
		cachedAxesVbo().draw();
		ofPopMatrix();
	} else {
		ofPushStyle();
		ofSetLineWidth(3);
		
		// draw x axis
		ofSetColor(ofColor::red);
		ofDrawLine(0, 0, 0, size, 0, 0);
		
		// draw y axis
		ofSetColor(ofColor::green);
		ofDrawLine(0, 0, 0, 0, size, 0);
		
		// draw z axis
		ofSetColor(ofColor::blue);
		ofDrawLine(0, 0, 0, 0, 0, size);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofDrawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) {
	
	ofColor c(255,0,0);
	
	ofPushStyle();
	
	if (x) {
		c.setHsb(0.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofDrawGridPlane(stepSize, numberOfSteps, labels);
	}
	if (y) {
		c.setHsb(255.0f / 3.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 0, -1);
		ofDrawGridPlane(stepSize, numberOfSteps, labels);
		ofPopMatrix();
	}
	if (z) {
		c.setHsb(255.0f * 2.0f / 3.0f, 200.0f, 255.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 1, 0);
		ofDrawGridPlane(stepSize, numberOfSteps, labels);
		ofPopMatrix();
	}
	
	if (labels) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		float labelPos = stepSize * (numberOfSteps + 0.5);
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		ofDrawBitmapString("x", labelPos, 0, 0);
		ofDrawBitmapString("y", 0, labelPos, 0);
		ofDrawBitmapString("z", 0, 0, labelPos);
		ofPopStyle();
	}
	ofPopStyle();
}


//--------------------------------------------------------------
void ofDrawGridPlane(float stepSize, size_t numberOfSteps, bool labels) {
	float scale = stepSize * numberOfSteps;
	
	ofPushStyle();
	for (int iDimension=0; iDimension<2; iDimension++)
	{
		for (size_t i=0; i <= numberOfSteps; i++)
		{
			float yz = i * stepSize;
			
			if (i == numberOfSteps || i == 0)
				ofSetLineWidth(2);   // central axis or cap line
			else if ( i % 2 == 0){
				ofSetLineWidth(1.5); // major
			} else {
				ofSetLineWidth(1);   // minor
			}
			
			if (iDimension == 0 ) {
				ofDrawLine(0, yz, -scale, 0, yz, scale);
				if (yz !=0) ofDrawLine(0, -yz, -scale, 0, -yz, scale);
			} else {
				ofDrawLine(0, -scale, yz, 0, scale, yz);
				if (yz !=0) ofDrawLine(0, -scale, -yz, 0, scale, -yz);
			}
		}
	}
	ofPopStyle();
	
	if (labels) {
		//draw numbers on axes
		ofPushStyle();
		ofSetColor(255, 255, 255);
		
//		float accuracy = ceil(-log()/log(10.0f));
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

		ofDrawBitmapString(ofToString(0), 0, 0, 0);

		for (float i = 1; i <= numberOfSteps; i++)
		{
			float yz = i * stepSize;
			ofDrawBitmapString(ofToString(yz), 0, yz, 0);
			ofDrawBitmapString(ofToString(-yz), 0, -yz, 0);
			ofDrawBitmapString(ofToString(yz), 0, 0, yz);
			ofDrawBitmapString(ofToString(-yz), 0, 0, -yz);
		}
		ofPopStyle();
	}
	
}

//--------------------------------------------------------------
void ofDrawArrow(const ofVec3f& start, const ofVec3f& end, float headSize) {
	
	//draw line
	ofDrawLine(start, end);
	
	//draw cone
	ofMatrix4x4 mat;
	mat.makeRotationMatrix( ofVec3f(0,1,0), start - end );
	ofPushMatrix();
	ofTranslate(end);
	ofMultMatrix(mat.getPtr());
	ofTranslate(0, headSize*0.5 ,0);
    ofDrawCone(headSize, headSize*2.);
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofDrawRotationAxes(float radius, float stripWidth, int circleRes){
	
	ofMesh axisXMesh;
	axisXMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	ofMesh axisYMesh;
	axisYMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	ofMesh axisZMesh;
	axisZMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
	for (int j = 0; j<=circleRes; j++) {
		float x = cos(TWO_PI * j/circleRes);
		float y = sin(TWO_PI * j/circleRes);
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f(-stripWidth, x*radius, y*radius));
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f( stripWidth, x*radius, y*radius));
		
		axisYMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisYMesh.addVertex(ofVec3f(x*radius, -stripWidth, y*radius));
		axisYMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisYMesh.addVertex(ofVec3f(x*radius,  stripWidth, y*radius));
		
		axisZMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisZMesh.addVertex(ofVec3f(x*radius, y*radius, -stripWidth));
		axisZMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisZMesh.addVertex(ofVec3f(x*radius, y*radius,  stripWidth));
	}
	
	ofPushStyle();
	ofEnableDepthTest();
	axisXMesh.draw();
	axisYMesh.draw();
	axisZMesh.draw();
	ofDrawAxis(radius);
	ofPopStyle();
	
}

