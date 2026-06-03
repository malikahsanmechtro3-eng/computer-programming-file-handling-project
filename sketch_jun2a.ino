// ======================================================
// ESP32 SMART PARKING SYSTEM
// ======================================================

String parkingID = "P001";

const String accessKey = "ACCE_123";

void setup() {

    Serial.begin(115200);

    delay(3000);

    Serial.println("ESP32 READY");
}

void loop() {

    if (Serial.available() > 0) {

        String command =
            Serial.readStringUntil('\n');

        command.trim();

        // SEND PARKING ID
        if (command == "SEND_KEY") {

            Serial.println(parkingID);
        }

        // CHECK CONNECTION
        else if (command == "CHECK_ID") {

            Serial.println("CONNECTED");
        }

        // SEND ACCESS KEY
        else if (command == "GET_ACCESS") {

            Serial.println(accessKey);
        }

        // CHANGE PARKING ID
        else if (command.startsWith("SET_ID:")) {

            parkingID = command.substring(7);

            parkingID.trim();

            Serial.println("NEW ID SET");
            Serial.println(parkingID);
        }

        // INVALID COMMAND
        else {

            Serial.println("INVALID COMMAND");
        }
    }
}
