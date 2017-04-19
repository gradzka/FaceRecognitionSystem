# FaceRecognitionSystem
Face recognition console application.

## Overview
**FaceRecognitionSystem** is the project of simple console application intended to recognition faces.

## Description
Project is based on simple console interface used to manipulate IP camera and start face recognition algorithm. First step is to provide the user with the module of taking picture using IP camera. It is necessary to make entry personal data (pictures of faces) into corps easier. User can type name of the folder and take any number of screenshots stored in it. Next step is to start learning algorithm based on data included in corps. After this process, user can choose one of two modes: infinite and time mode. Infinite mode runs until the escape button will be pressed. The second runs for a period of time defined by the user, what is more, this mode can also be exited by the escape button. Information of recognized and unrecognized people will be saved in the log file. Log file will be named the start date of the algorithm.


## Progress
- [x] analysis of the HTTP messages,
- [x] communication with the IP camera,
- [x] making screenshots on demand,
- [x] configuration file,
- [x] corps preparation,
- [x] face detection,
- [x] count people,
- [ ] start learning algorithm,
- [ ] program modes,
- [ ] face recognition,
- [ ] save results in log file,
- [ ] errors handling, tests.

## Tools
Microsoft Visual Studio 2017

## How to run

## How to compile

## Attributions
* http://opencv.org/
* https://github.com/yushulx/libcurl-sample
* http://www.cplusplus.com/forum/windows/105443/#msg569057
* http://stackoverflow.com/a/25383466
* http://stackoverflow.com/a/106223
* http://stackoverflow.com/questions/3572397/lib-curl-in-c-disable-printing
* http://codingstreet.com/c-code-to-hide-input-password/
* https://github.com/mimoo/DES
* http://stackoverflow.com/questions/18937892/c-string-to-binary-code-binary-code-to-string
* https://codereview.stackexchange.com/questions/25381/decimal-binary-hex-converter
* http://stackoverflow.com/questions/23344257/convert-a-string-of-binary-into-an-ascii-string-c
* http://stackoverflow.com/questions/236129/split-a-string-in-c

## License
MIT

## Credits
* Monika Grądzka,
* Robert Kazimierczak
