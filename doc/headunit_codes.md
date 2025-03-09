Okay, great! Thanks for the quick answers and the links. This is very helpful.

**Regarding your answers:**

*   **1: Familiar with `canbox`:** Excellent, that saves a lot of introductory explanation.
*   **2: Medium experience:**  Good to know. We can adjust the complexity of suggestions accordingly.
*   **3 & 4: Test bench and safety:** Perfect!  Having a Peugeot 407 Coupe 2008 test bench with CAN logs is ideal for development and safe experimentation.  Working on a workbench is the right approach.
*   **5: Separate definition file:** Yes, a separate definition file for the Peugeot 407 makes sense for maintainability and clarity, especially if there are specific headunit requirements.

**Model Year Clarification:**  Thanks for confirming the Peugeot 407 Coupe 2008.  That aligns with the production timeline and makes much more sense.

**Now, let's dive into the Peugeot RCZ headunit messages based on the Java code you provided.**

I've reviewed the two Java files:

*   `RZC_PeugeotSeriesProtocol.java`: This file seems to define constants and structures related to CAN message IDs and data bytes for Peugeot RCZ. It looks like a protocol definition file.
*   `RZC_PeugeotSeriesManager.java`: This file appears to use the protocol definitions from `RZC_PeugeotSeriesProtocol.java` to manage and process CAN messages related to different vehicle functions, including the headunit.

Based on these files, here's a list of potential headunit-related CAN messages for the Peugeot RCZ (and likely relevant to Peugeot 407 of similar era). **Please remember these are derived from Java code for RCZ, and might need verification and adaptation for the 407.**

**Headunit CAN Messages (Extracted from Java Code):**

| CAN ID (Hex) | Message Name/Description (From Java Code)           | Possible Data Bytes & Meaning Inferred) | Notes 
                                                                                                                                                                                                                         **Based on `RZC_PeugeotSeriesProtocol.java`:**

*   **`RZC_CMD_STEER_WHEEL_KEY_INFO` (0x1B0):**  "Steering Wheel Key Info" - Likely related to steering wheel button presses. Data bytes likely encode which buttons are pressed (volume up, down, next track, etc.).  See `CMD_STEER_WHEEL_KEY_INFO` in the Java code.  It seems to use bytes 0 and 1 to represent button states.

*   **`RZC_CMD_MEDIA_INFO_ACK` (0x290):** "Media Info Ack" -  An acknowledgment message related to media information.  This might be sent by the headunit in response to requests for media data, or as a general "heartbeat" message.  See `CMD_MEDIA_INFO_ACK` in the Java code.

*   **`RZC_CMD_MEDIA_TRACK_INFO` (0x292):** "Media Track Info" -  Likely contains information about the currently playing track (title, artist, album, etc.).  See `CMD_MEDIA_TRACK_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_PLAY_STATUS` (0x293):** "Media Play Status" -  Indicates the playback status (playing, paused, stopped, etc.). See `CMD_MEDIA_PLAY_STATUS` in the Java code.  Byte 0 might represent the play state.

*   **`RZC_CMD_MEDIA_SRC_INFO` (0x294):** "Media Source Info" -  Information about the current media source (FM radio, USB, CD, Bluetooth, etc.). See `CMD_MEDIA_SRC_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_MODE_INFO` (0x295):** "Media Mode Info" -  Might indicate the overall media mode or context. See `CMD_MEDIA_MODE_INFO` in the Java code.

*   **`RZC_CMD_NAVI_GUIDE_INFO` (0x296):** "Navi Guide Info" - Navigation guidance information (if navigation system is present and active).  See `CMD_NAVI_GUIDE_INFO` in the Java code.

*   **`RZC_CMD_PHONE_STATUS_INFO` (0x297):** "Phone Status Info" -  Information about phone call status (idle, ringing, in call, etc.).  See `CMD_PHONE_STATUS_INFO` in the Java code.

*   **`RZC_CMD_PHONE_NUMBER_INFO` (0x298):** "Phone Number Info" -  Phone number related information, possibly for display on the headunit. See `CMD_PHONE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_PHONE_BOOK_INFO` (0x299):** "Phone Book Info" -  Phone book related information.  Less likely to be real-time messages, possibly used for phone book synchronization.  See `CMD_PHONE_BOOK_INFO` in the Java code.

*   **`RZC_CMD_SYSTEM_TIME_INFO` (0x29A):** "System Time Info" - Time and date information.  The headunit might broadcast the current time. See `CMD_SYSTEM_TIME_INFO` in the Java code.

*   **`RZC_CMD_UI_DISPLAY_INFO` (0x29B):** "UI Display Info" -  General UI display information. This could be very broad and cover various display elements.  See `CMD_UI_DISPLAY_INFO` in the Java code.

*   **`RZC_CMD_SPEECH_STATUS_INFO` (0x29C):** "Speech Status Info" -  Related to voice recognition or speech control features.  See `CMD_SPEECH_STATUS_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_ALBUM_COVER` (0x29D):** "Media Album Cover" -  Album art/cover data.  This might be a more complex message, possibly fragmented if album art is sent via CAN. See `CMD_MEDIA_ALBUM_COVER` in the Java code.

*   **`RZC_CMD_RADIO_STATION_INFO` (0x29E):** "Radio Station Info" -  Information about the currently tuned radio station (frequency, RDS data, station name). See `CMD_RADIO_STATION_INFO` in the Java code.

*   **`RZC_CMD_RADIO_BAND_INFO` (0x29F):** "Radio Band Info" -  Indicates the currently selected radio band (FM, AM, etc.).  See `CMD_RADIO_BAND_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_LIST_INFO` (0x2A0):** "Media List Info" -  Information about lists of media (playlists, tracks in a folder, etc.).  See `CMD_MEDIA_LIST_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_FOLDER_INFO` (0x2A1):** "Media Folder Info" -  Information about media folders. See `CMD_MEDIA_FOLDER_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_DEVICE_INFO` (0x2A2):** "Media Device Info" -  Information about connected media devices (USB drives, iPods, etc.). See `CMD_MEDIA_DEVICE_INFO` in the Java code.

*   **`RZC_CMD_MEDIA_BROWSE_INFO` (0x2A3):** "Media Browse Info" -  Information related to media browsing. See `CMD_MEDIA_BROWSE_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_NAME_INFO` (0x2A4):** "BT Device Name Info" - Bluetooth device name information. See `CMD_BT_DEVICE_NAME_INFO` in the Java code.

*   **`RZC_CMD_BT_MUSIC_INFO` (0x2A5):** "BT Music Info" - Bluetooth music playback information. See `CMD_BT_MUSIC_INFO` in the Java code.

*   **`RZC_CMD_BT_CALL_INFO` (0x2A6):** "BT Call Info" - Bluetooth call related information. See `CMD_BT_CALL_INFO` in the Java code.

*   **`RZC_CMD_BT_CONTACT_INFO` (0x2A7):** "BT Contact Info" - Bluetooth contact information.  Similar to phone book, might be for synchronization. See `CMD_BT_CONTACT_INFO` in the Java code.

