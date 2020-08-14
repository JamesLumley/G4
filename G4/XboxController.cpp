#include "XboxController.h"

XboxController::XboxController(int playerNumber)
{
    //Sets the Controller Number
    _controllerNum = playerNumber - 1;
}

XINPUT_STATE XboxController::getState()
{
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    //Gets the state
    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}

bool XboxController::isConnected()
{
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    //Gets the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XboxController::vibrate(int leftVal, int rightVal)
{
    //Creates a Vibraton State
    XINPUT_VIBRATION Vibration;
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    //Sets the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    //Vibrates the controller
    XInputSetState(_controllerNum, &Vibration);
}