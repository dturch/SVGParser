'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

// get the client
const mysql = require('mysql2/promise');

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

	//added to prevent non.svg images from being added - Dario Turchi
	if (uploadFile.name.slice(uploadFile.name.length - 4) != ".svg") {
		return res.status(400).send(uploadFile.name + " cannot be uploaded! upload svg files only! Please go back and select a different file!");
	}

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

/******************** Your code goes here ******************** 
 * file    		app.js
 * author  		Dario Turchi
 * studentID 	0929012
 * lastEdit     April 13, 2020
 */
app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

let lib = ffi.Library('./libsvgparse', {
	'getSVGInfo': ['string', ['string']],
	'getShapesInfo': ['string', ['string']],
	'getAttrInfo': ['string', ['string']],
	'createSVG': ['string', ['string', 'string']],
	'isValid': ['bool', ['string']],
});

app.get('/getFiles', function (req, res) {
	var r = [];
	let files = fs.readdirSync('./uploads');
	let i = 0;
	while (i < files.length) {
		if (files[i].slice(files[i].length - 4) != ".svg" || !lib.isValid("uploads/" + files[i])) {
			console.log("invalid file: " + files[i] + " found in uploads/ folder!");
			files.splice(i, 1); // doesn't include invalid files! but its still inside upload folder!!!!
			i = 0;
		}
		console.log("valid file: " + files[i] + " found in uploads/ folder!");
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

app.get('/components-properties/:filename', function (req, res) {
	let file = req.params.filename;
	let c = lib.getAttrInfo(file);
	res.send(c);
});

app.get('/svgcreate', function (req, res) {
	let file = req.query.filename;
	let c = lib.createSVG(file, req.query.svgJSON);
	res.send(c);
});

app.get('/edit-svg/:filename', function (req, res) {
	let file = req.params.filename;
	let c = lib.createSVG(file, req.query.svgJSON);
	res.send(c);
});

app.get('/edit-attr/:filename', function (req, res) {
	let file = req.params.filename;
	let c = lib.getAttrInfo(file);
	res.send(c);
});

// app.get('/addRectangle', function (req, res) {
// 	let response = lib.addComponent(req.query.filename, JSON.stringify(req.query.rect));
// 	res.send(resonse);
// });

// app.get('/addCircle', function (req, res) {
// 	let response = lib.addComponent(req.query.filename, JSON.stringify(req.query.rect));
// 	res.send(resonse);
// });

//**********************A4 Functions **********************

let cred = {};
let connection;
// queries
let query_file = `CREATE TABLE IF NOT EXISTS FILE (
	svg_id INT NOT NULL AUTO_INCREMENT, 
	file_name VARCHAR(60) NOT NULL,
	file_title VARCHAR(256),
	file_description VARCHAR(256),
	n_rect INT NOT NULL,
	n_circ INT NOT NULL,
	n_path INT NOT NULL,
	n_group INT NOT NULL,
	creation_time DATETIME NOT NULL,
	file_size INT NOT NULL, PRIMARY KEY (svg_id))`;

let query_image_change = `CREATE TABLE IF NOT EXISTS IMG_CHANGE (
	change_id INT NOT NULL AUTO_INCREMENT, 
	change_type VARCHAR(256) NOT NULL,
	change_summary VARCHAR(256) NOT NULL,
	change_time DATETIME NOT NULL,
	svg_id INT NOT NULL,
	PRIMARY KEY (change_id),
	FOREIGN KEY (svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)`;

let query_download = `CREATE TABLE IF NOT EXISTS DOWNLOAD (
	download_id INT NOT NULL AUTO_INCREMENT, 
	d_descr VARCHAR(256),
	svg_id INT NOT NULL,
	PRIMARY KEY (download_id), 
	FOREIGN KEY (svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)`;


// dbms
app.get('/dbms', async function (req, res) {
	cred = {
		host: 'dursley.socs.uoguelph.ca',
		user: req.query.username,
		password: req.query.password,
		database: req.query.database
	}

	var status;

	try {
		connection = await mysql.createConnection(cred);

		await connection.execute(query_file);
		await connection.execute(query_image_change);
		await connection.execute(query_download);

		status = "Success";
	} catch (err) {
		console.log("Query error: " + err);
		status = "Failure";
	} finally {
		if (connection && connection.end) connection.end();
	}

	console.log(status);
	res.send(status);
});

app.get('/db-status', async function (req, res) {
	cred = {
		host: 'dursley.socs.uoguelph.ca',
		user: req.query.username,
		password: req.query.password,
		database: req.query.dbname
	}

	var r = [];

	try {
		connection = await mysql.createConnection(cred);

		console.log('Success');

		let [rows1, fields1] = await connection.execute('SELECT (SELECT COUNT(*) FROM FILE) AS fc;');
		let [rows2, fields2] = await connection.execute('SELECT (SELECT COUNT(*) FROM IMG_CHANGE) AS icc;');
		let [rows3, fields3] = await connection.execute('SELECT (SELECT COUNT(*) FROM DOWNLOAD) AS dc;');

		for (let row of rows1) {
			console.log(row.fc);
			r[0] = row.fc;
		}
		for (let row of rows2) {
			console.log(row.icc);
			r[1] = row.icc;
		}
		for (let row of rows3) {
			console.log(row.dc);
			r[2] = row.dc;
		}
	} catch (e) {
		console.log("Query error: " + e);
	} finally {
		//Close the connection  
		if (connection && connection.end) connection.end();
	}

	res.send(r);
});