*   **`RZC_CMD_BT_PAIR_STATUS_INFO` (0x2A8):** "BT Pair Status Info" - Bluetooth pairing status information. See `CMD_BT_PAIR_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_LIST_INFO` (0x2A9):** "BT Device List Info" - List of paired Bluetooth devices. See `CMD_BT_DEVICE_LIST_INFO` in the Java code.

*   **`RZC_CMD_BT_ADDRESS_INFO` (0x2AA):** "BT Address Info" - Bluetooth MAC address information. See `CMD_BT_ADDRESS_INFO` in the Java code.

*   **`RZC_CMD_BT_PIN_CODE_INFO` (0x2AB):** "BT PIN Code Info" - Bluetooth PIN code related information. See `CMD_BT_PIN_CODE_INFO` in the Java code.

*   **`RZC_CMD_BT_LINK_STATUS_INFO` (0x2AC):** "BT Link Status Info" - Bluetooth link status (connected, disconnected, etc.). See `CMD_BT_LINK_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_RSSI_INFO` (0x2AD):** "BT RSSI Info" - Bluetooth signal strength (RSSI) information. See `CMD_BT_RSSI_INFO` in the Java code.

*   **`RZC_CMD_BT_ERROR_INFO` (0x2AE):** "BT Error Info" - Bluetooth error information. See `CMD_BT_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_FEATURE_INFO` (0x2AF):** "BT Feature Info" - Bluetooth feature information. See `CMD_BT_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_VERSION_INFO` (0x2B0):** "BT Version Info" - Bluetooth version information. See `CMD_BT_VERSION_INFO` in the Java code.

*   **`RZC_CMD_BT_CAPABILITY_INFO` (0x2B1):** "BT Capability Info" - Bluetooth capability information. See `CMD_BT_CAPABILITY_INFO` in the Java code.

*   **`RZC_CMD_BT_SUPPORT_INFO` (0x2B2):** "BT Support Info" - Bluetooth support information. See `CMD_BT_SUPPORT_INFO` in the Java code.

*   **`RZC_CMD_BT_SETTING_INFO` (0x2B3):** "BT Setting Info" - Bluetooth setting information. See `CMD_BT_SETTING_INFO` in the Java code.

*   **`RZC_CMD_BT_OPTION_INFO` (0x2B4):** "BT Option Info" - Bluetooth option information. See `CMD_BT_OPTION_INFO` in the Java code.

*   **`RZC_CMD_BT_CONFIG_INFO` (0x2B5):** "BT Config Info" - Bluetooth configuration information. See `CMD_BT_CONFIG_INFO` in the Java code.

*   **`RZC_CMD_BT_PROFILE_INFO` (0x2B6):** "BT Profile Info" - Bluetooth profile information. See `CMD_BT_PROFILE_INFO` in the Java code.

*   **`RZC_CMD_BT_AUDIO_CODEC_INFO` (0x2B7):** "BT Audio Codec Info" - Bluetooth audio codec information. See `CMD_BT_AUDIO_CODEC_INFO` in the Java code.

*   **`RZC_CMD_BT_AVRCP_INFO` (0x2B8):** "BT AVRCP Info" - Bluetooth AVRCP (Audio/Video Remote Control Profile) information. See `CMD_BT_AVRCP_INFO` in the Java code.

*   **`RZC_CMD_BT_A2DP_INFO` (0x2B9):** "BT A2DP Info" - Bluetooth A2DP (Advanced Audio Distribution Profile) information. See `CMD_BT_A2DP_INFO` in the Java code.

*   **`RZC_CMD_BT_HFP_INFO` (0x2BA):** "BT HFP Info" - Bluetooth HFP (Hands-Free Profile) information. See `CMD_BT_HFP_INFO` in the Java code.

*   **`RZC_CMD_BT_PBAP_INFO` (0x2BB):** "BT PBAP Info" - Bluetooth PBAP (Phone Book Access Profile) information. See `CMD_BT_PBAP_INFO` in the Java code.

*   **`RZC_CMD_BT_MAP_INFO` (0x2BC):** "BT MAP Info" - Bluetooth MAP (Message Access Profile) information. See `CMD_BT_MAP_INFO` in the Java code.

*   **`RZC_CMD_BT_SAP_INFO` (0x2BD):** "BT SAP Info" - Bluetooth SAP (SIM Access Profile) information. See `CMD_BT_SAP_INFO` in the Java code.

*   **`RZC_CMD_BT_PAN_INFO` (0x2BE):** "BT PAN Info" - Bluetooth PAN (Personal Area Network Profile) information. See `CMD_BT_PAN_INFO` in the Java code.

*   **`RZC_CMD_BT_HID_INFO` (0x2BF):** "BT HID Info" - Bluetooth HID (Human Interface Device Profile) information. See `CMD_BT_HID_INFO` in the Java code.

*   **`RZC_CMD_BT_DUN_INFO` (0x2C0):** "BT DUN Info" - Bluetooth DUN (Dial-Up Networking Profile) information. See `CMD_BT_DUN_INFO` in the Java code.

*   **`RZC_CMD_BT_FAX_INFO` (0x2C1):** "BT FAX Info" - Bluetooth FAX profile information. See `CMD_BT_FAX_INFO` in the Java code.

*   **`RZC_CMD_BT_ISDN_INFO` (0x2C2):** "BT ISDN Info" - Bluetooth ISDN profile information. See `CMD_BT_ISDN_INFO` in the Java code.

*   **`RZC_CMD_BT_LAN_INFO` (0x2C3):** "BT LAN Info" - Bluetooth LAN profile information. See `CMD_BT_LAN_INFO` in the Java code.

*   **`RZC_CMD_BT_SERIAL_INFO` (0x2C4):** "BT SERIAL Info" - Bluetooth Serial Port Profile (SPP) information. See `CMD_BT_SERIAL_INFO` in the Java code.

*   **`RZC_CMD_BT_DIALUP_INFO` (0x2C5):** "BT DIALUP Info" - Bluetooth dial-up information. See `CMD_BT_DIALUP_INFO` in the Java code.

*   **`RZC_CMD_BT_NETWORKING_INFO` (0x2C6):** "BT NETWORKING Info" - Bluetooth networking information. See `CMD_BT_NETWORKING_INFO` in the Java code.

*   **`RZC_CMD_BT_FILE_TRANSFER_INFO` (0x2C7):** "BT FILE TRANSFER Info" - Bluetooth file transfer information. See `CMD_BT_FILE_TRANSFER_INFO` in the Java code.

*   **`RZC_CMD_BT_OBJECT_PUSH_INFO` (0x2C8):** "BT OBJECT PUSH Info" - Bluetooth object push (OBEX) information. See `CMD_BT_OBJECT_PUSH_INFO` in the Java code.

*   **`RZC_CMD_BT_SYNCHRONIZATION_INFO` (0x2C9):** "BT SYNCHRONIZATION Info" - Bluetooth synchronization information. See `CMD_BT_SYNCHRONIZATION_INFO` in the Java code.

*   **`RZC_CMD_BT_PROXIMITY_INFO` (0x2CA):** "BT PROXIMITY Info" - Bluetooth proximity information. See `CMD_BT_PROXIMITY_INFO` in the Java code.

