# Userstories

User stories that should be verified using either unit or description based
 testing for acceptance criteria.

## 1
As a player I want the turtles to move away from me when I 
come near such that I can try to increase my score by achieving 
the objective.

Standard Scenario:
- Given that I am a player that is actively playing the game 
and the game recognizes me as a active player using IMOVE  
- When I walk towards a turtle projected onto the ground
- Then I want the turtle to move away from me 
and I want to see a visual indication that I am 
indeed influencing the movement of the turtle (e.g. it looks afraid or surprised and moves away)
		
Edge Case:
- Given that I am a player that is actively playing the game 
		and the game recognizes me as a active player using IMOVE 
- When I walk towards a turtle projected onto the ground 
		and that turtle is at the edge of the playing area
- Then I want the turtle to reset back to the spawn location in the center
		so that the turtle does not become stuck at the edge of the 
		playing area 
- and I want to see a visual indication that the turtle is running 
back to the center because then I know that I will not be able to 
influence it and guide it to another position. 

## 2
As a player I want to know the rules and controls of the game, 
either by a tutorial or intuitive trial and error, 
to ensure that I have a pleasant experience playing the game.

Standard Scenario:
- Given that I am a player that has not joined the game yet
and that I have never played the game before. 
- When I decide to participate and join the playing area
- Then I want to be able to intuitively understand how the game
works by looking at the playing field and observing the resulting
interactions when I walk around

## 3

Standard Scenario:
- Given that I am a player that is actively playing the game
- and the game recognizes me as a active player using IMOVE
-and that there is at least one other active player in the playing area
- When I play the game together with the other player(s)
by making an effort to move the turtles to an objective area
- Then I want the turtles to move away from both me and the other player(s)
such that the turtle can still be guided effectively towards the 
objective area

Edge Case:
- Given that I am a player that is actively playing the game
- and the game recognizes me as a active player using IMOVE
- and that there is at least one other active player in the playing area
- When I play the game together with the other player(s)
and me and the other player(s) trap the turtle by surrounding it from 
different angles
- Then I want the turtle behaviour to remain stable such that the location of the turtle does
not osscilate between different players in an unnatural way
- and the turtle is able to move correctly again when it is not surrounded 
anymore

## 4
As a player I want to be able to have an interactive experience with the game before I play so that I feel invited to play the game.
 
Standard Scenario:
- Given that I am walking in the playing area
- and the game is not in progress yet
- When I walk around in the playing area
- and observe the projection of the playing field
- Then I want to see that the game responds to my presence by 
seeing visual indicators based on my location like: footsteps, animations of turtles responding to me or obstacles/trees that shake as I move past them.
    
Edge Case 1: 
- Given that I am walking in the playing area
- and the game is not in progress yet
- When I walk around in the playing area
- and observe the projection of the playing field
- and I am in the playing area for long enough to trigger the game to start
- Then I want to see a visual indication that the game is starting
- and I want to be able to still leave the playing field in time if I do not want to start the game
    
Edge Case 2: 
- Given that I am walking in the playing area
- and the game is not in progress yet
- When I walk around in the playing area
- and observe the projection of the playing field
- and I am in the playing area for long enough to trigger the game to start
- Then I want to see a visual indication that the game is starting
- and I want the game to start if I choose to stay in the playing field

## 5
As a player I want see my score go up when I help a turtle reach the sea,
because that means I play the game in the right way.

Standard Scenario:
- Given that I am a player that is actively playing the game
- and the game recognizes me as a active player using IMOVE
- When I walk towards a turtle projected onto the ground
- and I approach the turtle from a direction such that it 
will move into the sea (the objective area)
- Then I want the turtle to move away from me into the sea
- and I want to see a visual indication that the turtle is swimming into the sea
- and I want to see that my score increases

## 6
As a player i want to be able to track my score so that I know 
about my progress in the game.

Standard Scenario:
- Given that I am a player that could be actively playing the game
and that I am looking at the projection
- When a turtle is brought to the sea
- Then I want this action to be reflected in the score display and
I want to be able to see this score at all times such that I can look it up
if i forget the current score
		

Edge Case:
- Given that I am a player that could be actively playing the game
and that I am looking at the projection.
- When the last turtle is brought to the sea by any player
and the game finishes as a result
- Then I want to see the score one last time in the final screen

## 7
As a game supervisor I want to be able to easily launch and terminate the
game without the need of extensive knowledge about the software.

Standard Scenario:
- Given that I am a game supervisor that is supposed to manage the game at a public location
- When I have turned on the devices needed for the IMOVE setup (the projector, the camera and
a computer with the game software installed)
- Then I want to be able to launch the game with a single and accessible action in the
computer

Alternate Standard Scenario:
- Given that I am a game supervisor that is supposed to manage the game at a public location
- When I have turned on the devices needed for the IMOVE setup (the projector, the camera and
a computer with the game software installed) and the game is already running correctly
- Then I want to be able to terminate the game with a single and accessible action in the
computer and I want all processes to be able to be killed automatically at any moment and
I want to be able to relaunch the game at any moment without any effects from the fact that
the game already ran previously

