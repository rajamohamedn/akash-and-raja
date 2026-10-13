#define IN_1 15 // GPIO15 = D8
#define IN_2 13 // GPIO13 = D7

// LEFT MOTOR
#define ENB 12  // GPIO12 = D6
#define IN_3 2  // GPIO2 = D4
#define IN_4 0  // GPIO0 = D3

// =====================================================
// MOTOR SPEED
// ESP8266 PWM RANGE = 0 TO 1023
// =====================================================
int manualSpeed = 700;

// =====================================================
// MANUAL COMMAND FLAGS
// =====================================================
bool forwardCmd = false;
bool backCmd = false;
bool leftCmd = false;
bool rightCmd = false;

// =====================================================
// WEB CONTROL PAGE
// =====================================================
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>ESP8266 Manual Robot</title>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>
body {
  font-family: Arial, sans-serif;
  text-align: center;
  background: #111;
  color: #eee;
  margin: 0;
  padding: 15px;
}

h1 {
  margin-top: 15px;
  font-size: 28px;
}

p {
  font-size: 17px;
}

.grid {
  margin-top: 25px;
}

.row {
  margin: 8px 0;
}

.btn {
  width: 125px;
  height: 65px;
  margin: 6px;
  font-size: 18px;
  font-weight: bold;
  border: none;
  border-radius: 12px;
  background: #333;
  color: white;
  user-select: none;
  -webkit-user-select: none;
  touch-action: none;
}

.btn:active {
  transform: scale(0.95);
  background: #555;
}

.stop {
  background: #b71c1c;
}

.stop:active {
  background: #e53935;
}

.speedBox {
  margin-top: 35px;
}

input[type=range] {
  width: 80%;
  max-width: 400px;
}

#speedValue {
  font-size: 20px;
  font-weight: bold;
}
</style>
</head>

<body>
<h1>ESP8266 Manual Robot</h1>
<p>Hold the direction buttons to move</p>

<div class="grid">

  <!-- FORWARD -->
  <div class="row">
    <button class="btn"
      ontouchstart="sendCmd('f',1); return false;"
      ontouchend="sendCmd('f',0); return false;"
      onmousedown="sendCmd('f',1)"
      onmouseup="sendCmd('f',0)"
      onmouseleave="sendCmd('f',0)">
      ⬆ Forward
    </button>
  </div>

  <!-- LEFT / STOP / RIGHT -->
  <div class="row">
    <button class="btn"
      ontouchstart="sendCmd('l',1); return false;"
      ontouchend="sendCmd('l',0); return false;"
      onmousedown="sendCmd('l',1)"
      onmouseup="sendCmd('l',0)"
      onmouseleave="sendCmd('l',0)">
      ⬅ Left
    </button>

    <button class="btn stop" onclick="sendStop()">
      ⏹ Stop
    </button>

    <button class="btn"
      ontouchstart="sendCmd('r',1); return false;"
      ontouchend="sendCmd('r',0); return false;"
      onmousedown="sendCmd('r',1)"
      onmouseup="sendCmd('r',0)"
      onmouseleave="sendCmd('r',0)">
      ➡ Right
    </button>
  </div>

  <!-- BACKWARD -->
  <div class="row">
    <button class="btn"
      ontouchstart="sendCmd('b',1); return false;"
      ontouchend="sendCmd('b',0); return false;"
      onmousedown="sendCmd('b',1)"
      onmouseup="sendCmd('b',0)"
      onmouseleave="sendCmd('b',0)">
      ⬇ Backward
    </button>
  </div>

</div>

<!-- SPEED CONTROL -->
<div class="speedBox">
  <h2>Motor Speed</h2>

  <input
    type="range"
    min="0"
    max="1023"
    value="700"
    oninput="changeSpeed(this.value)">

  <p>
    Speed:
    <span id="speedValue">700</span>
  </p>
</div>

<script>
function sendCmd(dir, state) {
  fetch('/cmd?dir=' + dir + '&state=' + state)
    .catch(function(e) {
      console.log(e);
    });
}

function sendStop() {
  fetch('/stop')
    .catch(function(e) {
      console.log(e);
    });
}

