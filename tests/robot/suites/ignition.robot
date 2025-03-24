***Settings***
Documentation     Tests for Ignition (IGN) and Accessory (ACC) functionality.
Library           ../resources/CanboxKeywords.py
Resource          ../resources/settings.robot

Suite Setup       Setup Test Environment
Suite Teardown    Teardown Test Environment

***Variables***
${CAN_ID_IGNITION}    0B6  #  Replace with the *actual* CAN ID for your car
${EXPECTED_IGN_ON}    06    # Example - Replace with expected value
${EXPECTED_IGN_OFF}   00    # Example - Replace with expected value
${EXPECTED_ACC_ON}    02     # Example - replace with expected value.
${EXPECTED_ACC_OFF}    00     # Example - replace with expected value.
${BYTE_IGNITION}      0    # Byte 0 in the CAN message, as per your YAML
${MASK_IGNITION}   07   # mask b00000111
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

    Wait For Can Message    ${CAN_ID_IGNITION}    timeout=30    mask=${MASK_IGNITION}    data=${EXPECTED_IGN_ON}
    # Add checks for other expected CAN messages if necessary.

Ignition Off
    [Tags]  ignition  positive
    Log    Turning ignition OFF...

    Comment   Set Ignition OFF - Adapt this to your specific setup!

    Wait For Can Message    ${CAN_ID_IGNITION}    timeout=30    mask=${MASK_IGNITION}    data=${EXPECTED_IGN_OFF}

Accessory On
    [Tags]  accessory  positive
    Log    Turning accessory ON...

    Comment  Set Accessory ON (Ignition OFF) - Adapt this to your specific setup!

    Wait For Can Message    ${CAN_ID_IGNITION}    timeout=30    mask=${MASK_IGNITION}    data=${EXPECTED_ACC_ON}

Accessory Off
    [Tags]  accessory  positive
    Log    Turning accessory OFF...

    Comment  Set Accessory OFF - Adapt this to your specific setup!

    Wait For Can Message    ${CAN_ID_IGNITION}    timeout=30     mask=${MASK_IGNITION}    data=${EXPECTED_ACC_OFF}

