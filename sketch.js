let classifier;
let imageModelURL = 'https://teachablemachine.withgoogle.com/models/TDFDnQPHj/';
let video;
let flippedVideo;
let label = "";

// Serial port variable
let serial;
let portName = 'COM15'; // Ganti dengan nama port Arduino

function preload() {
  classifier = ml5.imageClassifier(imageModelURL + 'model.json');
}

function setup() {
  createCanvas(320, 260);
  video = createCapture(VIDEO);
  video.size(320, 240);
  video.hide();

  flippedVideo = ml5.flipImage(video);
  classifyVideo();

  // Inisialisasi serial port
  serial = new p5.SerialPort();
    // print version of p5.serialport library
  console.log('p5.serialport.js ' + serial.version);
  serial.list();
  serial.openPort(portName);
  serial.on('error', gotError);
  serial.on('list', printList);
  serial.on('connected', serverConnected);
 
  serial.on('data', gotData);
 
}

function draw() {
  background(0);
  image(flippedVideo, 0, 0);

  fill(255);
  textSize(16);
  textAlign(CENTER);
  text(label, width / 2, height - 4);
}

function classifyVideo() {
  flippedVideo = ml5.flipImage(video);
  classifier.classify(flippedVideo, gotResult);
  flippedVideo.remove();
}

function gotResult(error, results) {
  if (error) {
    console.error(error);
    return;
  }
  label = results[0].label;
 

  // Kirim instruksi ke Arduino berdasarkan label yang dikenali
  if (label === "idle") {
    serial.write('L');
  } else if (label === "user1") {
    serial.write('H');
  }

  classifyVideo();
}

function printList(portList) {
  for (let i = 0; i < portList.length; i++) {
    print(i + " " + portList[i]);
  }
}

function serverConnected() {
  print('Connected to the server.');
}

function portOpen() {
  print('Serial port opened.');
}

function gotError(err) {
  console.error('Serial port error: ' + err);
  // Handle the error as needed
}


function gotData() {
  let currentString = serial.readLine();
  console.log(currentString);
}
