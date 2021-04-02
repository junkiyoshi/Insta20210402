#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->cap.open("D:\\MP4\\Pexels Videos 1739010.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
		return;
	}

	cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
	cv::resize(src, this->frame, this->cap_size);
	cv::flip(this->frame, this->frame, 0);

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofTranslate(-640, -360);

	vector<ofMesh> mesh_list;
	for (int x = 0; x < 1280; x += 40) {

		for (int y = 0; y < 720; y += 40) {

			auto noise_value = ofNoise(x * 0.005, y * 0.001 + ofGetFrameNum() * 0.005);

			ofMesh mesh_1, mesh_2;

			mesh_1.addVertex(glm::vec3(x, y, 0));
			mesh_1.addVertex(glm::vec3(x + 40, y, 0));
			mesh_1.addVertex(glm::vec3(x + 40, y + 40, 0));

			mesh_1.addTexCoord(glm::vec3(x, y, 0));
			mesh_1.addTexCoord(glm::vec3(x + 40, y, 0));
			mesh_1.addTexCoord(glm::vec3(x + 40, y + 40, 0));

			mesh_2.addVertex(glm::vec3(x, y + 40, 0));
			mesh_2.addVertex(glm::vec3(x + 40, y + 40, 0));
			mesh_2.addVertex(glm::vec3(x, y, 0));

			mesh_2.addTexCoord(glm::vec3(x, y + 40, 0));
			mesh_2.addTexCoord(glm::vec3(x + 40, y + 40, 0));
			mesh_2.addTexCoord(glm::vec3(x, y, 0));

			mesh_list.push_back(mesh_1);
			mesh_list.push_back(mesh_2);
		}
	}

	for (auto& mesh : mesh_list) {

		auto rotate = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));
		auto speed = glm::vec3(ofRandom(5, 10), ofRandom(5, 10), ofRandom(5, 10));

		auto avg = (mesh.getVertex(0) + mesh.getVertex(1) + mesh.getVertex(2)) / 3;
		mesh.setVertex(0, mesh.getVertex(0) - avg);
		mesh.setVertex(1, mesh.getVertex(1) - avg);
		mesh.setVertex(2, mesh.getVertex(2) - avg);

		auto noise_value = ofNoise(glm::vec4(avg * 0.001, ofGetFrameNum() * 0.005));
		if (noise_value < 0.48) {
						
			avg += glm::vec3(0, 0, -250);
			rotate = glm::vec3();
		}
		else if (noise_value < 0.52) {

			avg += glm::vec3(0, 0, ofMap(noise_value, 0.48, 0.52, -250, 250));
			rotate.x += ofGetFrameNum() * speed.x;
			rotate.y += ofGetFrameNum() * speed.y;
			rotate.z += ofGetFrameNum() * speed.z;
		}
		else {

			avg += glm::vec3(0, 0, 250);
			rotate = glm::vec3();
		}

		ofPushMatrix();
		ofTranslate(avg);
		ofRotateZ(rotate.z);
		ofRotateY(rotate.y);
		ofRotateX(rotate.x);

		ofSetColor(255);
		this->image.bind();
		mesh.draw();
		this->image.unbind();

		if (glm::length(rotate) > 0) {
		
			ofSetColor(39);
			mesh.drawWireframe();
		}

		ofPopMatrix();
	}

	ofNoFill();
	ofSetColor(39);
	ofDrawRectangle(glm::vec3(0, 0, -250), 1280, 720);
	ofDrawRectangle(glm::vec3(0, 0, 250), 1280, 720);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}