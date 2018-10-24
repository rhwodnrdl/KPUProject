var express = require('express');
var session = require('express-session');
var bodyParser = require('body-parser');
var app = express();

var awsIot = require('aws-iot-device-sdk');

var device = awsIot.device({
	keyPath:'certs/1e564a5594-private.pem.key',
	certPath:'certs/1e564a5594-certificate.pem.crt',
	caPath:'certs/root-CA.crt',
	clientId:'WebApp',
	host:'a1jh97is05aht9.iot.us-west-2.amazonaws.com'
});

var AWS = require('aws-sdk'); // Load the AWS SDK for Node.js

// Set the region
AWS.config.update({region: 'us-west-2'});

// Create DynamoDB service object
var ddb = new AWS.DynamoDB({apiVersion: '2012-08-10'});

var params = {
  TableName: "Control_DB",
//  ProjectionExpression: "deviceId, mqttdata", // 기본키 아닌것만
//  ExpressionAttributeNames: {
//        "#time": "timestamp",
//  },
};

var ws1_topic_temp; var ws1_payload_temp;
var ws1_topic_humid; var ws1_payload_humid;
var ws1_topic_co2; var ws1_payload_co2;
var ws1_topic_lpg; var ws1_payload_lpg;
var ws2_topic_temp; var ws2_payload_temp;
var ws2_topic_humid; var ws2_payload_humid;
var ws2_topic_co2; var ws2_payload_co2;
var ws2_topic_lpg; var ws2_payload_lpg;
var ws3_topic_temp; var ws3_payload_temp;
var ws3_topic_humid; var ws3_payload_humid;
var ws3_topic_co2; var ws3_payload_co2;
var ws3_topic_lpg; var ws3_payload_lpg;

var ws1_control_fan;
var ws2_control_fan;
var ws3_control_fan;

var json;
var parse;

app.locals.pretty = true; // 최종적 html코드를 예쁘게 해줌
app.set('view engine', 'jade'); // express와 jade연결
app.set('views', './views'); // views 디렉터리 연결

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: false }));
app.use(session({
  secret: 'kkulppo1234!@#$1212!@!@',
  resave: false,
  saveUninitialized: true
}));

device.on('connect', function() {
	console.log('connect');
	device.subscribe('data/ws1/temp');
	device.subscribe('data/ws1/humid');
	device.subscribe('data/ws1/co2');
	device.subscribe('data/ws1/lpg');
	device.subscribe('data/ws2/temp');
	device.subscribe('data/ws2/humid');
	device.subscribe('data/ws2/co2');
	device.subscribe('data/ws2/lpg');
	device.subscribe('data/ws3/temp');
	device.subscribe('data/ws3/humid');
	device.subscribe('data/ws3/co2');
	device.subscribe('data/ws3/lpg');
});

device.on('message', function(topic, payload) {
console.log('message:', topic, payload.toString());

if (topic == 'data/ws1/temp') {
	ws1_topic_temp = topic;
	ws1_payload_temp = payload.toString();
}
else if (topic == 'data/ws1/humid') {
	ws1_topic_humid = topic;
	ws1_payload_humid = payload.toString();
}
else if (topic == 'data/ws1/co2') {
	ws1_topic_co2 = topic;
	ws1_payload_co2 = payload.toString();
}
else if (topic == 'data/ws1/lpg') {
	ws1_topic_lpg = topic;
	ws1_payload_lpg = payload.toString();
}
else if (topic == 'data/ws2/temp') {
	ws2_topic_temp = topic;
	ws2_payload_temp = payload.toString();
}
else if (topic == 'data/ws2/humid') {
	ws2_topic_humid = topic;
	ws2_payload_humid = payload.toString();
}
else if (topic == 'data/ws2/co2') {
	ws2_topic_co2 = topic;
	ws2_payload_co2 = payload.toString();
}
else if (topic == 'data/ws2/lpg') {
	ws2_topic_lpg = topic;
	ws2_payload_lpg = payload.toString();
}
else if (topic == 'data/ws3/temp') {
	ws3_topic_temp = topic;
	ws3_payload_temp = payload.toString();
}
else if (topic == 'data/ws3/humid') {
	ws3_topic_humid = topic;
	ws3_payload_humid = payload.toString();
}
else if (topic == 'data/ws3/co2') {
	ws3_topic_co2 = topic;
	ws3_payload_co2 = payload.toString();
}
else if (topic == 'data/ws3/lpg') {
	ws3_topic_lpg = topic;
	ws3_payload_lpg = payload.toString();
}
});

app.get('/dashboard/sectionA', function(req, res){
  //res.render('dashboard/sectionA');
	res.sendFile(__dirname + "/views/dashboard/sectionA.html")
});

app.get('/dashboard/sectionB', function(req, res){
	res.sendFile(__dirname + "/views/dashboard/sectionB.html")
});

