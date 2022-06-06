# About ofxLSL

#### ofApp Usage
- The user can provide the stream name they are looking for as a parameter to `lsl.start()` in `ofApp::setup()`
- If using the Python Marker Code (marker.py in the markerTest directory), the default stream name is `DataSyncMarker`

#### Dependency
- In order to run the example, liblsl64.dll must be in the app bin folder with the compiled .exe
- Because lsl libaries are written for x64 systems, ensure that Visual Studio Platforms are set to x64

#### Accuracy
- timestamp,_TS_, is 100% accurate
- timestampLocal, _TSC_, periodicity is at worst accurate to 1.8ms
  - average: 19 microseconds
- localClock, _LC_, periodicity is rather unreliable (>300ms)
- Can be more accurate if tags are sent and recieved on same computer

#### Python Marker Code
- **Marker.py**
  - begins stream that alternates printing "Hello" and "World"
  - Tags sent aproximately every 1.5 sec
- **MarkerCSV.py**
  - same basic function as Marker.py
  - writes the timestamps of the tags to an external file, _sentInfo.csv_
- These files can be used to test the basic functionality of ofxLSL in recieving string tags from a single stream
- Names of the streams are currently set to "DataSyncMarker", but can be changed to suit needs

