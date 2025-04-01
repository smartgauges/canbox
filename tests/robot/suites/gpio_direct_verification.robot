***Settings***
Documentation     Tests verifying GPIO output states directly via Renode monitor.
Resource          ../resources/settings.robot

# Connect to Monitor at the start, disconnect at the end
Suite Setup       Setup Test Environment
Test Teardown     Reset Renode Simulation
Suite Teardown    Teardown Test Environment

***Variables***
# --- CAN IDs and Data (Verify these!) ---
${CAN_ID_IGNITION}          036
${DATA_IGN_ON}              0E00062F010000A0 
${DATA_ACC_ON}              0000000003000000 
${DATA_IGN_ACC_OFF}         0000000000000000
${DATA_ILLUM_ON}            0E00062F010000A0 
${DATA_ILLUM_OFF}           0000000001000000 

***Variables***
# ... (Keep existing CAN_ID_IGNITION, GPIO names, Pin numbers, Timing) ...

# CAN Data for 0x036 (Byte 4 = Ign State, Byte 3 = Light Enable/Brightness)
# State Bits (Byte 4): 00=OFF, 01=IGN_ON, 03=ACC_ON
# Light Bits (Byte 3): Bit 5 (0x20)=Enable, Bits 0-3 (0x0F)=Brightness

# SCENARIO 1: Everything OFF
${CAN_DATA_ALL_OFF}             0000000000000000  # B4=00, B3=00

# SCENARIO 2: ACC ON, Lights OFF
${CAN_DATA_ACC_ON_LIGHTS_OFF}   0000000003000000  # B4=03, B3=00

# SCENARIO 3: ACC ON, Lights ON (e.g., Brightness 10 = 0x0A)
${CAN_DATA_ACC_ON_LIGHTS_ON}    0000002A03000000  # B4=03, B3=2A (Enable Bit + Brightness A)

# SCENARIO 4: IGN ON, Lights OFF
${CAN_DATA_IGN_ON_LIGHTS_OFF}   0000000001000000  # B4=01, B3=00

# SCENARIO 5: IGN ON, Lights ON
${CAN_DATA_IGN_ON_LIGHTS_ON}    0000002A01000000  # B4=01, B3=2A

# SCENARIO 6: ACC/IGN OFF, Lights ON (e.g., Parking lights left on without key)
${CAN_DATA_IGN_OFF_LIGHTS_ON}   0000002A00000000  # B4=00, B3=2A

# --- Renode Peripheral Names and Pin Numbers ---
${GPIO_B}                   sysbus.gpioPortB
${GPIO_C}                   sysbus.gpioPortC
${PIN_NUM_REAR}             5   # PB5
${PIN_NUM_PARK}             8   # PB8
${PIN_NUM_ACC}              9   # PB9
${PIN_NUM_ILL}              13  # PC13

# --- Timing ---
${CAN_REPEAT_NUM}           3    # Fewer repeats might be needed
${CAN_REPEAT_DELAY}         0.05  # Shorter delay
${GPIO_VERIFY_TIMEOUT}      3   # Shorter timeout might work
${POLLING_INTERVAL}         0.05  # Check GPIO state every 50ms

***Keywords***
Send CAN And Verify Direct GPIO High
    [Arguments]    ${can_id}    ${can_data}    ${gpio_peripheral}    ${pin_number}    ${timeout}=${GPIO_VERIFY_TIMEOUT}
    Send Can Message in Background    ${can_id}    data=${can_data}    repeat=${CAN_REPEAT_NUM}    delay=${CAN_REPEAT_DELAY}
    GPIO Pin Should Be Directly    ${gpio_peripheral}    ${pin_number}    1    timeout=${timeout}   polling_interval=${POLLING_INTERVAL}

Send CAN And Verify Direct GPIO Low
    [Arguments]    ${can_id}    ${can_data}    ${gpio_peripheral}    ${pin_number}    ${timeout}=${GPIO_VERIFY_TIMEOUT}
    Send Can Message in Background    ${can_id}    data=${can_data}    repeat=${CAN_REPEAT_NUM}    delay=${CAN_REPEAT_DELAY}
    GPIO Pin Should Be Directly    ${gpio_peripheral}    ${pin_number}    0    timeout=${timeout}   polling_interval=${POLLING_INTERVAL}

