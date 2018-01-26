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
- [X] start learning algorithm,
- [X] program modes,
- [X] face recognition,
- [X] save results in log file,
- [X] errors handling, tests.

## Tools
Microsoft Visual Studio 2017

## Attributions
* http://opencv.org/
* https://youtu.be/Eu7NFeg43T4
* http://www.cplusplus.com/forum/windows/105443/#msg569057
* http://stackoverflow.com/a/25383466
* http://stackoverflow.com/a/106223
* http://stackoverflow.com/questions/3572397/lib-curl-in-c-disable-printing
* http://codingstreet.com/c-code-to-hide-input-password/
* https://github.com/B-Con/crypto-algorithms
* http://stackoverflow.com/questions/236129/split-a-string-in-c
* http://forbot.pl/blog/artykuly/programowanie/opencv-3-detekcja-i-rozpoznawanie-twarzy-id5664
* https://www.learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/
* http://stackoverflow.com/a/37494654
* http://stackoverflow.com/a/24271424
* http://stackoverflow.com/a/18685338

## Credits
* Monika Grądzka,
* Robert Kazimierczak
