#ifndef TENSOR_FIELD_EVENTHANDLERS_H
#define TENSOR_FIELD_EVENTHANDLERS_H

#include "pbge/pbge.h"
#include "DepthPeelingProcessor.h"
#include "PeelingAwareNode.h"

class CustomKeyboardEventHandler : public pbge::KeyboardEventHandler {
public:
    CustomKeyboardEventHandler(pbge::SceneGraph * graph, int cam_name, FieldParent * fieldParent) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
        this->field_parent = fieldParent;
    }

    bool keyDown(char key) {
        math3d::matrix44 m = cam_node->getTransformationMatrix();
        switch(key) {
            case 'W': cam_node->translate(0, 1.0f, 0); break;
            case 'S': cam_node->translate(0, -1.0f, 0); break;
            case 'A': cam_node->translate(-1.0f, 0, 0); break;
            case 'D': cam_node->translate(1.0f, 0, 0); break;
            case 'Q': cam_node->translate(0, 0, 1.0f); break;
            case 'E': cam_node->translate(0, 0, -1.0f); break;
            case 'Z': field_parent->setAlphaCorrection(field_parent->getAlphaCorrection() + 0.1f); break;
            case 'X': field_parent->setAlphaCorrection(field_parent->getAlphaCorrection() - 0.1f); break;
        }
        return true;
    }

    bool keyUp(char key) {
        return true;
    }
private:
    pbge::TransformationNode * cam_node;
    FieldParent * field_parent;
};

class EffectToggler : public pbge::KeyboardEventHandler {
public:
    EffectToggler(pbge::FramebufferImageProcessor * _inversor,
                  pbge::FramebufferImageProcessor * _redder,
                  pbge::FramebufferImageProcessor * _lens,
                  DepthPeelingProcessor * _depthPeeling) {
        inversor = _inversor;
        redder = _redder;
        lens = _lens;
        depthPeeling = _depthPeeling;
    }
    bool keyDown(char key) {
        return false;
    }
    bool keyUp(char key) {
        if(key == '1') {
            inversor->toggle();
            return true;
        } else if (key == '2') {
            redder->toggle();
            return true;
        } else if (key == '3') {
            lens->toggle();
            return true;
        } else if (key == '4') {
            depthPeeling->toggle();
            return true;
        }
        return false;
    }
private:
    pbge::FramebufferImageProcessor * inversor;
    pbge::FramebufferImageProcessor * redder;
    pbge::FramebufferImageProcessor * lens;
    DepthPeelingProcessor * depthPeeling;
};





class CustomMouseEventHandler : public pbge::MouseEventHandler {
public:
    CustomMouseEventHandler(pbge::SceneGraph * graph, int cam_name) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
		leftButtonDown = false;
        rightButtonDown = false;
        prev_x = 0;
        prev_y = 0;
		vertical_rotation = 0;
		horizontal_rotation = 0;
		degreeScale = 0.03f;
    }

    bool buttonDown(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = true;
            prev_x = x;
            prev_y = y;
        }
        else if(button == pbge::R_MOUSE_BUTTON) rightButtonDown = true;
		return true;
    }
    bool buttonUp(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = false;
            horizontal_rotation += (prev_x - x) * degreeScale;
			vertical_rotation += (prev_y - y) * degreeScale;
			prev_x = 0;
            prev_y = 0;
        }
        else if(button == pbge::R_MOUSE_BUTTON) rightButtonDown = false;
		return true;
    }
    bool move(int x, int y) {
        if(leftButtonDown) {
			math3d::matrix44 rotation = math3d::rotationMatrix(horizontal_rotation + (prev_x - x) * degreeScale, 0, 1.0f, 0);
            rotation = math3d::rotationMatrix(vertical_rotation + (prev_y - y) * degreeScale, 1.0f, 0, 0) * rotation;
            cam_node->setTransformationMatrix(rotation);
        }
		return true;
    }
private:
    bool leftButtonDown;
    bool rightButtonDown;
    int prev_x;
    int prev_y;
	float vertical_rotation;
	float horizontal_rotation;
	float degreeScale;
    pbge::TransformationNode * cam_node;
};


#endif