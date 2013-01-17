/*
 * xinputGamepad.cpp - part of xinputGamepad unit
 * copyright (c) 2011-2012, fluxrobotics
 */

#include "xinputGamepad.h"

const sf::Joystick::Axis XInput::mapAxesNums[] = {
   sf::Joystick::X,
   sf::Joystick::Y,
   sf::Joystick::Z,
   sf::Joystick::R,
   sf::Joystick::U,
   sf::Joystick::V,
   sf::Joystick::PovX,
   sf::Joystick::PovY,
};

int XInput::getRawAxisCount()
{
    const int NUM_AXES = (int)(sizeof(mapAxesNums)/sizeof(mapAxesNums[0]));
    return NUM_AXES;
}

bool XInput::initInput(int joystickId)
{
    // see docs. SFML provides only static functions for Joystick class
    // so no initialization for hardware is required

    m_joystickId = joystickId;

    //m_f_haveJoystick = sf::Joystick::isConnected(m_joystickId);
    m_f_haveJoystick = true;

    return true;
}

bool XInput::releaseInput()
{
    m_joystickId = 0;
    m_f_haveJoystick = false;

    // nothing here yet

    return true;
}

bool XInput::updateState()
{
    if (!m_f_haveJoystick)
        return false;

    sf::Joystick::update();


    m_horz  = this->getScaledAxis(sf::Joystick::X);
    m_vert  = this->getScaledAxis(sf::Joystick::Y);

    // there is uncertainty in Joystick axes, try one of these
    m_rZ  = this->getScaledAxis(sf::Joystick::R); // this shall probably work for Joystick model: Logitech Force3D Pro
    // m_rZ  = this->getScaledAxis(sf::Joystick::V); // possible code for emulator

    m_throttle    = this->getScaledAxis(sf::Joystick::Z);

    for (int i = 0; i < 32; i++)
    {
        m_joyKeyState[i] = sf::Joystick::isButtonPressed(m_joystickId,i);
    }

    return true;
}




