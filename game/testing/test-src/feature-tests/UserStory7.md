# User Story 7
### Standard scenario

In order to assist game supervisors with running the game, the studio created a Deployment Guide
which currently is located both in the Readme as well as in the Emergent Architecture Design 
document.  
When the game will be compiled and distributed to a game supervisor that has no knowledge about
the codebase of the project, the studio will make sure to include this Deployment Guide next to
the game executable upon delivery.

This Deployment Guide provides game supervisors with a multitude of options and information.
- Dependency Setup
- Running the game standalone
- Running the game with IMOVE
- Running automated tests
- Running description-based tests
- Generating Code Coverage

Each and every one of these subsections of the Deployment Guide has been written to assist all
people that would read this.

### Alternate standard scenario

If a game supervisor tries to run the game, but there is already a version of the game running
which either does not show up or does not work, then the studio has made the product robust 
enough to allow the game supervisor to forcefully quit the game.

Forcefully quitting the game can be done in two ways:
- Using a key combination to forcefully close the corrupt version of the game.
    - Windows:&ensp;&ensp;Alt + F4
    - Linux:&ensp;&ensp;&ensp;&ensp;&ensp;&nbsp;Ctrl + Q
    - MacOS:&ensp;&ensp;&ensp;&ensp;Command + W
- Simply pulling the plug also does the trick, however this has the downside that the device
on which the game was running will be down for a certain amount of seconds due to it rebooting.