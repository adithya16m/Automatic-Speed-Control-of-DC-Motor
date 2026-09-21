/*
 * Automatic Speed Control of DC Motor using Arduino
 * Feedback-based PWM control
 *
 * Reconstructed from the supplied project report.
 *
 * IMPORTANT:
 * The supplied documents describe the control concept but do not specify
 * the exact Arduino pins, voltage-divider resistor values, ADC calibration,
 * or original source code. The values below are therefore configurable
 * defaults and must be matched to the actual hardware before use.
 *
 * Hardware concept:
 *   Variable DC input -> MOSFET PWM stage -> DC motor
 *   Vin sensing ---------------------------> A0
 *   Motor voltage sensing -----------------> A1
 *   Arduino PWM ----------------------------> D9 -> MOSFET gate driver/gate
 *
 * Do NOT connect a motor supply directly to an Arduino analog pin.
 * Use a correctly designed voltage divider / sensing circuit.
 * For higher-current motors, use an appropriate MOSFET driver and
 * flyback protection.
 */

const uint8_t VIN_SENSE_PIN    = A0;
const uint8_t MOTOR_SENSE_PIN  = A1;
const uint8_t PWM_PIN          = 9;

// ADC/reference configuration
const float ADC_REFERENCE = 5.0f;
const float ADC_COUNTS = 1023.0f;

// Set this to the actual voltage-divider ratio:
// sensed_voltage = real_voltage / DIVIDER_RATIO
//
// Example: if a divider produces 1/5 of the motor voltage,
// DIVIDER_RATIO = 5.0
const float VIN_DIVIDER_RATIO   = 5.0f;
const float MOTOR_DIVIDER_RATIO = 5.0f;

// Target motor voltage.
// Set this to the value appropriate for your motor and test setup.
float targetMotorVoltage = 5.0f;

// Control parameters
int pwmDuty = 0;
const int PWM_MIN = 0;
const int PWM_MAX = 255;

// Incremental feedback step.
// Larger values react faster but can produce more oscillation.
const int DUTY_STEP = 2;

// Small deadband to prevent constant hunting around the target.
const float VOLTAGE_DEADBAND = 0.05f;

// Sampling interval
const unsigned long CONTROL_INTERVAL_MS = 20;
unsigned long lastControlTime = 0;

float readVoltage(uint8_t pin, float dividerRatio)
{
  int adc = analogRead(pin);
  float sensedVoltage = (adc * ADC_REFERENCE) / ADC_COUNTS;
  return sensedVoltage * dividerRatio;
}

void setup()
{
  Serial.begin(115200);

  pinMode(VIN_SENSE_PIN, INPUT);
  pinMode(MOTOR_SENSE_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);

  analogWrite(PWM_PIN, 0);

  Serial.println("Automatic DC Motor Speed Control");
  Serial.println("Feedback control starting...");
}

void loop()
{
  unsigned long now = millis();

  if (now - lastControlTime < CONTROL_INTERVAL_MS)
    return;

  lastControlTime = now;

  float vin = readVoltage(VIN_SENSE_PIN, VIN_DIVIDER_RATIO);
  float motorVoltage = readVoltage(MOTOR_SENSE_PIN, MOTOR_DIVIDER_RATIO);

  float error = targetMotorVoltage - motorVoltage;

  /*
   * Basic closed-loop control:
   * - Motor voltage below target -> increase PWM
   * - Motor voltage above target -> decrease PWM
   *
   * This follows the control behavior described in the report.
   */
  if (error > VOLTAGE_DEADBAND)
  {
    pwmDuty += DUTY_STEP;
  }
  else if (error < -VOLTAGE_DEADBAND)
  {
    pwmDuty -= DUTY_STEP;
  }

  pwmDuty = constrain(pwmDuty, PWM_MIN, PWM_MAX);

  analogWrite(PWM_PIN, pwmDuty);

  Serial.print("Vin: ");
  Serial.print(vin, 2);
  Serial.print(" V | Vmotor: ");
  Serial.print(motorVoltage, 2);
  Serial.print(" V | Target: ");
  Serial.print(targetMotorVoltage, 2);
  Serial.print(" V | PWM: ");
  Serial.print(pwmDuty);
  Serial.print(" | Duty: ");
  Serial.print((100.0f * pwmDuty) / 255.0f, 1);
  Serial.println(" %");
}
