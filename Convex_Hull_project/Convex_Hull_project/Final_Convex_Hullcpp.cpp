#include <iostream>
#include <stack>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>


void GUI_VISUALIZATION(std::vector<std::tuple<int, int, double>>&plotted_points, std::stack<std::tuple<int, int, double>> &GS_stack) {
	//initialize the for the zoom function
	//initialize the stored size of the plotted points for our zoom function
	//this is because we want to be centered at the lowest point, which is added to the end of
	//plotted points in main
	sf::Font font;
	if (!font.loadFromFile("res/roboto/Roboto-Black.ttf")) {
		std::cout << "Error loading file" << std::endl;
	}
	// create the window that will open when the program is launched
	// first parameter is width, second is height, third is title of the window
	sf::RenderWindow window(sf::VideoMode(1500, 1500), "Convex Hull Generator");
	
	//Attempted to fix camera to be more like cartesian graph, could not figure out in time

	//This determines the number of points to put into the Convex Hull
	int numPoints = GS_stack.size();

	sf::ConvexShape convexHull(numPoints);
	// this is the fill color for the convex hull
	convexHull.setFillColor(sf::Color(0, 0, 0));
	// this sets the thickeness of the outline
	convexHull.setOutlineColor(sf::Color(255, 255, 255));
	convexHull.setOutlineThickness(1);
	// 0-4

	
	//Loop that runs for the stack amount and allows for us to make the hull
	for (int i = 0; i < numPoints; i++)
	{
		// loop thru the stack and put in the x and y values of the tuples instead
		// the +100 just moves the polygon away from the edge so that it is easier to visualize
		convexHull.setPoint(i, sf::Vector2f(std::get<0>(GS_stack.top()) + 100, std::get<1>(GS_stack.top()) + 100));
		GS_stack.pop();
	}
	// important to have the points in clockwise or counterclockwise order

//--------------------------------------------------------------------------------


	// the code below is how the window will open and what will display on the window
	sf::Event event;
	sf::Text xText;
	
	//These two counters allow us to dynamically change both the thickness of the convexhull and point outlines,
	//the size Muiltiplier allows us to change the scale to which we see the convex hull, as well as the change in
	//position for the points
	//All of this is in regards to our zoom features
	float outlineThicknessCounter = 1;
	int sizeMultiplier = 1;
	//These two integers work to allow us move the convexhull, points, and text when we want to see a certain point zoomed in.
	int moveX = 0;
	int moveY = 0;

	// this is the while loop that will display the gui and the visualization of the convex hull
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// if the user presses the window to close it, the window will close
			case sf::Event::Closed:
				window.close();
				break;
			//This event key states that if we press the U and Z keys, the actions inside will occur, such as zooming in or out
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Z) {
					//this condition checks to make sure there is always an outline that a user can see
					if (outlineThicknessCounter > 1) {
						outlineThicknessCounter -= 0.1;
					}
					//increases the size for both scale of hull and point position
					sizeMultiplier += 1;
				
				} else if (event.key.code == sf::Keyboard::U) {
					//this condition checks to make sure there is always an outline that a user can see that is not too large
					//if its too large it will cover the points
					if (sizeMultiplier > 1) {
						if (outlineThicknessCounter < 1) {
							outlineThicknessCounter += 0.1;
						}
						sizeMultiplier -= 1;
				
					}
				}
				//These four key presses work to move our "Camera" so that our entire hull can be seen
				else if (event.key.code == sf::Keyboard::Up) {
					moveY += 50;
				}
				else if (event.key.code == sf::Keyboard::Down) {
					moveY -= 50;
				}
				else if (event.key.code == sf::Keyboard::Left) {
					moveX += 50;
				}
				else if (event.key.code == sf::Keyboard::Right) {
					moveX -= 50;
				}
				break;
			default:
				break;
			}
		}

		//These lines of code set the scale and postion of our convex hull to adjust with the changing values, and allows for the zoom
		//feature to look semi-normal
		convexHull.setScale(sizeMultiplier, sizeMultiplier);
		convexHull.setPosition(100 - (100 * sizeMultiplier) + moveX, 100 - (100 * sizeMultiplier) + moveY);
		convexHull.setOutlineThickness(outlineThicknessCounter);
		window.clear(sf::Color::Black);
		window.draw(convexHull);

		// this is the loop that sets the points of the convex hull
		// have the x and y values of the stack be put into here

		// right now, the point is lying directly on top of the x and y value of the convex hull
		// the only problem is i have to now print out n number of points on there respective x,y

		for (int i = 0; i < convexHull.getPointCount(); i++) {
			//This prints the point values of only the points on the convex hull, the int wrapping rounds the value to a whole number
			//need to subtract 100 to represent the actual hull
			xText.setString("(" + std::to_string(int(convexHull.getPoint(i).x) - 100) + "," + std::to_string(int(convexHull.getPoint(i).y) - 100) + ")");
			//This sets the position, this is a long math equation as each step must be done
			//The sizemultipler moves both the text and allows for the numbers to grow with the points and not over grow
			//next we subtract the other size multiplier to instill this matching growth
			//after that we add 90 and 100 respectively inorder to make it so our point does not sit ontop of the point and will be slightly offset
			xText.setPosition((convexHull.getPoint(i).x)* sizeMultiplier - (100 * sizeMultiplier)+ 100 + moveX, (convexHull.getPoint(i).y * sizeMultiplier) - (100 * sizeMultiplier)+ 110 + moveY);
			xText.setFillColor(sf::Color::Red);
			xText.setCharacterSize(35);
			xText.setFont(font);
			
			window.draw(xText);
		}
		

		// this draws all of the points on the polygon at the correct points
		for (int i = 0; i < plotted_points.size(); i++) {
			sf::CircleShape point;
			point.setRadius(8);
			point.setFillColor(sf::Color(255, 255, 255));
			point.setOutlineColor(sf::Color(0, 0, 0));
			point.setOutlineThickness(outlineThicknessCounter+3);
			// change the values inside of set position to all of the points, not just the ones that are in 
				// the convex hull
			point.setPosition(std::get<0>(plotted_points[i])*sizeMultiplier + 94 + moveX, std::get<1>(plotted_points[i])*sizeMultiplier + 95 + moveY);

			window.draw(point);
		}
		window.display();
	}
}