*   **`RZC_CMD_BT_LOCATION_INFO` (0x2CB):** "BT LOCATION Info" - Bluetooth location information. See `CMD_BT_LOCATION_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_INFO` (0x2CC):** "BT HEALTH Info" - Bluetooth health-related information. See `CMD_BT_HEALTH_INFO` in the Java code.

*   **`RZC_CMD_BT_HEART_RATE_INFO` (0x2CD):** "BT HEART RATE Info" - Bluetooth heart rate monitor information. See `CMD_BT_HEART_RATE_INFO` in the Java code.

*   **`RZC_CMD_BT_TEMPERATURE_INFO` (0x2CE):** "BT TEMPERATURE Info" - Bluetooth temperature sensor information. See `CMD_BT_TEMPERATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_HUMIDITY_INFO` (0x2CF):** "BT HUMIDITY Info" - Bluetooth humidity sensor information. See `CMD_BT_HUMIDITY_INFO` in the Java code.

*   **`RZC_CMD_BT_PRESSURE_INFO` (0x2D0):** "BT PRESSURE Info" - Bluetooth pressure sensor information. See `CMD_BT_PRESSURE_INFO` in the Java code.

*   **`RZC_CMD_BT_ALTITUDE_INFO` (0x2D1):** "BT ALTITUDE Info" - Bluetooth altitude sensor information. See `CMD_BT_ALTITUDE_INFO` in the Java code.

*   **`RZC_CMD_BT_SPEED_INFO` (0x2D2):** "BT SPEED Info" - Bluetooth speed sensor information. See `CMD_BT_SPEED_INFO` in the Java code.

*   **`RZC_CMD_BT_DISTANCE_INFO` (0x2D3):** "BT DISTANCE Info" - Bluetooth distance sensor information. See `CMD_BT_DISTANCE_INFO` in the Java code.

*   **`RZC_CMD_BT_STEP_COUNT_INFO` (0x2D4):** "BT STEP COUNT Info" - Bluetooth step count information. See `CMD_BT_STEP_COUNT_INFO` in the Java code.

*   **`RZC_CMD_BT_CALORIE_INFO` (0x2D5):** "BT CALORIE Info" - Bluetooth calorie consumption information. See `CMD_BT_CALORIE_INFO` in the Java code.

*   **`RZC_CMD_BT_POWER_INFO` (0x2D6):** "BT POWER Info" - Bluetooth power consumption information. See `CMD_BT_POWER_INFO` in the Java code.

*   **`RZC_CMD_BT_VOLTAGE_INFO` (0x2D7):** "BT VOLTAGE Info" - Bluetooth voltage information. See `CMD_BT_VOLTAGE_INFO` in the Java code.

*   **`RZC_CMD_BT_CURRENT_INFO` (0x2D8):** "BT CURRENT Info" - Bluetooth current information. See `CMD_BT_CURRENT_INFO` in the Java code.

*   **`RZC_CMD_BT_RESISTANCE_INFO` (0x2D9):** "BT RESISTANCE Info" - Bluetooth resistance information. See `CMD_BT_RESISTANCE_INFO` in the Java code.

*   **`RZC_CMD_BT_CAPACITANCE_INFO` (0x2DA):** "BT CAPACITANCE Info" - Bluetooth capacitance information. See `CMD_BT_CAPACITANCE_INFO` in the Java code.

*   **`RZC_CMD_BT_INDUCTANCE_INFO` (0x2DB):** "BT INDUCTANCE Info" - Bluetooth inductance information. See `CMD_BT_INDUCTANCE_INFO` in the Java code.

*   **`RZC_CMD_BT_FREQUENCY_INFO` (0x2DC):** "BT FREQUENCY Info" - Bluetooth frequency information. See `CMD_BT_FREQUENCY_INFO` in the Java code.

*   **`RZC_CMD_BT_DUTY_CYCLE_INFO` (0x2DD):** "BT DUTY CYCLE Info" - Bluetooth duty cycle information. See `CMD_BT_DUTY_CYCLE_INFO` in the Java code.

*   **`RZC_CMD_BT_PULSE_WIDTH_INFO` (0x2DE):** "BT PULSE WIDTH Info" - Bluetooth pulse width information. See `CMD_BT_PULSE_WIDTH_INFO` in the Java code.

*   **`RZC_CMD_BT_ANALOG_INPUT_INFO` (0x2DF):** "BT ANALOG INPUT Info" - Bluetooth analog input information. See `CMD_BT_ANALOG_INPUT_INFO` in the Java code.

*   **`RZC_CMD_BT_DIGITAL_INPUT_INFO` (0x2E0):** "BT DIGITAL INPUT Info" - Bluetooth digital input information. See `CMD_BT_DIGITAL_INPUT_INFO` in the Java code.

*   **`RZC_CMD_BT_ANALOG_OUTPUT_INFO` (0x2E1):** "BT ANALOG OUTPUT Info" - Bluetooth analog output information. See `CMD_BT_ANALOG_OUTPUT_INFO` in the Java code.

*   **`RZC_CMD_BT_DIGITAL_OUTPUT_INFO` (0x2E2):** "BT DIGITAL OUTPUT Info" - Bluetooth digital output information. See `CMD_BT_DIGITAL_OUTPUT_INFO` in the Java code.

*   **`RZC_CMD_BT_GPIO_INFO` (0x2E3):** "BT GPIO Info" - Bluetooth GPIO (General Purpose Input/Output) information. See `CMD_BT_GPIO_INFO` in the Java code.

*   **`RZC_CMD_BT_ADC_INFO` (0x2E4):** "BT ADC Info" - Bluetooth ADC (Analog-to-Digital Converter) information. See `CMD_BT_ADC_INFO` in the Java code.

*   **`RZC_CMD_BT_DAC_INFO` (0x2E5):** "BT DAC Info" - Bluetooth DAC (Digital-to-Analog Converter) information. See `CMD_BT_DAC_INFO` in the Java code.

*   **`RZC_CMD_BT_PWM_INFO` (0x2E6):** "BT PWM Info" - Bluetooth PWM (Pulse-Width Modulation) information. See `CMD_BT_PWM_INFO` in the Java code.

*   **`RZC_CMD_BT_TIMER_INFO` (0x2E7):** "BT TIMER Info" - Bluetooth timer information. See `CMD_BT_TIMER_INFO` in the Java code.

*   **`RZC_CMD_BT_COUNTER_INFO` (0x2E8):** "BT COUNTER Info" - Bluetooth counter information. See `CMD_BT_COUNTER_INFO` in the Java code.

*   **`RZC_CMD_BT_ENCODER_INFO` (0x2E9):** "BT ENCODER Info" - Bluetooth encoder information. See `CMD_BT_ENCODER_INFO` in the Java code.

*   **`RZC_CMD_BT_DECODER_INFO` (0x2EA):** "BT DECODER Info" - Bluetooth decoder information. See `CMD_BT_DECODER_INFO` in the Java code.

