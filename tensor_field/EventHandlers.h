#ifndef TENSOR_FIELD_EVENTHANDLERS_H
#define TENSOR_FIELD_EVENTHANDLERS_H
#include <boost/smart_ptr/scoped_ptr.hpp>

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
            case 'Z': field_parent->stepMinForward(); break;
            case 'X': field_parent->stepMinBackward(); break;
            case 'V': field_parent->stepMaxForward(); break;
            case 'C': field_parent->stepMaxBackward(); break;
            case 'O': field_parent->scaleDown(); break;
            case 'P': field_parent->scaleUp(); break;
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
                  DepthPeelingProcessor * _depthPeeling,
                  FieldParent * _fieldParent) {
        inversor = _inversor;
        redder = _redder;
        lens = _lens;
        depthPeeling = _depthPeeling;
        fieldParent = _fieldParent;
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
            fieldParent->resetAlphaCorrection();
            return true;
        }
        return false;
    }
private:
    pbge::FramebufferImageProcessor * inversor;
    pbge::FramebufferImageProcessor * redder;
    pbge::FramebufferImageProcessor * lens;
    DepthPeelingProcessor * depthPeeling;
    FieldParent * fieldParent;
};





class CustomMouseEventHandler : public pbge::MouseEventHandler {
public:
    CustomMouseEventHandler(pbge::SceneGraph * graph, int cam_name): current_rotation(new math3d::matrix44(math3d::identity44)), rotation(new math3d::matrix44(math3d::identity44)) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
		leftButtonDown = false;
        prev_x = 0;
        prev_y = 0;
		degreeScale = 0.03f;
    }

    bool buttonDown(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = true;
            prev_x = x;
            prev_y = y;
        }
        return true;
    }
    bool buttonUp(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = false;
            *current_rotation *= *rotation;
        }
        return true;
    }
    bool move(int x, int y) {
        if(leftButtonDown) {
			*rotation = math3d::rotationMatrix((prev_x - x) * degreeScale, 0, 1.0f, 0);
            *rotation *= math3d::rotationMatrix((prev_y - y) * degreeScale, 1.0f, 0, 0);
            cam_node->setTransformationMatrix((*current_rotation) * (*rotation));
        }
		return true;
    }
private:
    bool leftButtonDown;
    int prev_x;
    int prev_y;
	float degreeScale;
    pbge::TransformationNode * cam_node;
    boost::scoped_ptr<math3d::matrix44> current_rotation;
    boost::scoped_ptr<math3d::matrix44> rotation;
};


#endif