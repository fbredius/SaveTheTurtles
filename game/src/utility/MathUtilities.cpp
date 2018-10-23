//
// Created by mariette on 23-5-18.
//

#include <iostream>
#include <cmath>
#include <cfloat>

#include "MathUtilities.h"
#include "../storage/Variables.h"

/**
 * Method that generates random x and y coordinates in the spawning area (defined by an ellipse).
 * @return spawnLocation - Returns a pair<double, double> containing the generated x and y coordinates.
 */
pair<double, double> MathUtilities::randomSpawnLocation() {
    double x = 0.0, y = 0.0;

    while (!isInSpawn(x, y)) {
        x = rand() % (int) ((Variables::PIXEL_HEIGHT / 10.0) * 3.0) +
            (Variables::PIXEL_WIDTH / 2.0 - (Variables::PIXEL_HEIGHT / 10.0 * 2.2) / 1.4);
        y = rand() % (int) (Variables::PIXEL_HEIGHT / 10.0 * 1.5) +
            (Variables::PIXEL_HEIGHT / 2.0 - (Variables::PIXEL_HEIGHT / 10.0 * 1.1) / 1.4);
    }

    return std::make_pair(x, y);
}

/**
 * Method that uses the given input x and y location to check if that point is in the spawn area.
 * @param xLoc - Input x location of object to check.
 * @param yLoc - Input x location of object to check.
 * @return isInSpawn - If the input location is in the spawn.
 */
bool MathUtilities::isInSpawn(int xLoc, int yLoc) {
    bool inXRange = false, inYRange = false;

    if (xLoc >= (Variables::PIXEL_WIDTH / 2.0 - (Variables::PIXEL_HEIGHT / 10.0 * 2.2) / 1.4)
        and xLoc <= (((Variables::PIXEL_HEIGHT / 10.0) * 3.0) + (Variables::PIXEL_WIDTH / 2.0
                                                                 - (Variables::PIXEL_HEIGHT / 10.0 * 2.2) / 1.4))) {
        inXRange = true;
    }

    if (yLoc >= (Variables::PIXEL_HEIGHT / 2.0 - (Variables::PIXEL_HEIGHT / 10.0 * 1.1) / 1.4)
        and yLoc <= ((Variables::PIXEL_HEIGHT / 10.0 * 1.5) + (Variables::PIXEL_HEIGHT / 2.0
                                                               - (Variables::PIXEL_HEIGHT / 10.0 * 1.1) / 1.4))) {
        inYRange = true;
    }

    return inXRange and inYRange;
}

/**
 * Method that calculates the position on the edge of a circle given the center, the old point and the radius.
 * @param objectX - X location of the object position.
 * @param objectY - Y location of the object position.
 * @param centerX - X location of the circle center.
 * @param centerY - Y location of the circle center.
 * @param radius - Radius of the circle.
 * @return pair<double, double> - The new x and y location.
 */
pair<double, double>
MathUtilities::calculateNewPos(double objectX, double objectY, double centerX, double centerY, double radius) {
    double length = sqrt(pow((centerX - objectX), 2) + pow((centerY - objectY), 2));
    double newX = ((objectX - centerX) * (radius / length)) + objectX;
    double newY = ((objectY - centerY) * (radius / length)) + objectY;
    if (objectX == centerX) {
        newX = objectX + 1;
    }
    if (objectY == centerY) {
        newY = objectY + 1;
    }
    return pair<double, double>(newX, newY);
}

/**
 * Method that looks for and returns the ObjectiveArea that is closest to a point.
 *
 * @param x - The x coordinate of the object.
 * @param y - The y coordinate of the object.
 * @return objectiveArea - The closest ObjectiveArea.
 */