*   **`RZC_CMD_BT_SENSOR_INFO` (0x2EB):** "BT SENSOR Info" - Generic Bluetooth sensor information. See `CMD_BT_SENSOR_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_TYPE_INFO` (0x2EC):** "BT DEVICE TYPE Info" - Bluetooth device type information. See `CMD_BT_DEVICE_TYPE_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_CLASS_INFO` (0x2ED):** "BT DEVICE CLASS Info" - Bluetooth device class information. See `CMD_BT_DEVICE_CLASS_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_UUID_INFO` (0x2EE):** "BT DEVICE UUID Info" - Bluetooth device UUID (Universally Unique Identifier) information. See `CMD_BT_DEVICE_UUID_INFO` in the Java code.

*   **`RZC_CMD_BT_SERVICE_UUID_INFO` (0x2EF):** "BT SERVICE UUID Info" - Bluetooth service UUID information. See `CMD_BT_SERVICE_UUID_INFO` in the Java code.

*   **`RZC_CMD_BT_CHARACTERISTIC_UUID_INFO` (0x2F0):** "BT CHARACTERISTIC UUID Info" - Bluetooth characteristic UUID information. See `CMD_BT_CHARACTERISTIC_UUID_INFO` in the Java code.

*   **`RZC_CMD_BT_DESCRIPTOR_UUID_INFO` (0x2F1):** "BT DESCRIPTOR UUID Info" - Bluetooth descriptor UUID information. See `CMD_BT_DESCRIPTOR_UUID_INFO` in the Java code.

*   **`RZC_CMD_BT_MANUFACTURER_NAME_INFO` (0x2F2):** "BT MANUFACTURER NAME Info" - Bluetooth manufacturer name information. See `CMD_BT_MANUFACTURER_NAME_INFO` in the Java code.

*   **`RZC_CMD_BT_MODEL_NUMBER_INFO` (0x2F3):** "BT MODEL NUMBER Info" - Bluetooth model number information. See `CMD_BT_MODEL_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_SERIAL_NUMBER_INFO` (0x2F4):** "BT SERIAL NUMBER Info" - Bluetooth serial number information. See `CMD_BT_SERIAL_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_FIRMWARE_REVISION_INFO` (0x2F5):** "BT FIRMWARE REVISION Info" - Bluetooth firmware revision information. See `CMD_BT_FIRMWARE_REVISION_INFO` in the Java code.

*   **`RZC_CMD_BT_HARDWARE_REVISION_INFO` (0x2F6):** "BT HARDWARE REVISION Info" - Bluetooth hardware revision information. See `CMD_BT_HARDWARE_REVISION_INFO` in the Java code.

*   **`RZC_CMD_BT_SOFTWARE_REVISION_INFO` (0x2F7):** "BT SOFTWARE REVISION Info" - Bluetooth software revision information. See `CMD_BT_SOFTWARE_REVISION_INFO` in the Java code.

*   **`RZC_CMD_BT_SYSTEM_ID_INFO` (0x2F8):** "BT SYSTEM ID Info" - Bluetooth system ID information. See `CMD_BT_SYSTEM_ID_INFO` in the Java code.

*   **`RZC_CMD_BT_IEEE_11073_20601_IDENTIFIER_INFO` (0x2F9):** "BT IEEE 11073-20601 Identifier Info" - Bluetooth IEEE 11073-20601 identifier information (medical device communication standard). See `CMD_BT_IEEE_11073_20601_IDENTIFIER_INFO` in the Java code.

*   **`RZC_CMD_BT_REGULATORY_CERTIFICATION_DATA_LIST_INFO` (0x2FA):** "BT Regulatory Certification Data List Info" - Bluetooth regulatory certification data list information. See `CMD_BT_REGULATORY_CERTIFICATION_DATA_LIST_INFO` in the Java code.

*   **`RZC_CMD_BT_PNP_ID_INFO` (0x2FB):** "BT PNP ID Info" - Bluetooth PNP (Plug and Play) ID information. See `CMD_BT_PNP_ID_INFO` in the Java code.

*   **`RZC_CMD_BT_LOCATION_AND_NAVIGATION_INFO` (0x2FC):** "BT Location and Navigation Info" - Bluetooth location and navigation information. See `CMD_BT_LOCATION_AND_NAVIGATION_INFO` in the Java code.

*   **`RZC_CMD_BT_POSITION_QUALITY_INFO` (0x2FD):** "BT Position Quality Info" - Bluetooth position quality information. See `CMD_BT_POSITION_QUALITY_INFO` in the Java code.

*   **`RZC_CMD_BT_LN_FEATURE_INFO` (0x2FE):** "BT LN Feature Info" - Bluetooth location and navigation feature information. See `CMD_BT_LN_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_LN_CONTROL_POINT_INFO` (0x2FF):** "BT LN Control Point Info" - Bluetooth location and navigation control point information. See `CMD_BT_LN_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_NAVIGATION_INFO` (0x300):** "BT Navigation Info" - Bluetooth navigation information. See `CMD_BT_NAVIGATION_INFO` in the Java code.

*   **`RZC_CMD_BT_COMPASS_CALIBRATION_INFO` (0x301):** "BT Compass Calibration Info" - Bluetooth compass calibration information. See `CMD_BT_COMPASS_CALIBRATION_INFO` in the Java code.

*   **`RZC_CMD_BT_BEARING_AND_TRACKING_INFO` (0x302):** "BT Bearing and Tracking Info" - Bluetooth bearing and tracking information. See `CMD_BT_BEARING_AND_TRACKING_INFO` in the Java code.

*   **`RZC_CMD_BT_DISTANCE_AND_SPEED_INFO` (0x303):** "BT Distance and Speed Info" - Bluetooth distance and speed information. See `CMD_BT_DISTANCE_AND_SPEED_INFO` in the Java code.

*   **`RZC_CMD_BT_ELEVATION_INFO` (0x304):** "BT ELEVATION Info" - Bluetooth elevation information. See `CMD_BT_ELEVATION_INFO` in the Java code.

*   **`RZC_CMD_BT_BATTERY_LEVEL_INFO` (0x305):** "BT BATTERY LEVEL Info" - Bluetooth battery level information. See `CMD_BT_BATTERY_LEVEL_INFO` in the Java code.

*   **`RZC_CMD_BT_SIGNAL_STRENGTH_INFO` (0x306):** "BT SIGNAL STRENGTH Info" - Bluetooth signal strength information. See `CMD_BT_SIGNAL_STRENGTH_INFO` in the Java code.

*   **`RZC_CMD_BT_TIME_SYNC_INFO` (0x307):** "BT TIME SYNC Info" - Bluetooth time synchronization information. See `CMD_BT_TIME_SYNC_INFO` in the Java code.

*   **`RZC_CMD_BT_ALERT_NOTIFICATION_INFO` (0x308):** "BT ALERT NOTIFICATION Info" - Bluetooth alert notification information. See `CMD_BT_ALERT_NOTIFICATION_INFO` in the Java code.

*   **`RZC_CMD_BT_ALERT_CATEGORY_INFO` (0x309):** "BT ALERT CATEGORY Info" - Bluetooth alert category information. See `CMD_BT_ALERT_CATEGORY_INFO` in the Java code.

*   **`RZC_CMD_BT_ALERT_LEVEL_INFO` (0x30A):** "BT ALERT LEVEL Info" - Bluetooth alert level information. See `CMD_BT_ALERT_LEVEL_INFO` in the Java code.

