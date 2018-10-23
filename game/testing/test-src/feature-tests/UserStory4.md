# User Story 4
### Standard Scenario

The studio was not able to implement all ideas with regard to an interactive environment.
However, the studio was able to implement the following to facilitate the interactive experience described in this user story:
- A dust trail follows the player, this is purely aesthetic but it shows the player that he or she is being tracked and during user testing some people started to play with the trail by running around in circles.
- The GameIdleState, which allows players to interact with turtles wihout a scoring system or hazards like obstacles.

Further additions could be:
- Palm trees, bushes or other entities that shake/sway as a person moves past them.

So while there could be some improvements, the acceptance criteria were met for the most part and the groundwork is there for future additions to the interactive environment.


### Edge Case 1
This was implemented using the StartingArea that people can stand in. Unfortunately it is not possible to automatically test the reactions of people to this implementation.
But user testing showed that most people that were not interested in starting the game did not accidentally start the game.

## Edge Case 2
People that really wanted to start the game immediately walked over to the StartingArea which has a visual of a big *Start Game* button. 
At first it was not clear that the game had started, but with the addition of tutorial arrows and obstacles there were enough changes in the playing field to indicate that a change in the game state was occurring.