ObjectiveArea MathUtilities::calculateClosestObjectiveArea(double x, double y) {

    auto minDistance = DBL_MAX;
    ObjectiveArea *closestObjectiveArea = nullptr;

    list<ObjectiveArea *>::iterator it;
    for (it = MathUtilities::objectiveAreas.begin(); it != MathUtilities::objectiveAreas.end(); ++it) {

        double distance = distanceBetweenPoints(x, y, (*it)->getX(), (*it)->getY());

        if (distanceBetweenPoints(x, y, (*it)->getX(), (*it)->getY()) < minDistance) {
            minDistance = distance;
            closestObjectiveArea = (*it);
        }
    }

    return *closestObjectiveArea;
}

/**
 * Calculates the euclidean distance between two pairs of points.
 * @param p1 - The first point.
 * @param p2 - The second point.
 * @return dist - The distance between the two points.
 */
double MathUtilities::calculateDistance(pair<double, double> p1, pair<double, double> p2) {
    double x = p1.first - p2.first; //calculating number to square in next step
    double y = p1.second - p2.second;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);

    return dist;
}

/**
 * Used to calculate the circumference of the ellipse.
 * @param r1 - The width of the ellipse.
 * @param r2 - The height of the ellipse.
 * @param theta - The angle.
 * @return the length of a certain piece of the ellipse.
 */
static double computeDpt(double r1, double r2, double theta) {
    double dpt_sin = pow(r1 * sin(theta), 2.0);
    double dpt_cos = pow(r2 * cos(theta), 2.0);

    return sqrt(dpt_sin + dpt_cos);
}

/**
 * Generates points in the shape of an ellipse.
 * @param numberOfPoints - Amount of points calculated on the ellipse.
 * @return points - list of points.
 */
map<int, pair<double, double>> *MathUtilities::generateEllipse(int numberOfPoints) {
    double r1 = Variables::PIXEL_WIDTH * Variables::OBSTACLE_PLACING;
    double r2 = Variables::PIXEL_HEIGHT * Variables::OBSTACLE_PLACING;

    double theta = 0.0;
    double twoPi = M_PI * 2.0;
    double deltaTheta = 0.0001;
    double numIntegrals = round(twoPi / deltaTheta);
    double circ = 0.0;

    /* integrate over the ellipse to get the circumference */
    for (double k = 0.0; k < numIntegrals; k = k + 1.0) {
        theta += k * deltaTheta;
        circ += computeDpt(r1, r2, theta);
    }

    // Amount of points calculated on the ellipse.
    int nextPoint = 0;
    double run = 0.0;
    theta = 0.0;

    // Calculate the points on the ellipse.
    map<int, pair<double, double>> *points = new map<int, pair<double, double>>;
    for (double i = 0.0; i < numIntegrals; i = i + 1.0) {
        theta += deltaTheta;
        double subIntegral = numberOfPoints * run / circ;
        if ((int) subIntegral >= nextPoint) {
            double x = (Variables::PIXEL_WIDTH / 2) + (r1 * cos(theta));
            double y = (Variables::PIXEL_HEIGHT / 2) + (r2 * sin(theta));
            (*points)[nextPoint] = make_pair(x, y);
            nextPoint++;
        }
        run += computeDpt(r1, r2, theta);
    }
    return points;
}

/**
 * Generates a list of random points along an ellipse.
 * @return finalPoints - A list of random points.
 */
list<pair<double, double>> *MathUtilities::generatePoints(map<int, pair<double, double>> *points, int numberOfPoints) {

    list<pair<double, double>> *finalPoints = new list<pair<double, double>>;
    // Take a specified amount of random indexes to select the points.
    // The total obstacles should not be greater than 6, because the screen will be cluttered
    // and the while loop might never finish.
    for (int j = 0; j < Variables::TOTAL_OBSTACLES; j++) {
        int index = rand() % numberOfPoints;
        int iterations = 0;
        while (!checkSpacing(finalPoints, (*points)[index])) {
            // If it can not find another valid point, break out of the loop.
            if (iterations > 10) {
                break;
            }
            index = rand() % numberOfPoints;
            iterations++;
        }
        // If it can not find one point after 10 iterations, it will probably not find another.
        if (iterations > 10) {
            break;
        }
        finalPoints->push_back((*points)[index]);
    }
    return finalPoints;
}

