'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
	res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
	//Feel free to change the contents of style.css to prettify your Web app
	res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
	fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
		const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
		res.contentType('application/javascript');
		res.send(minimizedContents._obfuscatedCode);
	});
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
	if (!req.files) {
		return res.status(400).send('No files were uploaded.');
	}

	let uploadFile = req.files.uploadFile;

	// Use the mv() method to place the file somewhere on your server
	uploadFile.mv('uploads/' + uploadFile.name, function (err) {
		if (err) {
			return res.status(500).send(err);
		}

		res.redirect('/');
	});
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
	fs.stat('uploads/' + req.params.name, function (err, stat) {
		if (err == null) {
			res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
		} else {
			console.log('Error in file downloading route: ' + err);
			res.send('');
		}
	});
});

//******************** Your code goes here ******************** 

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

let lib = ffi.Library('./libsvgparse', {
	'getSVGInfo': ['string', ['string']],
	'getShapesInfo': ['string', ['string']],
	'createSVG': ['string', ['string', 'string']],
	'addComponent': ['void', ['string', 'string']],
	'isValid': ['bool', ['string']],
});

app.get('/getFiles', function (req, res) {
	var r = [];
	let files = fs.readdirSync('./uploads');
	let i = 0;
	while (i < files.length) {
		if (files[i].slice(files[i].length - 4) != ".svg" || !lib.isValid("uploads/" + files[i])) {
			console.log("invalid file: " + files[i] + "found in uploads/ folder!");
			files.splice(i, 1);
			i = 0;
		}
		console.log("valid file: "+files[i]+" found in uploads/ folder!");
		let c = lib.getSVGInfo(files[i]);
		var stats = fs.statSync('./uploads/' + files[i]);
		var kb = Math.round(stats.size / 1024);
		let jsonObj = JSON.parse(c);
		jsonObj["filename"] = files[i];
		jsonObj.sizeKB = kb
		r[i] = JSON.stringify(jsonObj);
		i++;
	}
	res.send(r);
});

app.get('/components/:filename', function (req, res) {
	let file = req.params.filename;
	let c = lib.getShapesInfo(file);
	res.send(c);
});

app.get('/svgcreate', function (req, res) {
	let file = req.query.filename;
	let c = lib.createSVG(file, req.query.svgJSON);
	res.send(c);
});

app.get('/addRectangle', function (req, res) {
	let response = lib.addComponent(req.query.filename, JSON.stringify(req.query.rect));
	res.send(resonse);
});

app.get('/addCircle', function (req, res) {
	let response = lib.addComponent(req.query.filename, JSON.stringify(req.query.rect));
	res.send(resonse);
});