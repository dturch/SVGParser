# CIS*2750 Assignment 4 - README
## Installation and init Start of Node App
### 1. npm install
- run 'npm install' command in the root of SVGApp/ directory 
    !!!NOTE RUN THIS while SSH into cis2750.socs.uoguelph.ca

### 2. create libsvgparse.so
- go to parser/ directory
- run 'make' commmand
- make will create libsvgparse.so in root of SVGApp/

### 3. Running Server
- go to root of SVGApp/ directory
- run 'npm run dev xxxxx' command (where xxxxx is port number)

### Database section (New)
credentials to login
------------------------
Username:   dturchi
Password:   0929012
database:   dturchi
------------------------
- db actions will show themselves once logged in.
- database can log in, connect to dturchi, can also CREATE TABLES IF NOT EXIST
- upload all VALID files to dturchi.FILE table
- clear all rows in all tables in dturchi database
- refresh database status
- Currently Query 1 is working 

Not implemented
-----------------------
- Queries 2-6 (Partial UI is there with minimal feedback from index.js on button clicks)
- Changes aren't tracked
- Downloads aren't tracked