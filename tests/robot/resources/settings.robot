***Settings***
Documentation     Global settings and resources for canbox Robot Framework tests.

Library           OperatingSystem  # For interacting with the OS (e.g., running Renode)
Library           Process          # For managing processes (Renode, potentially cansend)
Library           String
Library           Collections
Library           CanboxKeywords.py  # Custom keywords

***Variables***
${RENODE_CLI}         renode  # Or the full path to your renode executable
${RENODE_SCRIPT}      volvo_od2/volvo_od2.resc  #  Path to your Renode script.
${CAN_INTERFACE}     vcan0
${SERIAL_PTY}        /tmp/uart    # The PTY created by Renode
${LOG_DIR}           logs  # Keep logs separate
${CANBOX_LOG_FILE}    ${LOG_DIR}/canbox_renode.log
${ROBOT_LOG_DIR}      ${LOG_DIR}/robot
${ROBOT_LOG}      	 ${ROBOT_LOG_DIR}/log.html
${ROBOT_REPORT}      ${ROBOT_LOG_DIR}/report.html
${ROBOT_OUTPUT}      ${ROBOT_LOG_DIR}/output.xml
${RENODE_PID}

***Keywords***
Start Renode
    # check for any running renode processes
    ${process_result} =    Run Process    pgrep    renode
    Run Keyword If  '${process_result.rc}' == '0'    Fail    Renode instance already running.
    
    # Create log directory if it doesn't exist
    Create Directory    ${LOG_DIR}
    Create Directory    ${ROBOT_LOG_DIR}
    
    # Start Renode in the background.
    ${process_result} =  Start Process   ${RENODE_CLI}   -P 3456   ${RENODE_SCRIPT}   shell=True    cwd=${CURDIR}/../../../
    ${RENODE_PID} =  Set Variable  ${process_result}
    Set Suite Variable  ${RENODE_PID} 
    Log    Renode started with PID: ${RENODE_PID.pid}


Stop Renode
     Log    RENODE_PID in Teardown: ${RENODE_PID}
     Terminate Process    ${RENODE_PID}    kill=True

Execute
    [Arguments]    ${command}
    ${output} =    Run And Return RC And Output    ${command}
    Log    ${output[1]}
    Should Be Equal As Integers    ${output[0]}    0


Setup Test Environment
    Setup CAN Bus
    Start Renode
    Sleep  10
    Setup Serial

Teardown Test Environment
    Close Serial
    Stop Renode
    Close Can
    

Reset Renode Simulation
    [Documentation]    Sends 'sysbus Reset' to the Renode monitor to reset the emulation.
    Log    Resetting Renode simulation via monitor command 'sysbus Reset'...
    Execute Monitor Command    sysbus Reset    # Execute the reset command
    Sleep    0.1s    # Short pause to allow the emulated firmware to start rebooting
