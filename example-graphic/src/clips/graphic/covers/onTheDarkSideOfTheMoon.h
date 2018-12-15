#pragma once
#include "clips/graphic.h"
namespace clips {
    class onTheDarkSideOfTheMoon : public ofxLiveSet::clip::graphic {
    public:
        onTheDarkSideOfTheMoon(std::string name = "onTheDarkSideOfTheMoon") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _parameters.add(_primaryColor);
        }

        void update(){
            glm::vec2 a(-100, 100);
            glm::vec2 b(100, 100);
            glm::vec2 c(0, a.y - (b.x - a.x)*sqrt(3)/2);

            glm::vec2 d(-_width/2, 100);
            glm::vec2 e(0, 0);

            begin();
            ofSetColor(_primaryColor);
            ofSetLineWidth(1.2);
            ofNoFill();
            ofDrawTriangle(a, b, c);

            auto intersection = getIntersection(a, c, d, e);
            ofDrawLine(d.x, d.y, intersection.x, intersection.y);


            //TODO: calculate angle
            glm::vec2 f(intersection.x + 50, intersection.y+10);
            glm::vec2 g(intersection.x + 60, intersection.y-5);
            ofFill();
            ofDrawTriangle(intersection,f,g);
            ofNoFill();

            auto outTopIntersection = getIntersection(intersection, g, b, c);
            auto outBottomIntersection = getIntersection(intersection, f, b, c);
 
            auto redC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6);
            auto orangeC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6*2);
            auto yellowC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6*3);
            auto greenC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6*4);
            auto blueC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6*5);
            auto purpleC = getPointOnLineSegment(outTopIntersection, outBottomIntersection, 1.0/6*6);

            auto redB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6);
            auto orangeB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6*2);
            auto yellowB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6*3);
            auto greenB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6*4);
            auto blueB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6*5);
            auto purpleB = getPointOnLineSegment(glm::vec2(_width/2, outTopIntersection.y - 10), glm::vec2(_width/2, outBottomIntersection.y + 10), 1.0/6*6);

            ofPath red;
            red.setFillColor(ofColor::red);
            red.lineTo(outTopIntersection.x, outTopIntersection.y);
            red.lineTo(_width/2, outTopIntersection.y - 10);
            red.lineTo(redB.x, redB.y);
            red.lineTo(redC.x, redC.y);
            red.close();
            red.draw();


            ofPath orange;
            orange.setFillColor(ofColor::orange);
            orange.lineTo(redC.x, redC.y);
            orange.lineTo(redB.x, redB.y);
            orange.lineTo(orangeB.x, orangeB.y);
            orange.lineTo(orangeC.x, orangeC.y);
            orange.close();
            orange.draw();

            ofPath yellow;
            yellow.setFillColor(ofColor::yellow);
            yellow.lineTo(orangeC.x, orangeC.y);
            yellow.lineTo(orangeB.x, orangeB.y);
            yellow.lineTo(yellowB.x, yellowB.y);
            yellow.lineTo(yellowC.x, yellowC.y);
            yellow.close();
            yellow.draw();


            ofPath green;
            green.setFillColor(ofColor::green);
            green.lineTo(yellowC.x, yellowC.y);
            green.lineTo(yellowB.x, yellowB.y);
            green.lineTo(greenB.x, greenB.y);
            green.lineTo(greenC.x, greenC.y);
            green.close();
            green.draw();

            ofPath blue;
            blue.setFillColor(ofColor::lightBlue);
            blue.lineTo(greenC.x, greenC.y);
            blue.lineTo(greenB.x, greenB.y);
            blue.lineTo(blueB.x, blueB.y);
            blue.lineTo(blueC.x, blueC.y);
            blue.close();
            blue.draw();

            ofPath purple;
            purple.setFillColor(ofColor::purple);
            purple.lineTo(blueC.x, blueC.y);
            purple.lineTo(blueB.x, blueB.y);
            purple.lineTo(purpleB.x, purpleB.y);
            purple.lineTo(purpleC.x, purpleC.y);
            purple.close();
            purple.draw();

            end();
            setNewFrame();
        }

        glm::vec2 getIntersection(glm::vec2 startA, glm::vec2 endA, glm::vec2 startB, glm::vec2 endB){
            float a1 = endA.y - startA.y;
            float b1 = startA.x - endA.x; 
            float c1 = a1*(startA.x) + b1*(startA.y); 
        
            double a2 = endB.y - startB.y;
            double b2 = startB.x - endB.x; 
            double c2 = a2*(startB.x) + b2*(startB.y); 
        
            double determinant = a1*b2 - a2*b1; 
        
            if (determinant == 0) 
            { 
                // The lines are parallel. This is simplified 
                return glm::vec2(0,0); //TODO: inf, inf
            } 
            else
            { 
                double x = (b2*c1 - b1*c2)/determinant; 
                double y = (a1*c2 - a2*c1)/determinant; 
                return glm::vec2(x,y);
            } 
        }

        glm::vec2 getPointOnLineSegment(glm::vec2 start, glm::vec2 end, float position){
            auto length = end - start;
            return start + position * length;
        }
        ofParameter<ofColor> _primaryColor;
    };
};