app.get('/dashboard/sectionC', function(req, res){
	res.sendFile(__dirname + "/views/dashboard/sectionC.html")
});

app.get('/control/sectionA', function(req, res){
	res.sendFile(__dirname + "/views/control/sectionA.html")
});

app.get('/control/sectionB', function(req, res){
	res.sendFile(__dirname + "/views/control/sectionB.html")
});

app.get('/control/sectionC', function(req, res){
	res.sendFile(__dirname + "/views/control/sectionC.html")
});

app.get('/auto/sectionA', function(req, res){
	res.sendFile(__dirname + "/views/auto/sectionA.html")
});

app.get('/auto/sectionB', function(req, res){
	res.sendFile(__dirname + "/views/auto/sectionB.html")
});

app.get('/auto/sectionC', function(req, res){
	res.sendFile(__dirname + "/views/auto/sectionC.html")
});

app.post('/ws1_reload', function(req, res){ // ajax 응답
	var responseData = {'result' : 'ok',
		'topic_temp' : ws1_topic_temp, 'payload_temp' : ws1_payload_temp,
		'topic_humid' : ws1_topic_humid, 'payload_humid' : ws1_payload_humid,
		'topic_co2' : ws1_topic_co2, 'payload_co2' : ws1_payload_co2,
		'topic_lpg' : ws1_topic_lpg, 'payload_lpg' : ws1_payload_lpg,
	};
	res.json(responseData);
});

app.post('/ws2_reload', function(req, res){ // ajax 응답
	var responseData = {'result' : 'ok',
		'topic_temp' : ws2_topic_temp, 'payload_temp' : ws2_payload_temp,
		'topic_humid' : ws2_topic_humid, 'payload_humid' : ws2_payload_humid,
		'topic_co2' : ws2_topic_co2, 'payload_co2' : ws2_payload_co2,
		'topic_lpg' : ws2_topic_lpg, 'payload_lpg' : ws2_payload_lpg,
	};
	res.json(responseData);
});

app.post('/ws3_reload', function(req, res){ // ajax 응답
	var responseData = {'result' : 'ok',
		'topic_temp' : ws3_topic_temp, 'payload_temp' : ws3_payload_temp,
		'topic_humid' : ws3_topic_humid, 'payload_humid' : ws3_payload_humid,
		'topic_co2' : ws3_topic_co2, 'payload_co2' : ws3_payload_co2,
		'topic_lpg' : ws3_topic_lpg, 'payload_lpg' : ws3_payload_lpg,
	};
	res.json(responseData);
});