/**
 * Adapted version of the generatePoints method that takes in a list of existing locations that should be avoided.
 * This is used when spawning a single obstacle and when it is desired to prevent the obstacle from spawning inside
 * other obstacles or turtles.
 * @param points - The points where the new obstacle could spawn.
 * @param existing - The points that should be avoided.
 * @param numberOfPoints - The number of potential points.
 * @return newLocation - a new point generated in a way that is as far away from existing points as possible.
 */
pair<double, double> *MathUtilities::generatePointWithExistingPoints(map<int, pair<double, double>> *points,
                                                                     list<pair<double, double>> *existing,
                                                                     int numberOfPoints) {
    int index = rand() % numberOfPoints;
    int iterations = 0;
    while (!checkSpacing(existing, (*points)[index])) {
        // If it can not find another valid point, break out of the loop.
        if (iterations > 10) {
            break;
        }
        index = rand() % numberOfPoints;
        iterations++;
    }

    return &((*points)[index]);
}

/**
 * Checks whether the next point is not too close to the others.
 * @param points - Previously chosen points.
 * @param newPoint - The new point under question.
 * @return boolean - Whether the point will be accepted or not.
 */
bool MathUtilities::checkSpacing(list<pair<double, double>> *points, pair<double, double> p2) {
    double minDistance = Variables::OBSTACLE_SPACING * Variables::PIXEL_HEIGHT;
    for (pair<double, double> p1 : *points) {
        if (MathUtilities::calculateDistance(p1, p2) < minDistance) {
            return false;
        }
    }
    return true;
}

/**
 * Method that calculates the distance between two points.
 * @param x1 - the x coordinate of the first point.
 * @param y1 - the y coordinate of the first point.
 * @param x2 - the x coordinate of the second point.
 * @param y2 - the y coordinate of the second point.
 * @return Returns the distance as a double.
 */
double MathUtilities::distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    double distanceX = fabs(x1 - x2);
    double distanceY = fabs(y1 - y2);

    return sqrtf(static_cast<float>(pow(distanceX, 2) + pow(distanceY, 2)));
}

/**
 * Method for calculating the angle between two points.
 * @param x1 - the x coordinate of the first point.
 * @param y1 - the y coordinate of the first point.
 * @param x2 - the x coordinate of the second point.
 * @param y2 - the y coordinate of the second point.
 * @return Returns the angle between the two points in degrees.
 */
double MathUtilities::angleBetweenPoints(double x1, double y1, double x2, double y2) {
    double x_difference = x2 - x1;
    double y_difference = y2 - y1;
    return atan2(y_difference, x_difference) * (180 / M_PI);
}

/**
 * Method that does a linear interpolation between two values over time.
 * @param currentTime - The current time in the time scale of the linear interpolation.
 * @param totalTime - The end time in the time scale of the linear interpolation.
 * @param startValue - The starting value, this is returned at currentTime / totalTime = 0.
 * @param endValue - The end value, this is returned at currentTime / totalTime = 1.0.
 * @return currentValue - The value returned by the linear interpolation based on the currentTime.
 */
double MathUtilities::linearInterpolation(double currentTime, double totalTime, double startValue, double endValue) {
    double factor = currentTime / totalTime;

    if (factor < 0.0) {
        factor = 0.0;
    }

    if (factor > 1.0) {
        factor = 1.0;
    }

    double currentValue = (1 - factor) * startValue + factor * endValue;
    return currentValue;
}

/**
 * Definition of the private objectiveAreas list.
 */
std::list<ObjectiveArea *> MathUtilities::objectiveAreas;

/**
 * Setter for the ObjectiveAreas, this is set from the GameManager and needed for calculateClosestObjectiveArea.
 * @param objectiveAreas - A list with the current set of Objective Areas.
 */
void MathUtilities::setObjectiveAreas(const list<ObjectiveArea *> &objectiveAreas) {
    MathUtilities::objectiveAreas = objectiveAreas;
}