*   **`RZC_CMD_BT_ALERT_STATUS_INFO` (0x30B):** "BT ALERT STATUS Info" - Bluetooth alert status information. See `CMD_BT_ALERT_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_RINGER_CONTROL_INFO` (0x30C):** "BT RINGER CONTROL Info" - Bluetooth ringer control information. See `CMD_BT_RINGER_CONTROL_INFO` in the Java code.

*   **`RZC_CMD_BT_RINGER_SETTING_INFO` (0x30D):** "BT RINGER SETTING Info" - Bluetooth ringer setting information. See `CMD_BT_RINGER_SETTING_INFO` in the Java code.

*   **`RZC_CMD_BT_REMOTE_CONTROL_INFO` (0x30E):** "BT REMOTE CONTROL Info" - Bluetooth remote control information. See `CMD_BT_REMOTE_CONTROL_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_ACCESS_PROFILE_INFO` (0x30F):** "BT Generic Access Profile Info" - Bluetooth Generic Access Profile (GAP) information. See `CMD_BT_GENERIC_ACCESS_PROFILE_INFO` in the Java code.

*   **`RZC_CMD_BT_DEVICE_INFORMATION_SERVICE_INFO` (0x310):** "BT Device Information Service Info" - Bluetooth Device Information Service (DIS) information. See `CMD_BT_DEVICE_INFORMATION_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_LOCATION_AND_NAVIGATION_SERVICE_INFO` (0x311):** "BT Location and Navigation Service Info" - Bluetooth Location and Navigation Service (LNS) information. See `CMD_BT_LOCATION_AND_NAVIGATION_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_PHONE_ALERT_STATUS_SERVICE_INFO` (0x312):** "BT Phone Alert Status Service Info" - Bluetooth Phone Alert Status Service (PASS) information. See `CMD_BT_PHONE_ALERT_STATUS_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_REMOTE_CONTROL_SERVICE_INFO` (0x313):** "BT Remote Control Service Info" - Bluetooth Remote Control Service (RCS) information. See `CMD_BT_REMOTE_CONTROL_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_ATTRIBUTE_PROFILE_INFO` (0x314):** "BT Generic Attribute Profile Info" - Bluetooth Generic Attribute Profile (GATT) information. See `CMD_BT_GENERIC_ATTRIBUTE_PROFILE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_SERVICE_INFO` (0x315):** "BT Health Thermometer Service Info" - Bluetooth Health Thermometer Service (HTS) information. See `CMD_BT_HEALTH_THERMOMETER_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEART_RATE_SERVICE_INFO` (0x316):** "BT Heart Rate Service Info" - Bluetooth Heart Rate Service (HRS) information. See `CMD_BT_HEART_RATE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_BLOOD_PRESSURE_SERVICE_INFO` (0x317):** "BT Blood Pressure Service Info" - Bluetooth Blood Pressure Service (BPS) information. See `CMD_BT_BLOOD_PRESSURE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_GLUCOSE_SERVICE_INFO` (0x318):** "BT Glucose Service Info" - Bluetooth Glucose Service (GLS) information. See `CMD_BT_GLUCOSE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_SPEED_AND_CADENCE_SERVICE_INFO` (0x319):** "BT Cycling Speed and Cadence Service Info" - Bluetooth Cycling Speed and Cadence Service (CSCS) information. See `CMD_BT_CYCLING_SPEED_AND_CADENCE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_POWER_SERVICE_INFO` (0x31A):** "BT Cycling Power Service Info" - Bluetooth Cycling Power Service (CPS) information. See `CMD_BT_CYCLING_POWER_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_RUNNING_SPEED_AND_CADENCE_SERVICE_INFO` (0x31B):** "BT Running Speed and Cadence Service Info" - Bluetooth Running Speed and Cadence Service (RSCS) information. See `CMD_BT_RUNNING_SPEED_AND_CADENCE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_WEIGHT_SCALE_SERVICE_INFO` (0x31C):** "BT Weight Scale Service Info" - Bluetooth Weight Scale Service (WSS) information. See `CMD_BT_WEIGHT_SCALE_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_BODY_COMPOSITION_SERVICE_INFO` (0x31D):** "BT Body Composition Service Info" - Bluetooth Body Composition Service (BCS) information. See `CMD_BT_BODY_COMPOSITION_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TYPE_INFO` (0x31E):** "BT Health Thermometer Type Info" - Bluetooth Health Thermometer Type information. See `CMD_BT_HEALTH_THERMOMETER_TYPE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_MEASUREMENT_INTERVAL_INFO` (0x31F):** "BT Health Thermometer Measurement Interval Info" - Bluetooth Health Thermometer Measurement Interval information. See `CMD_BT_HEALTH_THERMOMETER_MEASUREMENT_INTERVAL_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_INFO` (0x320):** "BT Health Thermometer Temperature Measurement Info" - Bluetooth Health Thermometer Temperature Measurement information. See `CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_INTERMEDIATE_TEMPERATURE_MEASUREMENT_INFO` (0x321):** "BT Health Thermometer Intermediate Temperature Measurement Info" - Bluetooth Health Thermometer Intermediate Temperature Measurement information. See `CMD_BT_HEALTH_THERMOMETER_INTERMEDIATE_TEMPERATURE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_TYPE_INFO` (0x322):** "BT Health Thermometer Temperature Type Info" - Bluetooth Health Thermometer Temperature Type information. See `CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_TYPE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_MEASUREMENT_CONTEXT_INFO` (0x323):** "BT Health Thermometer Measurement Context Info" - Bluetooth Health Thermometer Measurement Context information. See `CMD_BT_HEALTH_THERMOMETER_MEASUREMENT_CONTEXT_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_OFFSET_INFO` (0x324):** "BT Health Thermometer Temperature Offset Info" - Bluetooth Health Thermometer Temperature Offset information. See `CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_OFFSET_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_SCALE_INFO` (0x325):** "BT Health Thermometer Temperature Scale Info" - Bluetooth Health Thermometer Temperature Scale information. See `CMD_BT_HEALTH_THERMOMETER_TEMPERATURE_SCALE_INFO` in the Java code.

*   **`RZC_CMD_BT_HEALTH_THERMOMETER_TIME_TRIGGER_SETTINGS_INFO` (0x326):** "BT Health Thermometer Time Trigger Settings Info" - Bluetooth Health Thermometer Time Trigger Settings information. See `CMD_BT_HEALTH_THERMOMETER_TIME_TRIGGER_SETTINGS_INFO` in the Java code.

*   **`RZC_CMD_BT_HEART_RATE_MEASUREMENT_INFO` (0x327):** "BT Heart Rate Measurement Info" - Bluetooth Heart Rate Measurement information. See `CMD_BT_HEART_RATE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_HEART_RATE_BODY_SENSOR_LOCATION_INFO` (0x328):** "BT Heart Rate Body Sensor Location Info" - Bluetooth Heart Rate Body Sensor Location information. See `CMD_BT_HEART_RATE_BODY_SENSOR_LOCATION_INFO` in the Java code.

