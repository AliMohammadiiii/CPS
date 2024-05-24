pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors



ApplicationWindow {
    id: root

    readonly property int defaultFontSize: 22
    readonly property int imageSize: width / 2

    property var recordedPathdis: []
    property var recordedPathdir: []
    property var newPathdis: []
    property var newPathdir: []
    property bool isRecording: false
    property bool isAuth: false

    width: 420
    height: 760
    visible: true
    title: "Motion Based Auth"

    function reset(): void {
        recordedPathdis = [];
        recordedPathdir = [];
        newPathdis = [];
        newPathdir = [];
        
        calibrate();

    }

    function calibrate(): void {
        accelerometer.cax += accelerometer.ax
        accelerometer.cay += accelerometer.ay
        accelerometer.caz += accelerometer.az
        rotation.rx = 0
        rotation.ry = 0
        rotation.rz = 0

        position.px = 0
        position.py = 0
        position.pz = 0

        accelerometer.vx = 0
        accelerometer.vy = 0
        accelerometer.vz = 0

    }

     function kalmanFilter() {
        var r = 10.0;  // Measurement noise covariance
        var q = 0.1;   // Process noise covariance
        var a = 1.0;   // State transition matrix
        var c = 1.0;   // Measurement matrix
        var b = 0.0;   // Control input matrix

        var cov = NaN; // Covariance
        var x = NaN;   // State estimate

        this.filter = function(measurement) {
            if (isNaN(x)) {
                x = 1 / c * measurement;
                cov = 1 / c * q * (1 / c);
            } else {
                var prediction = this.predict();
                var uncertainty = this.uncertainty();
                var gain = uncertainty * c * (1 / (c * uncertainty * c + q));
                x = prediction + gain * (measurement - c * prediction);
                cov = uncertainty - gain * c * uncertainty;
            }
            return x;
        };

        this.predict = function() {
            return a * x + b * 0; // No control input in this example
        };

        this.uncertainty = function() {
            return a * cov * a + r;
        };

        this.lastMeasurement = function() {
            return x;
        };

        this.setMeasurementNoise = function(noise) {
            r = noise;
        };

        this.setProcessNoise = function(noise) {
            q = noise;
        };
    }

    function startStopRecording() : void {
        isRecording = !isRecording
    }
    
    function startStopAuth() : void {
        isAuth = !isAuth
    }
    function arraysEqual(arr1, arr2) {
        if (arr1.length !== arr2.length) {
            return false;
        }
        for (var i = 0; i < arr1.length; i++) {
            if (arr1[i] !== arr2[i]) {
                return false;
            }
        }
        return true;
    }
    function auth() : void {
        console.log("recorded path:", recordedPathdis);
        console.log("auth path:", newPathdis);

        console.log("recorded path Dir:", recordedPathdir);
        console.log("auth path Dir:", newPathdir);
        if (arraysEqual(newPathdis, recordedPathdis) && arraysEqual(newPathdir, recordedPathdir))
        {
            authStatus.text = "Authenticated"
        }
        else
        {
            authStatus.text = "not authorized"
        }
    }
    function addPath(x) : void {
        console.log("Added Path")
        if (isRecording) {
            recordedPathdis.push(x)
            recordedPathdir.push(gyroscope.direction)
        } else if(isAuth) {
            newPathdis.push(x)
            newPathdir.push(gyroscope.direction)
        }


    }


    StackView {
        id: stack
        anchors.fill: parent
        anchors.margins: width / 10


        initialItem: Item {
            ColumnLayout {
                id: initialItem
                anchors.fill: parent
                anchors.topMargin: 20
                anchors. bottomMargin: 20
                spacing: 5

                Gyroscope {
                    id: gyroscope

                    property real lastTimeStamp: 0
                    property real x: 0
                    property real y: 0
                    property real z: 0
                    property real direction: 0
                    property var kalmanGx: new kalmanFilter()
                    property var kalmanGy: new kalmanFilter()
                    property var kalmanGz: new kalmanFilter()

                    property int underThrCount: 0

                    active: true
                    dataRate: 20

                    onReadingChanged: {
                        x = kalmanGx.filter((reading as GyroscopeReading).x)
                        y = kalmanGy.filter((reading as GyroscopeReading).y)
                        z = kalmanGz.filter((reading as GyroscopeReading).z)

                        let firstCall = false
                        if (lastTimeStamp == 0) {
                            firstCall = true
                        }
                        let newLast = reading.timestamp
                        let timeSinceLast = (newLast - lastTimeStamp) / 1000000
                        lastTimeStamp = newLast

                        if (firstCall === true)
                            return

                        let thr = 4

                        if ((x <= thr && x >= -thr) && (y <= thr && y >= -thr) && (z <= thr && z >= -thr)) {
                            underThrCount += 1
                        } else {
                            underThrCount = 0
                        }

                        if (underThrCount === 4) {
                            console.log("2")
                        }

                        rotation.rx += (x > thr || x < -thr) ? (x * timeSinceLast) : 0
                        rotation.ry += (y > thr || y < -thr) ? (y * timeSinceLast) : 0
                        rotation.rz += (z > thr || z < -thr) ? (z * timeSinceLast) : 0

                        if (rotation.rz < 45 && rotation.rz > -45)
                            {direction = 0;}
                        else if ((rotation.rz < 135 && rotation.rz > 45) || (rotation.rz < -135 && rotation.rz > -225))
                            {direction = 1;}
                        else if ((rotation.rz < 225 && rotation.rz > 135) || (rotation.rz < -45 && rotation.rz > -135))
                            {direction = 2;}
                        else
                            {direction = 3;}
                    }
                }
                Accelerometer {
                    id: accelerometer

                    property real vx: 0
                    property real vy: 0
                    property real vz: 0


                    property real ax: 0
                    property real ay: 0
                    property real az: 0

                    property real cax: 0
                    property real cay: 0
                    property real caz: 0

                    property real directionx: 0
                    property real directiony: 0
                    active: true
                    dataRate: 100

                    property real lastTimeStamp: 0


                    property real thr: 0.4
                    // Kalman filter variables
                    property var kalmanAx: new kalmanFilter()
                    property var kalmanAy: new kalmanFilter()
                    property var kalmanAz: new kalmanFilter()

                    property int underThrCount: 0



                    onReadingChanged: {
                        // Apply Kalman filter to each axis
                        ax = kalmanAx.filter((reading as AccelerometerReading).x - cax);
                        ay = kalmanAy.filter((reading as AccelerometerReading).y - cay);
                        az = kalmanAz.filter((reading as AccelerometerReading).z - caz);

                        if (underThrCount >= 20)
                        {
                            directionx = (ax > 0) ? 0 : 1;
                            directiony = (ay > 0) ? 0 : 1;
                        }
                        
                        
                        let firstCall = false
                        if (lastTimeStamp == 0) {
                            firstCall = true
                        }
                        let newLast = reading.timestamp
                        let timeSinceLast = (newLast - lastTimeStamp) / 1000000
                        lastTimeStamp = newLast

                        if (firstCall === true)
                            return

                        let thr = 0.5

                        if ((ax <= thr && ax >= -thr) && (ay <= thr && ay >= -thr) && (az <= thr && az >= -thr)) {
                            underThrCount += 1
                        } else {
                            underThrCount = 0
                        }

                        

                        vx += (ax > thr || ax < -thr) ? (ax * timeSinceLast) : 0
                        vy += (ay > thr || ay < -thr) ? (ay * timeSinceLast) : 0
                        vz += (az > thr || az < -thr) ? (az * timeSinceLast) : 0

                        position.px += vx * timeSinceLast * 100
                        position.py += vy * timeSinceLast * 100
                        position.pz += vz * timeSinceLast * 100


                        if (underThrCount === 20) {
                            if(Math.abs(position.px) > Math.abs(position.py))
                            {
                                if (Math.abs(position.px) > 2)
                                {
                                    if (directionx == 1)
                                        {addPath(0);}
                                    else
                                        {addPath(1);}
                                }
                            }

                            else
                            {
                                if (Math.abs(position.py) > 2)
                                {
                                    if (directiony == 1)
                                        {addPath(2);}
                                    else
                                        {addPath(3);}
                                }
                            }
                            // root.recordedPathdir.push(Gyroscope.direction);
                            // console.log("directionx:", directionx);
                            // console.log("directiony:", directiony);
                            // console.log("position x:", position.px);
                            // console.log("position y:", position.py);
                            // console.log("position y:", root.recordedPathdir.length);
                            // console.log("dir:", Gyroscope.direction);
                            position.px = 0;
                            position.py = 0;
                            vx = 0
                            vy = 0
                            vz = 0
                        }

                    }
                }

                component NamedProgressBar: ColumnLayout {
                    property alias text: axes.text
                    property alias value: bar.value
                    Text {
                        id: axes
                        font.pixelSize: root.defaultFontSize
                        Layout.fillWidth: true
                    }
                    ProgressBar {
                        id: bar
                        Layout.fillWidth: true
                    }
                }

                ColumnLayout {
                    id: position
                    spacing: 0
                    Layout.fillWidth: true
                    Layout.topMargin: 20

                    property real px: 0
                    property real py: 0
                    property real pz: 0

                    xText: "Position(X): " + px.toFixed(2)
                    xValue: 0.5 + (px / 100)
                    yText: "Position(Y): " + py.toFixed(2)
                    yValue: 0.5 + (py / 100)
                    zText: "Position(Z): " + pz.toFixed(2)
                    zValue: 0.5 + (pz / 100)

                    property alias xText: xPar.text
                    property alias xValue: xPar.value
                    property alias yText: yPar.text
                    property alias yValue: yPar.value
                    property alias zText: zPar.text
                    property alias zValue: zPar.value

                    NamedProgressBar {
                        id: xPar
                    }

                    NamedProgressBar {
                        id: yPar
                    }

                    NamedProgressBar {
                        id: zPar
                    }
                }


                ColumnLayout {
                    id: rotation
                    spacing: 0
                    Layout.fillWidth: true
                    Layout.topMargin: 20

                    property real rx: 0
                    property real ry: 0
                    property real rz: 0

                    xText: "Position(X): " + rx.toFixed(2)
                    xValue: 0.5 + (rx / 100)
                    yText: "Position(Y): " + ry.toFixed(2)
                    yValue: 0.5 + (ry / 100)
                    zText: "Position(Z): " + rz.toFixed(2)
                    zValue: 0.5 + (rz / 100)

                    property alias xText: xPar2.text
                    property alias xValue: xPar2.value
                    property alias yText: yPar2.text
                    property alias yValue: yPar2.value
                    property alias zText: zPar2.text
                    property alias zValue: zPar2.value

                    NamedProgressBar {
                        id: xPar2
                    }

                    NamedProgressBar {
                        id: yPar2
                    }

                    NamedProgressBar {
                        id: zPar2
                    }
                }

                Text {
                    text: "distance: " + root.recordedPathdis.join(", ")
                }

                Text {
                    id: authStatus
                    text: "Auth: "
                }
                
                Button {
                    id: startStopButton
                    text: root.isRecording ? "Stop Recording" : "Start Recording"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    onClicked: {
                        root.startStopRecording()
                        startStopButton.text = root.isRecording ? "Stop Recording" : "Start Recording"
                    }
                }

                Button {
                    id: startStopAuthButton
                    text: root.isAuth ? "Stop Auth" : "Start Auth"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    onClicked: {
                        root.startStopAuth()
                        startStopAuthButton.text = root.isAuth ? "Stop Auth" : "Start Auth"
                        if (!root.isAuth)
                        {
                            root.auth()
                        }
                        else
                        {
                            root.newPathdis = []
                            root.newPathdir = []
                        }
                    }
                }


                Button {
                    text: "Calibrate"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    onClicked: root.calibrate()
                }
                Button {
                    text: "Reset"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    onClicked: root.reset()
                }
            }
        }
    }



    footer: Rectangle {
        height: 60
        color: SensorSupport.hasGyroscope() && SensorSupport.hasAccelerometer() ? "green" : "red"

        Text {
            anchors.centerIn: parent
            text: SensorSupport.hasGyroscope() ? (SensorSupport.hasAccelerometer() ? "Sensors are ready!" : "Accelerometer not available!" )
                    : (SensorSupport.hasAccelerometer() ? "Gyroscope not available!" : "Gyroscope and Accelerometer not available!")
        }
    }
}
