***Settings***
Documentation     Tests for the RZC PSA (0xFD Header) Serial Protocol Output.
Resource          ../resources/settings.robot
Library           String

Suite Setup       Setup Test Environment
Test Teardown     Reset Renode Simulation
Suite Teardown    Teardown Test Environment

***Variables***
# --- CAN IDs from peugeot_407.c ---
${CAN_ID_SWC}               0x21F  # Steering Wheel Controls
${CAN_ID_PARKTRONIC}        0x0E1  # Parking Sensors
${CAN_ID_TEMP_VOLT}         0x168  # Outside Temp / Battery Voltage
${CAN_ID_DOORS}             0x131  # Doors Status
${CAN_ID_LIGHTS}            0x128  # Light Status / Park Brake indicator
${CAN_ID_REVERSE}           0x0F6  # Reverse Gear Status
${CAN_ID_TRIP_INST}         0x221  # Trip Computer - Instantaneous
${CAN_ID_TRIP_1}            0x2A1  # Trip Computer - Page 1 (From B7, check 407!)
${CAN_ID_TRIP_2}            0x261  # Trip Computer - Page 2 (From B7, check 407!)
${CAN_ID_WHEEL}             ???    # TODO: Determine actual 407 Wheel Angle CAN ID

# --- RZC Protocol ---
${RZC_HEADER}               FD
${SERIAL_TIMEOUT}           3
${CAN_REPEAT}               3
${CAN_DELAY}                0.1

# --- RZC DataTypes ---
${RZC_TYPE_BUTTON}          02
${RZC_TYPE_WHEEL}           29
${RZC_TYPE_RADAR_REV}       32
${RZC_TYPE_TEMP}            36
${RZC_TYPE_STATUS}          38
${RZC_TYPE_TRIP0}           33
${RZC_TYPE_TRIP1}           34
${RZC_TYPE_TRIP2}           35

***Test Cases***

Test RZC Button Press - Volume Up
    [Tags]    rzc protocol button swc positive
    Clear Serial Log
    ${can_data_vol_up}=    Set Variable    0000080000000000
    Send CAN Message    ${CAN_ID_SWC}    ${can_data_vol_up}

    ${rzc_len}=     Set Variable    04
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_BUTTON}
    ${rzc_data}=    Set Variable    140100
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial_press}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    ${rzc_data_release}=    Set Variable    140000
    ${rzc_cs_release}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data_release}
    ${expected_serial_release}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data_release}${rzc_cs_release}

    # Use the new keyword - ignore_case=True is good for hex
     Wait For Serial Hex String    ${expected_serial_press}    timeout=${SERIAL_TIMEOUT}
     Wait For Serial Hex String    ${expected_serial_release}    timeout=${SERIAL_TIMEOUT}

Test RZC Button Press - Volume Down
    [Tags]    rzc protocol button swc positive
    Clear Serial Log
    ${can_data_vol_down}=    Set Variable    0000040000000000
    Send CAN Message    ${CAN_ID_SWC}    ${can_data_vol_down}

    ${rzc_len}=     Set Variable    04
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_BUTTON}
    ${rzc_data}=    Set Variable    150100
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial_press}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    ${rzc_data_release}=    Set Variable    150000
    ${rzc_cs_release}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data_release}
    ${expected_serial_release}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data_release}${rzc_cs_release}

     Wait For Serial Hex String    ${expected_serial_press}    timeout=${SERIAL_TIMEOUT}
     Wait For Serial Hex String    ${expected_serial_release}    timeout=${SERIAL_TIMEOUT}

Test RZC Button Press - Seek Next
    [Tags]    rzc protocol button swc positive
    Clear Serial Log
    ${can_data_next}=    Set Variable    0000000000000080
    Send CAN Message    ${CAN_ID_SWC}    ${can_data_next}

    ${rzc_len}=     Set Variable    04
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_BUTTON}
    ${rzc_data}=    Set Variable    120100
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial_press}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    ${rzc_data_release}=    Set Variable    120000
    ${rzc_cs_release}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data_release}
    ${expected_serial_release}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data_release}${rzc_cs_release}

     Wait For Serial Hex String    ${expected_serial_press}    timeout=${SERIAL_TIMEOUT}
     Wait For Serial Hex String    ${expected_serial_release}    timeout=${SERIAL_TIMEOUT}

