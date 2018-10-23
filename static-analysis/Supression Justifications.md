# Supression Justifications

This document contains justifications for all CppCheck suppressions in CppCheckSuppression.txt

### CMakeLists.txt

It throws a lot of false positives here since this is not C++ code.

### noCopyConstructor:game/src/control/GameManager.h

We will never make a copy of a GameManager. The old one will be deleted with all its contents and a new one will be made.
Thus we do not need a copy constructor.

### noCopyConstructor:game/src/gameEntities/Player.h

We will never make a copy of one player. It can only be destroyed. When we need a new one we make an entirely new one.

### unsafeClassCanLeak:game/src/control/GameManager.h

Suggestion for creating a destructor resulted in bugs when using IMOVE because data would get removed.
It also resulted in tests breaking.

### unsafeClassCanLeak:game/src/control/gameStates/GameIdleState.h

Same reason as above. This warning seems to be kind of inconsistent since GameIdleState is similar to other GameStates
but it does not get thrown there.

### unusedPrivateFunction:game/src/control/gameStates/GameFinishedState.h

False positive.

### unusedPrivateFunction:game/src/control/gameStates/GameRunningState.h

False positive.

### unusedPrivateFunction:../game/src/control/gameStates/GameStartState.h

False positive.