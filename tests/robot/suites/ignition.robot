***Settings***
Documentation     Tests for Ignition (IGN) and Accessory (ACC) functionality
...               by verifying the RZC Vehicle Status (0x38) serial output HEX. # Updated doc
Resource          ../resources/settings.robot
Library           String

Suite Setup       Setup Test Environment
Test Teardown     Reset Renode Simulation
Suite Teardown    Teardown Test Environment

***Variables***
# --- CAN Triggers (Based on peugeot_407_ms_036_ign_light_handler) ---
${CAN_ID_IGNITION}          036
# CAN Data for 0x036: Byte 4, Bits 0-1 define state
${CAN_DATA_IGN_ON}          0000000001000000  # State 0x01
${CAN_DATA_ACC_ON}          0000000003000000  # State 0x03
${CAN_DATA_OFF}             0000000000000000  # State 0x00

# --- RZC Protocol Verification ---
${RZC_HEADER}               FD
${RZC_TYPE_STATUS}          38
${SERIAL_TIMEOUT}           3  #s

# --- Expected Serial Strings (Calculated Above) ---
${RZC_STATUS_ALL_OFF}       FD07380000000000003F
${RZC_STATUS_ACC_IGN_ON}    FD073800080000000047  # Assumes Park Assist becomes available

# --- Timing ---
${CAN_REPEAT_NUM}           3
${CAN_REPEAT_DELAY}         0.05

***Keywords***
Send CAN And Verify Serial Hex Status 
    [Arguments]    ${can_data}    ${expected_serial_hex}    ${timeout}=${SERIAL_TIMEOUT}
    Clear Serial Log
    Send Can Message in Background      ${CAN_ID_IGNITION}    data=${can_data}    repeat=${CAN_REPEAT_NUM}    delay=${CAN_REPEAT_DELAY}
    Sleep    0.3s    #Keep a small delay
    # Use the new keyword
    Wait For Serial Hex String    ${expected_serial_hex}    timeout=${timeout}

***Test Cases***
Ignition ON Sends Vehicle Status Frame HEX
    [Tags]  ignition acc serial hex positive
    Log    Sending CAN for IGN ON, verifying RZC Status Frame (${RZC_TYPE_STATUS}) HEX...
    Send CAN And Verify Serial Hex Status    ${CAN_DATA_IGN_ON}    ${RZC_STATUS_ACC_IGN_ON}

Ignition OFF Sends Vehicle Status Frame HEX (All OFF)
    [Tags]  ignition acc serial hex positive
    Log    Sending CAN for IGN OFF, verifying RZC Status Frame (${RZC_TYPE_STATUS}) shows OFF state HEX...
    Send CAN And Verify Serial Hex Status    ${CAN_DATA_OFF}    ${RZC_STATUS_ALL_OFF} 

Accessory ON Sends Vehicle Status Frame HEX
    [Tags]  accessory acc serial hex positive
    Log    Sending CAN for ACC ON, verifying RZC Status Frame (${RZC_TYPE_STATUS}) HEX...
    Send CAN And Verify Serial Hex Status    ${CAN_DATA_ACC_ON}    ${RZC_STATUS_ACC_IGN_ON}

Accessory OFF Sends Vehicle Status Frame HEX (All OFF)
    [Tags]  accessory acc serial hex positive
    Log    Sending CAN for OFF state (implies ACC OFF), verifying RZC Status Frame (${RZC_TYPE_STATUS}) shows OFF state HEX...
    Send CAN And Verify Serial Hex Status    ${CAN_DATA_OFF}    ${RZC_STATUS_ALL_OFF} 