A CYOA story game(adventure_stories): 
A school evaluative project to demonstrate object-oriented principles, algorithms, and data structures in C++.

This project generates 4 executables:
- cyoa-step1: Prints out all the pages in the story. 
    - Parser in cyoa.cpp to parse the story.txt file to retrieve page and choice metadata for building the story.
- cyoa-step2: Allows users to play the story.
    
- cyoa-step3: Prints out all the possible paths to win the story
    - Applies a common graph algorithm to traverse the story and find all winning paths
   
- cyoa-step4: Adds extra functionality to the gameplay

Structure:
- src >> header: all header files with definitions of classes
- src >> impl: all implementations of classes

