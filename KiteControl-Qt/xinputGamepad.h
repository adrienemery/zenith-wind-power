/*
 * xinputGamepad - extract of XInput class
 * copyright (c) 2011-2012, fluxrobotics
 */
#ifndef H_XINPUT_GAMEPAD_H
#define H_XINPUT_GAMEPAD_H

#include "SFML/Window.hpp"

class XInput
{
private:
    bool m_f_haveJoystick;
    bool m_joyKeyState[32];

    // axes - vertical, horizontal, rotation, throttle
    float m_vert, m_horz, m_rZ, m_throttle;

    int m_joystickId;

    float getScaledAxis(sf::Joystick::Axis axisName)
    {
        if (!m_f_haveJoystick)
            return 0.0f;

        // scaling value. This is done to limit output below absolute value 1.0f
        const float JOYSTICK_SCALE = 1.0f/100.000001f;
        return sf::Joystick::getAxisPosition(m_joystickId,axisName)*JOYSTICK_SCALE;
    }

    static const sf::Joystick::Axis mapAxesNums[];

public:
    bool initInput(int joystickId = 0);
    bool updateState();
    bool releaseInput();

    explicit XInput() : m_f_haveJoystick(false)
    {

    }

    virtual ~XInput()
    {
        releaseInput();
    }

    bool haveJoystick() { return m_f_haveJoystick; }

    bool isKeyPressed(int key)
    {
        if (key < 32) return m_joyKeyState[key];
        return false;
    }

    // Normalized outputs are all FLOAT in range [-1.0f to 1.0f]
    float getHorizontal()
    {
        return m_horz;
        ;
    }

    float getVertical()
    {
        return m_vert;

    }

    float getRotationZ()
    {
        return m_rZ;
    }

    float getThrottle()
    {
        return m_throttle;
    }

    int getRawAxisCount();

    float getRawAxis(int axis_id)
    {
        if (axis_id < getRawAxisCount())
            return this->getScaledAxis(mapAxesNums[axis_id]);
        else
            return 0.0f;
    }
};

#endif