app.post('/ws1_control', function(req, res){
	if(req.body.control == "fan_on") {
		console.log("fan_on...");
		device.publish('ws1/control/fan', "{\"data\" : \"2\"}"); // 설비 동작
		console.log("publish : {\"data\" : \"2\"}");
		ws1_control_fan = 2; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_off") {
		console.log("fan_off...");
		device.publish('ws1/control/fan', "{\"data\" : \"1\"}"); // 설비 정지
		console.log("publish : {\"data\" : \"1\"}");
		ws1_control_fan = 1; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_auto") {
		console.log("fan_auto...");
		device.publish('ws1/control/fan', "{\"data\" : \"3\"}"); // 설비 자동제어
		console.log("publish : {\"data\" : \"3\"}");
		ws1_control_fan = 3; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "light_on") {
		console.log("light_on...");
		device.publish('ws1/control/light', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "light_off") {
		console.log("light_off...");
		device.publish('ws1/control/light', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	else if (req.body.control == "buzzer_on") {
		console.log("buzzer_on...");
		device.publish('ws1/control/buzzer', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "buzzer_off") {
		console.log("buzzer_off...");
		device.publish('ws1/control/buzzer', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	var responseData = {'result' : 'ok'}
	res.json(responseData)
});

app.post('/ws2_control', function(req, res){
	if(req.body.control == "fan_on") {
		console.log("fan_on...");
		device.publish('ws2/control/fan', "{\"data\" : \"2\"}"); // 설비 동작
		console.log("publish : {\"data\" : \"2\"}");
		ws2_control_fan = 2; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_off") {
		console.log("fan_off...");
		device.publish('ws2/control/fan', "{\"data\" : \"1\"}"); // 설비 정지
		console.log("publish : {\"data\" : \"1\"}");
		ws2_control_fan = 1; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_auto") {
		console.log("fan_auto...");
		device.publish('ws2/control/fan', "{\"data\" : \"3\"}"); // 설비 자동제어
		console.log("publish : {\"data\" : \"3\"}");
		ws2_control_fan = 3; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "light_on") {
		console.log("light_on...");
		device.publish('ws2/control/light', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "light_off") {
		console.log("light_off...");
		device.publish('ws2/control/light', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	else if (req.body.control == "light_auto") {
		console.log("light_auto...");
		device.publish('ws2/control/light', "{\"data\" : \"3\"}");
		console.log("publish : {\"data\" : \"3\"}");
	}
	else if (req.body.control == "buzzer_on") {
		console.log("buzzer_on...");
		device.publish('ws2/control/buzzer', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "buzzer_off") {
		console.log("buzzer_off...");
		device.publish('ws2/control/buzzer', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	else if (req.body.control == "buzzer_auto") {
		console.log("buzzer_auto...");
		device.publish('ws2/control/buzzer', "{\"data\" : \"3\"}");
		console.log("publish : {\"data\" : \"3\"}");
	}
	var responseData = {'result' : 'ok'}
	res.json(responseData)
});

app.post('/ws3_control', function(req, res){
	if(req.body.control == "fan_on") {
		console.log("fan_on...");
		device.publish('ws3/control/fan', "{\"data\" : \"2\"}"); // 설비 동작
		console.log("publish : {\"data\" : \"2\"}");
		ws3_control_fan = 2; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_off") {
		console.log("fan_off...");
		device.publish('ws3/control/fan', "{\"data\" : \"1\"}"); // 설비 정지
		console.log("publish : {\"data\" : \"1\"}");
		ws3_control_fan = 1; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "fan_auto") {
		console.log("fan_auto...");
		device.publish('ws3/control/fan', "{\"data\" : \"3\"}"); // 설비 자동제어
		console.log("publish : {\"data\" : \"3\"}");
		ws3_control_fan = 3; // 설비의 현재 상황을 알기위한 변수
	}
	else if (req.body.control == "light_on") {
		console.log("light_on...");
		device.publish('ws3/control/light', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "light_off") {
		console.log("light_off...");
		device.publish('ws3/control/light', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	else if (req.body.control == "light_auto") {
		console.log("light_auto...");
		device.publish('ws3/control/light', "{\"data\" : \"3\"}");
		console.log("publish : {\"data\" : \"3\"}");
	}
	else if (req.body.control == "buzzer_on") {
		console.log("buzzer_on...");
		device.publish('ws3/control/buzzer', "{\"data\" : \"2\"}");
		console.log("publish : {\"data\" : \"2\"}");
	}
	else if (req.body.control == "buzzer_off") {
		console.log("buzzer_off...");
		device.publish('ws3/control/buzzer', "{\"data\" : \"1\"}");
		console.log("publish : {\"data\" : \"1\"}");
	}
	else if (req.body.control == "buzzer_auto") {
		console.log("buzzer_auto...");
		device.publish('ws3/control/buzzer', "{\"data\" : \"3\"}");
		console.log("publish : {\"data\" : \"3\"}");
	}
	var responseData = {'result' : 'ok'}
	res.json(responseData)
});

app.post('/ws1_setting_temp', function(req, res){
	if(ws1_control_fan == "1" || ws1_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws1/setting/temp', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("temp setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws1_setting_humid', function(req, res){
	if(ws1_control_fan == "1" || ws1_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws1/setting/humid', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("humid setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws1_setting_co2', function(req, res){
	if(ws1_control_fan == "1" || ws1_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws1/setting/co2', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("co2 setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws1_setting_lpg', function(req, res){
	device.publish('ws1/setting/lpg', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
	console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
	console.log("lpg setting :",req.body.setting);

	var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
	res.json(responseData)
});

app.post('/ws2_setting_temp', function(req, res){
	if(ws2_control_fan == "1" || ws2_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws2/setting/temp', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("temp setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws2_setting_humid', function(req, res){
	if(ws2_control_fan == "1" || ws2_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws2/setting/humid', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("humid setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws2_setting_co2', function(req, res){
	if(ws2_control_fan == "1" || ws2_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws2/setting/co2', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("co2 setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws2_setting_lpg', function(req, res){

	device.publish('ws2/setting/lpg', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
	console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
	console.log("lpg setting :",req.body.setting);

	var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
	res.json(responseData)
});

app.post('/ws3_setting_temp', function(req, res){
	if(ws3_control_fan == "1" || ws3_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws3/setting/temp', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("temp setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws3_setting_humid', function(req, res){
	if(ws3_control_fan == "1" || ws3_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws3/setting/humid', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("humid setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws3_setting_co2', function(req, res){
	if(ws3_control_fan == "1" || ws3_control_fan == "2"){
		var responseData = {'result' : 'nok'} // 수동제어 중일때는 nok
		res.json(responseData)
	}
	else {
		device.publish('ws3/setting/co2', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
		console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
		console.log("co2 setting :",req.body.setting);

		var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
		res.json(responseData)
	}
});

app.post('/ws3_setting_lpg', function(req, res){

	device.publish('ws3/setting/lpg', "{\"data\" : \""+req.body.setting+"\"}"); // 임계값 설정
	console.log("publish : {\"data\" : \""+req.body.setting+"\"}");
	console.log("lpg setting :",req.body.setting);

	var responseData = {'result' : 'ok'} // 자동제어 중일때는 ok
	res.json(responseData)
});

function sortJSON(data, key, way) {
    return data.sort(function(a, b) {
        var x = a[key]; var y = b[key];
        if (way === '123' ) { return ((x < y) ? -1 : ((x > y) ? 1 : 0)); }
        if (way === '321') { return ((x > y) ? -1 : ((x < y) ? 1 : 0)); }
    });
}

app.post('/ws1_dbload', function(req, res){ // ajax 응답

	ddb.scan(params, function(err, data) {
	  if (err) {
	    console.log("Error", err);
	  } else {

			var responseData = [];

			data.Items.forEach(function(element) {

				if(element.Topic.S.indexOf("ws1") != -1){

					var topic = element.Topic.S;
					var data = element.data.M.data.S;
					var timestamp = element.Timestamp.S;

					var tns_json = {};

					tns_json['timestamp'] = timestamp;
					tns_json['topic'] = topic;
					tns_json['data'] = data;

					responseData.push(tns_json);
				}

		});

		var result_response = sortJSON(responseData, "timestamp", '321');

		var obj_length = responseData.length;

		//result_response['len'] = obj_length;

		console.log(obj_length);

		res.json(result_response);

	};
});
});


app.post('/ws2_dbload', function(req, res){ // ajax 응답

	ddb.scan(params, function(err, data) {
	  if (err) {
	    console.log("Error", err);
	  } else {

			var responseData = [];

			data.Items.forEach(function(element) {

				if(element.Topic.S.indexOf("ws2") != -1){

					var topic = element.Topic.S;
					var data = element.data.M.data.S;
					var timestamp = element.Timestamp.S;

					var tns_json = {};

					tns_json['timestamp'] = timestamp;
					tns_json['topic'] = topic;
					tns_json['data'] = data;

					responseData.push(tns_json);
				}

		});

		var result_response = sortJSON(responseData, "timestamp", '321');

		var obj_length = responseData.length;

		//result_response['len'] = obj_length;

		console.log(obj_length);

		res.json(result_response);

	};
});
});

app.post('/ws3_dbload', function(req, res){ // ajax 응답

	ddb.scan(params, function(err, data) {
	  if (err) {
	    console.log("Error", err);
	  } else {

			var responseData = [];

			data.Items.forEach(function(element) {

				if(element.Topic.S.indexOf("ws3") != -1){

					var topic = element.Topic.S;
					var data = element.data.M.data.S;
					var timestamp = element.Timestamp.S;

					var tns_json = {};

					tns_json['timestamp'] = timestamp;
					tns_json['topic'] = topic;
					tns_json['data'] = data;

					responseData.push(tns_json);
				}

		});

		var result_response = sortJSON(responseData, "timestamp", '321');

		var obj_length = responseData.length;

		//result_response['len'] = obj_length;

		console.log(obj_length);

		res.json(result_response);

	};
});
});

app.get('/auth/logout', function(req, res){
  delete req.session.displayName;
  res.redirect('/welcome');
});

app.get('/welcome', function(req, res){
  if(req.session.displayName) {
    res.sendFile(__dirname + "/views/welcome.html");
  } else {
    res.redirect('/auth/login');
  }
});
app.post('/auth/login', function(req, res){
  var user = {
    username:'admin',  // 로그인시 사용 명
    password:'admin',
    displayName:'Admin'  // 화면에 사용할 닉네임
  };
  var uname = req.body.username;
  var pwd = req.body.password;
  if(uname === user.username && pwd === user.password){
    req.session.displayName = user.displayName;
    res.redirect('/welcome');
  } else {
  	res.sendFile(__dirname + "/views/loginError.html");
    //res.send('<script type="text/javascript">alert("아이디나 비밀번호를 확인해주세요.");</script>');
  }
});
app.get('/auth/login', function(req, res){
	res.sendFile(__dirname + "/views/login.html");
});
//지워
app.get('/graph', function(req, res){
	res.sendFile(__dirname + "/graph.html");
});
app.get('/ex', function(req, res){
	res.sendFile(__dirname + "/views/viewEx.html");
});
app.get('/template', function(req, res){
    res.render('temp', {time:Date(), title:'Jade'});
    // temp라고하는 템플릿 파일을 렌더링해서 웹페이지로 전송
    // 두 번째 인자에 변수를 담아서 jade 파일 내부에서
    //사용 할 수 있도록 전송할 수 있음
})
//
app.get('/', function(req, res){
  res.redirect('/auth/login');
});
app.listen(3003, function(){
  console.log('Connected 3003 port!');
});