***Test Cases***
Verify ACC Pin High Directly
    [Tags]    gpio  acc  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${DATA_ACC_ON}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify ACC Pin Low Directly
    [Tags]    gpio  acc  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${DATA_IGN_ACC_OFF}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify IGN ON Controls ACC Pin High Directly
    [Tags]    gpio  ign  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${DATA_IGN_ON}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify IGN OFF Clears ACC Pin Low Directly
    [Tags]    gpio  ign  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${DATA_IGN_ACC_OFF}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify ILL Pin High Directly
    [Tags]    gpio  illum  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${DATA_ILLUM_ON}    ${GPIO_C}    ${PIN_NUM_ILL}

Verify ILL Pin Low Directly
    [Tags]    gpio  illum  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${DATA_ILLUM_OFF}    ${GPIO_C}    ${PIN_NUM_ILL}

# --- ACC Pin Tests ---

Verify ACC Pin HIGH When ACC ON (Lights OFF)
    [Tags]    gpio  acc  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${CAN_DATA_ACC_ON_LIGHTS_OFF}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify ACC Pin HIGH When IGN ON (Lights OFF)
    [Tags]    gpio  acc  ign  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${CAN_DATA_IGN_ON_LIGHTS_OFF}    ${GPIO_B}    ${PIN_NUM_ACC}

Verify ACC Pin LOW When ALL OFF
    [Tags]    gpio  acc  ign  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${CAN_DATA_ALL_OFF}              ${GPIO_B}    ${PIN_NUM_ACC}

# --- ILL Pin Tests ---

Verify ILL Pin LOW When ALL OFF
    [Tags]    gpio  illum  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${CAN_DATA_ALL_OFF}              ${GPIO_C}    ${PIN_NUM_ILL}

Verify ILL Pin LOW When ACC ON But Lights OFF
    [Tags]    gpio  illum  direct
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${CAN_DATA_ACC_ON_LIGHTS_OFF}    ${GPIO_C}    ${PIN_NUM_ILL}

Verify ILL Pin HIGH When ACC ON And Lights ON
    [Tags]    gpio  illum  acc  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${CAN_DATA_ACC_ON_LIGHTS_ON}     ${GPIO_C}    ${PIN_NUM_ILL}

Verify ILL Pin HIGH When IGN ON And Lights ON
    [Tags]    gpio  illum  ign  direct
    Send CAN And Verify Direct GPIO High    ${CAN_ID_IGNITION}    ${CAN_DATA_IGN_ON_LIGHTS_ON}     ${GPIO_C}    ${PIN_NUM_ILL}

Verify ILL Pin LOW When IGN/ACC OFF Even If Lights ON
    [Tags]    gpio  illum  ign  acc  direct
    # This tests the '&& acc' part of the gpio_process logic for the ILL pin
    Send CAN And Verify Direct GPIO Low     ${CAN_ID_IGNITION}    ${CAN_DATA_IGN_OFF_LIGHTS_ON}    ${GPIO_C}    ${PIN_NUM_ILL}


# --- TODO: Add tests for REAR and PARK pins once CAN messages are known ---
# Verify REAR Pin High Directly
#    [Tags]    gpio  rear  direct  todo
#    ${CAN_ID_GEAR} = Set Variable  ???
#    ${DATA_REVERSE_ON} = Set Variable  ????????????????
#    Send CAN And Verify Direct GPIO High    ${CAN_ID_GEAR}    ${DATA_REVERSE_ON}    ${GPIO_B}    ${PIN_NUM_REAR}

# Verify REAR Pin Low Directly
#    [Tags]    gpio  rear  direct  todo
#    ${CAN_ID_GEAR} = Set Variable  ???
#    ${DATA_REVERSE_OFF} = Set Variable  ????????????????
#    Send CAN And Verify Direct GPIO Low     ${CAN_ID_GEAR}    ${DATA_REVERSE_OFF}    ${GPIO_B}    ${PIN_NUM_REAR}

# Verify PARK Pin Directly (State depends on your hw_gpio.c logic for PARK)
#    [Tags]    gpio  park  direct  todo
#    ${CAN_ID_PARK_BRAKE} = Set Variable  ???
#    ${DATA_PARK_ON} = Set Variable     ????????????????
#    ${DATA_PARK_OFF} = Set Variable    ????????????????
#    Send CAN And Verify Direct GPIO High/Low    ${CAN_ID_PARK_BRAKE} ${DATA_PARK_ON}  ${GPIO_B} ${PIN_NUM_PARK}
#    Send CAN And Verify Direct GPIO High/Low    ${CAN_ID_PARK_BRAKE} ${DATA_PARK_OFF} ${GPIO_B} ${PIN_NUM_PARK}