## 8
As a product owner I want the game to be scalable so that the game can be played on a playing field that is smaller or bigger than the original playing field.
 
Standard Scenario:
- Given that the game has to be run on a playing field of a different size
- When I configure IMOVE correctly for the new playing field
- and IMOVE provides the parameters of the playing field (e.g. height and width)
- and I launch the game
- Then I want the game to scale automatically by increasing the size of the playing area and the objective areas
- and I want the turtles and player hit boxes to remain of the same size in order to keep the proportions of the turtles with respect to the players correct
    
Edge Case:
- Given that the game has to be run on a playing field of a different size
- and that the new size is too small for any kind of a positive experience with the game (e.g. the size of the playing field is as big as a single player or turtle)
- When I configure IMOVE correctly for the new playing field
- and IMOVE provides the parameters of the playing field (e.g. height and width)
- and I launch the game
- Then I want to receive a warning from the game that the playing field might be way too small to facilitate a positive experience when playing the game 
- and I want to be made aware that the game is not officially supported at these dimensions
-  but I still want to be able to launch the game so that I can judge for myself whether I want to run the game in the current setup

## 9
As a product owner I want the game to be robust so that it does not require a lot
of assistance from employees to be playable.

Standard Scenario:
- Given I want the game to be run at a new arbitrary public location
and this location has an existing IMOVE setup
and the employee that will be launching/terminating the game (the game supervisor) has
no problems with starting an IMOVE setup including camera and projector.
- When the new game software is installed at this location
- Then I want the game supervisor to be able to launch and terminate without much difficulty
due to good documentation of the software
and I want the game to run all day without any problems or crashes.

## 10
As a game supervisor I do not want to have to continuously monitor the game to see if it still working correctly.

Standard Scenario:
- Given that the game is launched
- When I leave the game running throughout the day
- and I do not supervise whether the game still works throughout the day
- Then I want that the game to still run correctly and without problems when I return at the end of the day to terminate the game.
    
Edge Case:
- Given that I launch the game
- When I leave the game running throughout the day
- and and it turns out that the game crashed at some point during the day
- and I discover that this happened
- Then I want to be able to relaunch the game with a simple action like I would normally launch the game.

## 11
As a developer, I want to make the movement of the turtles look as
realistic and lifelike as possible, such that the player has the best
playing experience.

Standard Scenario:
- Given that I have implemented the movement of the turtles
- When the turtle is moving in random directions
when the game is in progress
- and when the movement of the turtle is affected by a nearby player
- Then I want the movement of the turtle gradually transition
when it changes directions to prevent unnatural behaviour
- and I want the turtle to still have some random deviations
in movement even when a player affects the movement
such that the turtle appears to have some character or an own will,
this will also affect gameplay positively because it adds an additional challenge

## 12 
As a game developer, I want to learn to program C++, such that
I can write clean and robust code for the game.

Standard Scenario
- Given that I have to write code for a game that has to be robust and stable
because it must be able to run for an entire day without supervision.
- When I start writing code for the game
- Then I want to start by learning about the important concepts of C++ that will enable
me to write robust code without issues that can cause problems when the code runs for
an extended period of time (e.g. memory leaks, uncaught exceptions, or incorrect use of pointers)

## 13
As a game developer, I want to know how the IMOVE framework works, such that the code that I write integrates well with the IMOVE framework.
 
Standard Scenario: 
- Given that I am working on the code of the game
- When I work on the code that has to integrate or interact with the IMOVE framework
- Then I want to understand how the IMOVE framework functions
- and I want to already have read the documentation of IMOVE so that I am prepared for working with IMOVE

## 14
As a game developer, I want to document what features need to be implemented at what
time, such that I can always rely on this planning to guide me through the project.

Standard Scenario:
- Given that the studio has created a Product Planning
- When I as a game developer start on a new sprint
and I am wondering what should be worked on during this sprint or any other sprint
- Then I can consult the roadmap in the Product Planning for a guideline on what should
be worked on

## 15
As a game developer, I want to document the game concept, such that I
can rely on this documentation when I am implementing features
(Game Design Document).

Standard Scenario:
- Given that the studio has created a Game Design Document
- When I as a game developer am working on a new feature for the game
- and I am in doubt about what the studio agreed upon with regard
to this specific feature
- Then I consult the Game Design Document for a definition of how the feature should work

## 16 
As a player I want to have more knowledge about the impact on the environment
of pollution and the endangerment of turtles, so that my experience is not only fun
but also educational.

Standard Scenario
- Given that the game is finished
- When I want to learn about the environment and turtles because the theme of
the game piqued my interest
- Then I want to see information projected onto the ground with regard to these subjects
and I want to be able to learn about the endangerment of turtles while playing the game.

Edge Case
- Given that the game is finished
- When I do not want to learn about the environment and turtles
- Then I want to be able to ignore the information with regard to these subjects
and I want to be able to continue playing.