Test RZC Button Press - Seek Previous
    [Tags]    rzc protocol button swc positive
    Clear Serial Log
    ${can_data_prev}=    Set Variable    0000000000000040
    Send CAN Message    ${CAN_ID_SWC}    ${can_data_prev}

    ${rzc_len}=     Set Variable    04
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_BUTTON}
    ${rzc_data}=    Set Variable    130100
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial_press}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    ${rzc_data_release}=    Set Variable    130000
    ${rzc_cs_release}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data_release}
    ${expected_serial_release}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data_release}${rzc_cs_release}

     Wait For Serial Hex String    ${expected_serial_press}    timeout=${SERIAL_TIMEOUT}
     Wait For Serial Hex String    ${expected_serial_release}    timeout=${SERIAL_TIMEOUT}

Test RZC Vehicle Status - Doors Closed, Lights Off, Brake Off
    [Tags]    rzc protocol status positive
    Clear Serial Log
    Send CAN Message    ${CAN_ID_DOORS}    0000000000000000
    Send CAN Message    ${CAN_ID_LIGHTS}   0000000000000000
    Send CAN Message    ${CAN_ID_REVERSE}  0000000000000000

    ${rzc_len}=     Set Variable    07
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_STATUS}
    ${rzc_data}=    Set Variable    000000000000
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

Test RZC Vehicle Status - FL Door Open, Park Lights On, Park Brake On
    [Tags]    rzc protocol status positive
    Clear Serial Log
    Send CAN Message    ${CAN_ID_DOORS}    0080000000000000
    Send CAN Message    ${CAN_ID_LIGHTS}   0000000000800040
    Send CAN Message    ${CAN_ID_REVERSE}  0000000000000000

    ${rzc_len}=     Set Variable    07
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_STATUS}
    ${rzc_data}=    Set Variable    800000030000
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

Test RZC Temperature - Positive
    [Tags]    rzc protocol temperature positive
    Clear Serial Log
    ${temp_c}=      Set Variable    25
    ${temp_raw_can_int}=    Evaluate    int((${temp_c} + 40.0) * 2.0)
    ${temp_raw_can_hex}=    Convert To Hex    ${temp_raw_can_int}    length=2    uppercase=True
    ${can_data}=    Set Variable    ${temp_raw_can_hex}00000000000000
    Send CAN Message    ${CAN_ID_TEMP_VOLT}    ${can_data}

    ${rzc_len}=     Set Variable    02
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_TEMP}
    ${rzc_data}=    Set Variable    19
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

Test RZC Temperature - Negative
    [Tags]    rzc protocol temperature positive
    Clear Serial Log
    ${temp_c}=      Set Variable    -5
    ${temp_raw_can_int}=    Evaluate    int((${temp_c} + 40.0) * 2.0)
    ${temp_raw_can_hex}=    Convert To Hex    ${temp_raw_can_int}    length=2    uppercase=True
    ${can_data}=    Set Variable    ${temp_raw_can_hex}00000000000000
    Send CAN Message    ${CAN_ID_TEMP_VOLT}    ${can_data}

    ${rzc_len}=     Set Variable    02
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_TEMP}
    ${rzc_data}=    Set Variable    85
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

Test RZC Radar - Rear Active, Some Detections
    [Tags]    rzc protocol radar positive
    Clear Serial Log
    ${can_data_rear_active}=    Set Variable    00400CD3FFFFAA01  # Placeholder - Verify B4/Front encoding if needed
    Send CAN Message    ${CAN_ID_PARKTRONIC}    ${can_data_rear_active}

    ${rzc_len}=     Set Variable    0A
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_RADAR_REV}
    ${rzc_data}=    Set Variable    02000204050505
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

Test RZC Radar - System Off
    [Tags]    rzc protocol radar positive
    Clear Serial Log
    ${can_data_inactive}=    Set Variable    0000000000000000
    Send CAN Message    ${CAN_ID_PARKTRONIC}    ${can_data_inactive}

    ${rzc_len}=     Set Variable    0A
    ${rzc_dtype}=   Set Variable    ${RZC_TYPE_RADAR_REV}
    ${rzc_data}=    Set Variable    03050505050505
    ${rzc_cs}=      Calculate RZC Checksum    ${rzc_len}    ${rzc_dtype}    ${rzc_data}
    ${expected_serial}=    Set Variable    ${RZC_HEADER}${rzc_len}${rzc_dtype}${rzc_data}${rzc_cs}

    Sleep    0.5s
     Wait For Serial Hex String    ${expected_serial}    timeout=${SERIAL_TIMEOUT}

# --- TODO: Add Tests for Trip Computer Pages (0x33, 0x34, 0x35) ---
# --- TODO: Add Test for Wheel Angle (0x29) ---
# --- TODO: Add Test for AC Info (0x21) ---