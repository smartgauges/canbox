***Settings***
Documentation     Tests for Steering Wheel Control functionality.
Resource          ../resources/settings.robot
Library           String

Suite Setup       Setup Test Environment
Test Teardown     Reset Renode Simulation
Suite Teardown    Teardown Test Environment

***Variables***
# 0x1B0 is used according with provided java files.
${CAN_ID_STEERING}   0x1B0  #  Replace with the *actual* CAN ID for your car. It could also be 0x126 or 0x217
${EXPECTED_WHEEL_KEY_INFO}   0x08 # Command for wheel key info in RZC_CMD_STEER_WHEEL_KEY_INFO
#Byte 0 can be 0x1 or 0x2, according with SteeringControlKeyInfo.java
${EXPECTED_VOLUME_UP}   01  #Replace with the value for volume up
${EXPECTED_VOLUME_DOWN}  02   # Replace with the value for volume down

***Test Cases***
# Volume Up
#     [Tags]  steering_wheel  negative
#     Log    Pressing volume up button...
#     Comment Send a command to press volume up button - Adapt to your specific setup.
#     Wait For Can Message    ${CAN_ID_STEERING}    timeout=5    data=${EXPECTED_WHEEL_KEY_INFO}${EXPECTED_VOLUME_UP}

# Volume Down
#     [Tags]  steering_wheel  positive
#     Log    Pressing volume down button...
# 	Comment Send a command to press volume down button - Adapt to your specific setup.
#     Wait For Can Message   ${CAN_ID_STEERING}    timeout=5    data=${EXPECTED_WHEEL_KEY_INFO}${EXPECTED_VOLUME_DOWN}

# Add more test cases for other steering wheel buttons (next/previous track, etc.)