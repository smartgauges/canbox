***Settings***
Documentation     Tests for Ignition (IGN) and Accessory (ACC) functionality.
Library           ../resources/CanboxKeywords.py
Resource          ../resources/settings.robot

Suite Setup       Setup Test Environment
Suite Teardown    Teardown Test Environment

***Variables***
${CAN_ID_IGNITION}    036  
${CAN_REPEAT_NUM}    5
${CAN_REPEAT_DELAY}    0.1
${CAN_MESSAGE_IGN_ON}    0E00062F010000A0
${CAN_MESSAGE_IGN_OFF}   00
${CAN_MESSAGE_ACC_ON}    0E00062F030000A0 
${EXPECTED_ACC_ON}    Acc:1
${EXPECTED_ACC_OFF}    Acc:0
${EXPECTED_IGN_ON}    Ign:1
${EXPECTED_IGN_OFF}    Ign:0
${RENODE_PID}

***Test Cases***
Ignition On
    [Tags]  ignition  positive
    Log    Turning ignition ON...

    # Send command or perform action to turn ignition ON.
    # This depends on your test setup - you might need to manually turn the key,
    # or you might have a way to simulate it (e.g., sending a CAN message).
    # For this example, I'll assume you have a way to turn the ignition ON
    # via a function called 'set_ignition_state'.  Adapt as needed.

    Comment    Set Ignition ON - Adapt this to your specific setup!
    Comment    For example, you might need to send a specific CAN message here.
    Comment    For now, we are simulating this with a comment.

    Send Can Message in Background   ${CAN_ID_IGNITION}     data=${CAN_MESSAGE_IGN_ON}  repeat=${CAN_REPEAT_NUM}  delay=${CAN_REPEAT_DELAY}
    
    Wait For Serial Regex   ${EXPECTED_ACC_ON} ${EXPECTED_IGN_ON}  timeout=2

Ignition Off
    [Tags]  ignition  positive
    Log    Turning ignition OFF...

    Comment   Set Ignition OFF - Adapt this to your specific setup!

    Wait For Serial Regex   ${EXPECTED_ACC_OFF} ${EXPECTED_IGN_OFF}   timeout=2

Accessory On
    [Tags]  accessory  positive
    Log    Turning accessory ON...

    Comment  Set Accessory ON (Ignition OFF) - Adapt this to your specific setup!

    Send Can Message in Background   ${CAN_ID_IGNITION}     data=${CAN_MESSAGE_ACC_ON}  repeat=${CAN_REPEAT_NUM}  delay=${CAN_REPEAT_DELAY}
    
    Wait For Serial Regex   ${EXPECTED_ACC_ON} ${EXPECTED_IGN_OFF}  timeout=10

Accessory Off
    [Tags]  accessory  positive
    Log    Turning accessory OFF...

    Comment  Set Accessory OFF - Adapt this to your specific setup!

    Wait For Serial Regex    ${EXPECTED_ACC_OFF} ${EXPECTED_IGN_OFF}  timeout=2

