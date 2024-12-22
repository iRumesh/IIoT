IIoT
==========
IIoT edge implementation with best practices, Ultimate Goal will be moving all project to esp-idf,
This project is focused on implementing robust production ready IIoT implementation with ESP Chip.

### directory brief

    .               
    ├── main                            # Main folder include all the files
    │   ├── main.ino     
    │   ├── config.h                    # All the configurations, such as wifi, mqtt, pin def.
    │   ├── cert.h                      # rootCACertificate
    │   └── README.md                   # Doc     
    |── flows.json                      
    |── README.md
    |── CHANGELOG.md
    |── Release Notes.md

### To Do
- [ ] WiFi Provisioning (Send WiFi Credentials to device)
- [ ] Checking AP & STA mode on ESP32 at the same time (Custom webpage for user configurations)
- [ ] Device Provisioning (Link Device, User Created Account, Data Sent from Device) //Scan QR Code
- [ ] Firmware-Over-The-Air (FOTA) with github public repo, HTTP
- [ ] Firmware-Over-The-Air (FOTA) with github public repo, HTTPS TLS/SSL
- [ ] Firmware-Over-The-Air (FOTA) with github private repo, HTTPS TLS/SSL
    - [ ] OTA Partitions configurations
    - [ ] Revert to old firmware based on event (button press/user input)
    - [ ] After OTA, Getting WiFi credentials from SPIFFS ot LittleFS
    - [ ] Moving root CA to SPIFFS or LittleFS
- [ ] Buffer data locally if network down, send when network is up
- [ ] Power Modes testing with ESP32 (Active mode, Modem Sleep mode, Light Sleep mode, Deep Sleep mode, Hibernation mode)
- [ ] Optimizing dual cores in ESP32
- [ ] Blocking, Non-Blocking Functions test
- [ ] Interrupt Handlers
- [ ] MQTT over WiFi
    - [ ] QoS 2, will, birth, retain messages
    - [ ] Testing what is the max packet size can be sent by choosen MQTT library
    - [ ] Testing on Sending long string in single json payload
    - [ ] Test & Verify
- [ ] HTTP(S) methods over WiFi (GET, POST, PUT, and DELETE) // With external server
- [ ] Soft reset 
- [ ] Hard reset
- [ ] RTOS Implementation
- [ ] IIoT Mobile Framework
    - [ ] ESP32 to Firebase (https://github.com/mobizt/FirebaseClient)
    - [ ] Testing what is the max packet size can be sent by choosen MQTT library
    - [ ] Testing on Sending long string in single json payload
    - [ ] Test & Verify
- [ ] IIoT FastAPI Framework
    - [ ] API with MQTT
    - [ ] Account Creation
    - [ ] Webdashboard
    - [ ] Database Link (MySQL, Influx)
    - [ ] Data Visualizations (Grafana)


### References
FOTA - https://www.youtube.com/watch?v=qCmdUtguwPw

