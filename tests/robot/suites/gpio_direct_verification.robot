***Settings***
Documentation     Tests verifying GPIO output states directly via Renode monitor.
Resource          ../resources/settings.robot

# Connect to Monitor at the start, disconnect at the end
Suite Setup       Setup Test Environment
Suite Teardown    Teardown Test Environment

***Variables***
# --- CAN IDs and Data (Verify these!) ---
${CAN_ID_IGNITION}          036
${DATA_IGN_ON}              0E00062F010000A0 
${DATA_ACC_ON}              0000000003000000 
${DATA_IGN_ACC_OFF}         0000000000000000
${DATA_ILLUM_ON}            0E00062F010000A0 
${DATA_ILLUM_OFF}           0000000001000000 

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
${GPIO_VERIFY_TIMEOUT}      2   # Shorter timeout might work
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