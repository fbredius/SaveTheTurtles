# User Story 8
### Standard Scenario

The studio added a lot of scalable parameters in the Variables class. Almost everything about the speed and size of entities in all of their states is configurable.

Furthermore, a lot of the aspects about the gameplay loop like duration of certain states and the amount of obstacles is configurable.

An important note is that sizes are in often in meters and speeds in meters/second. This makes configuring more natural because once the PIXELS_PER_METER variable is set correctly it is more easy to predict what certain variable changes will do the game in real world parameters.

### Edge Case

The game can be scaled for smaller values but the studio was not able to determine a scaling at which the game becomes unplayable because there was only one testing setup.
Thus, there was no way to make a decision on this and after this realization it was left out of the scope of this project.

## Possible Future Improvement

The studio was not able to do any input validation. However, it is debatable whether this would be useful since IMOVE does not use any input validation and it hard crash if used incorrectly. Because of this, someone who would not know what he or she is doing would be able to crash the software anyway by incorrectly setting the IMOVE variables.

A dedicated config file reader could be added, unfortunately there was no time left for this so all the variables are set as final values within the Variables class.

