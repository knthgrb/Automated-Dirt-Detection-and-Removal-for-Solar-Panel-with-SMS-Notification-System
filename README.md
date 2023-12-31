# Automated Solar Panel Cleaning System with Dirt Detection and SMS Notifications

## Components:

- Arduino Mega
- TCS3472 Color Sensor (4 sensors to cover the whole area of the solar panel)
- L298N Motor Driver to drive the DC motor
- 12V DC Motor for moving the modified wiper
- Relay
- 5V Water Pump connected to the relay
- 2 Limit Switches (to reverse and stop the DC motor)
- SIM900A GSM Module for sending notifications
- Water Level Sensor to monitor the water level in the water tank

## Functionality:

1. **Color Calibration:**

   - The color sensors initially store the color values of the clean solar panel.

2. **Real-time Monitoring:**

   - The sensors continuously monitor the color values in real time.

3. **Dirt Detection:**

   - If there is a significant difference between the stored values and real-time values, it indicates the presence of dirt on the solar panel.

4. **Automated Cleaning Process:**

   - The L298N motor driver activates the 12V DC motor to move the modified wiper across the solar panel.
   - Simultaneously, the water pump is activated to pump water for cleaning.

5. **GSM Notifications:**

   - The SIM900A GSM module sends notifications during the cleaning process.
   - Notifications are also sent when the water level is low in the water tank.

6. **Post-Cleaning Verification:**
   - After the cleaning process, the color sensors sense the panel again.
   - If there is a minimal difference between the sensed values and the stored values, the sensed values become the new stored values.
