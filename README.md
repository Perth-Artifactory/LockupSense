# LockupSense

A set of hardware/software that helps members lockup the space.

## Sensor boxes

Physical boxes that report back to https://healthchecks.io when online.

### Software

`ESP8266_monitor.ino`

Using code from https://github.com/Perth-Artifactory/esp8266_online_status to ping a healthchecks url every 10 seconds. A successfully online board will have the onboard LED lit.

### Hardware

An ESP8266 enclosed in an acrylic/MDF housing. Typically connected to a USB power supply plugged in to the same circut that is being monitored. All boxes should include references to https://perart.io/s for brevity which links back to this repository.

## Door checklist

### Software

In the interest of reducing hardware costs the door checklist is split into two pieces of software.

* Server: A script which checks in with the healthchecks.io API and formats the status of checks as required.
* Light control: An ESP8266 controlled via HTTP that sets the LEDs on the actual panel.

### Hardware

A series of panel mounted addressable LEDs on a board with check titles next to each one.
