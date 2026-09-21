# Hardware and Simulation Notes

## 1. Project Basis

This repository was organized from the supplied project report and project summary.

The documented project is an automatic speed control system for a DC motor using Arduino feedback control.

The report describes a potentiometer being used to vary the input voltage. Arduino measures input voltage and motor voltage and changes PWM duty cycle through a MOSFET switching stage.

## 2. Practical Prototype

The supplied photographs show the physical prototype:

- Arduino UNO
- Breadboard wiring
- DC motor
- MOSFET/power switching hardware
- Potentiometer
- Computer running Arduino IDE / serial monitoring

The photos are stored in `images/`.

## 3. Control Principle

The documented sequence is:

1. Read Vin.
2. Read Vmotor.
3. Compare motor voltage with the target.
4. Adjust PWM duty cycle.
5. Drive the MOSFET.
6. Repeat continuously.

The observed behavior reported in the source documentation is:

- Lower Vin → higher PWM duty.
- Higher Vin → lower PWM duty.
- Approximately constant motor voltage.
- Approximately constant motor speed.

## 4. Limitation

The documentation explicitly identifies the system as indirect speed control because it does not use an encoder.

Therefore, the repository should not describe this as direct RPM feedback control.

## 5. MATLAB Simulink

The source report describes a Simulink model containing:

- Reference speed
- Error detector
- Controller
- PWM generator
- MOSFET switching circuit
- DC motor model
- Feedback path

The original `.slx` file was not present in the supplied material.

`matlab/control_concept.m` is therefore provided as a transparent control-concept implementation rather than an original model file.

## 6. Reconstructed Arduino Firmware

The supplied documents did not include the original `.ino` file.

The repository's firmware is reconstructed from the documented algorithm. Before using it on hardware, verify:

- Arduino pin assignments
- Voltage-divider ratios
- Target voltage
- MOSFET gate-drive arrangement
- Motor supply voltage
- Current rating
- Flyback protection
- Ground/reference arrangement

## 7. Safety

Do not connect motor supply voltage directly to Arduino ADC pins.

Use appropriately rated sensing and switching components. A DC motor can generate voltage transients and significant current. Use suitable flyback protection, current limiting, fusing and thermal management for the actual power stage.
