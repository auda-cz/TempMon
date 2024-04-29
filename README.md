# TempMon
This project provides a web interface to monitor the temperature hand humidity provided by a SHTC3 sensor in my bedroom. It contains several sub-projects:

- [SaveTH](SaveTH/): contains 2 parts, recording sensor data and updating the database. The saving file structures are especially designed by the web interface
    - [mainFetch.cpp](SaveTH/src/mainFetch.cpp): executable to read temp and hum data from the sensor. By default, it makes 6 measurements with 6 ms interludes and take the average. The data will be appended into a `.json` named by the date. This is a direct implementation of the [`SHTC3x-pigpio`](https://github.com/auda-cz/SHTC3x-pigpio) interface.
    
        Usage:
    ` sudo ./mainFetch <filename> <avging number>`

    - [hourAvg.cpp](SaveTH/src/hourAvg.cpp): A really sketchy executable to read the last 12 measurement from the `.json` generated eariler and take average. This is especially designed for the web interface.
    Requires `nlohmann/json`
- [Web Interface](var/www/html/tm/): A forked project which appears to be stitches from a number of project. It reads from the `.json` database and present the data in a plot.

## License
This project is released under GNU GPL-3.