void InsertionSort(std::vector<std::tuple<int, int, double>> &bucket) {
	std::tuple<int, int, double> temp;
	for (int i = 0; i < bucket.size(); i++) {
		for (int j = i; j > 0; j--) {
			if (std::get<2>(bucket[j]) < std::get<2>(bucket[j - 1])) {
				temp = bucket[j];
				bucket[j] = bucket[j - 1];
				bucket[j - 1] = temp;
			}
			// consider if the points are colinear(same angle from lowest point), if so consider x value
			else if (std::get<2>(bucket[j]) == std::get<2>(bucket[j - 1])) {
				if (std::get<0>(bucket[j]) < std::get<0>(bucket[j - 1])) {
					temp = bucket[j];
					bucket[j] = bucket[j - 1];
					bucket[j - 1] = temp;
				}
			}
		}
	}
}

void BucketSort(std::vector<std::tuple<int, int, double>> &plotted_points) {
	// 1. Create n empty buckets that stores the tuples
	int n = plotted_points.size();
	std::vector<std::vector<std::tuple<int, int, double>>> buckets;
	buckets.resize(n);

	// Find the maximum angle value from an element from plotted_points. This value is used in a formula to calculate an index which places an element into
	// a specific bucket
	double max = std::get<2>(plotted_points[0]);
	for (int i = 0; i < plotted_points.size(); i++) {
		if (std::get<2>(plotted_points[i]) > max) {
			max = std::get<2>(plotted_points[i]);
		}
	}

	// 2. Put elements in their respective buckets by calculating an index using the formula below
	for (int i = 0; i < n; i++) {
		int bucketIndex = std::floor(n * std::floor(std::get<2>(plotted_points[i])) / max); 
		buckets[bucketIndex].push_back(plotted_points[i]);
	}

	// 3. Sort each individual bucket using Insertion Sort by looping through the bucket list
	for (int i = 0; i < n; i++) {
		InsertionSort(buckets[i]);
	}

	// 4. Loop through the bucket list to gradually update the unsorted values of plotted_points with the values in the buckets. 
	//    This will result in a sorted list when the loop terminates.
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < buckets[i].size(); j++) {
			plotted_points[index] = buckets[i][j];
			index++;
		}
	}
}

void findAngle(std::tuple<int, int, double> &lowest, std::tuple<int, int, double> &newPoint) {
	double angle;
	//use the math function atan2, this will spit out the angle in radians
	//convert to degrees by multiplying by 180, then dividing by pi using the built
	//in definition M_PI(due to not being flexible this was changes to 3.14)
	angle = std::atan2(std::get<1>(newPoint) - std::get<1>(lowest), std::get<0>(newPoint) - std::get<0>(lowest));
	angle = angle * 180 / 3.14;
	std::get<2>(newPoint) = angle;

}

std::tuple<int, int, double> findLowest(std::vector<std::tuple<int, int, double>>&plotted_points) {
	std::tuple<int, int, double> lowestPair = plotted_points[0];
	//iterate through vector to find lowest value
	for (int i = 0; i < plotted_points.size(); i++) {
		//if the y value is less than lowest's y, then lowest becomes the new pair
		if (std::get<1>(plotted_points[i]) < std::get<1>(lowestPair)) {
			lowestPair = plotted_points[i];
		}
		else if (std::get<1>(plotted_points[i]) == std::get<1>(lowestPair)) {
			//If lowestPair y ==  sorted_points y, use one with lowest X
			if (std::get<0>(plotted_points[i]) < std::get<0>(lowestPair)) {
				lowestPair = plotted_points[i];
			}
		}
	}
	return lowestPair;
}

