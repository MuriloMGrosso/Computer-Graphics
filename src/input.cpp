#include "input.h"
#include <GL/glut.h>

void input::mouseManager(int button, int state, int x, int y) {

}

void input::keyboardDown(unsigned char key, int x, int y) {
    pressedKeys[(int)key] = true;
}

void input::keyboardUp(unsigned char key, int x, int y) {
    pressedKeys[(int)key] = false;
}

void input::specialKeysDown(int key, int x, int y) {
    pressedSpecialKeys[key] = true;
}

void input::specialKeysUp(int key, int x, int y) {
    pressedSpecialKeys[key] = false;
}

bool input::isKeyPressed(unsigned char key) {
    return pressedKeys[(int)key];
}

bool input::isSpecialKeyPressed(int key) {
    return pressedSpecialKeys[key];
}

bool input::isUpPressed() {
    return 
        input::isKeyPressed('w') ||
        input::isKeyPressed('W') ||
        input::isSpecialKeyPressed(GLUT_KEY_UP);
}

bool input::isDownPressed() {
    return 
        input::isKeyPressed('s') || 
        input::isKeyPressed('S') ||
        input::isSpecialKeyPressed(GLUT_KEY_DOWN);
}

bool input::isLeftPressed() {
    return 
        input::isKeyPressed('a') || 
        input::isKeyPressed('A') ||
        input::isSpecialKeyPressed(GLUT_KEY_LEFT);
}

bool input::isRightPressed() {
    return 
        input::isKeyPressed('d') || 
        input::isKeyPressed('D') ||
        input::isSpecialKeyPressed(GLUT_KEY_RIGHT);
}

short input::getHorizontalAxis() {
    if(input::isLeftPressed() && !input::isRightPressed())
        return -1;
    if(!input::isLeftPressed() && input::isRightPressed())
        return 1;
    return 0;
}

short input::getVerticalAxis() {
    if(input::isDownPressed() && !input::isUpPressed())
        return -1;
    if(!input::isDownPressed() && input::isUpPressed())
        return 1;
    return 0;
}