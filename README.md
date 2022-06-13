# EcoSeed
Every plant needs a someone to keep an eye on it. Or maybe something? EcoSeed is a eco friendly system that takes good care of your plant and notifies you about its growth progress.

![logo-new](https://user-images.githubusercontent.com/71717457/172488299-8806ea04-cc92-4af0-a666-15fdc0f39026.png)
# Main Idea
Our idea is a system that will monitor a plant (its height, weight and soil) and also rotate it to a specific position where the amount of light is as much as possible. After that, we collect these information and upload them in a PHP database. To do this, we needed to learn some new things about the components that we used and also the way we wanted them to work.

# Functionality
The **Ecoseed** system consists of two parts. The first is the Esp32 with its accessories which sends the collected data to the second part which is the server which in turn, displays the data on a web application.

# Components
<ul>
    <li>1x <b>Esp Wroom 32</b></li>
    <li>2x <b>Stepper Motors</b></li>
    <li>1x <b>Ultrasonic Sensor</b></li>
    <li>1x <b>Soil Hygrometer Humidity Detection Module Moisture Sensor</b></li>
    <li>1x <b>RTC Module</b></li>
    <li>1x <b>I2C Module</b></li>
    <li>1x <b>16x2 Character LCD Module</b></li>
    <li>1x <b>LDR Sensor</b></li>
    <li>1x <b>Force Sensitive Sensor</b></li>
</ul>

# Detailed Description
First of all, there is a plant on top of a base where there are attached one stepper motor and the force sensitive sensor. On top of the plant, there is an ultrasonic sensor, which measures the height of the plant. In the soil of the plant, there is a moisture sensor which measures the soil moisture of the plant. At the highest point of the structure, there is a ldr attached to the second stepper motor, which rotates in a specific duration of time and finds the best spot with the most light. Then, it turns the flower pot in that direction. Finally, Esp32 collects the data and sents them to the server, where these data are processed and are displayed to the webserver.
