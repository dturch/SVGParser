## Installation of Node App

### 1. Install

```Bash
# From the root of the directory
npm install
```

### 2. Running Server

```Bash
# PORT is your personally given port number, e.g. 1234
npm run dev PORT
# Server will be accessible at http://localhost:PORT
```

## Directory Structure

```Bash
# This contains the Backend Node Server, with our Web Application and API
app.js

# These are the package configuration files for npm to install dependencies
package.json
package-lock.json

# This is the Frontend HTML file that you see when you visit the document root
public/index.html

# This is the Fro
public/index.jsntend browser JavaScript file

# This is the Frontend Custom Style Sheet file
public/style.css

# This is the directory for uploaded .vcf files
upload/

# This is the directory where you put all your C parser code
parser/
```
Note: tested and working on Node.js ver.12.13.0


### 2. create libsvgparse.so
- go to 'parser' directory
- run 'make' commmand
- make will compile shared library 'libsvgparse.so' in root of 'SVGApp' directory

### 3. Running Server
- go to root of 'SVGApp' directory
- run 'npm run dev xxxxx' command (where xxxxx is port number of your choice)

### 4. Access in Browser
- open browser of your choice (Chrome, Firefox, Edge)
- go to 'localhost:xxxxx' (where xxxxx is port number you chose to run server on in step 3)
