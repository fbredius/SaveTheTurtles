//
// Created by mariette on 23-5-18.
//

#ifndef GAME_MATHUTILITIES_H
#define GAME_MATHUTILITIES_H

#include <iostream>
#include <cmath>
#include <list>
#include <map>
#include "../storage/Variables.h"
#include "../gameEntities/ObjectiveArea.h"

using namespace std;

class MathUtilities {

private:
    static std::list<ObjectiveArea *> objectiveAreas;

public:
    static pair<double, double> randomSpawnLocation();

    static bool isInSpawn(int xLoc, int yLoc);

    static pair<double, double> calculateNewPos(double objectX, double objectY, double centerX, double centerY,
                                                double radius);

    static ObjectiveArea calculateClosestObjectiveArea(double xLocation, double yLocation);

    static double calculateDistance(pair<double, double> p1, pair<double, double> p2);

    static list <pair<double, double>> *generatePoints(map<int, pair<double, double>> *points, int numberOfPoints);

    static pair<double, double> *generatePointWithExistingPoints(map<int, pair<double, double>> *points,
                                                                 list<pair<double, double>> *existing,
                                                                 int numberOfPoints);

    static bool checkSpacing(list <pair<double, double>> *points, pair<double, double>);

    static map<int, pair<double, double>> *generateEllipse(int numberOfPoints);

    static double distanceBetweenPoints(double x1, double y1, double x2, double y2);

    static double angleBetweenPoints(double x1, double y1, double x2, double y2);

    static double linearInterpolation(double currentTime, double totalTime, double startValue, double endValue);

    static void setObjectiveAreas(const list<ObjectiveArea *> &objectiveAreas);
};

#endif //GAME_MATHUTILITIES_H