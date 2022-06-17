# About ofxLSL
- EmotiBit can synchronize data with LSL by reading timestamps from a marker stream.
- A marker stream is an LSL stream, that periodically spits out timestamped markers. These timestamped markers are picked up by EmotiBit, 
 to sync EmotiBit data to local computer clocks on the LSL network.
- This version of ofxLSL is based on lsl [`1.14.0`](https://github.com/sccn/liblsl/releases/tag/v1.14.0)

## Example for generating marker stream
You may generate a marker stream using the `marker.py` example in `markertest/py` folder.

### About the exmaples
- **Marker.py**
  - begins stream that alternates printing "Hello" and "World"
  - Tags sent aproximately every 1.5 sec

### Requirements
To run the `marker.py` example, you will need the following:

- Install Python
  - We recommend downloading anaconda to create and manage virtual environments.
    - Get anaconda from their website: https://www.anaconda.com/
  - Create an new environment using python 3.7.
    - Follow the instruction on [docs.anaconda](https://docs.anaconda.com/anaconda/navigator/tutorials/manage-environments/#creating-a-new-environment) to create an environment. 
- Install the following Python packages
  - Open anaconda prompt.
    - You may refer [docs.anaconda](https://docs.anaconda.com/anaconda/install/verify-install/#conda) to do so.
  - Activate the environment created in the previous step.
    - To activate, type `conda activate your_env_name` to activate.
  - **psychopy**
    - Run `pip install psychopy` 
  - **liblsl**
    - Follow the instructions on liblsl-Python page: https://github.com/labstreaminglayer/liblsl-Python#installation
    - Brief instructions,
      - Run `conda install -c conda-forge liblsl`
      - Run `pip install pylsl`

### Running the example
- In the conda command prompt, with the correct environment activated, run the command `python marker.py`
- You will see a window pop up with the words "Hello" and "World" alternating.

### Usage Notes
- By deafult, the `markey.py`, example generates the LSL stream with the name `DataSyncMarker` and source_id = `12345`
  - You can change the name and stream_id of the marker stream to your needs, but **remember to change the name 
appropriately at the receiver as well!**

## Example to read LSL markers
- You may use the `ReadMarkerExample` example to test marker streams on the LSL network.
  - The Visual studio project files have been included (for easy build on Windows) 
- This example searches for (and connects to) the specified LSL stream.
- The stream should be specified in `ofApp.cpp`, as shown below
  - `receiveString = std::make_shared<ofxLSL::Receiver<string>>("stream_name", "stream_id");`
  - Example
    - `receiveString = std::make_shared<ofxLSL::Receiver<string>>("DataSyncMarker", "123345");`
### Notes
- For Windows users
  - Make sure the `lsl.dll` file is in bin folder when running the example.