function changeSpeed(value) {
  document.getElementById("speedValue").innerText = value;

  fetch('/speed?value=' + value)
    .catch(function(e) {
      console.log(e);
    });
}

// Stop robot if browser loses focus
window.addEventListener('blur', function() {
  sendStop();
});

// Stop robot when page becomes hidden
document.addEventListener('visibilitychange', function() {
  if (document.hidden) {
    sendStop();
  }
});
</script>

</body>
</html>
)=====";

// =====================================================
// MOTOR SPEED FUNCTION
// =====================================================
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  analogWrite(ENB, leftSpeed);
  analogWrite(ENA, rightSpeed);
}

// =====================================================
// STOP
// =====================================================
void driveStop() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);

  setMotorSpeed(0, 0);
}

// =====================================================
// FORWARD
// =====================================================
void driveForward(int speed) {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  setMotorSpeed(speed, speed);
}

// =====================================================
// BACKWARD
// =====================================================
void driveBackward(int speed) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);

  setMotorSpeed(speed, speed);
}

// =====================================================
// LEFT TURN
// =====================================================
void driveLeft(int speed) {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);

  setMotorSpeed(speed, speed);
}

// =====================================================
// RIGHT TURN
// =====================================================
void driveRight(int speed) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  setMotorSpeed(speed, speed);
}

// =====================================================
// MANUAL MOVEMENT HANDLER
// =====================================================
void handleManualMovement() {
  if (forwardCmd) {
    driveForward(manualSpeed);
  }
  else if (backCmd) {
    driveBackward(manualSpeed);
  }
  else if (leftCmd) {
    driveLeft(manualSpeed);
  }
  else if (rightCmd) {
    driveRight(manualSpeed);
  }
  else {
    driveStop();
  }
}

// =====================================================
// WEB PAGE
// =====================================================
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

// =====================================================
// COMMAND HANDLER
// =====================================================
void handleCmd() {
  if (!server.hasArg("dir") || !server.hasArg("state")) {
    server.send(400, "text/plain", "Missing parameters");
    return;
  }

  String dir = server.arg("dir");
  int state = server.arg("state").toInt();
  bool pressed = (state != 0);

  if (dir == "f") {
    forwardCmd = pressed;

    if (pressed) {
      backCmd = false;
      leftCmd = false;
      rightCmd = false;
    }
  }
  else if (dir == "b") {
    backCmd = pressed;

    if (pressed) {
      forwardCmd = false;
      leftCmd = false;
      rightCmd = false;
    }
  }
  else if (dir == "l") {
    leftCmd = pressed;

    if (pressed) {
      forwardCmd = false;
      backCmd = false;
      rightCmd = false;
    }
  }
  else if (dir == "r") {
    rightCmd = pressed;

    if (pressed) {
      forwardCmd = false;
      backCmd = false;
      leftCmd = false;
    }
  }
  else if (dir == "s") {
    forwardCmd = false;
    backCmd = false;
    leftCmd = false;
    rightCmd = false;

    driveStop();
  }

  server.send(200, "text/plain", "OK");
}

// =====================================================
// STOP HANDLER
// =====================================================
void handleStop() {
  forwardCmd = false;
  backCmd = false;
  leftCmd = false;
  rightCmd = false;

  driveStop();

  server.send(200, "text/plain", "STOPPED");
}

// =====================================================
// SPEED HANDLER
// =====================================================
void handleSpeed() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing speed");
    return;
  }

  manualSpeed = server.arg("value").toInt();
  manualSpeed = constrain(manualSpeed, 0, 1023);

  server.send(200, "text/plain", "Speed Updated");
}

// =====================================================
// NOT FOUND
// =====================================================
void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

  // MOTOR PINS
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // ESP8266 PWM
  analogWriteRange(1023);
  analogWriteFreq(1000);

  // INITIAL STOP
  driveStop();

  // WIFI ACCESS POINT
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);

  Serial.println();
  Serial.println("==============================");
  Serial.println("ESP8266 MANUAL ROBOT");

  // REGISTER WEB ROUTES
  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/stop", handleStop);
  server.on("/speed", handleSpeed);
  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("HTTP Server Started");
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop() {
  server.handleClient();
  handleManualMovement();
  delay(5);
}