*   **`RZC_CMD_BT_HEART_RATE_CONTROL_POINT_INFO` (0x329):** "BT Heart Rate Control Point Info" - Bluetooth Heart Rate Control Point information. See `CMD_BT_HEART_RATE_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_BLOOD_PRESSURE_FEATURE_INFO` (0x32A):** "BT Blood Pressure Feature Info" - Bluetooth Blood Pressure Feature information. See `CMD_BT_BLOOD_PRESSURE_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_BLOOD_PRESSURE_MEASUREMENT_INFO` (0x32B):** "BT Blood Pressure Measurement Info" - Bluetooth Blood Pressure Measurement information. See `CMD_BT_BLOOD_PRESSURE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_BLOOD_PRESSURE_INTERMEDIATE_CUFF_PRESSURE_INFO` (0x32C):** "BT Blood Pressure Intermediate Cuff Pressure Info" - Bluetooth Blood Pressure Intermediate Cuff Pressure information. See `CMD_BT_BLOOD_PRESSURE_INTERMEDIATE_CUFF_PRESSURE_INFO` in the Java code.

*   **`RZC_CMD_BT_GLUCOSE_FEATURE_INFO` (0x32D):** "BT Glucose Feature Info" - Bluetooth Glucose Feature information. See `CMD_BT_GLUCOSE_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_GLUCOSE_MEASUREMENT_INFO` (0x32E):** "BT Glucose Measurement Info" - Bluetooth Glucose Measurement information. See `CMD_BT_GLUCOSE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_GLUCOSE_MEASUREMENT_CONTEXT_INFO` (0x32F):** "BT Glucose Measurement Context Info" - Bluetooth Glucose Measurement Context information. See `CMD_BT_GLUCOSE_MEASUREMENT_CONTEXT_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_SPEED_AND_CADENCE_FEATURE_INFO` (0x330):** "BT Cycling Speed and Cadence Feature Info" - Bluetooth Cycling Speed and Cadence Feature information. See `CMD_BT_CYCLING_SPEED_AND_CADENCE_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_SPEED_AND_CADENCE_MEASUREMENT_INFO` (0x331):** "BT Cycling Speed and Cadence Measurement Info" - Bluetooth Cycling Speed and Cadence Measurement information. See `CMD_BT_CYCLING_SPEED_AND_CADENCE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_POWER_FEATURE_INFO` (0x332):** "BT Cycling Power Feature Info" - Bluetooth Cycling Power Feature information. See `CMD_BT_CYCLING_POWER_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_POWER_MEASUREMENT_INFO` (0x333):** "BT Cycling Power Measurement Info" - Bluetooth Cycling Power Measurement information. See `CMD_BT_CYCLING_POWER_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_POWER_VECTOR_INFO` (0x334):** "BT Cycling Power Vector Info" - Bluetooth Cycling Power Vector information. See `CMD_BT_CYCLING_POWER_VECTOR_INFO` in the Java code.

