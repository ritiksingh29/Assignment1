# Assignment1
- project_folder/
    - server.cpp  (C++ back-end)
    - index.html  (HTML front-end)
    - app.js      (JavaScript logic for front-end)
    - data.sql    (to save the rules and evaluation if needed)
 To run the assignmen
    Open the server.cpp and run the code git clone https://github.com/CrowCpp/Crow.git to download the crow library to setup the server.
         then follow the command line to activate the server.
           i. g++ -std=c++11 -o server server.cpp -lpthread
           ii. ./server
           iii. http://localhost:8080/

  then open the UI.html to open the create and evaluate the rule.

1. server.cpp (C++ Back-End with AST and Crow Framework)
This C++ file defines the AST structure, rule creation, combination, and evaluation logic. The Crow framework is used to handle HTTP requests.
2. index.html (Front-End HTML Form)
This is the user interface to input rules and evaluate them.
3. app.js (Front-End JavaScript Logic)
This JavaScript file handles the interaction between the front-end and the back-end.


          