void getData(std::string fileName, std::vector<std::tuple<int, int, double>>&plotted_points) {
	//opens file
	std::ifstream fileOpen(fileName);
	bool isOpen = fileOpen.is_open();
	std::string row;
	//traverses file until the end of file is reached
	while (getline(fileOpen, row)) {
		std::vector<std::tuple<int, int, double>> vecRow;
		std::stringstream rowStrings(row);
		int xCord, yCord;
		rowStrings >> xCord;
		rowStrings >> yCord;
		plotted_points.push_back(std::make_tuple(xCord, yCord, 0));
	}


}

//Function that calculates the cross product(Used to determine clockwise or counter Clockwise)
int crossProduct(int x1, int y1, int x2, int y2, int x3, int y3) {
	int crossProductValue;
	crossProductValue = (x2 - x1)*(y3 - y1) - (x3 - x1) * (y2 - y1);
	return crossProductValue;

}

//This is an easily callable function to get item two spaces into the stack
std::tuple<int, int, double> previousPointStack(std::stack<std::tuple<int, int, double>> &GS_stack) {
	std::tuple<int, int, double> tempPoint = GS_stack.top();
	GS_stack.pop();
	std::tuple<int, int, double> previousPoint = GS_stack.top();
	GS_stack.push(tempPoint);
	return previousPoint;
}

//Function that fills the stack following the Graham Scan Algorithm
void graham_scan(std::stack<std::tuple<int, int, double>> &GS_stack, std::vector<std::tuple<int, int, double>> &plotted_points, std::tuple<int, int, double> lowestPair) {
	//Initialize a tuple to hold next point inside loop
	std::tuple<int, int, double> nextPoint;

	//We know that both the lowest point and first point of the vector will be in the stack, so we can push them in before the loop
	GS_stack.push(lowestPair);
	GS_stack.push(plotted_points[0]);

	//This loop from the second element on due to us knowing the the lowest and first element of the vector will be in the hull and on the stack
	for (int i = 1; i < plotted_points.size(); i++) {
		//push current value into the stack
		GS_stack.push(plotted_points[i]);

		//Conditional to make sure that when we reach final point it is compared back to the first value
		if (i + 1 < plotted_points.size()) {
			nextPoint = plotted_points[i + 1];
		}
		else {
			nextPoint = lowestPair;
		}

		//While loop
			//This will continously call the cross Product until it produces a value above 0
			//Inorder to allow the value to change, we pop a value out and call the previousPointStack Function to get the value right before the new top
			//We call the new top in the next set of x and y
			//The only values that remain consistent is our next value, as we are only editing the points we have visited in order to produce the convex hull
		while (crossProduct(std::get<0>(previousPointStack(GS_stack)), std::get<1>(previousPointStack(GS_stack)), std::get<0>(GS_stack.top()), std::get<1>(GS_stack.top()), std::get<0>(nextPoint), std::get<1>(nextPoint)) <= 0) {
			GS_stack.pop();
		}
	}



}


int main(int argc, char**argv) {
	//initialize vector of sorted points
	//A vector of tuples, the tuples have x val, y val, and angle in relation to intial point
	std::vector<std::tuple<int, int, double>> plotted_points;

	//initializes stack
	std::stack<std::tuple<int, int, double>> graham_scan_stack;

	std::string textFile = argv[1];
	//Read file of points, put into a vector
	getData(argv[1], plotted_points);


	//Find lowest point in set
	//Initialize tuple to hold lowest pair, set to lowest function
	std::tuple<int, int, double> lowestPair = findLowest(plotted_points);


	//Find angles between initial point and ALL other points
	int startPoint;
	for (int i = 0; i < plotted_points.size(); i++) {
		findAngle(lowestPair, plotted_points[i]);
		//Finds the lowest point again and saves its index
		if (std::get<2>(plotted_points[i]) == 0 && std::get<0>(plotted_points[i]) == std::get<0>(lowestPair) && std::get<1>(plotted_points[i]) == std::get<1>(lowestPair)) {
			startPoint = i;
		}
	}
	//removes the lowest point from the vector so that we do not check during stack stage
	plotted_points.erase(plotted_points.begin() + startPoint);

	//Sort the vector to determine the order we look at the points
	// Determine order of points based on angles
	BucketSort(plotted_points);


	//Call function to begin Gramham Scan
	graham_scan(graham_scan_stack, plotted_points, lowestPair);


	//push the lowest point back into the hull inorder to graph it
	plotted_points.push_back(lowestPair);
	GUI_VISUALIZATION(plotted_points, graham_scan_stack);



}