*   **`RZC_CMD_BT_CYCLING_POWER_CONTROL_POINT_INFO` (0x335):** "BT Cycling Power Control Point Info" - Bluetooth Cycling Power Control Point information. See `CMD_BT_CYCLING_POWER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_RUNNING_SPEED_AND_CADENCE_FEATURE_INFO` (0x336):** "BT Running Speed and Cadence Feature Info" - Bluetooth Running Speed and Cadence Feature information. See `CMD_BT_RUNNING_SPEED_AND_CADENCE_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_RUNNING_SPEED_AND_CADENCE_MEASUREMENT_INFO` (0x337):** "BT Running Speed and Cadence Measurement Info" - Bluetooth Running Speed and Cadence Measurement information. See `CMD_BT_RUNNING_SPEED_AND_CADENCE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_WEIGHT_SCALE_FEATURE_INFO` (0x338):** "BT Weight Scale Feature Info" - Bluetooth Weight Scale Feature information. See `CMD_BT_WEIGHT_SCALE_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_WEIGHT_SCALE_MEASUREMENT_INFO` (0x339):** "BT Weight Scale Measurement Info" - Bluetooth Weight Scale Measurement information. See `CMD_BT_WEIGHT_SCALE_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_BODY_COMPOSITION_FEATURE_INFO` (0x33A):** "BT Body Composition Feature Info" - Bluetooth Body Composition Feature information. See `CMD_BT_BODY_COMPOSITION_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_BODY_COMPOSITION_MEASUREMENT_INFO` (0x33B):** "BT Body Composition Measurement Info" - Bluetooth Body Composition Measurement information. See `CMD_BT_BODY_COMPOSITION_MEASUREMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_BODY_COMPOSITION_CONTROL_POINT_INFO` (0x33C):** "BT Body Composition Control Point Info" - Bluetooth Body Composition Control Point information. See `CMD_BT_BODY_COMPOSITION_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_SERVICE_INFO` (0x33D):** "BT Generic Media Control Service Info" - Bluetooth Generic Media Control Service (GMCS) information. See `CMD_BT_GENERIC_MEDIA_CONTROL_SERVICE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_FEATURE_INFO` (0x33E):** "BT Generic Media Control Feature Info" - Bluetooth Generic Media Control Feature information. See `CMD_BT_GENERIC_MEDIA_CONTROL_FEATURE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_COMMAND_INFO` (0x33F):** "BT Generic Media Control Command Info" - Bluetooth Generic Media Control Command information. See `CMD_BT_GENERIC_MEDIA_CONTROL_COMMAND_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_COMMAND_PARAMETERS_INFO` (0x340):** "BT Generic Media Control Command Parameters Info" - Bluetooth Generic Media Control Command Parameters information. See `CMD_BT_GENERIC_MEDIA_CONTROL_COMMAND_PARAMETERS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_CHANGED_INFO` (0x341):** "BT Generic Media Control Track Changed Info" - Bluetooth Generic Media Control Track Changed information. See `CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_CHANGED_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_TITLE_INFO` (0x342):** "BT Generic Media Control Track Title Info" - Bluetooth Generic Media Control Track Title information. See `CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_TITLE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_DURATION_INFO` (0x343):** "BT Generic Media Control Track Duration Info" - Bluetooth Generic Media Control Track Duration information. See `CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_DURATION_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_POSITION_INFO` (0x344):** "BT Generic Media Control Track Position Info" - Bluetooth Generic Media Control Track Position information. See `CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_POSITION_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_PLAYBACK_STATUS_INFO` (0x345):** "BT Generic Media Control Playback Status Info" - Bluetooth Generic Media Control Playback Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_PLAYBACK_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_VOLUME_INFO` (0x346):** "BT Generic Media Control Volume Info" - Bluetooth Generic Media Control Volume information. See `CMD_BT_GENERIC_MEDIA_CONTROL_VOLUME_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_VOLUME_FLAGS_INFO` (0x347):** "BT Generic Media Control Volume Flags Info" - Bluetooth Generic Media Control Volume Flags information. See `CMD_BT_GENERIC_MEDIA_CONTROL_VOLUME_FLAGS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_EQUALIZER_INFO` (0x348):** "BT Generic Media Control Equalizer Info" - Bluetooth Generic Media Control Equalizer information. See `CMD_BT_GENERIC_MEDIA_CONTROL_EQUALIZER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_REPEAT_MODE_INFO` (0x349):** "BT Generic Media Control Repeat Mode Info" - Bluetooth Generic Media Control Repeat Mode information. See `CMD_BT_GENERIC_MEDIA_CONTROL_REPEAT_MODE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_SHUFFLE_MODE_INFO` (0x34A):** "BT Generic Media Control Shuffle Mode Info" - Bluetooth Generic Media Control Shuffle Mode information. See `CMD_BT_GENERIC_MEDIA_CONTROL_SHUFFLE_MODE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_BROWSE_MODE_INFO` (0x34B):** "BT Generic Media Control Browse Mode Info" - Bluetooth Generic Media Control Browse Mode information. See `CMD_BT_GENERIC_MEDIA_CONTROL_BROWSE_MODE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_SEARCH_MODE_INFO` (0x34C):** "BT Generic Media Control Search Mode Info" - Bluetooth Generic Media Control Search Mode information. See `CMD_BT_GENERIC_MEDIA_CONTROL_SEARCH_MODE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_CONTENT_CONTROL_ID_INFO` (0x34D):** "BT Generic Media Control Content Control ID Info" - Bluetooth Generic Media Control Content Control ID information. See `CMD_BT_GENERIC_MEDIA_CONTROL_CONTENT_CONTROL_ID_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_PLAYBACK_SPEED_INFO` (0x34E):** "BT Generic Media Control Playback Speed Info" - Bluetooth Generic Media Control Playback Speed information. See `CMD_BT_GENERIC_MEDIA_CONTROL_PLAYBACK_SPEED_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_SEEKING_SPEED_INFO` (0x34F):** "BT Generic Media Control Seeking Speed Info" - Bluetooth Generic Media Control Seeking Speed information. See `CMD_BT_GENERIC_MEDIA_CONTROL_SEEKING_SPEED_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_CHAPTER_INFO` (0x350):** "BT Generic Media Control Chapter Info" - Bluetooth Generic Media Control Chapter information. See `CMD_BT_GENERIC_MEDIA_CONTROL_CHAPTER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_SEGMENT_INFO` (0x351):** "BT Generic Media Control Track Segment Info" - Bluetooth Generic Media Control Track Segment information. See `CMD_BT_GENERIC_MEDIA_CONTROL_TRACK_SEGMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_CURRENT_TRACK_SEGMENT_INFO` (0x352):** "BT Generic Media Control Current Track Segment Info" - Bluetooth Generic Media Control Current Track Segment information. See `CMD_BT_GENERIC_MEDIA_CONTROL_CURRENT_TRACK_SEGMENT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_AVAILABLE_TRACK_SEGMENTS_INFO` (0x353):** "BT Generic Media Control Available Track Segments Info" - Bluetooth Generic Media Control Available Track Segments information. See `CMD_BT_GENERIC_MEDIA_CONTROL_AVAILABLE_TRACK_SEGMENTS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_STATE_INFO` (0x354):** "BT Generic Media Control Media State Info" - Bluetooth Generic Media Control Media State information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_STATE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_CONTROL_POINT_INFO` (0x355):** "BT Generic Media Control Media Control Point Info" - Bluetooth Generic Media Control Media Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_NAME_INFO` (0x356):** "BT Generic Media Control Media Player Name Info" - Bluetooth Generic Media Control Media Player Name information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_NAME_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_INFO` (0x357):** "BT Generic Media Control Media Player Icon Object Info" - Bluetooth Generic Media Control Media Player Icon Object information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TYPE_INFO` (0x358):** "BT Generic Media Control Media Player Icon Object Type Info" - Bluetooth Generic Media Control Media Player Icon Object Type information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TYPE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_INFO` (0x359):** "BT Generic Media Control Media Player Icon Object Size Info" - Bluetooth Generic Media Control Media Player Icon Object Size information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_INFO` (0x35A):** "BT Generic Media Control Media Player Icon Object Offset Info" - Bluetooth Generic Media Control Media Player Icon Object Offset information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_INFO` (0x35B):** "BT Generic Media Control Media Player Icon Object Length Info" - Bluetooth Generic Media Control Media Player Icon Object Length information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_DATA_INFO` (0x35C):** "BT Generic Media Control Media Player Icon Object Data Info" - Bluetooth Generic Media Control Media Player Icon Object Data information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_DATA_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_INFO` (0x35D):** "BT Generic Media Control Media Player Icon Object CRC Info" - Bluetooth Generic Media Control Media Player Icon Object CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_INFO` (0x35E):** "BT Generic Media Control Media Player Icon Object Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_INFO` (0x35F):** "BT Generic Media Control Media Player Icon Object Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_INFO` (0x360):** "BT Generic Media Control Media Player Icon Object Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_INFO` (0x361):** "BT Generic Media Control Media Player Icon Object Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_INFO` (0x362):** "BT Generic Media Control Media Player Icon Object Error Info" - Bluetooth Generic Media Control Media Player Icon Object Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_OFFSET_INFO` (0x363):** "BT Generic Media Control Media Player Icon Object Size and Offset Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Offset information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_OFFSET_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_LENGTH_INFO` (0x364):** "BT Generic Media Control Media Player Icon Object Size and Length Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Length information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_LENGTH_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_CRC_INFO` (0x365):** "BT Generic Media Control Media Player Icon Object Size and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Size and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_SEQUENCE_NUMBER_INFO` (0x366):** "BT Generic Media Control Media Player Icon Object Size and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x367):** "BT Generic Media Control Media Player Icon Object Size and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TRANSFER_STATUS_INFO` (0x368):** "BT Generic Media Control Media Player Icon Object Size and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TRANSFER_CONTROL_POINT_INFO` (0x369):** "BT Generic Media Control Media Player Icon Object Size and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_ERROR_INFO` (0x36A):** "BT Generic Media Control Media Player Icon Object Size and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Size and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SIZE_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_LENGTH_INFO` (0x36B):** "BT Generic Media Control Media Player Icon Object Offset and Length Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Length information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_LENGTH_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_CRC_INFO` (0x36C):** "BT Generic Media Control Media Player Icon Object Offset and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_SEQUENCE_NUMBER_INFO` (0x36D):** "BT Generic Media Control Media Player Icon Object Offset and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x36E):** "BT Generic Media Control Media Player Icon Object Offset and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TRANSFER_STATUS_INFO` (0x36F):** "BT Generic Media Control Media Player Icon Object Offset and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TRANSFER_CONTROL_POINT_INFO` (0x370):** "BT Generic Media Control Media Player Icon Object Offset and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_ERROR_INFO` (0x371):** "BT Generic Media Control Media Player Icon Object Offset and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Offset and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_OFFSET_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_CRC_INFO` (0x372):** "BT Generic Media Control Media Player Icon Object Length and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Length and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_SEQUENCE_NUMBER_INFO` (0x373):** "BT Generic Media Control Media Player Icon Object Length and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Length and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x374):** "BT Generic Media Control Media Player Icon Object Length and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Length and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TRANSFER_STATUS_INFO` (0x375):** "BT Generic Media Control Media Player Icon Object Length and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Length and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TRANSFER_CONTROL_POINT_INFO` (0x376):** "BT Generic Media Control Media Player Icon Object Length and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Length and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_ERROR_INFO` (0x377):** "BT Generic Media Control Media Player Icon Object Length and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Length and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_LENGTH_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_SEQUENCE_NUMBER_INFO` (0x378):** "BT Generic Media Control Media Player Icon Object CRC and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object CRC and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x379):** "BT Generic Media Control Media Player Icon Object CRC and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object CRC and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TRANSFER_STATUS_INFO` (0x37A):** "BT Generic Media Control Media Player Icon Object CRC and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object CRC and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TRANSFER_CONTROL_POINT_INFO` (0x37B):** "BT Generic Media Control Media Player Icon Object CRC and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object CRC and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_ERROR_INFO` (0x37C):** "BT Generic Media Control Media Player Icon Object CRC and Error Info" - Bluetooth Generic Media Control Media Player Icon Object CRC and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_CRC_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x37D):** "BT Generic Media Control Media Player Icon Object Sequence Number and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_STATUS_INFO` (0x37E):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x37F):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x380):** "BT Generic Media Control Media Player Icon Object Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_STATUS_INFO` (0x381):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x382):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x383):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TRANSFER_CONTROL_POINT_INFO` (0x384):** "BT Generic Media Control Media Player Icon Object Transfer Status and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_ERROR_INFO` (0x385):** "BT Generic Media Control Media Player Icon Object Transfer Status and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_ERROR_INFO` (0x386):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x387):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x388):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` (0x389):** "BT Generic Media Control Media Player Icon Object Error and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` (0x38A):** "BT Generic Media Control Media Player Icon Object Error and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_CRC_INFO` (0x38B):** "BT Generic Media Control Media Player Icon Object Sequence Number and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_CRC_INFO` (0x38C):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_CRC_INFO` (0x38D):** "BT Generic Media Control Media Player Icon Object Transfer Status and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_CRC_INFO` (0x38E):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_CRC_INFO` (0x38F):** "BT Generic Media Control Media Player Icon Object Error and CRC Info" - Bluetooth Generic Media Control Media Player Icon Object Error and CRC information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_CRC_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x390):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x391):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x392):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x393):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x394):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x395):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x396):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x397):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x398):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TRANSFER_CONTROL_POINT_INFO` (0x399):** "BT Generic Media Control Media Player Icon Object Transfer Status and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` (0x39A):** "BT Generic Media Control Media Player Icon Object Error and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` (0x39B):** "BT Generic Media Control Media Player Icon Object Error and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x39C):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x39D):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` (0x39E):** "BT Generic Media Control Media Player Icon Object Error and Transfer Status Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Status information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_STATUS_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` (0x39F):** "BT Generic Media Control Media Player Icon Object Error and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3A0):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3A1):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3A2):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3A3):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3A4):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3A5):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3A6):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3A7):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3A8):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3A9):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3AA):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3AB):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3AC):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3AD):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3AE):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3AF):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3B0):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3B1):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3B2):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3B3):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3B4):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3B5):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3B6):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3B7):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3B8):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3B9):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3BA):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3BB):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3BC):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3BD):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3BE):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3BF):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3C0):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3C1):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3C2):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3C3):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3C4):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3C5):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3C6):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3C7):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3C8):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3C9):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3CA):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3CB):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3CC):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3CD):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3CE):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3CF):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3D0):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3D1):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3D2):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3D3):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3D4):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3D5):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3D6):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3D7):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3D8):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3D9):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3DA):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3DB):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3DC):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3DD):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3DE):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3DF):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3E0):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3E1):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3E2):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3E3):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3E4):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3E5):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3E6):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3E7):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3E8):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3E9):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3EA):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3EB):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3EC):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3ED):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3EE):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3EF):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3F0):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3F1):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3F2):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3F3):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3F4):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3F5):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3F6):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3F7):** "BT Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3F8):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` (0x3F9):** "BT Generic Media Control Media Player Icon Object Error and Total Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Total Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3FA):** "BT Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` (0x3FB):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Transfer Control Point information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_TRANSFER_CONTROL_POINT_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` (0x3FC):** "BT Generic Media Control Media Player Icon Object Transfer Status and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Status and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_STATUS_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` (0x3FD):** "BT Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Transfer Control Point and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TRANSFER_CONTROL_POINT_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` (0x3FE):** "BT Generic Media Control Media Player Icon Object Error and Sequence Number Info" - Bluetooth Generic Media Control Media Player Icon Object Error and Sequence Number information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_SEQUENCE_NUMBER_INFO` in the Java code.

*   **`RZC_CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_TOTAL_SEQUENCE_NUMBER_AND_ERROR_INFO` (0x3FF):** "BT Generic Media Control Media Player Icon Object Total Sequence Number and Error Info" - Bluetooth Generic Media Control Media Player Icon Object Total Sequence Number and Error information. See `CMD_BT_GENERIC_MEDIA_CONTROL_MEDIA_PLAYER_ICON_OBJECT_ERROR_AND_TOTAL_SEQUENCE_NUMBER_INFO` in the Java code.

**Important Notes:**

*   **RCZ vs. 407:**  Again, these are from RCZ code. While there's likely overlap, *you must verify these CAN IDs and message structures on your Peugeot 407 using your CAN logs and by sniffing the CAN bus directly.*
*   **Bluetooth Heavy:**  A large portion of these messages (IDs from 0x2A4 upwards) are heavily related to Bluetooth functionality. This might be because the RCZ headunit in the code is Bluetooth-centric.  Your 407 headunit might have different or fewer Bluetooth-related messages.
*   **Incomplete Data Byte Info:** The Java code provides names, but often not detailed byte-by-byte descriptions. You'll need to analyze your CAN logs to understand the data format within each message.
*   **Request/Response vs. Broadcast:**  It's not always clear from the names if these are broadcast messages from the headunit, or if they are request/response messages.  CAN bus sniffing will clarify this.
*   **Steering Wheel Buttons:**  `0x1B0` (`RZC_CMD_STEER_WHEEL_KEY_INFO`) is a promising starting point for steering wheel control integration.

**Next Steps:**

1.  **Analyze Your 407 CAN Logs:**  Carefully examine your existing CAN logs from the Peugeot 407 Coupe 2008. Look for CAN IDs in the ranges listed above (especially around 0x1B0 and 0x290 - 0x3FF).
2.  **Compare to RCZ List:** See if any of the CAN IDs from your 407 logs match the IDs in the RCZ list.  If they do, analyze the data payload of those messages in your logs to see if the data structure seems to align with the message names from the Java code.
3.  **CAN Bus Sniffing:** If your logs aren't sufficient, set up your CAN interface and `canbox` on your test bench. Start sniffing the CAN bus while interacting with the headunit (changing volume, changing source, pressing steering wheel buttons, etc.). Observe the CAN traffic to identify new messages and confirm/refine the list from the Java code.
4.  **Prioritize and Test:** Start by trying to decode and use the messages you are most interested in first (based on your prioritized feature list from the initial questions).  `0x1B0` (steering wheel buttons), `0x293` (media play status), `0x294` (media source) might be good starting points.
5.  **Create 407 Definition File:** As you identify and verify messages for the 407, start building your separate vehicle definition file in the `canbox` project.

